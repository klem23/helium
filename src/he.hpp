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

#ifndef HE_HPP
#define HE_HPP

#include <vector>


#include "sample.hpp"
#include "plugin_iface.hpp"
#include "hydro_dk_scan.hpp"

//#define H_NB_INST 32

#define H_PATH_0 "/usr/share/hydrogen/data/drumkits/"
#define H_PATH_1 "/usr/local/share/hydrogen/data/drumkits/"
#define H_PATH_2 "~/.hydrogen/data/"

typedef struct{
	float* dk_nb;
	float* mute[H_NB_INST];
	float* vol[H_NB_INST];
	float* pan[H_NB_INST];
}he_param;

class He : public plugin_iface{
private:
	float* midi_in;
	float* audio_out_L;
	float* audio_out_R;

	uint32_t s_rate;
	float main_vol;

	sample *drumkit[H_NB_INST];
	bool loading;

	hydro_dk_scan* hds;
	uint32_t actual_dk;

public:
	He(uint32_t sampling_rate);
	~He(void);

	void check_param(he_param* hp);
	void add_sample(int id, sample* smpl);
	uint32_t get_srate(void);

	void set_input(float* ptr);
	void set_output_L(float* ptr);
	void set_output_R(float* ptr);
	void process(int nb_sample);
	void process_2(int nb_sample);
};


#endif
