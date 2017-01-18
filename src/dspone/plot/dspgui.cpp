#include <dspone/plot/dspgui.h>

#include <dspone/plot/dspplotQwt.h>
#include <qt4/Qt/qapplication.h>


namespace dsp
{

DspGui::DspGui(ShortTimeProcess *process, thread_processing_run_t *f)
{
    std::string title="QT Gui";
    int argc = 1;
    char *argv[argc];
    argv[0] = const_cast<char*>(title.c_str());

    qt_app_.reset(new QApplication(argc, argv));

    plot_.reset(new DspPlot(f, process));

}


void DspGui::start()
{
    qt_app_->exec();
}



}
