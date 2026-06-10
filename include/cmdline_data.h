/*==============================================================================
 HEADER: cmdline_data.h		[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date: 01.05.2026
 Purpose: Definitions of global variables and parameters
 Language: C
 Use: '#include "cmdline_data.h"
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#ifndef _cmdline_data_h
#define _cmdline_data_h

#include "common_defs.h"

struct cmdline_data{
    double r;
    double theta1;
    double thetap1;
    double theta2;
    double thetap2;
    double ellmin;
    double ellmax;
    string clsfile;
    int m;
    int mp;
    int ppp;

    //B Numerical parameters
    int Nr;
    double rmin;
    double rmax;
    //E

    string rootDir;
    short verbose;
    short verbose_log;
    int numthreads;
    string options;
    //E

    string version;

    string paramfile;

//B socket:
#ifdef ADDONS
#include "cmdline_data_include.h"
#endif
//E

};

#endif // ! _cmdline_data_h
