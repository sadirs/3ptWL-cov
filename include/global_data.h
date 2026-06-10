/*==============================================================================
 HEADER: global_data.h		[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date: 01.05.2026
 Purpose: Definitions of global variables and parameters
 Language: C
 Use: '#include "global_data.h"
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#ifndef _global_data_h
#define _global_data_h

#include "common_defs.h"

struct global_data{
    double cpuinit;
    long cpurealinit;                               // get time of the day
    clock_t time;

    string headline0;
    string headline1;
    string headline2;
    string headline3;
    FILE *outlog;
    char mode[2];
    
    int fsky;
    int n_data;

    //B memory flags
    bool cmd_allocated;
    bool gd_allocated;
    bool rootDirFlagFree;
    bool optionsFlag;
    bool flagPrint;
    bool rootDirFlag;
    bool clsfileFlag;
    //E

    real cputotalinout;
    real cputotal;
    INTEGER bytes_tot;
    char logfilePath[MAXLENGTHOFFILES];
    char tmpDir[MAXLENGTHOFFILES];
    
    double ell_data[n_data_max],Cls_data[n_data_max];
    
    double intval;

//B socket:
#ifdef ADDONS
#include "global_data_include.h"
#endif
//E

};

#endif // ! _global_data_h

