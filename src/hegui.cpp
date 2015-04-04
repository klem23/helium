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

#include <QLabel>
#include <QFrame>
#include <QCheckBox>
#include <QSpinBox>
#include <iostream>

#include "hegui.hpp"

DrumKit::DrumKit(){
  /*clean previous drumkit*/
  for(uint8_t i = 0; i < H_NB_INST; i++){
    dk[i] = NULL;
  }
}

DrumKit::~DrumKit(){
	/*clean previous drumkit*/
	for(uint8_t i = 0; i < H_NB_INST; i++){
		delete dk[i];
		dk[i] = NULL;
	}
}

void DrumKit::add_sample(int id, sample *smpl){
	if(smpl != NULL){
		dk[id] = smpl;
	}
}

uint32_t DrumKit::get_srate(){
  return 48000;
}

void DrumKit::clean(){
	for(int i = 0; i < H_NB_INST; i++){
		delete dk[i];
		dk[i] = NULL;
	}
}

QString* DrumKit::get_name(int id){
  if(dk[id] != NULL){
    return new QString(dk[id]->Name().c_str());
  }else{
    return NULL;
  }
}


float DrumKit::get_vol(int id){
  if(dk[id] != NULL){
	return dk[id]->Vol();
  }else{
	return 0.0;
  }
}

float DrumKit::get_pan(int id){
  if(dk[id] != NULL){
	return dk[id]->Pan();
  }else{
	return 0.0;
  }
}

float DrumKit::get_mute(int id){
  if(dk[id] != NULL){
	return dk[id]->Mute();
  }else{
	return 0.0;
  }
}

HeGui::HeGui(QWidget *parent)
	:QScrollArea(parent)
	,scanner(new hydro_dk_scan(false))
	,lay(new QHBoxLayout()){

	for(uint32_t i = 0; i < scanner->get_dk_nb(); i++){
		QString qs(scanner->get_name(i).c_str());
		qlist.push_back(qs);
	}

	QVBoxLayout *vlay_cmd = new QVBoxLayout();
	/* drumkit list*/
	dk_list.addItems(qlist);
	dk_list.setFixedWidth(200);
	vlay_cmd->addWidget(&dk_list);

	/* use note off midi message*/
	QCheckBox* qcb_note_off = new QCheckBox("Use Note Off");
	vlay_cmd->addWidget(qcb_note_off);
	/* use note on velocity midi message*/
	QCheckBox* qcb_note_velocity = new QCheckBox("Use Velocity");
	vlay_cmd->addWidget(qcb_note_velocity);

	/*keyboard key shift for first sample*/
	QSpinBox* qsb_keyshift = new QSpinBox();
	qsb_keyshift->setRange(0, 72);
	qsb_keyshift->setValue(48);

	vlay_cmd->addWidget(qsb_keyshift);

	qfl_cmd = new QFrame();
	qfl_cmd->setLayout(vlay_cmd);
	lay->addWidget(qfl_cmd);

	QVBoxLayout *vlay = new QVBoxLayout();
	QLabel *lab_pan = new QLabel("Pan");
	QLabel *lab_vol = new QLabel("Vol");
	QLabel *lab_mute = new QLabel("Mute");
	vlay->addWidget(lab_pan);
	vlay->addWidget(lab_vol);
	vlay->addWidget(lab_mute);
	qfl = new QFrame();
	qfl->setFrameShape(QFrame::Box);
	qfl->setLayout(vlay);

	//lay->addLayout(vlay);
	lay->addWidget(qfl);

	/*load default drumkit*/
	scanner->load_drumkit(0, (plugin_iface*)&dk);

	for(int i = 0; i < H_NB_INST; i++){
		qf[i] = NULL;
		mute_state[i] = false;
	}

	display_drumkit();

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	setFixedHeight(400);

	connect(&dk_list, SIGNAL(currentIndexChanged(int)), this, SLOT(dk_change(int)));
	connect(qcb_note_off, SIGNAL(stateChanged(int)), this, SLOT(note_off_check(int)));
	connect(qcb_note_velocity, SIGNAL(stateChanged(int)), this, SLOT(velocity_check(int)));
	connect(qsb_keyshift, SIGNAL(valueChanged(int)), this, SLOT(key_shift_change(int)));

#ifdef LV2_GUI
  lv2_ctrl = NULL;
  lv2_write = NULL;
#endif

}

HeGui::~HeGui()
{
  for(int i = 0; i < H_NB_INST; i++){
      if(qf[i] != NULL){
          delete qf[i];
          qf[i] = NULL;
        }
    }
	delete lay;
}

void HeGui::display_drumkit(){

	for(int i = 0; i < H_NB_INST; i++){
		if(qf[i] != NULL){
			lay->removeWidget(qf[i]);

			delete qf[i];
			qf[i] = NULL;
		}
	}

	delete lay;
	lay = new QHBoxLayout();
  	lay->addWidget(qfl_cmd);
  	lay->addWidget(qfl);


	for(int i = 0; i < H_NB_INST; i++){
		if(dk.get_name(i) != NULL){
			//QVBoxLayout *
			vlay[i] = new QVBoxLayout();

			/*pan knob*/
			pan_knob_tab[i] = new QwtKnob();
			pan_knob_tab[i]->setKnobWidth(30);
			pan_knob_tab[i]->setScale(-1, 1);
			pan_knob_tab[i]->setValue(dk.get_pan(i)*10);
			// pan_knob_tab[i]->setValue(5);
			pan_knob_tab[i]->updateGeometry();
			send_data(i * 3 + PAN_SHIFT, dk.get_pan(i));
			vlay[i]->addWidget(pan_knob_tab[i]);


			/*vol knob*/
			vol_knob_tab[i] = new QwtSlider(NULL, Qt::Vertical);
			vol_knob_tab[i]->setScalePosition(QwtSlider::RightScale);
			vol_knob_tab[i]->setBackgroundStyle(QwtSlider::Groove);
			vol_knob_tab[i]->setScale(0, 10);
			vol_knob_tab[i]->setValue(dk.get_vol(i)*100.0);
			vol_knob_tab[i]->setFixedHeight(150);
			vol_knob_tab[i]->updateGeometry();
			send_data(i * 3 + VOL_SHIFT, dk.get_vol(i));
			vlay[i]->addWidget(vol_knob_tab[i]);

			/*mute button*/
			mute_butt_tab[i] = new QPushButton("Mute");
			if(dk.get_mute(i) > 0.0){
				mute_butt_tab[i]->setStyleSheet("background-color : red");
				mute_state[i] = true;
				send_data(i * 3 + MUTE_SHIFT, 1.1);
			}else{
				mute_butt_tab[i]->setStyleSheet("background-color : grey");
				mute_state[i] = false;
				send_data(i * 3 + MUTE_SHIFT, 0.0);
			}
			vlay[i]->addWidget(mute_butt_tab[i]);

			/*instrument name : kick, snare, cymbal etc*/
			QLabel* ql = new QLabel(*dk.get_name(i));
			ql->setAlignment(Qt::AlignCenter);
			ql->setFrameShape(QFrame::Box);
			ql->updateGeometry();
			vlay[i]->addWidget(ql);


			qf[i] = new QFrame();
			qf[i]->setLineWidth(3);
			qf[i]->setMidLineWidth(3);
			qf[i]->setFrameShape(QFrame::Box);
			qf[i]->setLayout(vlay[i]);

			//qf[i]->setFixedWidth(100);
			lay->addWidget(qf[i]);
			//this->window()->setFixedSize(800,300);
			//this->window()->updateGeometry();
			//updateGeometry();
			//lay.adjustSize();

			connect(vol_knob_tab[i], SIGNAL(valueChanged(double)), this, SLOT(send_data(double)));
			connect(pan_knob_tab[i], SIGNAL(valueChanged(double)), this, SLOT(send_data(double)));
			connect(mute_butt_tab[i], SIGNAL(clicked()), this, SLOT(mute_click()));

		}
	}
	//show();

	wd = new QWidget();
	wd->setLayout(lay);
	setWidget(wd);

}

void HeGui::dk_change(int index){

	send_data(3, index);

	dk.clean();
	scanner->load_drumkit(index, (plugin_iface*)&dk);
	display_drumkit();
	//  setFixedWidth(1600);
	//adjustSize();
	updateGeometry();

}

#ifdef LV2_GUI
void HeGui::set_lv2_ctrl(LV2UI_Controller lc){
	//if(lc != NULL){
	lv2_ctrl = lc;
	display_drumkit();
	//}
}

void HeGui::set_lv2_write_fn(LV2UI_Write_Function lw){
	//if(lw != NULL){
	lv2_write = lw;
	display_drumkit();
	//}
}

void HeGui::lv2_send_data(uint32_t index, double val){
	float value = val;
	if((lv2_write != NULL)&&(lv2_ctrl != NULL))
		lv2_write(lv2_ctrl, index, sizeof(float), 0, &value);
}

#endif

void HeGui::send_data(uint32_t index, double val){
#ifdef LV2_GUI
			lv2_send_data(index, val);
#endif
}

void HeGui::send_data(double val){
	QObject* src = QObject::sender();
	//QObject* src = sender();
	bool found = false;
	uint8_t i = 0;

	while((found == false)&&(i < H_NB_INST)){
		if(src == vol_knob_tab[i]){
			found = true;
			send_data(i * 3 + VOL_SHIFT, val / 100);
		}
		if(src == pan_knob_tab[i]){
			found = true;
			send_data(i * 3 + PAN_SHIFT, val/10);
		}
		i++;
	}
}

void HeGui::mute_click(void){
	QObject* src = QObject::sender();
	//QObject* src = sender();
	bool found = false;
	uint8_t i = 0;

	while((found == false)&&(i < H_NB_INST)){
		if(src == mute_butt_tab[i]){
			found = true;
			if(mute_state[i]){
				mute_state[i] = false;
				mute_butt_tab[i]->setStyleSheet("background-color : grey");
				send_data(i * 3 + MUTE_SHIFT, 0.0);
			}else{
				mute_state[i] = true;
				mute_butt_tab[i]->setStyleSheet("background-color : red");
				send_data(i * 3 + MUTE_SHIFT, 1.1);
			}
		}
		i++;
	}
}

void HeGui::note_off_check(int state){

	if(state == Qt::Checked){
		send_data(4, 1.1);
	}else{
		send_data(4, 0.0);
	}
}

void HeGui::velocity_check(int state){

	if(state == Qt::Checked){
		send_data(5, 1.1);
	}else{
		send_data(5, 0.0);
	}
}

void HeGui::key_shift_change(int val){
	double value = val;
	send_data(6, value);
}
