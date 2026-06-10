// Use:
//#include "startrun_include_wlcf_05.h"
//
// in: addons/addons_include/source/startrun/startrun_include_05.h
//

#ifndef _startrun_include_wlcf_05_h
#define _startrun_include_wlcf_05_h

gd->Omega0_m = cmd->Omega0_b + cmd->Omega0_cdm + cmd->Omega0_nu;
gd->Omega0_de = 1.0 - gd->Omega0_m;

class_call_cballs(Initial(cmd, gd), errmsg, errmsg);

//B allocating gv memory
gv.zMax = 3.*cmd->zbin;
gv.zMin = 0.0;
gv.Nz   = 200;

gv.zT        = allocate(gv.Nz * sizeof(double));
gv.chiOfzT   = allocate(gv.Nz * sizeof(double));
gv.DpT       = allocate(gv.Nz * sizeof(double));
gv.NstepsforgL=1000;
int Nsteps=gv.NstepsforgL;
gv.chiforgLT = allocate(Nsteps * sizeof(double));
gv.gLT       = allocate(Nsteps * sizeof(double));
//E
//B allocating gv memory
iv.chiQuadSteps=cmd->chiQuadSteps;
iv.chiMinInt = 10;
iv.zT_chiint     = allocate(iv.chiQuadSteps * sizeof(double));
iv.chiT_chiint   = allocate(iv.chiQuadSteps * sizeof(double));
iv.DpT_chiint    = allocate(iv.chiQuadSteps * sizeof(double));
iv.rsigma_chiint = allocate(iv.chiQuadSteps * sizeof(double));
iv.neff_chiint   = allocate(iv.chiQuadSteps * sizeof(double));
iv.q_chiint      = allocate(iv.chiQuadSteps * sizeof(double));
//E

class_call_cballs(background(cmd, gd, cmd->zbin), errmsg, errmsg);
if (scanopt(cmd->options, "compute-CkappaT"))
    class_call_cballs(Cls_kappa(cmd, gd), errmsg, errmsg);

//B freeing arrays' memory
//B iv
free(iv.q_chiint);
free(iv.neff_chiint);
free(iv.rsigma_chiint);
free(iv.DpT_chiint);
free(iv.chiT_chiint);
free(iv.zT_chiint );
//E
//B gv
free(gv.gLT);
free(gv.chiforgLT);
free(gv.DpT);
free(gv.chiOfzT);
free(gv.zT);
//E
//E

#endif	// ! _startrun_include_wlcf_05_h
