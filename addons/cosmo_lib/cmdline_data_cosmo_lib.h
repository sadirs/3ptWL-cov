// Use:
//#include "cmdline_data_cosmo_lib.h"

//
// in: addons/addons_include/include/cmdline_data_include.h
//

#ifndef _cmdline_data_cosmo_lib_h
#define _cmdline_data_cosmo_lib_h

//B background cosmological parameters
double h;
double Omega0_b;
double Omega0_cdm;
double Omega0_nu;
double ns;
double w;
//E

//B integration
double zbin;
int chiQuadSteps;
int Wg;
//E

//B I/O
string prefix;
string fnamePS;
string fWgchi;
//E

#endif	// ! _cmdline_data_cosmo_lib_h
