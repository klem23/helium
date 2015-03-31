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
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"

#include "hegui.hpp"


#define HEUI_URI "http://github.com/klem23/helium#ui";


using namespace std;

LV2UI_Descriptor *heUIDesc = NULL;

LV2UI_Handle gui_instantiate(const struct _LV2UI_Descriptor* descriptor,
                          const char*                     plugin_uri,
                          const char*                     bundle_path,
                          LV2UI_Write_Function            write_function,
                          LV2UI_Controller                controller,
                          LV2UI_Widget*                   widget,
                          const LV2_Feature* const*       features)
{

	HeGui* hg = new HeGui();

	hg->set_lv2_ctrl(controller);
	hg->set_lv2_write_fn(write_function);
	*widget = hg;

	return (LV2UI_Handle)hg;
}

void gui_cleanup(LV2UI_Handle handle)
{
	HeGui* hg = (HeGui*)handle;
	delete hg;
}

void gui_port_event(LV2UI_Handle handle, uint32_t port, uint32_t buffer_size, uint32_t format, const void *buffer)
{

}

const void *gui_extension(const char *uri)
{
    return NULL;
}

void init(void){
    heUIDesc = (LV2UI_Descriptor*)malloc(sizeof(LV2UI_Descriptor));

    heUIDesc->URI = HEUI_URI;
    heUIDesc->instantiate = gui_instantiate;
    heUIDesc->cleanup = gui_cleanup;
    heUIDesc->port_event = gui_port_event;
    heUIDesc->extension_data = gui_extension;
}


const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	if(!heUIDesc) init();

	switch(index){
		case 0:
			return heUIDesc;
		default:
			return NULL;
	}
}


