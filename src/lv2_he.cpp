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
		//drumkit_select = (float*)data;
		g_he_param.dk_nb = (float*)data;
		break;
	case 4:
                g_he_param.mute[0] = (float*)data;
                break;
        case 5:
                g_he_param.vol[0] = (float*)data;
                break;
	case 6:
                g_he_param.pan[0] = (float*)data;
                break;
	case 7:
                g_he_param.mute[1] = (float*)data;
                break;
        case 8:
                g_he_param.vol[1] = (float*)data;
                break;
        case 9:
                g_he_param.pan[1] = (float*)data;
                break;
        case 10:
                g_he_param.mute[2] = (float*)data;
                break;
        case 11:
                g_he_param.vol[2] = (float*)data;
                break;
        case 12:
                g_he_param.pan[2] = (float*)data;
                break;
        case 13:
                g_he_param.mute[3] = (float*)data;
                break;
        case 14:
                g_he_param.vol[3] = (float*)data;
                break;
        case 15:
                g_he_param.pan[3] = (float*)data;
                break;
        case 16:
                g_he_param.mute[4] = (float*)data;
                break;
        case 17:
                g_he_param.vol[4] = (float*)data;
                break;
        case 18:
                g_he_param.pan[4] = (float*)data;
                break;
        case 19:
                g_he_param.mute[5] = (float*)data;
                break;
        case 20:
                g_he_param.vol[5] = (float*)data;
                break;
        case 21:
                g_he_param.pan[5] = (float*)data;
                break;
        case 22:
                g_he_param.mute[6] = (float*)data;
                break;
        case 23:
                g_he_param.vol[6] = (float*)data;
                break;
        case 24:
                g_he_param.pan[6] = (float*)data;
                break;
        case 25:
                g_he_param.mute[7] = (float*)data;
                break;
        case 26:
                g_he_param.vol[7] = (float*)data;
                break;
        case 27:
                g_he_param.pan[7] = (float*)data;
                break;
        case 28:
                g_he_param.mute[8] = (float*)data;
                break;
        case 29:
                g_he_param.vol[8] = (float*)data;
                break;
        case 30:
                g_he_param.pan[8] = (float*)data;
                break;
        case 31:
                g_he_param.mute[9] = (float*)data;
                break;
        case 32:
                g_he_param.vol[9] = (float*)data;
                break;
        case 33:
                g_he_param.pan[9] = (float*)data;
                break;
        case 34:
                g_he_param.mute[10] = (float*)data;
                break;
        case 35:
                g_he_param.vol[10] = (float*)data;
                break;
        case 36:
                g_he_param.pan[10] = (float*)data;
                break;
        case 37:
                g_he_param.mute[11] = (float*)data;
                break;
        case 38:
                g_he_param.vol[11] = (float*)data;
                break;
        case 39:
                g_he_param.pan[11] = (float*)data;
                break;
        case 40:
                g_he_param.mute[12] = (float*)data;
                break;
        case 41:
                g_he_param.vol[12] = (float*)data;
                break;
        case 42:
                g_he_param.pan[12] = (float*)data;
                break;
        case 43:
                g_he_param.mute[13] = (float*)data;
                break;
        case 44:
                g_he_param.vol[13] = (float*)data;
                break;
        case 45:
                g_he_param.pan[13] = (float*)data;
                break;
        case 46:
                g_he_param.mute[14] = (float*)data;
                break;
        case 47:
                g_he_param.vol[14] = (float*)data;
                break;
        case 48:
                g_he_param.pan[14] = (float*)data;
                break;
        case 49:
                g_he_param.mute[15] = (float*)data;
                break;
        case 50:
                g_he_param.vol[15] = (float*)data;
                break;
        case 51:
                g_he_param.pan[15] = (float*)data;
                break;
        case 52:
                g_he_param.mute[16] = (float*)data;
                break;
        case 53:
                g_he_param.vol[16] = (float*)data;
                break;
        case 54:
                g_he_param.pan[16] = (float*)data;
                break;
        case 55:
                g_he_param.mute[17] = (float*)data;
                break;
        case 56:
                g_he_param.vol[17] = (float*)data;
                break;
        case 57:
                g_he_param.pan[17] = (float*)data;
                break;
        case 58:
                g_he_param.mute[18] = (float*)data;
                break;
        case 59:
                g_he_param.vol[18] = (float*)data;
                break;
        case 60:
                g_he_param.pan[18] = (float*)data;
                break;
        case 61:
                g_he_param.mute[19] = (float*)data;
                break;
        case 62:
                g_he_param.vol[19] = (float*)data;
                break;
        case 63:
                g_he_param.pan[19] = (float*)data;
                break;
        case 64:
                g_he_param.mute[20] = (float*)data;
                break;
        case 65:
                g_he_param.vol[20] = (float*)data;
                break;
        case 66:
                g_he_param.pan[20] = (float*)data;
                break;
        case 67:
                g_he_param.mute[21] = (float*)data;
                break;
        case 68:
                g_he_param.vol[21] = (float*)data;
                break;
        case 69:
                g_he_param.pan[21] = (float*)data;
                break;
        case 70:
                g_he_param.mute[22] = (float*)data;
                break;
        case 71:
                g_he_param.vol[22] = (float*)data;
                break;
        case 72:
                g_he_param.pan[22] = (float*)data;
                break;
        case 73:
                g_he_param.mute[23] = (float*)data;
                break;
        case 74:
                g_he_param.vol[23] = (float*)data;
                break;
        case 75:
                g_he_param.pan[23] = (float*)data;
                break;
        case 76:
                g_he_param.mute[24] = (float*)data;
                break;
        case 77:
                g_he_param.vol[24] = (float*)data;
                break;
        case 78:
                g_he_param.pan[24] = (float*)data;
                break;
        case 79:
                g_he_param.mute[25] = (float*)data;
                break;
        case 80:
                g_he_param.vol[25] = (float*)data;
                break;
        case 81:
                g_he_param.pan[25] = (float*)data;
                break;
        case 82:
                g_he_param.mute[26] = (float*)data;
                break;
        case 83:
                g_he_param.vol[26] = (float*)data;
                break;
        case 84:
                g_he_param.pan[26] = (float*)data;
                break;
        case 85:
                g_he_param.mute[27] = (float*)data;
                break;
        case 86:
                g_he_param.vol[27] = (float*)data;
                break;
        case 87:
                g_he_param.pan[27] = (float*)data;
                break;
        case 88:
                g_he_param.mute[28] = (float*)data;
                break;
        case 89:
                g_he_param.vol[28] = (float*)data;
                break;
        case 90:
                g_he_param.pan[28] = (float*)data;
                break;
        case 91:
                g_he_param.mute[29] = (float*)data;
                break;
        case 92:
                g_he_param.vol[29] = (float*)data;
                break;
        case 93:
                g_he_param.pan[29] = (float*)data;
                break;
        case 94:
                g_he_param.mute[30] = (float*)data;
                break;
        case 95:
                g_he_param.vol[30] = (float*)data;
                break;
        case 96:
                g_he_param.pan[30] = (float*)data;
                break;
        case 97:
                g_he_param.mute[31] = (float*)data;
                break;
        case 98:
                g_he_param.vol[31] = (float*)data;
                break;
        case 99:
                g_he_param.pan[31] = (float*)data;
                break;

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
