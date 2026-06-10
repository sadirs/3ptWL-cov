// Use:
//#include "datastruc_tables_cosmo_lib.h"

//
// in: addons/addons_include/include/datastruc_tables_include.h
//

#ifndef _datastruc_tables_cosmo_lib_h
#define _datastruc_tables_cosmo_lib_h


typedef struct
{
    clock_t time;
    double Dp, r_sigma, n_eff;
    double z;
    double *chiOfzT, *zT, *DpT;
    double zMin, zMax;
    int Nz;
    double chiBin;
    double *gLT, *chiforgLT;
    int NstepsforgL;
} global_vars, *global_vars_ptr;

global global_vars gv;


typedef struct
{
    double chiMaxInt, chiMinInt;
    double *chiT;
    int chiQuadSteps, Nell;
    double *chiT_chiint, *zT_chiint, *DpT_chiint, *rsigma_chiint, *neff_chiint;
    double *q_chiint, *kT, *ellT;
    double **BmVectors, **BmVectorsp;
    double ellmin, ellmax;
} integration_vars, *integration_vars_ptr;

global integration_vars iv;

typedef struct
{
    double *r1, *r2, **result;
} zeta_vars, *zeta_vars_ptr;

global zeta_vars zv;

#endif	// ! _datastruc_tables_cosmo_lib_h
