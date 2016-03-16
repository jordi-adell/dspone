^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package dspone
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.1.8 (2014-11-17)
------------------
* Error check for negative FFT orders
  If order is below 4 (2⁴ = 16 samples) an error is printed.
  However if order is zero or negative, now an exception is thrown becuase
  it is not possible to do anything with these FFT orders.
  This will avoid the hang in refs #10184

* Contributors: Jordi Adell

0.1.7 (2014-11-10)
------------------

0.1.6 (2014-10-24)
------------------

* Patch to avoid errors when processing data channels in the case when 
  there are no input channels given, but there are output channels to be generated.

* Test with a constant signal for shorttimeprocess

* Now you can pass more than nchannels to the shorttimeprocess,
  or any of its inherit classes.
  Extra channels are considered datachannels and ar not analysed.
  Each frame can be modified from processParameterisation function
  accessing the _dataFrames structure, in the same way that
  _analysisFrames was used.


  
* Contributors: Jordi Adell
