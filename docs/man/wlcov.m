't" t
.TH wlcov 1 "April 2026" UNIX "LSST/S3 PROJECT"
.na
.nh   

.SH NAME
wlcov - code to compute the Gaussian piece of the 3PCF in a harmonic basis of scalar fields over the sphere within Limber approximation
.SH SYNOPSIS
\fBwlcov\fR [ \fIparameter_file_name\fR ] [ \fIoptions\fR ]
.sp

.SH DESCRIPTION
\fBwlcov\fR - computes the covariance matrix at a given input physical parameters.

.SH OPTIONS
All the options have the structure
.sp
$ \fIoption_name\fR=<option_value>
.sp
except option \fB-help\fR. No space between \fIoption_name\fR, '=' and <option_value>
.sp

Options and their possible values are:

.IP "\fB-help\fR" 12
By writting

.sp
$ wlcov --help
.sp

you will get the list of all parameters and their default values. An option may have an alias which is a short name of the option. If an option has an alias in the list above it comes after its description surrounded by brackets tagged with 'a:'. For example,

.sp
option_name=<option_value>	... Description ... [a: opt]
.sp
here 'opt' is the short name of the option. In the description of the options below, when an option has an alias it will be noted in the same way but before its description.

.IP "\fBparamfile\fR" 12
is the name file with the values of the input parameters. Overwrite parameters
values below. You may input this filename by only writing:
.sp
$ wlcov paramfile=parameters_input_file_name
.sp
Parameter input file may be created by hand with the editor of your choice. Comment lines start with an "#" or "%". Follow each name option with a blank-space, "=", blank-space and the option value (note the difference when using command line version instead of parameter file version). The order of the option lines does not matter.  Also you may create an example input file by executing
.sp
$ wlcov
.sp
This will run the \fBwlcov\fR code with default values and when it finish you will have in your running output directory (by default is 'Output', see below in 'rootDir' line) the file "parameters_null-wlcov-usedvalues". Now you may edit this file to adapt to your own run parameters. This file can be overwritten so it may be helpful to change this file name to whatever apropriate, and move it elsewhere.

.sp
Note that parameter alias are not allowed in a parameter file. Also behaviour of Mac and Linux plataforms are different when they reach the end of line in a text file. Then would be necessary to add a blank line at the end of the parameter file.

.IP "\fBr\fR" 12
is redshift to use.

.IP "\fBtheta1\fR" 12
physical parameter that characterize a lenght of a triangle.

.IP "\fBtheta2\fR" 12
physical parameter that characterize a lenght of a triangle.

.IP "\fBthetap1\fR" 12
physical parameter that characterize a lenght of a triangle.

.IP "\fBthetap2\fR" 12
thetap2 parameter.

.IP "\fBm\fR" 12
multipole index.

.IP "\fBmp\fR" 12
multipole index.

.IP "\fBppp\fR" 12
code parameter: points per decade in log scale of histograms.

.IP "\fBellmin\fR" 12
code parameter: lower limit of the l-integrals.

.IP "\fBellmin\fR" 12
code parameter: upper limit of the l-integrals.

.IP "\fBclsfile\fR" 12
[a: in] an input file that contains the Cl's, the weak lensing convergence angular power spectrum.

.IP "\fBrootDir\fR" 12
[a: root] gives output dir, where output files will be written. If this folder does not exist it will be created. If it does exist it will be overwritten.

.IP "\fBverbose\fR" 12
[a: verb] to print messages to standard output (stdout). There are four levels of information. When is "0", no information is written.

.IP "\fBverbose_log\fR" 12
[a: verblog] to save messages to a log file (in directory "tmp"). When is "0" no information is written.

.IP "\fBnumberThreads\fR" 12
[a: nthreads] To set the number of threads to use (OpenMP).

.IP "\fBoptions\fR" 12
[a: opt] you may give here various code behavior options, like 'smooth-pivot', 'stop', 'pre-processing', 'post-processing', etcetera.

.SH EXAMPLES

Note: It is assumed that working directory is wlcov, as was created when was git cloned. First create the executable, 'wlcov':

.sp
$ make clean; make all

.sp
cd tests

.sp
$ time ../wlcov clsfile=./input/Cls_ep2.txt ellmax=25 ppp=4 verbose=0 verbose_log=0

.sp
You will have computations written in files in a directory named with option \fBrootDir\fR. Not given here then it is used default 'Output'. If this directory does not exist, it will be created.
Also will be created a directory named "tmp" in this output folder, where a log file will be saved.

.sp
We finish with the following example:

.sp
$ time python python/kappa_cov.py

.sp
The documented Python wrapper example is available at docs/examples/python_wrapper.py.

.sp
Note: python version 3 was used with numpy and matplotlib modules installed.

.SH SEE ALSO
wlcovpy(3)

.SH COPYRIGHT
Copyright (C) 2026--
.br
Sofia Samario & Alejandro Aviles
.br
