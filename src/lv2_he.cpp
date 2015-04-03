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


#include <cstdlib>
#include <cstring>
#include <cmath>

#include "lv2.h"

#include "he.hpp"

#define HE_URI       "http://github.com/klem23/helium";

LV2_Descriptor *HeDesc = NULL;
float* drumkit_select;
he_param g_he_param;

void cleanupHe(LV2_Handle instance)
{
	He *plugin = (He *)instance;
	delete plugin;
	
	if(HeDesc != NULL){
		//free(HeDesc);
	}
}

void connectPortHe(LV2_Handle instance, uint32_t port,
			   void *data)
{
	He *plugin = (He *)instance;

	if(port < 7){
		switch (port) {
			case 0:
				plugin->set_input((float*)data);
				break;
			case 1:
				plugin->set_output_L((float*)data);
				break;
			case 2:
				plugin->set_output_R((float*)data);
				break;
			case 3:
				g_he_param.dk_nb = (float*)data;
				break;
			case 4:
				g_he_param.use_note_off = (float*)data;
				break;
			case 5:
				g_he_param.use_velocity = (float*)data;
				break;
			case 6:
				g_he_param.key_shift = (float*)data;
				break;
			default:
				break;
		}
	}else{
		uint8_t ctrl_type = (port - 7) % 3;
		switch(ctrl_type){
			case 0:
				g_he_param.mute[(port - 7)/ 3 ] = (float*)data;
				break;
			case 1:
				g_he_param.vol[(port - 7)/ 3 ] = (float*)data;
				break;
			case 2:
				g_he_param.pan[(port - 7)/ 3 ] = (float*)data;
				break;
			default:
				break;
		}
	}
}

LV2_Handle instantiateHe(const LV2_Descriptor *descriptor,
	    double s_rate, const char *path,
	    const LV2_Feature * const* features)
{
	/*plugin init*/
	He *plugin = new He(s_rate);
	return (LV2_Handle)plugin;
}


void runHe(LV2_Handle instance, uint32_t sample_count)
{
	He *plugin = (He *)instance;
	plugin->check_param(&g_he_param);
	plugin->process(sample_count);


//	if( plugin->loading == true){
//		return;
//	}

}
/*
const void* he_ext(const char * URI){
	if (strcmp(URI, LV2DYNPARAM_URI) == 0){
	//	return he_dynparam_init();
		return get_lv2dynparam_plugin_extension_data();
	}
	return NULL;
}
*/
void init()
{
	HeDesc =
	 (LV2_Descriptor *)malloc(sizeof(LV2_Descriptor));

	HeDesc->URI = HE_URI;
	HeDesc->activate = NULL;
	HeDesc->cleanup = cleanupHe;
	HeDesc->connect_port = connectPortHe;
	HeDesc->deactivate = NULL;
	HeDesc->instantiate = instantiateHe;
	HeDesc->run = runHe;
//	HeDesc->extension_data = he_ext;
	HeDesc->extension_data = NULL;
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
	if (!HeDesc) init();

	switch (index) {
	case 0:
		return HeDesc;
	default:
		return NULL;
	}

}
