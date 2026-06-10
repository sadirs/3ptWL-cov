/*==============================================================================
 MODULE: wlcovio.c		[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date:	01.05.2026
 Purpose: Routines to drive input and output data
 Language: C
 Use:
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#include "globaldefs.h"


/*
 EndRun routine:

 To be called in main:
    EndRun(&cmd, &gd);

 This routine is in charge of closing log file, printing a summary
    of the run and freeing the allocated memory

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
 Return (the error status):
    int SUCCESS or FAILURE
 */
int EndRun(struct cmdline_data* cmd, struct  global_data* gd)
{
    real cpuTotal = CPUTIME - gd->cpuinit;
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\nFinal CPU time : %lf %s\n",
                           cpuTotal, PRNUNITOFTIMEUSED);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "Final real time: %ld",
                           (rcpu_time()-gd->cpurealinit));
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           " %s\n\n", PRNUNITOFTIMEUSED); // Only work this way

    if (cmd->verbose_log>0)
        fclose(gd->outlog);

    EndRun_FreeMemory(cmd, gd);

    return SUCCESS;
}

//
// We must check the order of memory allocation and freeing!!!
//
int EndRun_FreeMemory(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    if (gd->gd_allocated == TRUE)
        EndRun_FreeMemory_gd(cmd, gd);

    if (gd->cmd_allocated == TRUE)
        EndRun_FreeMemory_cmd(cmd, gd);

    return SUCCESS;
}

int EndRun_FreeMemory_gd(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    gd->gd_allocated = FALSE;

    return SUCCESS;
}

int EndRun_FreeMemory_cmd(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    //B be aware of the allocation order...
    if (gd->optionsFlag == TRUE)
        free(cmd->options);
    if (gd->rootDirFlagFree==TRUE)
        free(cmd->rootDir);
    if (gd->clsfileFlag==TRUE)
        free(cmd->clsfile);

//B socket:
#ifdef ADDONS
#include "wlcovio_include_03.h"
#endif
//E

    //E

    gd->cmd_allocated = FALSE;

    return SUCCESS;
}

