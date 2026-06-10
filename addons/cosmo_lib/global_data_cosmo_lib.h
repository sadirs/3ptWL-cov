// Use:
//#include "global_data_cosmo_lib.h"

//
// in: addons/addons_include/include/global_data_include.h
//

#ifndef _global_data_cosmo_lib_h
#define _global_data_cosmo_lib_h

double Omega0_m;        // -> Omm
double Omega0_de;       // -> Omw

double Dpz0;

double k_data[n_data_max];
double pkz0_data[n_data_max];

bool prefixFlag;
bool fnamePSFlag;
bool fWgchiFlag;

double sigma8;

int n_ps_data;
int n_chi_data;
double chi_data[n_chi_data_max];
double Wg_chi_data[n_chi_data_max];

#endif	// ! _global_data_cosmo_lib_h
