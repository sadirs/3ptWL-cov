// Use:
//#include "startrun_cosmo_lib_02.h"
//
// in: addons/addons_include/source/startrun/startrun_include_02.h
//

#ifndef _startrun_cosmo_lib_02_h
#define _startrun_cosmo_lib_02_h

//B background cosmological parameters
cmd->h = GetdParam("h");
cmd->Omega0_b = GetdParam("Omega0_b");
cmd->Omega0_cdm = GetdParam("Omega0_cdm");
cmd->Omega0_nu = GetdParam("Omega0_nu");
cmd->ns = GetdParam("ns");
cmd->w = GetdParam("w");
//E

cmd->zbin = GetdParam("zbin");
cmd->chiQuadSteps = GetiParam("chiQuadSteps");
cmd->Wg = GetiParam("Wg");

cmd->prefix = GetParam("prefix");
cmd->fnamePS = GetParam("fnamePS");
cmd->fWgchi = GetParam("fWgchi");

#endif	// ! _startrun_cosmo_lib_02_h
