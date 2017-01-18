#ifdef DEBUG
#define _LOGGER
#endif

#include <dspone/dsplogger.h>

#include <dspone/rt/DummyShortTimeFourier.h>
#include <dspone/rt/DummyShortTimeProcess.h>
#include <dspone/rt/DummyTimeProcess.h>

#include <dspone/plot/dspgui.h>

#include <gtest/gtest.h>

namespace dsp {
namespace test {

void plot_signal_pocessing_run(ShortTimeProcess *process)
{
  int length = 200*1024;
  int outlength = length + process->getMaxLatency();
  double data[length];
  double out_data[outlength];

  std::vector<double*> in_channels, out_channels;
  in_channels.push_back(data);
  out_channels.push_back(out_data);


  for (double freq = 0; freq < M_PI; freq += M_PI/50 )
  {
    for (int i = 0; i < length; ++i)
    {
      data[i] = cos(i*freq/8);
    }
    process->process(in_channels, length, out_channels, outlength);
  }
}



TEST(plot, plot_stft)
{
  DummySTFT stft;
  DspGui gui(&stft, &plot_signal_pocessing_run);
  gui.start();
}


TEST(plot, plot_timep)
{
  DummyShortTimeProcess stp(512);
  DspGui gui(&stp, &plot_signal_pocessing_run);
  gui.start();
}


TEST(plot, band_pass)
{

}


}
}


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
