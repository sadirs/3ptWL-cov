// Use:
//#include "startrun_cosmo_lib_03.h"

//
// in: addons/addons_include/source/startrun/startrun_include_03.h
//

#ifndef _startrun_cosmo_lib_03_h
#define _startrun_cosmo_lib_03_h

//B background cosmological parameters
RPName(cmd->h,"h");
RPName(cmd->Omega0_b,"Omega0_b");
RPName(cmd->Omega0_cdm,"Omega0_cdm");
RPName(cmd->Omega0_nu,"Omega0_nu");
RPName(cmd->ns,"ns");
RPName(cmd->w,"w");
//E

RPName(cmd->zbin,"zbin");
IPName(cmd->chiQuadSteps,"chiQuadSteps");
IPName(cmd->Wg,"Wg");

SPName(cmd->prefix,"prefix",MAXLENGTHOFSTRSCMD);
SPName(cmd->fnamePS,"fnamePS",MAXLENGTHOFSTRSCMD);
SPName(cmd->fWgchi,"fWgchi",MAXLENGTHOFSTRSCMD);

#endif	// ! _startrun_cosmo_lib_03_h
