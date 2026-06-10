// Use:
//#include "protodefs_cosmo_lib.h"

//
// in: addons/addons_include/include/protodefs_include.h
//

#ifndef _protodefs_cosmo_lib_h
#define _protodefs_cosmo_lib_h

int Initial(struct cmdline_data* cmd, struct  global_data* gd);
int background(struct cmdline_data* cmd, struct  global_data* gd,
               double zBin);
int Cls_kappa(struct cmdline_data* cmd, struct  global_data* gd);

#endif	// ! _protodefs_cosmo_lib_h
