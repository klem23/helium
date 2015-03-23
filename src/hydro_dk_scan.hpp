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


#ifndef HYDRO_DK_SCAN_HPP
#define HYDRO_DK_SCAN_HPP

#include <vector>
#include <string>
#include <stdint.h>

#include "plugin_iface.hpp"

#define H_PATH_0 "/usr/share/hydrogen/data/drumkits/"
#define H_PATH_1 "/usr/local/share/hydrogen/data/drumkits/"
#define H_PATH_2 "~/.hydrogen/data/"

#define NB_HYDRO_PATH 3

using namespace std;

extern const char* hydro_path[NB_HYDRO_PATH];

typedef struct{
	string name;
	string path;
}dk_info;


class hydro_dk_scan{
	private:
		vector<dk_info> dklist;
		bool load_data;	

		void scan_path();

	public:
		hydro_dk_scan(bool sample_load_data = true);	
		~hydro_dk_scan();	

		string get_name(uint32_t index);
		string get_path(uint32_t index);
		string get_xml(uint32_t index);
		void load_drumkit(uint32_t index, plugin_iface* plugin);
		uint32_t get_dk_nb();

};

#endif
