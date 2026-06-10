// Use:
//#include "wlcov_pxd_05.h"

// included in addons/addons_include/source/cballs_include_05.h

#ifndef _wlcov_pxd_05_h
#define _wlcov_pxd_05_h

//B parameters section
int get_nthreads(struct  cmdline_data* cmd, int *value)
{
    *value = cmd->numthreads;
    return SUCCESS;
}
//E

//B global parameter section
int get_integral(struct  global_data* gd, double *value)
{
    *value = gd->intval;
    return SUCCESS;
}
//E

#endif	// ! _wlcov_pxd_05_h
