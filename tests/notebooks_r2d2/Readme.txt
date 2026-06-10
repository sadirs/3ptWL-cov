Make sure GSL is installed on your system before compiling.

$ gcc -Wall -I/path/to/gsl/include -c IntegralCovMatrix.c
$ gcc -Wall IntegralCovMatrix.o -L/path/to/gsl/lib -lgsl -lgslcblas -lm -o IntegralCovMatrix

After compilation, execute:

$ ./IntegralCovMatrix

Python usage

To run the code in Python, import the AnalyticalCov.py module, which contains the functions needed to compute the covariance matrix.

An example of how to use these functions is provided in Covariance_example.ipynb.

