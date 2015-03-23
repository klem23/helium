/*
	This file is part of the Noise Factory
	Copyright (C) 2007-2015 Clément Guedez <klem.dev@gmail.com>


    The Noise Factory is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Noise Factory is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Noise Factory.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <algorithm>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>


#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include "xml_parser.hpp"

#include "hydro_dk_scan.hpp"

using namespace std;
using namespace xercesc;


const char* hydro_path[NB_HYDRO_PATH] = { 
	"/usr/share/hydrogen/data/drumkits/",
	"/usr/local/share/hydrogen/data/drumkits/",
	"~/.hydrogen/data/drumkits/"
};

bool dk_info_sort(dk_info a, dk_info b){
	if(a.name.compare(b.name) <= 0){
		return true;
	}else{
		return false;
	}
}

hydro_dk_scan::hydro_dk_scan(bool sample_load_data /*=true*/){
	load_data = sample_load_data;
	scan_path();

}


hydro_dk_scan::~hydro_dk_scan(){

}

void hydro_dk_scan::scan_path(){

	/*list entry in common hydrogen path*/
	struct dirent **namelist;
	struct stat bstat;
	dk_info dki;

	for(int i = 0; i < NB_HYDRO_PATH; i++){ 

		int nb = scandir(hydro_path[i], &namelist, 0, alphasort);
		if (nb < 0)
			//perror("scandir failed");
			cerr << "Scan Hydrogen drumkit path fail for :" << hydro_path[i] << endl;
		else {
			while (nb--) {
				string dk_name(namelist[nb]->d_name);
				if((dk_name.compare(".")!= 0)&&(dk_name.compare("..")!=0)){
					//cout << "drumkit : " << dk_name << endl;
					//dk_list.push_back(dk);
					dki.name = dk_name;
					dki.path = hydro_path[i] + dk_name;
					string tmp_path = dki.path + "/drumkit.xml";
					if(stat(tmp_path.c_str(), &bstat) != -1)
						dklist.push_back(dki);
				}
				free(namelist[nb]);
			}
			free(namelist);
		}
	}

	//sort dklist in alphabetical order
	sort(dklist.begin(), dklist.end(), dk_info_sort);
	//print dk list
	uint32_t i = 0;
	for(vector<dk_info>::iterator it = dklist.begin(); it != dklist.end(); it++){
		cout << i << ":" << it->name << endl;
		i++;
	}

}

string hydro_dk_scan::get_name(uint32_t index){
	return dklist.at(index).name;
}

string hydro_dk_scan::get_path(uint32_t index){
	return dklist.at(index).path;
}

string hydro_dk_scan::get_xml(uint32_t index){
	return dklist.at(index).path + "/drumkit.xml";
}

uint32_t hydro_dk_scan::get_dk_nb(){
   return dklist.size();
}


void hydro_dk_scan::load_drumkit(uint32_t index, plugin_iface* plugin){

	/*initiate xerces XML parser*/
	try{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Error during XML Parser initialization! :\n";
		cout << "Exception message is: \n"
				<< message << "\n";
		XMLString::release(&message);
	}

	/*create xerces XML parser*/
	string path = get_path(index);
	xml_parser* xp = new xml_parser(path, plugin, load_data);

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
	parser->setContentHandler(xp);
	parser->setErrorHandler(xp);

	try {
		//cout << "dk_xml : " << dk_xml << endl;
		parser->parse(get_xml(index).c_str());
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n"
				<< message << "\n";
		XMLString::release(&message);
	}
	catch (const SAXParseException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n"
				<< message << "\n";
		XMLString::release(&message);
	}
	catch (...) {
		cout << "Unexpected Exception in XML Parser \n" ;
	}

	delete parser;
	delete xp;

	XMLPlatformUtils::Terminate();


}
