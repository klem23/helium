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
#include <dirent.h>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include "xml_parser.hpp"
#include "he.hpp"

#include "lv2/lv2plug.in/ns/ext/atom/util.h"

using namespace std;
using namespace xercesc;


He::He(uint32_t sampling_rate)
:midi_in(NULL),audio_out_L(NULL),audio_out_R(NULL)
,s_rate(sampling_rate),main_vol(1)
,loading(false),actual_dk(2)
,use_note_off(false),use_velocity(false),key_shift(48){

	for(uint32_t i = 0; i < H_NB_INST; i++){
		drumkit[i] = NULL;
	}

	hds = new hydro_dk_scan(true);
	hds->load_drumkit(actual_dk, (plugin_iface*)this);
}

He::~He(void){

	/*clean previous drumkit*/
	for(uint8_t i = 0; i < H_NB_INST; i++){
		delete drumkit[i];
		drumkit[i] = NULL;
	}
}

void He::check_param(he_param* hp){
/*	if(((uint32_t)*He_param != actual_dk)&&(*He_param < dk_list.size())){
		actual_dk = (uint32_t)*He_param;
		load_drumkit(H_PATH_0 + dk_list.at(actual_dk));
	}*/

	if((uint32_t)*hp->dk_nb != actual_dk){

		loading = true;

		/*clean previous drumkit*/
		for(uint8_t i = 0; i < H_NB_INST; i++){
			delete drumkit[i];
			drumkit[i] = NULL;
		}


		actual_dk = (uint32_t)*hp->dk_nb;
		hds->load_drumkit(actual_dk, (plugin_iface*)this);
		loading = false;
	}

	/*
	if((use_note_off)&&(*hp->use_note_off <= 0)){
		use_note_off = false;
	}else if((!use_note_off)&&(*hp->use_note_off > 0)){
		use_note_off = true;
	}

	if((use_velocity)&&(*hp->use_velocity <= 0)){
		use_velocity = false;
	}else if((!use_velocity)&&(*hp->use_velocity > 0)){
		use_velocity = true;
	}

	if(key_shift != (uint8_t)*hp->key_shift){
		key_shift = hp->key_shift 
	}

	for(uint8_t i = 0; i < H_NB_INST; i++){
		if(drumkit[i] != NULL){
			drumkit[i]->set_vol(*hp->vol[i]);
			drumkit[i]->set_pan(*hp->pan[i]);
			drumkit[i]->set_mute(*hp->mute[i]);
		}
	}*/ 

}

void He::add_sample(int id, sample* smpl){
	if((smpl != NULL)&&(id < H_NB_INST)){
		drumkit[id] = smpl;
	} 
}

uint32_t He::get_srate(void){
	return s_rate;
}

void He::set_input(float* ptr){
	midi_in = ptr;
}

void He::set_output_L(float* ptr){
	audio_out_L = ptr;
}

void He::set_output_R(float* ptr){
	audio_out_R = ptr;
}
/*
void He::process_2(int nb_sample){

	uint32_t event_count = 0;
	LV2_Event *event;
	uint8_t *events, *data, midi_type, midi_note;

	/*midi preprocess*//*
	LV2_Event_Buffer* m_in = (LV2_Event_Buffer*)midi_in;
	events = (uint8_t*) m_in->data;

	for(uint32_t pos = 0; pos < nb_sample; pos++){
		audio_out_L[pos] = 0;
		audio_out_R[pos] = 0;
		
		/*midi parsing*//*
		if(m_in->event_count > event_count){
 			event = (LV2_Event*)events;
			if(pos >= event->frames){
				data = events + sizeof(LV2_Event);
				midi_type = data[0];
				/*note on*//*
				if((midi_type & 0xF0) == 0x90){
					//midi_note = data[1] - 48;
					if(data[1] > 48){
						midi_note = data[1] - 48;
					}else{
						midi_note = 0;
					}
					printf("c midi note %u \n", midi_note); 
					cout << "midi_note " << midi_note << endl;
					if((drumkit[midi_note] != NULL)&&(midi_note < H_NB_INST)){
						drumkit[midi_note]->note_on();
					}
				}
				/*moving pointer to next event*//*
				event_count++;
				/* 64 bit padding*//*
				events += (sizeof(LV2_Event) + event->size + 7) & ~7;
			}
		}

		/*audio playback*//*
		for(uint32_t i = 0; i < H_NB_INST; i++){
			if(drumkit[i] != NULL){
				audio_out_L[pos] += drumkit[i]->get_val_L();
				audio_out_R[pos] += drumkit[i]->get_val_R();
			}
		}
	}
}
*/

void He::process(int nb_sample){

	uint8_t midi_note, *data, status_byte;
	/*midi input parsing v2*/
	LV2_Atom_Sequence* m_in = (LV2_Atom_Sequence*)midi_in;
	LV2_Atom_Event* ev = lv2_atom_sequence_begin(&m_in->body); 


	for(uint32_t pos = 0; pos < nb_sample; pos++){
		audio_out_L[pos] = 0;
		audio_out_R[pos] = 0;
		if(!loading){
			if(!lv2_atom_sequence_is_end(&m_in->body, m_in->atom.size, ev)){
				//		if(ev->body.type == midi_Event){

				if(ev->time.frames <= pos){
					data = (uint8_t*)LV2_ATOM_BODY(&ev->body);

					status_byte = data[0] & 0xF0;
					if( status_byte == 0x90){
						//midi_note = data[1] - 48;
						if(data[1] > key_shift){
							midi_note = data[1] - key_shift;
						}else{
							midi_note = 0;
						}
						if((midi_note < H_NB_INST)&&(drumkit[midi_note] != NULL)){
							if(use_velocity){
								drumkit[midi_note]->note_on(data[2]);
							}else{
								drumkit[midi_note]->note_on();
							}
						}
					}else if((use_note_off)&&(status_byte == 0x80)){
						if(data[1] > key_shift){
							midi_note = data[1] - key_shift;
						}else{
							midi_note = 0;
						} 
						if((midi_note < H_NB_INST)&&(drumkit[midi_note] != NULL)){
							drumkit[midi_note]->note_off();
						}
					}
					ev = lv2_atom_sequence_next(ev);
				}
				//}
			}


			/*audio playback*/
			for(uint32_t i = 0; i < H_NB_INST; i++){
				if(drumkit[i] != NULL){
					audio_out_L[pos] += drumkit[i]->get_val_L();
					audio_out_R[pos] += drumkit[i]->get_val_R();
				}
			}
		}
	}
}
