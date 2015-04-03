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

#ifndef HEGUI_H
#define HEGUI_H

#include <QWidget>
#include <QComboBox>
#include <QLayout>
#include <QScrollArea>
#include <QPushButton>

#include <qwt/qwt_knob.h>
#include <qwt/qwt_slider.h>

#include "hydro_dk_scan.hpp"

#ifdef LV2_GUI
	#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#endif

#define MUTE_SHIFT 7
#define VOL_SHIFT  8
#define PAN_SHIFT  9

class DrumKit : public plugin_iface
{
private:
	sample* dk[H_NB_INST];

public:
	DrumKit();
	~DrumKit();
	void add_sample(int id, sample *smpl);
	uint32_t get_srate();

	QString* get_name(int id);
	float get_vol(int id);
	float get_pan(int id);
	float get_mute(int id);

	void clean(void);
};


class HeGui : public QScrollArea
{
  Q_OBJECT

private:
  hydro_dk_scan* scanner;
  DrumKit dk;

  QStringList qlist;

  QWidget 		*wd;
  QHBoxLayout	*lay;
  QVBoxLayout	*vlay[H_NB_INST];
  QComboBox		dk_list;
  QFrame		*qfl;
  QFrame		*qf[H_NB_INST];
  QwtSlider*	vol_knob_tab[H_NB_INST];
  QwtKnob*		pan_knob_tab[H_NB_INST];
  QPushButton*	mute_butt_tab[H_NB_INST];
  bool 			mute_state[H_NB_INST];

  void display_drumkit();

#ifdef LV2_GUI
  LV2UI_Controller lv2_ctrl;
  LV2UI_Write_Function lv2_write;
#endif


public:
  HeGui(QWidget *parent = 0);
  ~HeGui();

#ifdef LV2_GUI
  void set_lv2_ctrl(LV2UI_Controller lc);
  void set_lv2_write_fn(LV2UI_Write_Function lw);
  void lv2_send_data(uint32_t index, double data);
#endif


public slots:
  void dk_change(int index);
  void send_data(double data);
  void send_data(uint32_t index, double data);
  void mute_click(void);
};

#endif // HEGUI_H
