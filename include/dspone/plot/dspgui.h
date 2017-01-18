/*
* dspgui.h
* Copyright 2017 (c) Jordi Adell
* Created on: 2017
* 	Author: Jordi Adell - adellj@gmail.com
*
* This file is part of DSPONE
*
* DSPONE is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DSPONE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DSPONE.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DSP_GUI_H__
#define __DSP_GUI_H__

#include <memory>

class QApplication;

namespace dsp
{

class DspPlot;
class ShortTimeProcess;

class DspGui
{
    public:
	typedef void thread_processing_run_t(ShortTimeProcess *stp);
	DspGui(ShortTimeProcess *process, thread_processing_run_t *f);

	void start();

    private:
	std::shared_ptr<QApplication> qt_app_;
	std::shared_ptr<DspPlot> plot_;
};

}

#endif
