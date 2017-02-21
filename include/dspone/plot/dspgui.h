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
#include <functional>

class QApplication;

namespace dsp
{

class DspPlot;
class ShortTimeProcess;

class DspGui
{
    public:
	typedef void thread_processing_run_t_(ShortTimeProcess &stp);
	typedef std::function<void(ShortTimeProcess &stp)> thread_processing_run_t;

	DspGui(ShortTimeProcess *process, thread_processing_run_t_ *f);
	DspGui(ShortTimeProcess &process, thread_processing_run_t_ *f);
	DspGui(ShortTimeProcess *process, thread_processing_run_t &f);
	DspGui(ShortTimeProcess &process, thread_processing_run_t &f);

	virtual ~DspGui();

	void start(int channel=0);

    private:
	void init(ShortTimeProcess &process, thread_processing_run_t &f);
	void init(ShortTimeProcess &process, thread_processing_run_t_ *f);
	thread_processing_run_t process_f_;
	ShortTimeProcess *process_;
};

}

#endif
