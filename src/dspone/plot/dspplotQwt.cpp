#include <dspone/plot/dspplotQwt.h>
#include <dspone/plot/qtDebug.h>

#include <qwt_sampling_thread.h>
#include <qwt_plot_layout.h>

#include <mutex>

Q_DECLARE_METATYPE(std::shared_ptr<double>)
Q_DECLARE_METATYPE(std::vector<double*>)


namespace dsp
{


DspPlot::DspPlot(thread_processing_run_t *f, ShortTimeProcess *stp) :
  processing_run(f),
  process_(stp)
{
  widget_.reset(new QWidget());
  layout_.reset(new QGridLayout(widget_.get()));
  timer_.start();
  init();
}

DspPlot::~DspPlot()
{
  if (signal_process_thread_->joinable())
    signal_process_thread_->join();
}

void DspPlot::init()
{
  config();

  if (!signal_process_thread_)
  {
    signal_process_thread_.reset(new std::thread(&DspPlot::signal_process, this));
    signal_process_thread_->detach();
  }

}


void DspPlot::plot_input_analysis(std::vector<double> signal)
{
  plot(signal, qwtc_in_anal_);
}

void DspPlot::plot_output_analysis(std::vector<double> signal)
{
  plot(signal, qwtc_out_anal_);
}

void DspPlot::plot_input_signal(std::vector<double> signal)
{
  plot(signal, qwtc_in_signal_);
}

void DspPlot::plot_output_signal(std::vector<double> signal)
{
  plot(signal, qwtc_out_signal_);
}

void DspPlot::plot(std::vector<double> signal, QwtPlotCurve &qwtc)
{
  QVector<double> y;
  QVector<double> x;

  y = QVector<double>::fromStdVector(signal);

  if (signal.size() > 0)
  {
    for (int i = 0; i < signal.size(); ++i)
    {
      x.push_back(i);
      }
  }
  qwtc.setSamples(x, y);

  if (timer_.hasExpired(75))
  {
    timer_.restart();
    qwtPlot_in_anal_.replot();
    qwtPlot_out_anal_.replot();
    qwtPlot_in_signal_.replot();
    qwtPlot_out_signal_.replot();
  }
}

void DspPlot::plot(std::vector<double*> signal,
		 int length,
		 QwtPlotCurve &qwtc,
		 std::shared_ptr<double> &x,
		 std::shared_ptr<double> &y)
{
  if (timer_.hasExpired(15))
  {
    std::shared_ptr<double> x_tmp(new double[length]);
    std::shared_ptr<double> y_tmp(new double[length]);

    if (signal.size() > 0)
    {
      for (int i = 0; i < length; ++i)
      {
	x_tmp.get()[i] = i;
	y_tmp.get()[i] = signal[0][i];
      }
    }

    qwtc.setRawSamples(x_tmp.get(), y_tmp.get(), length);

  x = x_tmp;
  y = y_tmp;

  length_ = length;

  timer_.restart();
  }
}


void DspPlot::initQwtCurve(QwtPlotCurve &qwtc)
{

}

void DspPlot::initQwtPlot(QwtPlot &qwtPlot)
{
  qwtPlot.setAutoReplot(false);
  qwtPlot.setCanvasBackground(QBrush(Qt::white));
  qwtPlot.resize(200,400);
  qwtPlot.setAutoFillBackground(true);
}

void DspPlot::config()
{

  qRegisterMetaType<std::shared_ptr<double> >("shared_ptr_double");
  qRegisterMetaType<std::vector<double*> >("vector_double_ptr");

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_input_signal(std::vector<double>)),
	  this,
	  SLOT(plot_input_signal(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_input_frame(std::vector<double>)),
	  this,
	  SLOT(plot_input_frame(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_input_analysis(std::vector<double>)),
	  this,
	  SLOT(plot_input_analysis(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_output_analysis(std::vector<double>)),
	  this,
	  SLOT(plot_output_analysis(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_output_frame(std::vector<double>)),
	  this,
	  SLOT(plot_output_frame(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  connect(&(process_->_impl.get()->_qtdebug),
	  SIGNAL(plot_output_signal(std::vector<double>)),
	  this,
	  SLOT(plot_output_signal(std::vector<double>)),
	  Qt::BlockingQueuedConnection);

  initQwtCurve(qwtc_in_anal_);
  initQwtCurve(qwtc_out_anal_);
  initQwtCurve(qwtc_in_signal_);
  initQwtCurve(qwtc_out_signal_);
  initQwtCurve(qwtc_in_frame_);
  initQwtCurve(qwtc_out_frame_);

  initQwtPlot(qwtPlot_in_frame_);
  initQwtPlot(qwtPlot_out_frame_);
  initQwtPlot(qwtPlot_in_anal_);
  initQwtPlot(qwtPlot_out_anal_);
  initQwtPlot(qwtPlot_in_signal_);
  initQwtPlot(qwtPlot_out_signal_);

  widget_->resize(1200, 600);

  qwtc_in_anal_.attach(&qwtPlot_in_anal_);
  qwtc_out_anal_.attach(&qwtPlot_out_anal_);
  qwtc_in_signal_.attach(&qwtPlot_in_signal_);
  qwtc_out_signal_.attach(&qwtPlot_out_signal_);
  qwtc_in_frame_.attach(&qwtPlot_in_frame_);
  qwtc_out_frame_.attach(&qwtPlot_out_frame_);

  qwtc_in_anal_.setPen(QPen(Qt::green));
  qwtc_out_anal_.setPen(QPen(Qt::red));
  qwtc_in_signal_.setPen(QPen(Qt::blue));
  qwtc_in_frame_.setPen(QPen(Qt::blue));
  qwtc_out_frame_.setPen(QPen(Qt::red));

  qwtPlot_in_anal_.setTitle(QString("Input analysis"));
  qwtPlot_out_anal_.setTitle(QString("Output analysis"));
  qwtPlot_in_frame_.setTitle(QString("Input frame"));
  qwtPlot_out_frame_.setTitle(QString("Output frame"));
  qwtPlot_in_signal_.setTitle(QString("Input signal"));
  qwtPlot_out_signal_.setTitle(QString("Output signal"));

  layout_->addWidget(&qwtPlot_in_frame_,  0, 0, 1, 1);
  layout_->addWidget(&qwtPlot_out_frame_, 1, 0, 1, 1);
  layout_->addWidget(&qwtPlot_in_anal_,   0, 2, 2, 1);
  layout_->addWidget(&qwtPlot_out_anal_,  0, 3, 2, 1);
  layout_->addWidget(&qwtPlot_in_signal_, 2, 0, 1, 4);
  layout_->addWidget(&qwtPlot_out_signal_,3, 0, 1, 4);

  widget_->show();
}


void DspPlot::signal_process()
{
  if (process_)
    (*processing_run)(process_);
}


}

