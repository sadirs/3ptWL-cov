// Use:
//#include "startrun_cosmo_lib_08.h"

//
// in: addons/addons_include/source/startrun/startrun_include_08.h
//

#ifndef _startrun_cosmo_lib_08_h
#define _startrun_cosmo_lib_08_h

//B background cosmological parameters
fprintf(fdout,FMTR,"h",cmd->h);
fprintf(fdout,FMTR,"Omega0_b",cmd->Omega0_b);
fprintf(fdout,FMTR,"Omega0_cdm",cmd->Omega0_cdm);
fprintf(fdout,FMTR,"Omega0_nu",cmd->Omega0_nu);
fprintf(fdout,FMTR,"ns",cmd->ns);
fprintf(fdout,FMTR,"w",cmd->w);
//E

fprintf(fdout,FMTR,"zbin",cmd->zbin);
fprintf(fdout,FMTI,"chiQuadSteps",cmd->chiQuadSteps);
fprintf(fdout,FMTI,"Wg",cmd->Wg);

fprintf(fdout,FMTT,"prefix",cmd->prefix);
fprintf(fdout,FMTT,"fnamePS",cmd->fnamePS);
fprintf(fdout,FMTT,"fWgchi",cmd->fWgchi);

#endif	// ! _startrun_octree_smoothing_08_h
