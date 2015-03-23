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
#include <cstdlib>

#include "xml_parser.hpp"

xml_parser::xml_parser(string dk_path_t, plugin_iface* plug, bool sample_load_data /*=true*/)
	:sax_tag_status(false),message(NULL),id(0),plugin(plug)
	,vol(-1),panL(-1),panR(-1),mute(-1)
	,dk_path(dk_path_t),load_data(sample_load_data){

	cout << "dk path loading" << dk_path << endl;

	instru_tag = XMLString::transcode("instrument");
	id_tag = XMLString::transcode("id");
	name_tag = XMLString::transcode("name");
	filename_tag = XMLString::transcode("filename");

	vol_tag = XMLString::transcode("volume");
	panL_tag = XMLString::transcode("pan_L");
	panR_tag = XMLString::transcode("pan_R");
	mute_tag = XMLString::transcode("isMuted");


	/*work around to support float notation*/
	setlocale(LC_NUMERIC, "C");

}

xml_parser::~xml_parser(){

	XMLString::release(&instru_tag);
	XMLString::release(&id_tag);
	XMLString::release(&name_tag);
	XMLString::release(&filename_tag);

	XMLString::release(&vol_tag);
	XMLString::release(&panL_tag);
	XMLString::release(&panR_tag);
	XMLString::release(&mute_tag);
}



void xml_parser::startElement(const XMLCh* const uri, 
                                const XMLCh* const localname,
                                const XMLCh* const qname, 
                                const Attributes& attrs)
{

	if(XMLString::compareString(localname, instru_tag) == 0){
		//cout << "intru enter" << endl;
		//sax_tag_status = true;
	}
	else if(XMLString::compareString(localname, id_tag) == 0){
		//cout << "id enter" << endl;
		sax_tag_status = true;
	}
	else if(XMLString::compareString(localname, name_tag) == 0){
		//cout << "name enter" << endl;
		sax_tag_status = true;
	}
	else if(XMLString::compareString(localname, filename_tag) == 0){
		//cout << "filename enter" << endl;
		sax_tag_status = true;
	}
	else if(XMLString::compareString(localname, vol_tag) == 0){
		//cout << "vol enter" << endl;
		sax_tag_status = true;
		vol = -1;
	}
	else if(XMLString::compareString(localname, panL_tag) == 0){
		//cout << "panL enter" << endl;
		sax_tag_status = true;
		panL = -1;
	}
	else if(XMLString::compareString(localname, panR_tag) == 0){
		//cout << "panR enter" << endl;
		sax_tag_status = true;
		panR = -1;
	}else if(XMLString::compareString(localname, mute_tag) == 0){
		//cout << "mute enter" << endl;
		sax_tag_status = true;
		mute = -1;
	}

}

void xml_parser::endElement( const XMLCh* const uri,
                               const XMLCh* const localname,
                               const XMLCh* const qname)
{

	if(XMLString::compareString(localname, instru_tag) == 0){
		//		sax_tag_status = false;
		if(!filename.empty()){
			/*create sample and add it to drumkit*/
			if(filename.size() == 1){
				sample* smpl = new sample(plugin->get_srate(), filename.back(), name, load_data);
				smpl->set_vol(vol);
				smpl->set_pan(panL, panR);
				smpl->set_mute(mute);
				plugin->add_sample(id, smpl);
			}else{
				sample* smpl = new sample(plugin->get_srate(), filename, name, load_data);
				smpl->set_vol(vol);
				smpl->set_pan(panL, panR);
				smpl->set_mute(mute);
				plugin->add_sample(id, smpl);
			}
		}
		filename.clear();
	}
	else if(XMLString::compareString(localname, id_tag) == 0){
		sax_tag_status = false;
		if(message == NULL){
			id = 0;
		}else{
			id = atoi(message);
		}
		XMLString::release(&message);
#ifdef debug
		cout << "id:" << id << endl;
#endif
	}
	else if(XMLString::compareString(localname, name_tag) == 0){
		sax_tag_status = false;
		if(message == NULL){
			name = "noname";
		}else{
			name = message;
		}
		XMLString::release(&message);
#ifdef debug
		cout << "name:" << name << endl;
#endif
	}
	else if(XMLString::compareString(localname, filename_tag) == 0){
		sax_tag_status = false;
		if(message != NULL){
			if(XMLString::stringLen(message) != 0){
				path = dk_path + "/" + message;
				filename.push_back(path);
				XMLString::release(&message);
#ifdef debug
				cout << "filename" << message << endl;
#endif
			}
		}
	}
	else if(XMLString::compareString(localname, vol_tag) == 0){
		sax_tag_status = false;
		if(message != NULL){
			vol = atof(message);
		}
		XMLString::release(&message);
#ifdef debug
		cout << "vol:" << vol << endl;
#endif
	}
	else if(XMLString::compareString(localname, panL_tag) == 0){
		sax_tag_status = false;
		if(message != NULL){
			panL = atof(message);
		}
		XMLString::release(&message);
#ifdef debug
		cout << "panL:" << panL << endl;
#endif
	}
	else if(XMLString::compareString(localname, panR_tag) == 0){
		sax_tag_status = false;
		if(message != NULL){
			panR = atof(message);
		}
		XMLString::release(&message);
#ifdef debug
		cout << "panR:" << panR << endl;
#endif
	}else if(XMLString::compareString(localname, mute_tag) == 0){
		sax_tag_status = false;
		if(message != NULL){
			if(strcmp(message,"false") == 0){
				mute = 0;
			}else{
				mute = 1;
			}
		}
		XMLString::release(&message);
#ifdef debug
		cout << "mute:" << mute << endl;
#endif
	}

}

void xml_parser::characters(const XMLCh* const chars, const XMLSize_t length){

	if(sax_tag_status == true){
		message = XMLString::transcode(chars);
		//cout << "char tag:" << message << ":"<<endl;
	}
}

void xml_parser::characters(const XMLCh* const chars, const unsigned int length){

	if(sax_tag_status == true){
		message = XMLString::transcode(chars);
		//cout << "char tag:" << message << ":"<<endl;
	}
}

