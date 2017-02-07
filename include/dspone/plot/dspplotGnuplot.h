#ifndef __MCA_MCAPLOT_H__
#define __MCA_MCAPLOT_H__

#include <dspone/plot/dspplotimpl.h>
#include <gnuplot-iostream.h>


namespace dsp
{


class dspPlotGnuplot : public dspPlotImpl
{
    public:
	dspPlotGnuplot(const std::string &name,
		       const std::string &title) :
	    dspPlotImpl(name, title)
	{
	    gp.set_auto_close(true);

	}

	~dspPlotGnuplot()
	{
	    gp << "quit\n";
	}

	void plot (const std::vector<boost::tuple<double, double> > &data)
	{
	    gp << "set xrange [-1:1]\nset yrange [0:15]\n";
	    // '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	    gp << "plot '-' with lines title '" << getName() <<  "' \n";
	    gp.send1d(data);
	}

	void plot(const double* values,
		  int length)
	{
	    data_.clear();
	    for (int i = 0; i < 10; ++i)
	    {
		data_.push_back(boost::tuple<double, double>(static_cast<double>(i), values[i]));
	    }
	    plot(data_);

	}

    private:
	Gnuplot gp;
	std::vector<boost::tuple<double, double> > data_;

};

}

#endif // __MCA_MCAPLOT_H_
