#include <dspone/dsp.h>
#include <dspone/dspHelpers.h>
#include <dspone/DspException.h>
#include <dspone/dsplogger.h>

#include <dspone/algorithm/signalPower.h>
#include <dspone/algorithm/fft.h>
#include <dspone/algorithm/gralCrossCorrelation.h>
#include <dspone/algorithm/gralCrossCorrelationImpl.h>

#include <dspone/filter/FIRFilter.h>
#include <dspone/filter/IIRFilter.h>
#include <dspone/filter/FFTWeightingFilter.h>
#include <dspone/filter/BandPassFIRFilter.h>
#include <dspone/filter/BandPassFFTWFilter.h>
#include <dspone/filter/FilterBankFFTW.h>
#include <dspone/filter/FilterBankMelScale.h>
#include <dspone/filter/PreEmphasisFilter.h>
#include <dspone/filter/MedianFilter.h>

#include <dspone/pf/ParticleFilter.hpp>
#include <dspone/pf/PredictionModel.hpp>
#include <dspone/pf/ObservationModel.hpp>
#include <dspone/pf/ResamplingModel.hpp>

#include <dspone/rt/ShortTimeFourierTransform.h>
#include <dspone/rt/DummyShortTimeProcess.h>
#include <dspone/rt/DummyShortTimeFourier.h>
#include <dspone/rt/FilterProcess.hpp>
#include <dspone/rt/TimeProcess.h>


// Implementations

#include <dspone/rt/STFTImpl.h>
#include <dspone/algorithm/fftImpl.h>

#include <dspone/dspMath.h>

#include <dspone/plot/dspplotQwt.h>

#include <wipp/wippstats.h>
#include <wipp/wipputils.h>
#include <wipp/wippsignal.h>
#include <wipp/wippfilter.h>

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

#include <stdlib.h>
#include <string>

namespace dsp {
namespace test {

inline std::string getAudioPath()
{
    std::string audio = "./audiofiles/";
    return audio;
}

inline std::string getTmpPath()
{
    std::string tmp = "./tmp/";
    boost::filesystem::create_directories(tmp);
    return tmp;
}

template <class T> std::string  saveBufferToFile(T* buffer, int length, std::string file)
{
    std::ofstream ofs(getTmpPath()+file);
    for (int i=0; i<length; ++i)
	ofs << buffer[i] << std::endl;
    return getTmpPath()+file;
}

void shortTimeProcess(ShortTimeProcess &shortTimeP);
void shortTimeProcessConstantSignal(ShortTimeProcess &shortTimeP, bool useInputDataChannels);
void testFilterBank(int order,  FilterBank &filterBank);
void testBandPassFilter(BandPassFilter &filter, double *filterGaindB, double *freqs);
void checkFilterBankBinsOverlap(double *coefs, int length, int nbins);


class DummyTimeProcess : public dsp::Timeprocess
{
  public:
    DummyTimeProcess(int nchannels, int frameLength) : dsp::Timeprocess(nchannels, frameLength)
    {

    }
    virtual ~DummyTimeProcess() {}

    virtual void processParametrisation(std::vector<double*> &analysisFrames, int analysisLength,
					std::vector<double*> &dataChannels, int dataLength)
    {
      int a = 1;
    }

};
TEST(GCC, same_signal)
{

  int order = 10;
  int length = 1 << order;

  dsp::FFT fft(order);
  dsp::GeneralisedCrossCorrelation gcc(length, dsp::GeneralisedCrossCorrelation::ONESIDEDFFT);

  dsp::Complex corr;
  dsp::Complex signal[length];
  dsp::Complex signal_fft[length];

  wipp::setZeros(reinterpret_cast<wipp::wipp_complex_t*>(signal), length);
  signal[length / 2 + 1].re = 1;

  fft.fwdTransform(signal, signal_fft, length);

  corr = gcc.calculateCorrelation(signal_fft, signal_fft, length, 0, dsp::GeneralisedCrossCorrelation::ONESIDEDFFT);

  EXPECT_DOUBLE_EQ(1, corr.re);
  EXPECT_DOUBLE_EQ(0, corr.im);

  EXPECT_DOUBLE_EQ(1, sqrt(corr.re*corr.re + corr.im*corr.im));
}

TEST(GCC, core)
{
  int length = 3;
  dsp::GeneralisedCrossCorrelationImpl gcc(length, dsp::GeneralisedCrossCorrelation::ONESIDEDFFT);
  dsp::BaseTypeC corr;
  dsp::BaseTypeC signal[length];

  double delay = 0;
  int numSteps = 0;
  wipp::wipp_complex_t value = {1, 1};


  wipp::setZeros(reinterpret_cast<wipp::wipp_complex_t*>(signal), length);
  wipp::set(value, reinterpret_cast<wipp::wipp_complex_t*>(signal), length);
  signal[length / 2 + 1].re = 20000;

  corr = gcc.calculateCorrelation(signal, signal, length, delay, dsp::GeneralisedCrossCorrelation::ONESIDEDFFT);
  EXPECT_DOUBLE_EQ (1, corr.re);
  EXPECT_DOUBLE_EQ(0, corr.im);

}

TEST(DigitalSignalProcessingTest, testFIRFilter)
{

    return;


    double coefs[2]={1,-1};

    int largeorder=50000;
    double largeFIRcoefs[largeorder];

    for (int i = 0; i < largeorder; ++i)
    {
	largeFIRcoefs[i] = static_cast<double>(rand())/RAND_MAX;
    }

    int iterations = 1000;

    int numTHreads = 0;

    std::string firfile = "interaction/speech/rh2filter.fir";

    int const length=10000;
    int16_t signal[length];
    int16_t filtered[length];

    math::set(1, signal, length);
    math::setZeros(filtered, length);

    {
	FIRFilter filter(coefs,2);
	for (int j = 0; j < iterations; ++j)
	{
	    if (j%iterations/10 == 0)
	    {
		INFO_STREAM("it: " << j);
	    }
	    filter.filterBuffer(signal,filtered,length);
	    for (int i = 1; i < length; i++)
	    {
	      EXPECT_FLOAT_EQ(0,filtered[i]);
	    }
	}
    }


    {
	FIRFilter filter(coefs,2);
	for (int j = 0; j < iterations; ++j)
	{
	    if (j%iterations/10 == 0)
	    {
		INFO_STREAM("it: " << j);
	    }
	    filter.filterBuffer(signal,filtered,length);
	    for (int i = 1; i < length; i++)
	    {
		EXPECT_FLOAT_EQ(filtered[i],0);
	    }
	}
    }


    {
	FIRFilter filter2(firfile);

	int16_t signal2[length];
	int16_t filtered2[length];

	for (int j=0; j < iterations; ++j)
	{
	    for (int i=0; i<length; ++i)
	    {
		signal2[i] = rand();
	    }
	    filter2.filterBuffer(signal2,filtered2,length);
	}
    }

    {
	FIRFilter filter2(firfile);

	int16_t signal2[length];
	int16_t filtered2[length];

	for (int j=0; j < iterations; ++j)
	{
	    for (int i=0; i<length; ++i)
	    {
		signal2[i] = rand();
	    }
	    filter2.filterBuffer(signal2,filtered2,length);
	}
    }

    {
	FIRFilter filter2(largeFIRcoefs, largeorder);

	int16_t signal2[length];
	int16_t filtered2[length];

	for (int j=0; j < iterations; ++j)
	{
	    for (int i=0; i<length; ++i)
	    {
		signal2[i] = rand();
	    }
	    filter2.filterBuffer(signal2,filtered2,length);
	}
    }

    {
	FIRFilter filter2(largeFIRcoefs, largeorder);
	int16_t signal2[length];
	int16_t filtered2[length];

	for (int j=0; j < iterations; ++j)
	{
	    for (int i=0; i<length; ++i)
	    {
		signal2[i] = rand();
	    }
	    filter2.filterBuffer(signal2,filtered2,length);
	}
    }
}

TEST(DigitalSignalProcessingTest, testIIRFilter)
{
    // This is a pole in -1 and a zero i 0.
    // so cos[pi*n] must be removed.

    double coefs[] = {1, 1, 1, 0};
    int16_t inSignal[] = {1, -1,  1, -1,  1, -1,  1, -1, 1};
    int16_t outSignal[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    IIRFilter filter(coefs, 4);
    filter.filterBuffer(inSignal, outSignal, 9);

    // first output is not zero, because
    // the unknown initial conditions of the filter
    // producing a transitory output.
    for (size_t i = 1; i < 9; ++i)
    {
      DEBUG_STREAM("OUTPUT (" << i << "): " << outSignal[i]);
      EXPECT_EQ((int16_t) 0, outSignal[i]);
    }
    DEBUG_STREAM("Tested");
}

TEST(DigitalSignalProcessingTest, interactiveIIRShape)
{

    return;

    std::string file; //   = util::InitOptions::singleton().getOptionValue("file","");
    //std::string sncoefs = util::InitOptions::singleton().getOptionValue("ncoefs","4");

    //          Gnuplot plot("lines");

    size_t ncoefs = 0; //= util::toInteger<size_t>(sncoefs);

    size_t length = 1024;
    size_t order = 10;
    int16_t delta[length];
    double fdelta[length];
    double ffttransform[length];
    double coefs[ncoefs];
    double magFFT[length/2];
    math::setZeros(delta, length);
    delta[0] = 1;

    std::ifstream ifs(file);
    size_t i = 0;
    std::string line;
    while(ifs)
    {
	ifs >> line;
	if (line.empty())
	    continue;
	coefs[i] = std::stof(line);
	DEBUG_STREAM("-> i:" << i << ": " << coefs[i]);
	++i;
    }
    ncoefs = i - 1;

    IIRFilter iirFilter(coefs, ncoefs);
    FFT fft(order);

    iirFilter.filter(delta, length);
    math::copy(delta, fdelta, length);

    //            plot.plot_x(&fdelta[0], length);

    fft.fwdTransform(fdelta, ffttransform, length);
    math::magnitude((Complex64f*)(ffttransform), magFFT, length/2);
    wipp::logn(magFFT, length/2);
    math::multC(20/log(10), magFFT, length/2);

    //          plot.plot_x(&magFFT[0], length/2);



    std::cin >> file;

}



TEST(DigitalSignalProcessingTest, testMedianFilter)
{
  int length = 20;
  int buffer[] =    {1, 2, 3, 20, 5, 6, -60, 8, 9, 10, 12, 9, 8, 7, 2, 5, 4, 3,-30, 1};
  int reference[] = {1, 2, 3,  5, 5, 6,   6, 8, 9,  9,  9, 9, 8, 7, 5, 4, 3, 3,  3, 1};
  int obuffer[length];
  dsp::MedianFilter filter(5);

  filter.filterBuffer(buffer, obuffer, length);

#ifdef _LOGGER
  std::cout << "LOGGER" << std::endl;
#endif

  for (size_t i = 0; i < length; ++i)
  {
    EXPECT_EQ(reference[i], obuffer[i]);
    ERROR_STREAM("REF: " << reference[i] << "\t VAL: " << obuffer[i]);
  }
}


TEST(DigitalSignalProcessingTest, testFIRFilerInteractiveCommandLine)
{

    return;

    std::string indir  ;//= util::InitOptions::singleton().getOptionValue("indir","");
    std::string outdir ;//= util::InitOptions::singleton().getOptionValue("outdir","");
    std::string firfile ;//= util::InitOptions::singleton().getOptionValue("fir","");
    std::string listfile ;//= util::InitOptions::singleton().getOptionValue("list","");

    if (indir.empty() || outdir.empty() || firfile.empty() || listfile.empty())
    {
	INFO_STREAM("use: --indir --outdir --list and --fir ("
			<< indir << ", " << outdir << ", " << listfile << ", " << firfile << ")");
	return;
    }

    FIRFilter filter(firfile);

    std::ifstream ilfs(listfile.c_str());

    while(ilfs)
    {
	std::string file;
	ilfs >> file;
	if (file.empty())
	    continue;

	std::string infile = indir + "/" + file + ".raw";
	std::string outfile = outdir + "/" + file + ".raw";

	std::ifstream ifs(infile.c_str(),std::ios::binary);
	std::ofstream ofs(outfile.c_str(),std::ios::binary);

	if (!ifs)
	{
	    ERROR_STREAM("unable to open file " << infile);
	    return;
	}

	if (!ofs)
	{
	    ERROR_STREAM("unable to open output file " << outfile);
	}

	int buffersize = 1024*10;
	int16_t inbuffer[buffersize];
	int16_t outbuffer[buffersize];

	while(ifs)
	{
	    ifs.read((char *)inbuffer,buffersize*sizeof(int16_t));
	    int nRead = ifs.gcount();
	    filter.filterBuffer(inbuffer,outbuffer,nRead/sizeof(int16_t));
	    ofs.write((char *)outbuffer,nRead);
	}
	ifs.close();
	ofs.close();
    }
}

TEST(DigitalSignalProcessingTest, testDummyShortTimeProcess)
{
    int maxorder = 13;
    for (int order = 7; order < maxorder; ++order)
    {
	INFO_STREAM("Testing order: " << order);
	DummyShortTimeProcess shortTimePData(1 << order, true);
	DummyShortTimeProcess shortTimeP(1 << order);

	INFO_STREAM("Constant data without input data channels");
	shortTimeProcessConstantSignal(shortTimePData, false);
	INFO_STREAM("Constant data with input data channels");
	shortTimeProcessConstantSignal(shortTimePData, true);
	shortTimeProcessConstantSignal(shortTimeP, true);

	INFO_STREAM("Real input signal");
	shortTimeProcess(shortTimePData);
	shortTimeProcess(shortTimeP);
    }
}

TEST(DigitalSignalProcessingTest, testDummySTFT)
{


  DummySTFT *a = new DummySTFT();
  delete a;

  DummySTFT stft;
  ShortTimeProcess *shortTimeP =dynamic_cast<ShortTimeProcess *>(&stft);
  shortTimeProcess(*shortTimeP);
}

TEST(DigitalSignalProcessingTest, testDummySTFTAnalysis)
{
    DummySTFTAnalysis *a = new DummySTFTAnalysis();
    delete a;

    DummySTFTAnalysis stft;
    ShortTimeAnalysis *shortTimeP =dynamic_cast<ShortTimeAnalysis *>(&stft);
}

TEST(DigitalSignalProcessingTest, testBandPassFIRFilter)
{
    int order = 9;
    int length = 1 << order;
    float lowFreq = 0.1;
    float highFreq  = 0.2;
    int NFREQS = length/2;
    double filterGaindB[NFREQS];
    double freqs[NFREQS];

    BandPassFIRFilter filter(order, lowFreq, highFreq);
    testBandPassFilter(filter, filterGaindB, freqs);
    lowFreq = filter.getLowFreq();
    highFreq = filter.getHighFreq();

    EXPECT_FLOAT_EQ(0.1, lowFreq);
    EXPECT_FLOAT_EQ(0.2, highFreq);

    for (int f = 1; f< NFREQS; ++f)
    {
	float sinFreq = freqs[f];
	DEBUG_STREAM("F: " << freqs[f] << " Gain: " << filterGaindB[f]);
	if (sinFreq < lowFreq)
	{
	  EXPECT_LT(filterGaindB[f], -1);
	}
	else if (sinFreq < highFreq)
	{
	    //EXPECT_TRUE("Attenuation is different than expected.", filterGaindB[f] > -1.2);
	}
	else
	{
	  EXPECT_LT(filterGaindB[f], -1);
	}
    }

    double min=0;
    double max=0;
    wipp::min(&filterGaindB[1], NFREQS-2, &min);
    wipp::max(&filterGaindB[1], NFREQS-2, &max);

    EXPECT_LT(min, -30);
    EXPECT_GT(max, -1);

}

TEST(DigitalSignalProcessingTest, testBandPassFFTWFilter)
{
    int order = 9;
    int length = 1 << order;
    const float lowFreq_orig = 0.1;
    const float highFreq_orig = 0.2;
    float lowFreq = lowFreq_orig;
    float highFreq  = highFreq_orig;
    int NFREQS = length/2;
    double filterGaindB[NFREQS];
    double freqs[NFREQS];
    double coefs[NFREQS];

    BandPassFFTWFilter filter(order, lowFreq, highFreq, BandPassFFTWFilter::RECTANGULAR);
    lowFreq = filter.getLowFreq();
    highFreq = filter.getHighFreq();
    testBandPassFilter(filter, filterGaindB, freqs);

    DEBUG_STREAM("Constructed a band pass filter with: " << lowFreq_orig << ", " << highFreq_orig<< " and got " << lowFreq << ", " << highFreq);

    EXPECT_NEAR(lowFreq_orig, lowFreq, 0.005);
    EXPECT_NEAR(highFreq_orig, highFreq, 0.005);

    filter.getCoeficients(coefs, NFREQS);

    for (int f = 1; f< NFREQS; ++f)
    {
	float sinFreq = freqs[f];
	if (sinFreq < 0.99*lowFreq)
	{
	    DEBUG_STREAM("Gain: < lowFreq " << filterGaindB[f] << ", f: " << sinFreq);
	    EXPECT_LT(filterGaindB[f], -30);
	}
	else if (1.01*lowFreq < sinFreq && sinFreq < 0.99*highFreq)
	{
	    DEBUG_STREAM("Gain: < pass band" << filterGaindB[f] << ", f: " << sinFreq);
	    EXPECT_GE(filterGaindB[f], 0);
	}
	else if(sinFreq> 1.01*highFreq)
	{
	    DEBUG_STREAM("Gain: > highFreq" << filterGaindB[f] << ", f: " << sinFreq);
	    EXPECT_LT(filterGaindB[f], -30);
	}
    }

    double min=0;
    double max=0;
    wipp::min(&filterGaindB[1], NFREQS-2, &min);
    wipp::max(&filterGaindB[1], NFREQS-2, &max);

    EXPECT_NEAR(-1081, min,1);
    EXPECT_NEAR(5, max,1);

}

TEST(DigitalSignalProcessingTest, testFFTweightingFilter)
{
    int order = 9;
    int length = (1 << order);
    int clength = (1 << (order-1)) + 1; // length/2 + 1
    float lowFreq = 0.1;
    float highFreq = 0.2;
    double coefs[clength];

    int16_t signal[length];
    int16_t filtered[length];

    math::setZeros(coefs, clength);
    int iLf = lowFreq*length;
    int iHf = highFreq*length;
    lowFreq  = static_cast<float>(iLf)/length;
    highFreq = static_cast<float>(iHf)/length;

    math::set(1, &coefs[iLf], iHf-iLf);

    FFTWeightingFilter filter(coefs, clength);

    for (int f = 1; f < length/2; ++f)
    {
	float sinFreq = (f*1.0F)/512;
	for (int i = 0; i < length; ++i)
	    signal[i] = pow(2,14)*sin(2*M_PI*sinFreq*i);

	filter.filterBuffer(signal, filtered, length);
	double signalPower = calculateLogPowerTemporal(signal, length);
	double filteredPower = calculateLogPowerTemporal(filtered, length);
	float filterGaindB = (filteredPower - signalPower);


	DEBUG_STREAM("f: " << sinFreq << ", G: " << filterGaindB << ", signal power: " << signalPower <<", filetered power: " << filteredPower);

	if (sinFreq < lowFreq)
	{
	    EXPECT_LT(filterGaindB, -1);
	}
	else if (sinFreq < highFreq)
	{
	    EXPECT_GT(filterGaindB, -1);
	}
	else
	{
	    EXPECT_LT(filterGaindB, -1);
	}
    }
}

TEST(DigitalSignalProcessingTest, testFilterBankFIRMelScale)
{
    int nbins = 32;
    int order = 9;
    int sampleRate = 16000;
    std::vector<double> freqs;
    FilterBankFIRMelScale melFilterBank(order, nbins, sampleRate);
    melFilterBank.getCenterFrequencies(freqs);

    for (size_t i=0; i<freqs.size(); ++i)
    {
	DEBUG_STREAM("MF: " << freqs.at(i));
    }

    // We only need 2^(order-1) coefs, because of the symetric of FFT for real signals
    int coefLength = 1 << (order -1);
    double coefs[coefLength*melFilterBank.getNBins()];
    math::set(0, coefs, coefLength*melFilterBank.getNBins());
    melFilterBank.getFiltersCoeficients(coefs, coefLength*melFilterBank.getNBins());

    //          checkFilterBankBinsOverlap(coefs, coefLength, melFilterBank.getNBins());


    testFilterBank(order, melFilterBank);
}

TEST(DigitalSignalProcessingTest, testFilterBankFFTWMelScale)
{
    int nbins = 32;
    int order = 9;
    int sampleRate = 16000;
    std::vector<double> freqs;
    FilterBankFFTWMelScale melFilterBank(order, nbins, sampleRate);
    melFilterBank.getCenterFrequencies(freqs);

    for (size_t i=0; i<freqs.size(); ++i)
    {
	DEBUG_STREAM("MF: " << freqs.at(i));
    }

    // We only need 2^(order-1) coefs, because of the symetric of FFT for real signals
    int coefLength = 1 << (order -1);
    double coefs[coefLength*melFilterBank.getNBins()];
    math::set(0, coefs, coefLength*melFilterBank.getNBins());
    melFilterBank.getFiltersCoeficients(coefs, coefLength*melFilterBank.getNBins());

    //          checkFilterBankBinsOverlap(coefs, coefLength, melFilterBank.getNBins());


    testFilterBank(order, melFilterBank);
}

TEST(DigitalSignalProcessingTest, testFilterBankFFTW)
{
    int order = 9;
    std::vector<double> freqs;
    std::vector<double> freqsPiece;

    float delta=0.005;
    for (float f=0; f<=0.5; f+=delta)
    {
	if (0.1 < f && f < 0.4)
	{
	    freqsPiece.push_back(f);
	}
	freqs.push_back(f);
	DEBUG_STREAM("f: "  << freqs.back());
	delta *= 1.2;
    }
    freqs.push_back(0.5);

    DEBUG_STREAM("Testing filter bank with " << freqs.size() << " bins.");

    FilterBankFFTW filterBank(order, freqs);
    FilterBankFFTW filterBankPiece(order, freqsPiece);

    // We only need 2^(order-1) + 1 coefs, because of the symetric of FFT for real signals
    int coefLength = (1 << (order -1)) + 1;
    double coefs[coefLength*filterBank.getNBins()];
    math::set(0, coefs, coefLength*filterBank.getNBins());
    int coefscopied = filterBank.getFiltersCoeficients(coefs, coefLength*filterBank.getNBins());

    EXPECT_FLOAT_EQ(coefLength*filterBank.getNBins(), coefscopied);

    checkFilterBankBinsOverlap(coefs, coefLength, filterBank.getNBins());

    DEBUG_STREAM("Testing full freq range");
    testFilterBank(order, filterBank);
    DEBUG_STREAM("Testing a specific range");
    testFilterBank(order, filterBankPiece);
}

TEST(DigitalSignalProcessingTest, testFilterBankFIR)
{
    int order = 1 << 10;
    std::vector<double> freqs;
    float delta=0.01;
    for (float f=0; f<=0.5; f+=delta)
    {
	freqs.push_back(f);
	DEBUG_STREAM("f: "  << freqs.back());
	delta *= 1.2;
    }
    freqs.push_back(0.5);

    FilterBankFIR filterBank(order, freqs);
    testFilterBank(order, filterBank);
}

TEST(DigitalSignalProcessingTest, testParticleFilter)
{

    int size = 20;

    ParticleSet<> particles(size, 0);
    ParticleSet<> newParticles(size);
    ParticleSet<> weights(size);

    for(int i = 0; i < size; ++i)
    {
	particles.set(i,i);
	weights.set(i,i);
    }

    particles = particles * weights;
    particles = particles + 0.4;

    double initValue = 0;
    double initDerivative = 0;
    auto predictionModel = make_prediction_model(initValue,initDerivative);

    predictionModel->update(particles);
    predictionModel->updateModel(1);


    double mean = 0;
    double stddev = 1;
    auto observationModel = make_observation_model<double>(mean, stddev);

    weights = observationModel->getWeights(particles);
    observationModel->updateModel();

    auto resamplingModel = make_resampling_model<double, double>();
    resamplingModel->resample(particles, newParticles, weights);

    auto filter = make_particle_filter<double>(50.0, 100, 0,
				       std::make_pair<double,double>(0,1),
				       observationModel, predictionModel, resamplingModel);

    DEBUG_STREAM("particle: " << filter.getParticles() << std::endl);

    double p = -100000;
    for (int i = 0; i < 4000 ; ++i)
    {
	p = filter.updateFilter();


	DEBUG_STREAM("Estimate: " << p);
    }
    DEBUG_STREAM("\nparticle: " << filter.getParticles());
    // This lag might be affected by modifications in the prediction and observation models.
    EXPECT_NEAR( 0 , p, 0.5);
}

TEST(DigitalSignalProcessingTest, testFilterProcessor)
{
    double coefs[] = {1, 1, 1, 0};
    int length = 4;
    int channels = 2;
    std::unique_ptr<FilterProcess> f(FilterProcess::make_FilterProcess<IIRFilter>(channels, coefs, length));
    IIRFilterModule iir(2, coefs, length);
    FIRFilterModule fir(2, coefs, length);
    BandPassFIRFilterModule bp2(2, 5, 0.1, 0.34);
}

TEST(DigitalSignalProcessingTest, testIPPGhostsInteractive)
{
  return;


    INFO_STREAM("Testing DOUBLEs");
    for (double numerator = 10; numerator >= 0; --numerator)
    {
	for (double denominator = 1e-10; denominator >= 1e-300; denominator = denominator*1e-10)
	{
	    double result = 1000;

	    INFO_STREAM("Trying to divide " << numerator <<  " by " << denominator);
	    result = numerator/denominator;
	    INFO_STREAM("Result " << result);
	    //              CPPUNIT_ASSERT(result == 0);


	    INFO_STREAM("Trying to divide " << numerator <<  " by " << denominator << " with IPP function");
	    math::div(&denominator, &numerator, &result, 1);
	    INFO_STREAM("Result " << result);
	    //              CPPUNIT_ASSERT(result == 0);
	}
    }

    INFO_STREAM("Testing FLOATs");
    for (float numerator = 0; numerator <= 10; ++numerator)
    {
	for (float denominator = 1e-10; denominator >= 1e-42; denominator = denominator*1e-10)
	{
	    float result = 1000;

	    INFO_STREAM("----------------------");
	    INFO_STREAM("Trying to divide " << numerator <<  " by " << denominator);
	    result = numerator/denominator;
	    INFO_STREAM("Result " << result);
	    //              CPPUNIT_ASSERT(result == 0);


	    INFO_STREAM("Trying to divide " << numerator <<  " by " << denominator << " with IPP function");
	    math::div(&denominator, &numerator, &result, 1);
	    INFO_STREAM("Result " << result);
	    //              CPPUNIT_ASSERT(result == 0);
	}
    }



}

TEST(TimeProcess, constructor_destructor)
{

  DummyTimeProcess *dtp;
  dtp = new DummyTimeProcess(1, 1024);
  delete dtp;
}


TEST(TimeProcess, process)
{
  int frame_length = 1 << 10;
  DummyTimeProcess dtp(1, frame_length);
  int nframes = 500;
  double constant = 1000;
  int length = nframes*frame_length;

  double buffer[length];
  double output[length + dtp.getMaxLatency()];

  wipp::wipp_rand_t *rand;
  wipp::init_rand_gaussian(&rand, 0, 100);
  wipp::rand(rand, buffer, length);

  wipp::set(constant, buffer, length);
  wipp::setZeros(output, length);

  std::vector<double*> in_channels, out_channels;

  in_channels.push_back(buffer);
  out_channels.push_back(output);

  dtp.process(in_channels, length, out_channels, length);

  double mean, stddev;
  wipp::mean(output, length, &mean);
  wipp::stddev(output, length, &stddev);

  EXPECT_NEAR(mean, constant, 0.03*constant);
  EXPECT_LT(stddev, 0.08*constant);
}

TEST(FFT, constructor_destructor)
{

  dsp::FFTImpl ffti(9);
  dsp::STFTImpl stfti(4,9);
}


TEST(FFT, dummyfft)
{
  int frame_length = 1 << 10;
  int nframes = 20;
  int length = frame_length*nframes;
  double buffer[length];
  dsp::DummySTFT dSTFT;
  double output[length+dSTFT.getMaxLatency()];

  double constant = 1000;

  std::vector<double*> in_channels, out_channels;

  in_channels.push_back(buffer);
  out_channels.push_back(output);

  wipp::wipp_rand_t *rand;
  wipp::init_rand_gaussian(&rand, 0, 15);
  wipp::rand(rand, buffer, length);

  wipp::set(constant, buffer, length);

  dSTFT.process(in_channels, length, out_channels, length);

  double mean, stddev;
  wipp::mean(output, length, &mean);
  wipp::stddev(output, length, &stddev);

  EXPECT_NEAR(mean, constant, 0.03*constant);
  EXPECT_LT(stddev, 0.15*constant);


}


TEST(DigitalSignalProcessingTest, testPreEmphasisFilter)
{
  //    std::string infile  = getAudioPath() + "music16000Hz.raw";
  //    std::string outfile = getTmpPath() + "preEmph.raw";
  //    std::ifstream istr(infile);
  //    std::ofstream ostr(outfile);

  //    int buffersize=1024;
  //    int16_t buffIn[buffersize];

  //    PreEmphasisFilter preEmphFilter(0.97);

  //    while (istr)
  //    {
  //	istr.read(reinterpret_cast<char*>(buffIn), buffersize*2);
  //	unsigned int read_bytes = istr.gcount();
  //	unsigned int read_samples = read_bytes/2;
  //	preEmphFilter.filter(buffIn, read_samples);
  //	ostr.write(reinterpret_cast<char*>(buffIn), read_bytes);
  //    }

  size_t length = 10;
  int16_t testsignal[] =      {1000, 1000, 1000, 1000,    0,    0,    0,    0,    0,    0};
//  int16_t referencesignal[] = {1000, 1970, 2911, 3824, 3709, 3598, 3490, 3385, 3283, 3185};
  int16_t referencesignal[] = {1000, 30, 970, 58, -56, 54, -53, 51, -50, 48};

  int16_t filteredsignal[length];

  saveBufferToFile(testsignal, length, "orig");
  saveBufferToFile(referencesignal, length, "ref");

  PreEmphasisFilter preEmphResultTest(0.97);

  preEmphResultTest.filterBuffer(testsignal, filteredsignal, length);

  saveBufferToFile(filteredsignal, length, "filt");


  for (size_t i = 0; i < length; ++i)
  {
    EXPECT_FLOAT_EQ(referencesignal[i], filteredsignal[i]);
  }

}


TEST(signal_power, helpers)
{
  const int length = 1024;
  double *signal = new double[length];

  SignalPtr signal_ptr(signal);
  SignalVector vptr_signal;
  std::vector<BaseType*> vsignal;
  vsignal.push_back(signal);
  vptr_signal.push_back(signal_ptr);

  wipp::wipp_rand_t *rand_state;
  wipp::init_rand_gaussian(&rand_state, 0, 100);
  wipp::rand(rand_state, signal, length);

  dsp::calculateLinearPowerFFT(signal, length);
  dsp::calculateLinearPowerFFT(vsignal, length);
  dsp::calculateLinearPowerFFT(signal_ptr, length);

  dsp::calculateLogPowerFFT(signal, length);
  dsp::calculateLogPowerFFT(vsignal, length);
  dsp::calculateLogPowerFFT(signal_ptr, length);

  dsp::calculateLinearPowerTemporal(signal, length);
  dsp::calculateLinearPowerTemporal(vsignal, length);
  dsp::calculateLinearPowerTemporal(signal_ptr, length);

  dsp::calculateLogPowerTemporal(signal, length);
  dsp::calculateLogPowerTemporal(vsignal, length);
  dsp::calculateLogPowerTemporal(signal_ptr, length);


}

TEST(signal_power, class_api)
{
  const int length = 1024;

  /// --- Check that all functions return at least
  /// the exact same value.

  double *signal = new double[length];
  SignalPtr signal_ptr(signal);
  SignalVector vptr_signal;
  std::vector<BaseType*> vsignal;
  vsignal.push_back(signal);
  vptr_signal.push_back(signal_ptr);

  wipp::wipp_rand_t *rand_state;
  wipp::init_rand_gaussian(&rand_state, 0, 100);
  wipp::rand(rand_state, signal, length);

  dsp::SignalPower::FFTLogPower(signal, length);
  dsp::SignalPower::FFTLogPower(signal_ptr, length);
  dsp::SignalPower::FFTLogPower(vptr_signal, length);
  dsp::SignalPower::FFTLogPower(vsignal, length);

  dsp::SignalPower::FFTPower(signal, length);
  dsp::SignalPower::FFTPower(vsignal, length);
  dsp::SignalPower::FFTPower(vptr_signal, length);
  dsp::SignalPower::FFTPower(signal_ptr, length);

  dsp::SignalPower::logPower(signal, length);
  dsp::SignalPower::logPower(signal_ptr, length);
  dsp::SignalPower::logPower(vptr_signal, length);
  dsp::SignalPower::logPower(vsignal, length);

  dsp::SignalPower::power(signal, length);
  dsp::SignalPower::power(vsignal, length);
  dsp::SignalPower::power(vptr_signal, length);
  dsp::SignalPower::power(signal_ptr, length);

}


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


void test_qt(ShortTimeProcess *process)
{
  std::string title="QT Gui";
  int argc = 1;
  char *argv[argc];
  argv[0] = const_cast<char*>(title.c_str());

  QApplication app(argc, argv);
  DspPlot plot(&plot_signal_pocessing_run, process);

  app.exec();
}

TEST(plot, plot_stft)
{
  DummySTFT stft;
  test_qt(&stft);
}


TEST(plot, plot_timep)
{
  DummyShortTimeProcess stp(512);
  test_qt(&stp);
}

void shortTimeProcess(ShortTimeProcess &shortTimeP)
{
    // Testing different buffer sizes
    float step = 4.5;
    for (float sizemultiple = 1; sizemultiple < 30; sizemultiple += step)
    {
	std::string infile  = getAudioPath() + "music16000Hz.raw";
	std::ifstream ifs(infile);
	const uint16_t constant = 1000;

	int bufferSampleSize = sizemultiple*shortTimeP.getFrameSize();
	int bufferOutSampleSize = bufferSampleSize + shortTimeP.getMaxLatency();
	INFO_STREAM("Using buffersize: " << bufferSampleSize << " and the window size is " << shortTimeP.getFrameSize());
	int bufferBytesSize = bufferSampleSize*2;
	int readBytes = -1, readSamples = -1;
	int16_t sum = -1;
	int16_t datasum = -1;

	int16_t inbuffer[bufferSampleSize];
	int16_t inDatabuffer[bufferSampleSize];

	math::set(constant, inbuffer, bufferSampleSize);
	math::set(constant, inDatabuffer, bufferSampleSize);

	int16_t outbuffer[bufferOutSampleSize];
	int16_t outDatabuffer[bufferOutSampleSize];
	int16_t difference[bufferOutSampleSize];
	math::setZeros(difference, bufferSampleSize);

	int16_t dataDifference[bufferOutSampleSize];
	math::setZeros(dataDifference, bufferSampleSize);

	std::vector<int16_t *> inVectorSignal;
	std::vector<int16_t *> outVectorSignal;

	inVectorSignal.push_back(inbuffer);
	inVectorSignal.push_back(inDatabuffer);
	outVectorSignal.push_back(outbuffer);
	outVectorSignal.push_back(outDatabuffer);

	bool firstcall = true;

	EXPECT_TRUE(ifs) << "Unable to open file " << infile;

	while(ifs)
	{
	    ifs.read((char*)inbuffer, bufferBytesSize);
	    readBytes = ifs.gcount();
	    readSamples = readBytes / 2;

	    int latency = shortTimeP.getLatency();
	    int processedSamples = shortTimeP.process(inVectorSignal, readSamples, outVectorSignal, bufferOutSampleSize);
	    wipp::sub(inbuffer, &outbuffer[latency], difference, processedSamples-latency);
	    wipp::sub(inDatabuffer, &outDatabuffer[latency], dataDifference, processedSamples-latency);

	    if (firstcall)
	    {
		firstcall = false;
	    }
	    else
	    {
		if (processedSamples > latency)
		{
		    wipp::abs(difference, processedSamples-latency);
		    math::sum(&difference[0], processedSamples-latency, &sum);
		    wipp::abs(dataDifference, processedSamples-latency);
		    math::sum(dataDifference, processedSamples-latency, &datasum);

		    EXPECT_EQ(0, (int) sum);
		    EXPECT_EQ(0, (int) datasum);
		}
	    }

	}
    }
}

void shortTimeProcessConstantSignal(ShortTimeProcess &shortTimeP, bool useInputDataChannels)
{
    // Testing different buffer sizes
    float step = 5;
    for (float sizemultiple = step; sizemultiple < 200; sizemultiple += step)
    {

	const uint16_t constant = 1000;

	int bufferSampleSize = sizemultiple*shortTimeP.getFrameSize();
	int bufferOutSampleSize = bufferSampleSize + shortTimeP.getMaxLatency();
	INFO_STREAM("Using buffersize: " << bufferSampleSize << " " << bufferOutSampleSize << " and the window size is " << shortTimeP.getFrameSize());

	int16_t sum = -1;
	int16_t datasum = -1;

	int16_t inbuffer[bufferSampleSize];
	int16_t inDatabuffer[bufferSampleSize];

	math::set(constant, inbuffer, bufferSampleSize);
	math::set(constant, inDatabuffer, bufferSampleSize);

	int16_t outbuffer[bufferOutSampleSize];
	int16_t outDatabuffer[bufferOutSampleSize];
	int16_t difference[bufferOutSampleSize];
	math::setZeros(difference, bufferSampleSize);

	int16_t dataDifference[bufferOutSampleSize];
	math::setZeros(dataDifference, bufferSampleSize);

	std::vector<int16_t *> inVectorSignal;
	std::vector<int16_t *> outVectorSignal;

	inVectorSignal.push_back(inbuffer);
	if (useInputDataChannels)
	    inVectorSignal.push_back(inDatabuffer);
	outVectorSignal.push_back(outbuffer);
	outVectorSignal.push_back(outDatabuffer);

	int processedSamples = shortTimeP.process(inVectorSignal, bufferSampleSize, outVectorSignal, bufferOutSampleSize);
	int latency = shortTimeP.getLatency();
	wipp::sub(inbuffer, &outbuffer[latency], difference, processedSamples-latency);
	wipp::sub(inDatabuffer, &outDatabuffer[latency], dataDifference, processedSamples-latency);

	DEBUG_STREAM("buffsize: " << bufferSampleSize << " order " << shortTimeP.getFrameSize() << " PS:" << processedSamples << " L:" << latency);
	wipp::abs(difference, processedSamples-latency);
	math::sum(difference, processedSamples-latency, &sum);

	wipp::abs(dataDifference, processedSamples-latency);
	math::sum(dataDifference, processedSamples-latency, &datasum);

	EXPECT_LE((int) sum, (processedSamples-latency)*constant/1000);
	EXPECT_LE((int) datasum, (processedSamples-latency)*constant/1000);
	break;
    }
}



void testFilterBank(int order,  FilterBank &filterBank)
{

    std::vector<double> freqs;
    filterBank.getCenterFrequencies(freqs);

    double pFreqs[freqs.size()];
    double pPower[freqs.size()];

    int length = order;
    if (order <= 10)
	length = 1 << order;

    int16_t signal[length];
    int16_t residual[length];
    int16_t filtered[length*filterBank.getNBins()];

    float sinFreq = freqs.front();
    for (; sinFreq < freqs.back(); sinFreq+=0.1)
    {
	for (int i=0; i<length; ++i)
	{
	    signal[i] = pow(2,14)*cos(2*M_PI*sinFreq*i);
	}
	wipp::window(signal, length, wipp::wippHANN);

	DEBUG_STREAM("input power: " << calculateLogPowerTemporal(signal, length));

	std::cout << " F: " << sinFreq << std::endl;

	for (int j=0; j<5; ++j)
	  filterBank.filterBuffer(signal, residual, filtered, length, length*filterBank.getNBins());

	for (int i=0; i<filterBank.getNBins(); ++i)
	{
	    pPower[i] = calculateLogPowerTemporal(&filtered[length*i],length);
	    pFreqs[i] = freqs.at(i);

	    DEBUG_STREAM("S: " << calculateLogPowerTemporal(signal, length)
			     << " F" << i << ": " << calculateLogPowerTemporal(&filtered[length*i],length)
		    << " R" << i << ": " << calculateLogPowerTemporal(residual, length) - calculateLogPowerTemporal(signal, length)
		    << " sinFreq: " << sinFreq << ", c: " << freqs.at(i)
		       );
	}

	math::subC(sinFreq, pFreqs, freqs.size());
	wipp::abs(pFreqs, freqs.size());

	double min;
	size_t pIdx, fIdx;
	wipp::maxidx(pPower, freqs.size(), &min, &pIdx);
	wipp::minidx(pFreqs, freqs.size(), &min, &fIdx);

	DEBUG_STREAM("P: " << pPower[pIdx] << " i: " << pIdx);
	DEBUG_STREAM("F: " << pFreqs[fIdx] << " i: " << fIdx);


	EXPECT_FLOAT_EQ(fIdx, pIdx);

	//            EXPECT_FLOAT_EQ("The maximum power does not correspond to the bin whose central frequency is closer to the one of the input sinusoidal",
	//                                         fIdx,
	//                                         pIdx);
    }

}

void testBandPassFilter(BandPassFilter &filter,
			double *filterGaindB,
			double *freqs)
{
    int order = 9;
    float lowFreq = filter.getLowFreq();
    float highFreq  = filter.getHighFreq();

    DEBUG_STREAM("LF: " << lowFreq << " HF: " << highFreq);

    int length = 1 << order;
    int NFREQS = length/2;

    double test_signal[length];
    int16_t signal[length];
    int16_t filtered[length];
    double coefs[length];

    std::ostringstream ossf, ossg;
    ossf.precision(3);
    ossg.precision(3);

    math::set(0, filterGaindB, NFREQS);
    math::set(0, freqs, NFREQS);

    for (int f = 1; f< NFREQS; ++f)
    {
	double sinFreq = (f*1.0F)/length;
	double A = pow(2,14);
	for (int i = 0; i < length; ++i)
	{
	  test_signal[i] = 10*sin(2*M_PI*sinFreq*i) + sinFreq;
	  signal[i] = A*sin(2*M_PI*sinFreq*i);
	}

	filter.filterBuffer(signal, filtered, length);
	filter.getCoeficients(coefs, length/2+1);

	freqs[f] = sinFreq;
	double powerfilt = calculateLogPowerTemporal(filtered, length) ;
	double powersign = calculateLogPowerTemporal(signal, length);
	filterGaindB[f] = (powerfilt - powersign);
	DEBUG_STREAM("Freq: " << sinFreq  <<
		     "; filt: " << powerfilt <<
		     " dB; signal: " << powersign <<
		     " dB; gain: " << filterGaindB[f]);
    }

}

void checkFilterBankBinsOverlap(double *coefs, int length, int nbins)
{

    double coefsum[length];
    math::set(0, coefsum, length);
    for (int i=0; i<nbins; ++i)
    {
	math::add(&coefs[length*i], coefsum, length);
    }
    double mean=0, stdev=0;
    wipp::mean(coefsum, length, &mean);
    math::stddev(coefsum, length, &stdev);

    DEBUG_STREAM("coef mean: " << mean << ", stdev: " << stdev << "N: " << length);
    EXPECT_NEAR(1, mean, 0.02);
    EXPECT_NEAR(0, stdev, 0.3);

}

}
}


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

