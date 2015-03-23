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


#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <vector>
#include <string>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include "plugin_iface.hpp"

using namespace std;
XERCES_CPP_NAMESPACE_USE

class xml_parser : public DefaultHandler
{
	private:
		bool	sax_tag_status;
		char	* message;

		/*sample reference*/
		vector<string> filename;	
		string	name;
		int	id;

		string path;
		plugin_iface* plugin;

		/*main tag*/
		XMLCh* instru_tag;
		XMLCh* id_tag;
		XMLCh* name_tag;
		XMLCh* filename_tag;

		/*sample parameter*/
		XMLCh* vol_tag;
		float vol;
		XMLCh* panL_tag;
		float panL;
		XMLCh* panR_tag;
		float panR;
		XMLCh* mute_tag;
		float mute;

		string dk_path;
		bool load_data;

	public:
		xml_parser(string dk_path_t, plugin_iface* plug, bool sample_load_data = true);
		~xml_parser();

		//void startDocument();
		//void endDocument();

		void startElement(const XMLCh* const uri, 
				const XMLCh* const localname, 
				const XMLCh* const qname, 
				const Attributes& attrs);

		void endElement( const XMLCh* const uri,
				const XMLCh* const localname,
				const XMLCh* const qname);

		/*support differences between xerces version*/ 
		void characters(const XMLCh* const chars, const XMLSize_t length);
		void characters(const XMLCh* const chars, const unsigned int length);


		/*error*/
		//void warning(const SAXParseException& exc);
		//void error(const SAXParseException& exc);
		//void fatalError(const SAXParseException& exc);

};

#endif
