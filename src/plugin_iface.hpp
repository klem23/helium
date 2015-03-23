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


#ifndef PLUGIN_IFACE
#define PLUGIN_IFACE

#include "sample.hpp"

#define H_NB_INST 32

using namespace std;

class plugin_iface{

	public:
	virtual ~plugin_iface(void) {};
	virtual uint32_t get_srate(void) = 0;
	virtual void add_sample(int id, sample* smpl) = 0;

};
#endif
