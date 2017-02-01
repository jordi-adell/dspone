#include <dspone/dsplogger.h>

#include <dspone/rt/DummyShortTimeFourier.h>
#include <dspone/rt/DummyShortTimeProcess.h>
#include <dspone/rt/DummyTimeProcess.h>

#include <dspone/plot/dspgui.h>

#include <sndfile.h>

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

  double freq, time;
  freq = time = 0;
  freq = 0;
  int iters = 50;
  double phase = 0;


  for (int j = 0; j < iters; ++j)
  {
    for (int i = 0; i < length; ++i, ++time)
    {
      // COS(2*PI*f*t);
      data[i] = cos(2.0F*M_PI*freq*time + phase);
      freq += 0.1F/(iters*length);
    }
    process->process(in_channels, length, out_channels, outlength);
    usleep(5*1000);
  }

}

void plot_file_process_run(ShortTimeProcess *process)
{
  std::string input_file = TEST_AUDIO_DIR;
  input_file += "/example16000.wav";
  SF_INFO sf_info_in;
  SNDFILE *snd_file_in  = sf_open(input_file.c_str(), SFM_READ, &sf_info_in);


  EXPECT_FALSE(snd_file_in == NULL);
  if (snd_file_in == NULL)
    return;

  int nframes = 1024;
  int out_n_frames = nframes + process->getMaxLatency();
  int nchannels = sf_info_in.channels;
  int sampling_rate = sf_info_in.samplerate;

  EXPECT_EQ(1,nchannels);

  double data[nframes * nchannels];
  double out_data[out_n_frames * nchannels + process->getMaxLatency()];

  std::vector<double*> in_channels, out_channels;
  in_channels.push_back(data);
  out_channels.push_back(out_data);

  int read_frames;
  while ( (read_frames = sf_readf_double(snd_file_in, data, nframes) ) > 0)
  {
    process->process(in_channels, read_frames, out_channels, out_n_frames);
    usleep(1000000.0F * read_frames / sampling_rate );
  }

}


TEST(plot, plot_stft)
{
  DummySTFT stft;
  DspGui gui(&stft, &plot_signal_pocessing_run);
  gui.start();
}

TEST(file, file_stft)
{
  DummySTFT stft;
  DspGui gui(&stft, &plot_file_process_run);
  gui.start();
}

TEST(plot, plot_timep)
{
  DummyShortTimeProcess stp(512);
  DspGui gui(&stp, &plot_signal_pocessing_run);
  gui.start();
}

}
}





int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
