// Use:
//#include "cmdline_defs_cosmo_lib.h"
//
// in: addons/addons_include/include/cmdline_defs_include.h
//

#ifndef _cmdline_defs_cosmo_lib_h
#define _cmdline_defs_cosmo_lib_h

//B background cosmological parameters
"h=0.7",                                        ";reduced Hubble parameter",
"Omega0_b=0.046",                               ";baryons", ":Ob",
"Omega0_cdm=0.233",                             ";cold dark matter", ":Om",
"Omega0_nu=0.0",                                ";Massive neutrinos", ":Onu",
"ns=0.97",                                      ";Spectral index of linear P(k)",
"w=-1",                                         ";Equation of state of dark energy",
//E

//B integration
"zbin=0.5078",                                  ";zbins",
"chiQuadSteps=300",                             ";Bm moments upto mMax",":chiqst",
"Wg=0",                                         ";=0 Wg(chi) Dirac Delta at chibin=chi(zBin), =1 Wg(chi) from file fWgchi",
//E

//B I/O
"prefix=run1_",                                 ";Output file of the points analysed (default ext: .txt)", ":pre",
"fnamePS=./input/linear_pk_Takahashi_z0.txt",   ";Linear power spectrum", ":ps",
"fWgchi=./input/Wg_Takahashi_z05078.txt",       ";Takahashi file name", ":fwgchi",
//E

#endif	// ! _cmdline_defs_cosmo_lib_h
