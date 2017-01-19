/*
* QtDebug.h
* Copyright 2016 (c) Jordi Adell
* Created on: 2015
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
#ifndef __DSP_QTDEBUG_H__
#define __DSP_QTDEBUG_H__


#ifdef QT_FOUND
#define QT_DEBUG
#include <qt4/Qt/qobject.h>
#endif

#include <boost/shared_array.hpp>

namespace dsp
{

class QtDebug
	#ifdef QT_DEBUG
	: public QObject
	#endif
{
#ifdef QT_DEBUG
	Q_OBJECT
#endif
	typedef std::vector<double> PlotData;

    public:

	enum QtDebugSignal {IN_SIGNAL, IN_FRAME, IN_ANALYSIS, OUT_ANALYSIS, OUT_FRAME, OUT_SIGNAL};

	QtDebug();
	virtual ~QtDebug();

	template<typename T> void plot(const std::vector<T*> &data, int length, QtDebugSignal signal);
	template<typename T> void plot(const T* data, int length, QtDebugSignal signal);
	template<typename T> void plot(const std::vector<boost::shared_array<T> > &data, int length, QtDebugSignal signal);

#ifdef QT_DEBUG
    signals:

	void plot_input_signal(std::vector<double> data);
	void plot_output_signal(std::vector<double> data);
	void plot_input_frame(std::vector<double> data);
	void plot_output_frame(std::vector<double> data);
	void plot_input_analysis(std::vector<double> data);
	void plot_output_analysis(std::vector<double> data);
#endif

    private:

	void send_signal(const PlotData &data, QtDebugSignal);

};


template<typename T>
void QtDebug::plot(const std::vector<T*> &data, int length, QtDebugSignal signal)
{
    if (data.size() > 0)
    {
	PlotData tmp(length);
	for (int i = 0; i < length; ++i)
	    tmp[i] = data[0][i];
	send_signal(tmp, signal);
    }
}


template<typename T>
void QtDebug::plot(const T* data, int length, QtDebugSignal signal)
{
    PlotData tmp(length);
    for (int i = 0; i < length; ++i)
	tmp[i] = data[i];
    send_signal(tmp, signal);
}

template<typename T>
void QtDebug::plot(const std::vector<boost::shared_array<T> > &data, int length, QtDebugSignal signal)
{
    if (data.size() > 0)
    {
	PlotData tmp(length);
	for (int i = 0; i < length; ++i)
	    tmp[i] = data[0][i];
	send_signal(tmp, signal);
    }
}

}


#endif
