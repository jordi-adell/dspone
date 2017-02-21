#include <dspone/plot/dspplotQwt.h>
#include <dspone/plot/qtDebug.h>

#include <qwt_sampling_thread.h>
#include <qwt_plot_layout.h>

#include <Qt/qapplication.h>

#include <mutex>

Q_DECLARE_METATYPE(std::shared_ptr<double>)
Q_DECLARE_METATYPE(std::vector<double*>)


namespace dsp
{


DspPlot::DspPlot(thread_processing_run_t f, ShortTimeProcess *stp) :
  processing_run(f),
  process_(stp)
{
  allocate();
  init();
}

DspPlot::DspPlot(thread_processing_run_t f, ShortTimeProcess &stp) :
  processing_run(f),
  process_(&stp)
{
  allocate();
  init();
}

DspPlot::DspPlot(thread_processing_run_t_ *f, ShortTimeProcess *stp) :
  processing_run(std::bind(f, std::placeholders::_1)),
  process_(stp)
{
  allocate();
  init();
}

DspPlot::DspPlot(thread_processing_run_t_ *f, ShortTimeProcess &stp) :
  processing_run(std::bind(f, std::placeholders::_1)),
  process_(&stp)
{
  allocate();
  init();
}

void DspPlot::allocate()
{
  wipp::init_cirular_buffer<double>(&cbuffer_in_signal_, 512);
  wipp::init_cirular_buffer<double>(&cbuffer_out_signal_, 512);

  widget_ = new QWidget(); // Not using a smart pointer because QGridLayout will take care.
  layout_ = new QGridLayout(widget_);
}

DspPlot::~DspPlot()
{
  wipp::delete_circular_buffer(&cbuffer_in_signal_);
  wipp::delete_circular_buffer(&cbuffer_out_signal_);

  if (signal_process_thread_)
  {
    if (signal_process_thread_->joinable())
      signal_process_thread_->join();
  }
}

void DspPlot::init()
{
  timer_.start();
  config();
}


void DspPlot::start(int channel)
{
  QtDebug &qtdebug = process_->_impl.get()->_qtdebug;

  qtdebug.set_plot_channel(channel);
  if (!signal_process_thread_)
  {
    signal_process_thread_.reset(new std::thread(&DspPlot::signal_process, this));
    signal_process_thread_->detach();
  }
}

void DspPlot::plot_input_frame(std::vector<double> signal)
{
  plot(signal, qwtc_in_frame_);
}

void DspPlot::plot_output_frame(std::vector<double> signal)
{
  plot(signal, qwtc_out_frame_);
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
  plot(signal, cbuffer_in_signal_,  qwtc_in_signal_);
}

void DspPlot::plot_output_signal(std::vector<double> signal)
{
  plot(signal, cbuffer_out_signal_ ,qwtc_out_signal_);
}



void DspPlot::plot(std::vector<double> signal, wipp::wipp_circular_buffer_t *cbuffer, QwtPlotCurve &qwtc)
{

  size_t consumed = 0;
  size_t free, skipped, size, stored;

  while (consumed < signal.size())
  {
    wipp::cf_free(cbuffer, &free);
    if (free < signal.size() - consumed)
      wipp::cf_skip(cbuffer, signal.size() - consumed, &skipped);
    wipp::cf_write(cbuffer, &signal[0], signal.size(), &stored);
    consumed += stored;
  }

  wipp::cf_size(cbuffer, &size);
  QVector<double> y(size, 0);
  QVector<double> x(size, 0);

  wipp::cf_next(cbuffer, &y[0], size, &stored);

  for (int i = 0; i < size; ++i)
    x[i] = i;

  plot(x, y, qwtc);
}

void DspPlot::plot(std::vector<double> signal, QwtPlotCurve &qwtc)
{
  int size = signal.size();
  QVector<double> y(size);
  QVector<double> x(size);

  y = QVector<double>::fromStdVector(signal);

  for (int i = 0; i < signal.size(); ++i)
    x[i] = i;

  plot(x, y, qwtc);
}


void DspPlot::plot(const QVector<double> &x, const QVector<double> &y, QwtPlotCurve &qwtc)
{
  qwtc.setSamples(x, y);
  update_plots();
}

void DspPlot::update_plots()
{
  if (timer_.hasExpired(75))
  {
    qwtPlot_in_anal_.replot();
    qwtPlot_out_anal_.replot();
    qwtPlot_in_signal_.replot();
    qwtPlot_out_signal_.replot();
    qwtPlot_in_frame_.replot();
    qwtPlot_out_frame_.replot();
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
    processing_run(*process_);

  signal_process_thread_.reset();

  QApplication::quit();
}


}

