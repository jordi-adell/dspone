#include <dspone/plot/qtDebug.h>


namespace dsp {

QtDebug::QtDebug()
{

}


QtDebug::~QtDebug()
{

}



void QtDebug::send_signal(const PlotData &data, QtDebugSignal signal)
{
#ifdef QT_DEBUG
  switch (signal) {
    case IN_SIGNAL:
      emit plot_input_signal(data);
    break;
    case IN_FRAME:
      emit plot_input_frame(data);
    break;
    case IN_ANALYSIS:
      emit plot_input_analysis(data);
    break;
    case OUT_ANALYSIS:
      emit plot_output_analysis(data);
    break;
    case OUT_FRAME:
      emit plot_output_frame(data);
    break;
    case OUT_SIGNAL:
      emit plot_output_signal(data);
    break;

    default:
    break;
  }
#endif
}



}
