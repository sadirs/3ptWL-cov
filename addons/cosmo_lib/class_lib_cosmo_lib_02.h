// Use:
//#include "class_lib_cosmo_lib_02.h"

//
// in: addons/addons_include/source/addons/class_lib_include_02.h
//

#ifndef _class_lib_cosmo_lib_02_h
#define _class_lib_cosmo_lib_02_h

//B background cosmological parameters
cmd->h = 0.7;
cmd->Omega0_b=0.046;
cmd->Omega0_cdm=0.233;
cmd->Omega0_nu=0.0;
cmd->ns=0.97;
cmd->w=-1.0;
//E

cmd->zbin=0.5078;
cmd->chiQuadSteps=300;
cmd->Wg=0;

cmd->prefix = "run1_";
cmd->fnamePS = "./input/linear_pk_Takahashi_z0.txt";
cmd->fWgchi = "./input/Wg_Takahashi_z05078.txt";

#endif	// ! _class_lib_cosmo_lib_02_h
