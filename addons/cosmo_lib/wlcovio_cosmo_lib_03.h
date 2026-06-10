// Use:
//#include "wlcovio_cosmo_lib_03.h"

//
// in: addons/addons_include/source/wlcovio/wlcovio_include_03.h
//

#ifndef _wlcovio_cosmo_lib_03_h
#define _wlcovio_cosmo_lib_03_h

if (gd->fWgchiFlag==TRUE)
    free(cmd->fWgchi);
if (gd->fnamePSFlag==TRUE)
    free(cmd->fnamePS);
if (gd->prefixFlag==TRUE)
    free(cmd->prefix);

#endif	// ! _class_lib_cosmo_lib_02_h
