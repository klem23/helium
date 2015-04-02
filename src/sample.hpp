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


#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

class sample{
	private:
		string name;
		float** data;

		uint32_t lgth;
		uint32_t channels;
		uint32_t s_rate;
		uint32_t inc;
		float velocity;

		float vol;
		float panL;
		float panR;
		bool mute;
		
		bool data_loaded;

	public:
		sample(const sample & smpl);
		sample(uint32_t samplerate, string path, string name_t = "no_name", bool load_data = true);
		sample(uint32_t samplerate, vector<string> path, string name_t, bool load_data = true);
		~sample(void);

		string Name(void);
		float Vol(void);
		float Pan(void);
		float Mute(void);

		void note_on(void);
		void note_on(uint8_t vel);
		void note_off(void);
		float get_val_L(void);
		float get_val_R(void);

		uint32_t get_lgth(void);
		uint32_t get_chan(void);

		void merge(float** out);

		void set_vol(float vol_t);
		void set_pan(float pan_t);
		void set_pan(float panL_t, float panR_t);

		void set_mute(float val);
};

#endif
