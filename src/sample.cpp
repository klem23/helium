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

#include <cstring>
#include <iostream>

#include <sndfile.h>
#include <samplerate.h>

#include "sample.hpp"

sample::sample(const sample & smpl)
	:name(smpl.name),data(NULL)
	,lgth(smpl.lgth),channels(smpl.channels),s_rate(smpl.s_rate),inc(0),velocity(1.0)
	,vol(smpl.vol),panL(smpl.panL),panR(smpl.panR),mute(smpl.mute)
	,data_loaded(false){

	if(smpl.data_loaded){
		data = new float*[channels];
		if(data != NULL){
			for(uint8_t i = 0; i < channels; i++){
				data[i] = new float[lgth];
				if(data[i] != NULL){
					memset(data[i], 0, lgth * sizeof(float));
					memcpy(data[i], smpl.data[i], lgth * sizeof(float));
					data_loaded = true;
				}
			}
		}
	}	
}

sample::sample(uint32_t samplerate, string path, string name_t /*= "no_name"*/, bool load_data /*=rue*/ )
	:name(name_t),data(NULL)
	,lgth(0),channels(0),s_rate(samplerate),inc(0),velocity(1.0)
	,vol(1),panL(1),panR(1),mute(false)
	,data_loaded(false){

	SNDFILE* smpl_file;
	SF_INFO smpl_info;

	memset(&smpl_info, 0, sizeof(smpl_info));
	smpl_file = sf_open(path.c_str(), SFM_READ, &smpl_info);

  	if(smpl_file != NULL){
		channels = smpl_info.channels;
		lgth = smpl_info.frames;

		//cout << "audio info : " << channels << " # " << smpl_info.samplerate << " # " << lgth << " # " << lgth * channels << endl;

		if(load_data){
			/*read file*/
			float* sndf_buff = new float[ lgth * channels ];
			memset(sndf_buff, 0, (lgth * channels) * sizeof(float));
			float* src_buff;
			if(sndf_buff == NULL){
				cout << "Error : Unable to allocate buffer for reading file " << name_t << " with path " << path << endl;
			}else{
				sf_readf_float(smpl_file, sndf_buff, lgth);

				/*resample if srate of plugin != srate of file*/ 
				if(s_rate != smpl_info.samplerate){
					double ratio = (double)smpl_info.samplerate / (double)s_rate ;
					uint32_t src_lgth = lgth * ((double)s_rate / (double)smpl_info.samplerate);
					src_lgth++; /*rounding up*/
					src_buff = new float[ src_lgth * channels ];
					memset(src_buff, 0, (src_lgth * channels) * sizeof(float));
					/*run samplerate lib*/
					SRC_DATA src_conf;
					src_conf.data_in = sndf_buff;
					src_conf.input_frames = lgth;
					src_conf.data_out = src_buff;
					src_conf.output_frames = src_lgth;
					src_conf.src_ratio = ratio;
					int error = src_simple(&src_conf, SRC_SINC_FASTEST, channels);

					if(error == 0){
						lgth = src_lgth;
						delete sndf_buff;
						sndf_buff = src_buff;
					}else{
						cout << " Samplerate change failed  : " << src_strerror(error) << endl;
					}
				}

				/*allocate data*/		
				data = new float*[channels];
				if(data != NULL){
                			for(uint8_t i = 0; i < channels; i++){
						data[i] = new float[lgth];
						memset(data[i], 0, lgth * sizeof(float));
					}
				}

	
				/*deinterleave data*/
				uint32_t k = 0;
				for(uint32_t j = 0; j < lgth; j++){
					for(uint8_t i = 0; i < channels; i++){
						data[i][j] += sndf_buff[k+i];	
					}
					k += channels;
				}
				delete sndf_buff;
				data_loaded = true;
			}
			sf_close(smpl_file);
		}/*else{
			cout << "ERROR : unable to load audio file " << name_t << " with path : " << path << endl;
		}*/
	}else{
		cout << "ERROR : unable to load audio file " << name_t << " with path : " << path << endl;
	}

}

sample::sample(uint32_t samplerate, vector<string> path, string name_t, bool load_data /*=true*/)
	:name(name_t),data(NULL)
	,lgth(0),channels(0),s_rate(samplerate),inc(0),velocity(1.0)
	,vol(1),panL(1),panR(1),mute(false)
	,data_loaded(false){

	cout << "sample_loading : " << name_t << endl;

	vector<string>::iterator it;
	//vector<sample*> smpl_list;
	//vector<sample*>::iterator s_it;
	vector<sample> smpl_list;
	vector<sample>::iterator s_it;

	/*creating samples*/	
	sample* tmp_smpl;
	for( it = path.begin(); it != path.end(); it++){
		tmp_smpl = new sample(samplerate, *it, "no_name", load_data);
		smpl_list.push_back(*tmp_smpl);
	}

	/*getting the biggest parameter of the samples*/
	for(s_it = smpl_list.begin(); s_it != smpl_list.end(); s_it++){
		if(s_it->get_chan() > channels){
			channels = s_it->get_chan();
		}
		if(s_it->get_lgth() > lgth){
			lgth = s_it->get_lgth();
		}

	}

	/* allocate data*/
	if(load_data){
		data = new float*[channels];
		if(data != NULL){
                	for(uint8_t i = 0; i < channels; i++){
				data[i] = new float[lgth];
				memset(data[i], 0, lgth * sizeof(float));
			}
		}

	/*merge sample*/
		for(s_it = smpl_list.begin(); s_it != smpl_list.end(); s_it++){
			s_it->merge(data);
		}
		data_loaded = true;
	}

	/*clean sample*/
/*	for(s_it = smpl_list.begin(); s_it != smpl_list.end(); s_it++){
		smpl_list.erase(s_it);
	}

	s_it = smpl_list.begin();
	while(s_it != smpl_list.end()){
		smpl_list.erase(s_it);
	}

	*/


}

sample::~sample(void){

	if(data_loaded){
		for(uint8_t i = 0; i < channels; i++){
			delete[] data[i];
		}
		delete[] data;
	}
}

string sample::Name(void){
	return name;
}

void sample::note_on(){
	/* +1 for decrement before returning value*/
#ifdef debug
	cout << "note on lgth #" << lgth << endl;
#endif
	inc = lgth + 1;
}

void sample::note_on(uint8_t vel){
        /* +1 for decrement before returning value*/
#ifdef debug
        cout << "note on lgth #" << lgth << endl;
#endif
        inc = lgth + 1;
	velocity = (float)vel/127.0;
}

void sample::note_off(){
        inc = 0;
}

float sample::get_val_L(void){
	if((data_loaded)&&(!mute)){
		if(inc == 0){
			return 0;
		}else{
			inc--;
			return velocity * vol * panL * data[0][lgth - inc];
		}
	}else{
		if(inc != 0) inc--;
		return 0;
	}
}

float sample::get_val_R(void){
	if((data_loaded)&&(!mute)){
		if(inc == 0){
			return 0;
		}else{
			inc--;
			if( channels == 2){
				return velocity * vol * panR * data[1][lgth - inc];
			}else{
				return velocity * vol * panR * data[0][lgth - inc];
			}
		}
	}else{
		if(inc != 0) inc--;
		return 0;
	}
}

uint32_t sample::get_lgth(void){
	return lgth;
}

uint32_t sample::get_chan(void){
	return channels;
}

void sample::merge(float** out){

	for(uint32_t j = 0; j < lgth; j++){
		for(uint8_t i = 0; i < channels; i++){
			out[i][j] += data[i][j];
		}
	}
	
}


void sample::set_vol(float vol_t){
	if(vol_t != vol){
		if((vol_t >= 0.0)&&(vol_t <= 1.0)){
			vol = vol_t;
		}
	}
}

void sample::set_pan(float pan_t){
	if(pan_t > 0.5){
		panL = (1.0 - pan_t) * 2.0;
		panR = 1.0;
	}else{
		panL = 1.0;
		panR = pan_t * 2.0;
	}
}

void sample::set_pan(float panL_t, float panR_t){
	if((panL_t >= 0.0)&&(panL_t <= 1.0)){
		panL = panL_t;
	}

	if((panR_t >= 0.0)&&(panR_t <= 1.0)){
		panR = panR_t;
	}
}

float sample::Vol(void){
	return vol;
}

float sample::Pan(void){
	//return panR/panL;
	if(panL >= 1.0){
		return panR / 2.0;
	}else{
		return 1.0 - panL / 2.0;
	}
}

float sample::Mute(void){
	if(mute)
		return 1.0;
	else
		return 0.0;
}

void sample::set_mute(float val){
	if((mute)&&(val == 0.0)){
		mute = false;
	}
	
	if((!mute)&&(val != 0.0)){
		mute = true;
	}
}
