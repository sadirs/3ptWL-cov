// Use:
//#include "startrun_cosmo_lib_07.h"

//
// in: addons/addons_include/source/startrun/startrun_include_07.h
//

#ifndef _startrun_cosmo_lib_07_h
#define _startrun_cosmo_lib_07_h

//B background cosmological parameters
if (cmd->h <= 0.0 || cmd->h > 1.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for h (%g)\n", routineName, cmd->h);
if (cmd->Omega0_b < 0.0 || cmd->Omega0_b > 1.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for Omega0_b (%g)\n",
               routineName, cmd->Omega0_b);
if (cmd->Omega0_cdm < 0.0 || cmd->Omega0_cdm > 1.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for Omega0_cdm (%g)\n",
               routineName, cmd->Omega0_cdm);
if (cmd->Omega0_nu < 0.0 || cmd->Omega0_nu > 1.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for Omega0_nu (%g)\n",
               routineName, cmd->Omega0_nu);
if (cmd->ns <= 0.0 || cmd->ns > 1.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for ns (%g)\n",routineName, cmd->ns);
//E

if (cmd->zbin < 0.0)
    WLCOV_FAIL(cmd, "\n%s: absurd value for zbin (%g)\n",routineName, cmd->zbin);

if (cmd->chiQuadSteps <= 2)
    WLCOV_FAIL(cmd, "\n%s: absurd value for chiQuadSteps (%d)\n",
               routineName, cmd->chiQuadSteps);

if (cmd->Wg != 0 && cmd->Wg != 1)
    WLCOV_FAIL(cmd, "\n%s: absurd value for Wg (%d)\n",routineName, cmd->Wg);

#endif	// ! _startrun_octree_smoothing_07_h
