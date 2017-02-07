#include <dspone/plot/dspgui.h>

#include <dspone/plot/dspplotQwt.h>
#include <qt4/Qt/qapplication.h>

#define TITLE "QT Gui"

namespace dsp
{

DspGui::DspGui(ShortTimeProcess *process, thread_processing_run_t_ *f)
{
  init(*process, f);
}

DspGui::DspGui(ShortTimeProcess &process, thread_processing_run_t_ *f)
{
  init(process, f);
}

DspGui::DspGui(ShortTimeProcess *process, thread_processing_run_t &f)
{
  init(*process, f);
}

DspGui::DspGui(ShortTimeProcess &process, thread_processing_run_t &f)
{
  init(process, f);
}

void DspGui::init(ShortTimeProcess &process, thread_processing_run_t &f)
{
  process_ = &process;
  process_f_  = f;
}

void DspGui::init(ShortTimeProcess &process, thread_processing_run_t_ *f)
{
  process_ = &process;
  process_f_  = std::bind(f, std::placeholders::_1);
}


void DspGui::start()
{
  std::string title = TITLE;
  int argc = 1;
  char *argv[argc];
  argv[0] = const_cast<char*>(title.c_str());
  qt_app_.reset(new QApplication(argc, argv));
  plot_.reset(new DspPlot(process_f_ , process_));

  plot_->start();
  qt_app_->exec();
}



}
