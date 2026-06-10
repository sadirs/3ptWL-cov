// Use:
//#include "protodefs_pxd.h"

#ifndef _protodefs_pxd_h
#define _protodefs_pxd_h

//B PXD functions

//B flags
int get_histograms_allocated(struct global_data* gd, short *value);
//E

//B parameters section
int get_nthreads(struct  cmdline_data* cmd,
              int *value);
//E

//B global parameter section
int get_integral(struct  global_data* gd, double *value);
//E

#endif	// ! _protodefs_pxd_h
