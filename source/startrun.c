/*==============================================================================
 MODULE: startrun.c				[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date: 01.05.2026
 Purpose: routines to initialize the main code
 Language: C
 Use: 'StartRun();'
 Mayor revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// We must check the order of memory allocation and deallocation!!!
// Here and in EndRun in cballsio.c
//

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#include "globaldefs.h"

#ifdef CLASSLIB
#define WLCOV_FAIL(cmd, ...)                                      \
    do {                                                          \
        snprintf((cmd)->error_message, _ERRORMSGSIZE_, __VA_ARGS__); \
        return FAILURE;                                           \
    } while (0)
#else
#define WLCOV_FAIL(cmd, ...) error(__VA_ARGS__)
#endif

local int ReadParameterFile(struct  cmdline_data*, struct  global_data*,
                             char *);
local int startrun_parameterfile(struct  cmdline_data*, struct  global_data*);
local int startrun_cmdline(struct  cmdline_data*, struct  global_data*);
local void ReadParametersCmdline(struct  cmdline_data*, struct  global_data*);
local void ReadParametersCmdline_short(struct  cmdline_data*, 
                                       struct  global_data*);
local int CheckParameters(struct  cmdline_data*, struct  global_data*);

//B I/O directories:
local int setFilesDirs_log(struct cmdline_data*, struct global_data* gd);
local int setFilesDirs(struct cmdline_data*, struct global_data* gd);
//E
local int print_make_info(struct cmdline_data* cmd,
                     struct  global_data* gd);

//B socket:
#ifdef ADDONS
#include "startrun_include_00.h"
#endif
//E

/*
 StartRun routine:

 To be called in main:
 StartRun(&cmd, &gd, argv[0], HEAD1, HEAD2, HEAD3);
 
 This routine is in charge of setting all global structures in order to
    the comutation process run smoothly with all parameters given
    by the user, set and checked.

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
    * `head0`: Input: string
    * `head1`: Input: string
    * `head2`: Input: string
    * `head3`: Input: string
 Return (the error status):
    int SUCCESS or FAILURE
 */
#ifndef CLASSLIB
int StartRun(struct  cmdline_data* cmd, struct  global_data* gd, 
             string head0, string head1, string head2, string head3)
{
    string routineName = "StartRun";
    double cpustart = CPUTIME;

    gd->headline0 = head0; gd->headline1 = head1;
    gd->headline2 = head2; gd->headline3 = head3;

    printf("\n%s\n%s: %s\n\t %s\n",
           gd->headline0, gd->headline1, gd->headline2, gd->headline3);
    printf("Version = %s\n", getversion());

    //B move all these to Startrun_Common... or make an appropriate change
    gd->cmd_allocated = FALSE;
    gd->cputotalinout = 0.;
    gd->cputotal = 0.;
    gd->bytes_tot = 0;
    //E

    cmd->paramfile = GetParam("paramfile");
    if (*(cmd->paramfile) == '-') {
        WLCOV_FAIL(cmd, "bad parameter %s\n", cmd->paramfile);
    }
    
    if (!strnull(cmd->paramfile))
		startrun_parameterfile(cmd, gd);
    else
		startrun_cmdline(cmd, gd);

    gd->bytes_tot += sizeof(struct  global_data);
    gd->bytes_tot += sizeof(struct cmdline_data);
    verb_print_min_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                "\n%s: Total allocated %g MByte storage so far.\n",
                        routineName, gd->bytes_tot*INMB);

    class_call_cballs(SetNumberThreads(cmd), errmsg, errmsg);
    gd->cputotalinout += CPUTIME - cpustart;
    verb_print_min_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                        "\n%s CPU time: %g %s\n",
                        routineName, CPUTIME - cpustart, PRNUNITOFTIMEUSED);

    return SUCCESS;
}

#else // ! CLASSLIB

#include "input.h"

int StartRun(struct  cmdline_data* cmd, struct  global_data* gd,
             string head0, string head1, string head2, string head3)
{
    string routineName = "StartRun";
    struct file_content fc;

    double cpustart = CPUTIME;

    gd->headline0 = head0; gd->headline1 = head1;
    gd->headline2 = head2; gd->headline3 = head3;
    printf("\n%s\n%s: %s\n\t %s\n",
           gd->headline0, gd->headline1, gd->headline2, gd->headline3);
    printf("Version = %s\n", getversion());

    //B move all these to Startrun_Common... or make an appropriate change
    gd->cmd_allocated = FALSE;
    gd->cputotalinout = 0.;
    gd->cputotal = 0.;
    gd->bytes_tot = 0;
    //E

    cmd->paramfile = GetParam("paramfile");
    if (*(cmd->paramfile) == '-') {
        WLCOV_FAIL(cmd, "bad parameter %s\n", cmd->paramfile);
    }

    if (!strnull(cmd->paramfile)) {
        class_call_cballs(input_find_file(cmd, gd, cmd->paramfile, &fc, errmsg),
                          errmsg, errmsg);

        if (input_read_from_file(cmd, gd, &fc, errmsg) == FAILURE) {
            parser_free(&fc);
            class_call_cballs(FAILURE, errmsg, errmsg);
        }

        class_call_cballs(parser_free(&fc), errmsg, errmsg);
        
    } else {
        startrun_cmdline(cmd, gd);
    }

    if (!strnull(cmd->paramfile))
        class_call_cballs(StartRun_Common(cmd, gd), errmsg, errmsg);

        if (!strnull(cmd->paramfile))
            PrintParameterFile(cmd, gd, cmd->paramfile);

    gd->bytes_tot += sizeof(struct  global_data);
    gd->bytes_tot += sizeof(struct cmdline_data);
    verb_print(cmd->verbose,
               "\n%s: Total allocated %g MByte storage so far.\n",
               routineName, gd->bytes_tot*INMB);

    class_call_cballs(SetNumberThreads(cmd), errmsg, errmsg);

    gd->cputotalinout += CPUTIME - cpustart;
    verb_print(cmd->verbose, "\n%s CPU time: %g %s\n",
               routineName, CPUTIME - cpustart, PRNUNITOFTIMEUSED);

    return SUCCESS;
}
#endif // ! CLASSLIB


local int startrun_parameterfile(struct  cmdline_data* cmd,
                                 struct  global_data* gd)
{
    class_call_cballs(ReadParameterFile(cmd, gd, cmd->paramfile), errmsg, errmsg);
    ReadParametersCmdline_short(cmd, gd);

//B socket:
#ifdef ADDONS
#include "startrun_include_01.h"
#endif
//E

	StartRun_Common(cmd, gd);
    PrintParameterFile(cmd, gd, cmd->paramfile);

    return SUCCESS;
}


#define parameter_null	"parameters_null-wlcov"

//B Section for reading parameters from the command line

local int startrun_cmdline(struct  cmdline_data* cmd, struct  global_data* gd)
{
	ReadParametersCmdline(cmd, gd);
	StartRun_Common(cmd, gd);
//    if (gd->flagPrint==TRUE && gd->rootDirFlag==TRUE) {
        PrintParameterFile(cmd, gd, parameter_null);
//    }

    return SUCCESS;
}

local void ReadParametersCmdline(struct  cmdline_data* cmd, 
                                 struct  global_data* gd)
{
// Every item in cmdline_defs.h must have an item here::

    //B Parameters related to background cosmology
    cmd->r = GetdParam("r");
    cmd->theta1 = GetdParam("theta1");
    cmd->thetap1 = GetdParam("thetap1");
    cmd->theta2 = GetdParam("theta2");
    cmd->thetap2 = GetdParam("thetap2");
    cmd->m = GetiParam("m");
    cmd->mp = GetiParam("mp");
    cmd->ppp = GetiParam("ppp");
    //E

    //B Numerical parameters
    cmd->Nr = GetiParam("Nr");
    cmd->rmin = GetdParam("rmin");
    cmd->rmax = GetdParam("rmax");
    //E

    //B Parameters for I/O
    cmd->rootDir = GetParam("rootDir");
    cmd->clsfile = GetParam("clsfile");
    cmd->ellmax = GetdParam("ellmax");
    cmd->ellmin = GetdParam("ellmin");
    //E

    //B Miscellaneous parameters
    cmd->verbose = GetiParam("verbose");
    cmd->verbose_log = GetiParam("verbose_log");
#ifdef OPENMPCODE
    cmd->numthreads = GetiParam("numberThreads");
#else
    cmd->numthreads = GetiParam("numberThreads");
    cmd->numthreads = 1;
#endif
    cmd->options = GetParam("options");
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_02.h"
#endif
//E
}

local void ReadParametersCmdline_short(struct  cmdline_data* cmd, struct  global_data* gd)
{
//B Here add parameters needed to be read after reading parameter file
//    cmd->ellmax = GetdParam("ellmax");
//E
}

//E

#undef parameter_null

//B Section of parameter reading from a file
local int ReadParameterFile(struct  cmdline_data* cmd,
                             struct  global_data* gd, char *fname)
{
// Every item in cmdline_defs.h must have an item here::
#define DOUBLE 1
#define STRING 2
#define INT 3
#define LONG 6
#define BOOLEAN 4
#define MAXTAGS 300
#define MAXCHARBUF 1024

    string routineName = "ReadParameterFile";
    FILE *fd;

  int  i,j,nt;
  int  id[MAXTAGS];
  void *addr[MAXTAGS];
  char tag[MAXTAGS][50];
  int  errorFlag=0;

    size_t str_size[MAXTAGS];

    int input_verbose = 2;
    verb_print(input_verbose, "\nparsing input parameters...\n");

  nt=0;

    //B Parameters related to the cosmological background
    RPName(cmd->r,"r");
    RPName(cmd->theta1,"theta1");
    RPName(cmd->thetap1,"thetap1");
    RPName(cmd->theta2,"theta2");
    RPName(cmd->thetap2,"thetap2");
    IPName(cmd->m,"m");
    IPName(cmd->mp,"mp");
    IPName(cmd->ppp,"ppp");
    //E

    //B Numerical parameters
    IPName(cmd->Nr,"Nr");
    RPName(cmd->rmin,"rmin");
    RPName(cmd->rmax,"rmax");
    //E

    //B Parameters to control the I/O file(s)
    // Input catalog parameters
    SPName(cmd->clsfile,"clsfile",MAXLENGTHOFSTRSCMD);
    // Output parameters
    SPName(cmd->rootDir,"rootDir",MAXLENGTHOFSTRSCMD);
    //E

    //B Set of parameters needed to integrate
    RPName(cmd->ellmax,"ellmax");
    RPName(cmd->ellmin,"ellmin");
    //E

    //B Miscellaneous parameters
    IPName(cmd->verbose,"verbose");
    IPName(cmd->verbose_log,"verbose_log");
#ifdef OPENMPCODE
    IPName(cmd->numthreads,"numberThreads");
#else
    IPName(cmd->numthreads,"numberThreads");
    cmd->numthreads = 1;
#endif
    SPName(cmd->options,"options",MAXLENGTHOFSTRSCMD);
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_03.h"
#endif
//E

//B
#ifndef _LINE_LENGTH_MAX_
#define _LINE_LENGTH_MAX_ 1024
#endif
#define _ARGUMENT_LENGTH_MAX_ 1024
        char line[_LINE_LENGTH_MAX_];
        char name[_ARGUMENT_LENGTH_MAX_];
        char value[_ARGUMENT_LENGTH_MAX_];
        char * phash;
        char * pequal;
        char * left;
        char * right;
//E

    if((fd=fopen(fname,"r"))) {
        while (fgets(line, MAXCHARBUF, fd) != NULL) {
//B
            pequal=strchr(line,'=');
            if (pequal == NULL)
                continue;
            phash=strchr(line,'#');
            if ((phash != NULL) && (phash-pequal<2))
                continue;
            phash=strchr(line,'%');
            if ((phash != NULL) && (phash-pequal<2))
                continue;

            left=line;
            while (left[0]==' ') {
              left++;
            }
            if(left[0]=='\'' || left[0]=='\"'){
              left++;
            }
            right=pequal-1;

            while (right >= left && right[0] == ' ') {
              right--;
            }

            if (right >= left && (right[0] == '\'' || right[0] == '\"')) {
              right--;
            }
            

            if (right-left < 0) {
                fprintf(stdout,
        "Error in file %s: there is no variable name before '=' in line: '%s'\n",
                    fname, line);
                errorFlag=1;
                continue;
            }

            if ((size_t)(right - left + 1) >= sizeof(name)) {
                if (fd != NULL) fclose(fd);
                WLCOV_FAIL(cmd, "%s: parameter name too long\n", routineName);
            }

            memcpy(name, left, (size_t)(right - left + 1));
            name[right - left + 1] = '\0';


            left = pequal+1;
            while (left[0]==' ') {
              left++;
            }

            if (phash == NULL)
              right = line+strlen(line)-1;
            else
              right = phash-1;

            while (right >= left && right[0] <= ' ') {
                right--;
            }

            if (right < left)
                continue;

            if ((size_t)(right - left + 1) >= sizeof(value)) {
                if (fd != NULL) fclose(fd);
                WLCOV_FAIL(cmd, "%s: parameter value too long\n", routineName);
            }
            
            memcpy(value, left, (size_t)(right - left + 1));
            value[right - left + 1] = '\0';
//E

            for(i=0,j=-1;i<nt;i++)
                if(strcmp(name,tag[i])==0) {
                    j=i;
                    tag[i][0]=0;
                    break;
                }
            if(j>=0) {
                switch(id[j]) {
                    case DOUBLE:
                        *((double*)addr[j])=atof(value);
                        break;

                    case STRING:
                        if (copy_checked((char *)addr[j], str_size[j], value, name) != 0) {
                            if (fd != NULL) fclose(fd);
                            WLCOV_FAIL(cmd, "%s: string parameter '%s' too long\n",
                                       routineName, name);
                        }
                        break;

                    case INT:
                        *((int*)addr[j])=atoi(value);
                        break;
                    case LONG:
                        *((long*)addr[j])=atol(value);
                        break;
                    case BOOLEAN:
                        if (strchr("tTyY1", *value) != NULL) {
                            *((bool*)addr[j])=TRUE;
                        } else
                            if (strchr("fFnN0", *value) != NULL)  {
                                *((bool*)addr[j])=FALSE;
                            } else {
                                if (fd != NULL) fclose(fd);
                                WLCOV_FAIL(cmd, "%s: %s=%s not bool\n", routineName, name, value);
                            }
                        break;
                }
            } else {
                fprintf(stdout, "\n%s: Error in file %s: Tag '%s' %s\n",
                        routineName, fname, name,
                        "not allowed or multiple defined...");
                errorFlag=1;
            }
        } // ! while loop
        fclose(fd);
    } else {
        fprintf(stdout,"Parameter file %s not found.\n", fname);
        errorFlag=2;
        WLCOV_FAIL(cmd, "Parameter file %s not found.\n", fname);
    }

    if (errorFlag == 1)
        WLCOV_FAIL(cmd, "%s: parameter file '%s' contains unknown or duplicated tags\n",
                   routineName, fname);
    
    for(i=0;i<nt;i++) {
        if(*tag[i]) {
            if (cmd->verbose>2)
                fprintf(stdout,
                    "Warning! I miss a value for tag '%s' in parameter file '%s'.\n",
                    tag[i],fname);
            switch(id[i]) {
                case DOUBLE:
                    *((double*)addr[i])=GetdParam(tag[i]);
                    break;
                    
                case STRING:
                    if (copy_checked((char *)addr[i], str_size[i], GetParam(tag[i]), tag[i]) != 0)
                        WLCOV_FAIL(cmd, "%s: default string parameter '%s' too long\n",
                                   routineName, tag[i]);
                    break;

                case INT:
                    *((int*)addr[i])=GetiParam(tag[i]);
                    break;
                case LONG:
                    *((long*)addr[i])=GetlParam(tag[i]);
                    break;
                case BOOLEAN:
                    *((bool*)addr[i])=GetbParam(tag[i]);
                    break;
            }
            errorFlag=1;
        }
    }

#undef DOUBLE
#undef STRING
#undef INT
#undef BOOLEAN
#undef MAXTAGS
#undef MAXCHARBUF
    
    return SUCCESS;
}
//E

int StartRun_Common(struct  cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "StartRun_Common";

#ifndef USEGSL
#error `USEGSL` is not defined. Switch it on in Makefile_settings
#endif

    if (gd->cmd_allocated != TRUE)
        gd->cmd_allocated = TRUE;

    gd->gd_allocated = FALSE;

    if (strlen(cmd->rootDir)==0 || strnull(cmd->rootDir))
        gd->rootDirFlag = FALSE;
    else
        gd->rootDirFlag = TRUE;

    gd->flagPrint = TRUE;

    if (scanopt(cmd->options, "make-info"))
        print_make_info(cmd, gd);

//B socket:
#ifdef ADDONS
#include "startrun_include_04.h"
#endif
//E

    class_call_cballs(StartOutput(cmd, gd), errmsg, errmsg);
    class_call_cballs(setFilesDirs(cmd, gd), errmsg, errmsg);
    class_call_cballs(setFilesDirs_log(cmd, gd), errmsg, errmsg);

    gd->mode[0] = 'w';
    gd->mode[1] = '\0';
    if (cmd->verbose_log>0) {                       // gd->outlog is defined
        if (!(gd->outlog = fopen(gd->logfilePath, gd->mode))) {
            WLCOV_FAIL(cmd, "\n%s: error opening file '%s'\n",
                       routineName, gd->logfilePath);
        }
    }

    class_call_cballs(CheckParameters(cmd, gd), errmsg, errmsg);
    class_call_cballs(startrun_memoryAllocation(cmd, gd), errmsg, errmsg);

    gd->fsky = 1;                                   // should be for full-sky
                                                    //  move to cmd structure
    double rArcMin,theta1ArcMin, theta2ArcMin, thetap1ArcMin, thetap2ArcMin,radToArcMin;
    
    radToArcMin = 180.0*60.0 / M_PI;
    rArcMin       = cmd->r *radToArcMin;
    theta1ArcMin   = cmd->theta1*radToArcMin;
    theta2ArcMin   = cmd->theta2*radToArcMin;
    thetap1ArcMin  = cmd->thetap1*radToArcMin;
    thetap2ArcMin  = cmd->thetap2*radToArcMin;

    //B Print the values: given and in arcmin
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "r= %.7f  = %.2f arcmin\n",
                           cmd->r, rArcMin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "theta1= %.7f = %.2f arcmin\n",
                           cmd->theta1, theta1ArcMin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "theta2= %.7f = %.2f arcmin\n",
                           cmd->theta2, theta2ArcMin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "thetap1= %.7f = %.2f arcmin\n",
                           cmd->thetap1, thetap1ArcMin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "thetap2= %.7f = %.2f arcmin\n",
                           cmd->thetap2, thetap2ArcMin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "ellmin= %.3f\n", cmd->ellmin);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "ellmax= %.3f\n", cmd->ellmax);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "clsfile: %s\n", cmd->clsfile);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "m= %d\n", cmd->m);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "mp= %d\n", cmd->mp);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "ppp (points_per_period)= %d\n", cmd->ppp);
    //E

    //B reading Cls file
    FILE *fp;
    double ell_data, Cls_data;
    int line;
    gd->n_data=0;
    fp=fopen(cmd->clsfile,"r");
    if(fp!=NULL){
        while((line=fscanf(fp, "%lf %lf", &ell_data, &Cls_data))!=EOF) {
            if (line != 2) {
                fclose(fp);
                WLCOV_FAIL(cmd, "\nCls file must have two columns of values... exiting\n\n");
            }

            if (gd->n_data >= n_data_max) {
                fclose(fp);
                WLCOV_FAIL(cmd, "n_data_max should be larger than %s\n",
                           "the number of data lines");
            }

            gd->ell_data[gd->n_data] = ell_data;
            gd->Cls_data[gd->n_data] = Cls_data;
            gd->n_data++;
        }  // ell   Ckappa

        fclose(fp);
    } else
        WLCOV_FAIL(cmd, "\nnot given a Cls file... exiting\n\n");

    if (gd->n_data < 2)
        WLCOV_FAIL(cmd, "\nCls file must have at least two rows of values... exiting\n\n");

    for (int i=0; i<gd->n_data; i++) {
        if (!isfinite(gd->ell_data[i]))
            WLCOV_FAIL(cmd, "\nCls file must have finite ell values... exiting\n\n");
        if (!isfinite(gd->Cls_data[i]))
            WLCOV_FAIL(cmd, "\nCls file must have finite Cls values... exiting\n\n");
        if (gd->ell_data[i] <= 0.)
            WLCOV_FAIL(cmd, "\nCls file must have ell values positive... exiting\n\n");
        if (gd->Cls_data[i] <= 0.)
            WLCOV_FAIL(cmd, "\nCls file must have positive Cls values... exiting\n\n");
    }

    for (int i=0; i<gd->n_data-1; i++) {
        if (gd->ell_data[i] >= gd->ell_data[i+1])
            WLCOV_FAIL(cmd, "\nCls file must have ell values in ascending order... %s\n\n",
                       "exiting");
    }
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_05.h"
#endif
//E

    gd->gd_allocated = TRUE;

    return SUCCESS;
}


//B Section of parameter check
local int CheckParameters(struct  cmdline_data* cmd, struct  global_data* gd)
{
// If it is necessary: an item in cmdline_defs.h must have an item here::
    string routineName = "CheckParameters";

    if (cmd->r <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: r (%g) must be positive.\n", routineName, cmd->r);

    if (cmd->theta1 <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: theta1 (%g) must be positive.\n", routineName, cmd->theta1);

    if (cmd->thetap1 <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: thetap1 (%g) must be positive.\n", routineName, cmd->thetap1);

    if (cmd->theta2 <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: theta2 (%g) must be positive.\n", routineName, cmd->theta2);

    if (cmd->thetap2 <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: thetap2 (%g) must be positive.\n", routineName, cmd->thetap2);

    if (cmd->ellmin <= 0.0)
        WLCOV_FAIL(cmd, "\n%s: ellmin (%g) must be positive.\n", routineName, cmd->ellmin);

    if (cmd->ellmax < 0.0)
        WLCOV_FAIL(cmd, "\n%s: ellmax (%g) can not be less than 0.\n",
                   routineName, cmd->ellmax);

    if (cmd->ellmax <= cmd->ellmin)
        WLCOV_FAIL(cmd, "\n%s: ellmax (%g) must be greater than ellmin (%g)\n",
                   routineName, cmd->ellmax, cmd->ellmin);

    if (cmd->m < 0)
        WLCOV_FAIL(cmd, "\n%s: m parameter (%d) can not be less than 0.\n",
                   routineName, cmd->m);

    if (cmd->mp < 0)
        WLCOV_FAIL(cmd, "\n%s: mp parameter (%d) can not be less than 0.\n",
                   routineName, cmd->mp);

    if (cmd->ppp <= 0)
        WLCOV_FAIL(cmd, "\n%s: ppp parameter (%d) must be greater than 0.\n",
                   routineName, cmd->ppp);

    if (cmd->verbose < 0 || cmd->verbose > 3)
        WLCOV_FAIL(cmd, "\n%s: verbose parameter (%d) can not be less than 0 %s\n",
                   routineName, cmd->verbose, "and not greater than 3.");
    if (cmd->verbose_log < 0 || cmd->verbose_log > 3)
        WLCOV_FAIL(cmd, "\n%s: verbose_log parameter (%d) can not be less than 0 %s\n",
                   routineName, cmd->verbose_log, "and not greater than 3.");

    if (cmd->numthreads < 1)
        WLCOV_FAIL(cmd, "\n%s: numthreads parameter (%d) can not be less than 1.\n",
                   routineName, cmd->numthreads);

    //B Numerical parameters
    if (cmd->Nr <= 1)
        WLCOV_FAIL(cmd, "\n%s: Nr parameter (%d) can not be less than 2\n",
                   routineName, cmd->Nr);
    if (cmd->rmin <= 0)
        WLCOV_FAIL(cmd, "\n%s: rmin parameter (%g) must be positive\n",
                   routineName, cmd->rmin);
    if (cmd->rmax <= 0)
        WLCOV_FAIL(cmd, "\n%s: rmax parameter (%g) must be positive\n",
                   routineName, cmd->rmax);
    if (cmd->rmax <= cmd->rmin)
        WLCOV_FAIL(cmd, "\n%s: rmax parameter (%g) must be greater than rmin (%g)\n",
                   routineName, cmd->rmax, cmd->rmin);
    //E

//B socket:
#ifdef ADDONS
#include "startrun_include_07.h"
#endif
//E

    return SUCCESS;
}
//E


#define FMTT    "%-35s = %s\n"
#define FMTTS    "%-35s = \"%s\"\n"
#define FMTI    "%-35s = %d\n"
#define FMTIL    "%-35s = %ld\n"
#define FMTR	"%-35s = %g\n"

//B Section of parameter writing to a file
int PrintParameterFile(struct  cmdline_data *cmd,
                       struct  global_data* gd, char *fname)
{
// Every item in cmdline_defs.h must have an item here::
    string routineName = "PrintParameterFile";

    int nwrite;

    FILE *fdout;
    char buf[BUFFERSIZE];
    int  errorFlag=0;

    if (gd->flagPrint==TRUE && gd->rootDirFlag==TRUE) {
        //B Look for "/" if fname is composed: path and filename
        int ndefault = 0;
        int ipos;
        char *dp=NULL;
        for (int i=0; i< strlen(fname); i++) {
            if(fname[i] == '/') {
                ipos = i+1;
                ndefault++;
            }
        }
        
        if (ndefault == 0) {
            nwrite = snprintf(buf, sizeof(buf), "%s/%s%s",
                              cmd->rootDir, fname, "-usedvalues");
            if (nwrite < 0 || (size_t)nwrite >= sizeof(buf))
                WLCOV_FAIL(cmd, "\n%s: output path too long\n",routineName);
        } else {
            size_t dplen = strlen(fname) - ipos;
            dp = (char*) malloc((dplen + 1) * sizeof(char));
            if (dp == NULL)
                WLCOV_FAIL(cmd, "\n%s: not enough memory\n",routineName);
            nwrite = snprintf(dp, dplen + 1, "%s", fname + ipos);
            if (nwrite < 0 || (size_t)nwrite >= dplen + 1) {
                free(dp);
                WLCOV_FAIL(cmd, "\n%s: filename too long\n",routineName);
            }

            verb_print_q(3,cmd->verbose,
                         "PrintParameterFile: '/' counts %d pos %d and %s\n",
                         ndefault, ipos, dp);
            nwrite = snprintf(buf, sizeof(buf), "%s/%s%s",
                              cmd->rootDir, dp, "-usedvalues");
            if (nwrite < 0 || (size_t)nwrite >= sizeof(buf)) {
                free(dp);
                WLCOV_FAIL(cmd, "\n%s: output path too long\n",routineName);
            }

        }
        //E
        
        if(!(fdout=fopen(buf,"w"))) {
            fprintf(stdout,"error opening file '%s' \n",buf);
            errorFlag=1;
        } else {
            fprintf(fdout,FMTR,"r",cmd->r);
            fprintf(fdout,FMTR,"theta1",cmd->theta1);
            fprintf(fdout,FMTR,"thetap1",cmd->thetap1);
            fprintf(fdout,FMTR,"theta2",cmd->theta2);
            fprintf(fdout,FMTR,"thetap2",cmd->thetap2);
            fprintf(fdout,FMTI,"m",cmd->m);
            fprintf(fdout,FMTI,"mp",cmd->mp);
            fprintf(fdout,FMTI,"ppp",cmd->ppp);

            //B Numerical parameters
            fprintf(fdout,FMTI,"Nr",cmd->Nr);
            fprintf(fdout,FMTR,"rmin",cmd->rmin);
            fprintf(fdout,FMTR,"rmax",cmd->rmax);
            //E

            //B Parameters to control the I/O file(s)
            // Input catalog parameters
            fprintf(fdout,FMTT,"clsfile",cmd->clsfile);
            // Output parameters
            fprintf(fdout,FMTT,"rootDir",cmd->rootDir);
            //E

            //B Set of parameters needed to integrate
            fprintf(fdout,FMTR,"ellmax",cmd->ellmax);
            fprintf(fdout,FMTR,"ellmin",cmd->ellmin);
            //E

            //B Miscellaneous parameters
            fprintf(fdout,FMTI,"verbose",cmd->verbose);
            fprintf(fdout,FMTI,"verbose_log",cmd->verbose_log);
            fprintf(fdout,FMTI,"numberThreads",cmd->numthreads);
            fprintf(fdout,FMTT,"options",cmd->options);
            //E
            
//B socket:
#ifdef ADDONS
#include "startrun_include_08.h"
#endif
//E
            
            fprintf(fdout,"\n\n");
            fclose(fdout);
        }
        
        if(errorFlag) {
            WLCOV_FAIL(cmd, "\n%s: errorFlag\n",routineName);
        }
        
        if (dp != NULL)
            free(dp);
    } // ! gd->flagPrint==TRUE && gd->rootDirFlag==TRUE

    return SUCCESS;
}
//E

#undef FMTT
#undef FMTTS
#undef FMTI
#undef FMTR

// Free allocated memory in reverse order as were allocated
int startrun_memoryAllocation(struct  cmdline_data *cmd,
                                     struct  global_data* gd)
{
    INTEGER bytes_tot_local=0;

#ifdef PXD
#endif

//B socket:
#ifdef ADDONS
// this is empty and can be remove these 3 lines
#include "startrun_include_10.h"                    // should be sync with
                                                    //  "wlcovio_include_10.h"
#endif
//E

    gd->bytes_tot += bytes_tot_local;

    return SUCCESS;
}

#ifdef OPENMPCODE
int SetNumberThreads(struct  cmdline_data *cmd)
{
    omp_set_num_threads(cmd->numthreads);

    return SUCCESS;
}
#else // dummy for no OPENMCODE
int SetNumberThreads(struct  cmdline_data *cmd)
{
    return SUCCESS;
}
#endif

local int print_make_info(struct cmdline_data* cmd,
                          struct  global_data* gd)
{
    verb_print(cmd->verbose,
               "\nprint_make_info:\n");

#ifdef OPENMPCODE
    verb_print(cmd->verbose, "using OpenMP\n");
#endif

#ifdef DEBUG
    verb_print(cmd->verbose, "DEBUG\n");
#endif

#ifdef USEGSL
    verb_print(cmd->verbose, "using GSL\n");
#endif

#ifdef ADDONS
    verb_print(cmd->verbose, "with ADDONS\n");
#endif

#ifdef CLASSLIB
    verb_print(cmd->verbose, "with CLASSLIB\n");
#endif

#ifdef PXD
    verb_print(cmd->verbose, "with PXD\n");
#endif

    return SUCCESS;
}

// I/O directories:
local int setFilesDirs_log(struct cmdline_data* cmd,
                             struct  global_data* gd)
{
    string routineName = "setFilesDirs_log";

    int nwrite;

    if (cmd->verbose_log>0) {                       // gd->logfilePath is defined

        nwrite = snprintf(gd->tmpDir, sizeof(gd->tmpDir), "%s/%s", cmd->rootDir, "tmp");
        if (nwrite < 0 || (size_t)nwrite >= sizeof(gd->tmpDir)) {
            WLCOV_FAIL(cmd, "\n%s: tmpDir too long\n\n",routineName);
        }
        
        double cpustart = CPUTIME;
        if (mkdir_p(gd->tmpDir, 0755) == 0) {
        } else {
            WLCOV_FAIL(cmd, "\n%s: Error creating directory: %s\n\n",
                       routineName, gd->tmpDir);
        }
        gd->cputotalinout += CPUTIME - cpustart;
        
        nwrite = snprintf(gd->logfilePath, sizeof(gd->logfilePath),
                          "%s/wlcov.log", gd->tmpDir);
        if (nwrite < 0 || (size_t)nwrite >= sizeof(gd->logfilePath)) {
            WLCOV_FAIL(cmd, "\n%s: logfilePath too long\n",routineName);
        }

    }

    return SUCCESS;
}

local int setFilesDirs(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "setFilesDirs";

    char outputDir[MAXLENGTHOFFILES];

    int nwrite;

    double cpustart = CPUTIME;

    int ndefault = 0;
    int *ipos;
    char *dp1;
    int lenDir = strlen(cmd->rootDir);
    int i;

    if (gd->rootDirFlag==TRUE) {
        
        int nslashs = MAXNSLASHS;
        ipos = (int*) malloc(nslashs * sizeof(int));
        if (ipos == NULL) {
            WLCOV_FAIL(cmd, "%s: not enough memory allocating ipos\n",routineName);
        }

        dp1 = (char*) malloc((lenDir + 1) * sizeof(char));
        if (dp1 == NULL) {
            free(ipos);
            WLCOV_FAIL(cmd, "%s: not enough memory allocating dp1\n",routineName);
        }

        for (i = 0; i < lenDir; i++) {
            if (cmd->rootDir[i] == '/') {
                if (ndefault >= nslashs) {
                    WLCOV_FAIL(cmd,
                               "%s: more '/' than %d in 'rootDir=%s'. Use only %d or none\n",
                               routineName, nslashs, cmd->rootDir, nslashs);
                }

                ipos[ndefault] = i + 1;
                ndefault++;
            }
        }

        if (ndefault == 0) {

            nwrite = snprintf(outputDir, sizeof(outputDir), "%s", cmd->rootDir);
            if (nwrite < 0 || (size_t)nwrite >= sizeof(outputDir)) {
                free(ipos);
                free(dp1);
                WLCOV_FAIL(cmd, "%s: outputDir too long\n",routineName);
            }
            
            if (mkdir_p(outputDir, 0755) == 0) {
            } else {
                WLCOV_FAIL(cmd, "%s: Error creating directory: %s",
                           routineName, outputDir);
            }
        } else {
            for (i=0; i<ndefault; i++) {
                
                nwrite = snprintf(dp1, ipos[i]+1, "%s", cmd->rootDir);
                if (nwrite < 0 || (size_t)nwrite >= (size_t)ipos[i]+1) {
                    free(ipos);
                    free(dp1);
                    WLCOV_FAIL(cmd, "%s: dp1 too long\n",routineName);
                }
                
                if (mkdir_p(dp1, 0755) == 0) {
                } else {
                    WLCOV_FAIL(cmd, "%s: Error creating directory: %s",
                               routineName, dp1);
                }
            }
            
            nwrite = snprintf(dp1, lenDir+1, "%s", cmd->rootDir);
            if (nwrite < 0 || (size_t)nwrite >= (size_t)lenDir+1) {
                free(ipos);
                free(dp1);
                WLCOV_FAIL(cmd, "%s: dp1 too long\n",routineName);
            }

            
            if (mkdir_p(dp1, 0755) == 0) {
            } else {
                WLCOV_FAIL(cmd, "%s: Error creating directory: %s",
                           routineName, dp1);
            }
        }
        gd->cputotalinout += CPUTIME - cpustart;

        free(ipos);

        free(dp1);
    } // ! rootDirFlag

    return SUCCESS;
}

int StartOutput(struct cmdline_data *cmd, struct  global_data* gd)
{
    if (cmd->verbose>=VERBOSEMININFO)
        if (! strnull(cmd->options))
            verb_print(cmd->verbose, "\n\toptions: %s\n", cmd->options);

    return SUCCESS;
}
