/*
* dspplotQwt.h
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

#ifndef __DSPPLOTQWT_H__
#define __DSPPLOTQWT_H__

#include <dspone/rt/ShortTimeProcess.h>
#include <dspone/rt/ShortTimeProcessImpl.h>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_global.h>
#include <qgridlayout.h>
#include <qelapsedtimer.h>


#include <memory>
#include <thread>
#include <iostream>

namespace dsp
{



class DspPlot : public QObject
{
	Q_OBJECT
    public:
	typedef void thread_processing_run_t(ShortTimeProcess *stp);
	DspPlot(thread_processing_run_t *f, ShortTimeProcess *stp);
	virtual ~DspPlot();



    private:
	void init();
	void signal_process();
	void initQwtCurve(QwtPlotCurve &qwtc);
	void initQwtPlot(QwtPlot &qwtPlot);
	void plot(std::vector<double> signal, QwtPlotCurve &qwtc);
	void plot(std::vector<double*> signal,
		  int length,
		  QwtPlotCurve &qwtc,
		  std::shared_ptr<double> &x,
		  std::shared_ptr<double> &y);

    public slots:
	void plot_input_analysis(std::vector<double> signal);
	void plot_output_analysis(std::vector<double> signal);
	void plot_input_signal(std::vector<double> signal);
	void plot_output_signal(std::vector<double> signal);

    private:

	void config();
	std::unique_ptr<std::thread> qt_thread_;
	std::unique_ptr<std::thread> signal_process_thread_;
	QwtPlotCurve qwtc_in_signal_;
	QwtPlotCurve qwtc_out_signal_;
	QwtPlotCurve qwtc_in_anal_;
	QwtPlotCurve qwtc_out_anal_;

	std::string qt_title_;

	std::shared_ptr<double> x_in_signal_;
	std::shared_ptr<double> y_in_signal_;
	std::shared_ptr<double> x_out_signal_;
	std::shared_ptr<double> y_out_signal_;
	std::shared_ptr<double> x_in_anal_;
	std::shared_ptr<double> y_in_anal_;
	std::shared_ptr<double> x_out_anal_;
	std::shared_ptr<double> y_out_anal_;




	int length_;
	const QObject *dspplot_;
	ShortTimeProcess *process_;
	thread_processing_run_t *processing_run;
	std::shared_ptr<QWidget> widget_;
	std::shared_ptr<QGridLayout> layout_;
	QwtPlot qwtPlot_in_signal_;
	QwtPlot qwtPlot_out_signal_;
	QwtPlot qwtPlot_in_anal_;
	QwtPlot qwtPlot_out_anal_;
	QElapsedTimer timer_;

};

}

#endif // DSPPLOTQWT_H
