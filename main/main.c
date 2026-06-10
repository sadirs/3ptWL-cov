/*==============================================================================
 NAME: main.c                [wlcov]
 Written by: Mario A. Rodriguez-Meza
 to drive 3PCF.CovMatrix-WL written by Sofia Samario & Alejandro Aviles
 Starting date: 01.05.2026
 Purpose: Main routine
 Language: C
 Major revision: 01.05.2026
 ===============================================================================
 Use: wlcov --help (or -h)
 Input:     Command line parameters, Parameters file, data catalogs
 Output: several histograms containing...
 Units:
 History:
 Acknowledgements:
 Comments and notes:
 References: Zeno project, Gadget, CLASS, NR, GSL...
 github: https://github.com/Ssamario/3PCF.CovMatrix-WL &
         https://github.com/rodriguezmeza/wlcov
 Publication: cite: (ArXiv ePrint: 2506.19811)
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

#define global
#include <stdio.h>
#include "globaldefs.h"
#include "cmdline_defs.h"

//B ==================== main ======================
/*
 Main routine:
 
 This routine is in charge of main computational flow
    as it is explained below in the comments.

 Arguments:
    * `argc`: Input: int
    * `argv`: Input: string array
 Return (the error status):
    int SUCCESS or FAILURE
 */
int main (int argc, char *argv[])
{
    struct cmdline_data cmd;                        // share command parameters
    struct global_data gd;                          // share global parameters

    gd.cpuinit = CPUTIME;                           // init register of cpu time
    gd.cpurealinit = rcpu_time();                   // init register of real time

    InitParam(argv, defv);                          // init command parameters
                                                    //  structure
    StartRun(&cmd, &gd, argv[0],                    // get parameters and
             HEAD1, HEAD2, HEAD3);                  //  init global structure
                                                    //  and do other useful
                                                    //  process, like param check,
                                                    //  read data points to analyze
    MainLoop(&cmd, &gd);                            // make tree and search data
    EndRun(&cmd, &gd);                              // close streams and free mem

    return SUCCESS;
}

