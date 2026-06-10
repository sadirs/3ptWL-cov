// Use:
//#include "class_lib_cosmo_lib_01.h"

//
// in: addons/addons_include/source/addons/class_lib_include_01.h
//

#ifndef _class_lib_cosmo_lib_01_h
#define _class_lib_cosmo_lib_01_h

#define COSMOLIB_FREE_STRINGS_ON_FAILURE()      \
    do {                                        \
        if (gd->fWgchiFlag == TRUE) {           \
            free(cmd->fWgchi);                  \
            cmd->fWgchi = NULL;                 \
            gd->fWgchiFlag = FALSE;             \
        }                                       \
        if (gd->fnamePSFlag == TRUE) {          \
            free(cmd->fnamePS);                 \
            cmd->fnamePS = NULL;                \
            gd->fnamePSFlag = FALSE;            \
        }                                       \
        if (gd->prefixFlag == TRUE) {           \
            free(cmd->prefix);                  \
            cmd->prefix = NULL;                 \
            gd->prefixFlag = FALSE;             \
        }                                       \
        BASE_FREE_STRINGS_ON_FAILURE();         \
    } while (0)

gd->prefixFlag = FALSE;
gd->fnamePSFlag = FALSE;
gd->fWgchiFlag = FALSE;

class_call_except(parser_read_double(pfc,"h",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->h = param1;
}

class_call_except(parser_read_double(pfc,"Omega0_b",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->Omega0_b = param1;
}

class_call_except(parser_read_double(pfc,"Omega0_cdm",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->Omega0_cdm = param1;
}

class_call_except(parser_read_double(pfc,"Omega0_nu",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->Omega0_nu = param1;
}

class_call_except(parser_read_double(pfc,"ns",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->ns = param1;
}

class_call_except(parser_read_double(pfc,"w",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->w = param1;
}

class_call_except(parser_read_double(pfc,"zbin",&param1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag1 == TRUE){
  cmd->zbin = param1;
}

class_call_except(parser_read_int(pfc,"chiQuadSteps",&param,&flag,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag == TRUE) cmd->chiQuadSteps = param;

class_call_except(parser_read_int(pfc,"Wg",&param,&flag,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

if (flag == TRUE) cmd->Wg = param;

class_call_except(parser_read_string(pfc,"prefix",&string1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););

gd->prefixFlag=FALSE;
if (flag1 == TRUE) {
    for (index=0;index<pfc->size;++index){
      if (strcmp(pfc->name[index],"prefix") == 0){
          
          cmd->prefix = (char*) malloc(MAXLENGTHOFSTRSCMD * sizeof(char));
          if (cmd->prefix == NULL) {
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          if (copy_checked(cmd->prefix, MAXLENGTHOFSTRSCMD,
                           pfc->value[index], "prefix") != 0) {
              free(cmd->prefix);
              cmd->prefix = NULL;
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          gd->prefixFlag = TRUE;

        break;
      }
    }
}

class_call_except(parser_read_string(pfc,"fnamePS",&string1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););
gd->fnamePSFlag=FALSE;
if (flag1 == TRUE) {
    for (index=0;index<pfc->size;++index){
      if (strcmp(pfc->name[index],"fnamePS") == 0){

          cmd->fnamePS = (char*) malloc(MAXLENGTHOFSTRSCMD * sizeof(char));
          if (cmd->fnamePS == NULL) {
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          if (copy_checked(cmd->fnamePS, MAXLENGTHOFSTRSCMD,
                           pfc->value[index], "fnamePS") != 0) {
              free(cmd->fnamePS);
              cmd->fnamePS = NULL;
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          gd->fnamePSFlag = TRUE;

        break;
      }
    }
}

class_call_except(parser_read_string(pfc,"fWgchi",&string1,&flag1,errmsg),
                  errmsg,
                  errmsg,
                  COSMOLIB_FREE_STRINGS_ON_FAILURE(););
gd->fWgchiFlag=FALSE;
if (flag1 == TRUE) {
    for (index=0;index<pfc->size;++index){
      if (strcmp(pfc->name[index],"fWgchi") == 0){
          cmd->fWgchi = (char*) malloc(MAXLENGTHOFSTRSCMD * sizeof(char));

          if (cmd->fWgchi == NULL) {
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          if (copy_checked(cmd->fWgchi, MAXLENGTHOFSTRSCMD,
                           pfc->value[index], "fWgchi") != 0) {
              free(cmd->fWgchi);
              cmd->fWgchi = NULL;
              COSMOLIB_FREE_STRINGS_ON_FAILURE();
              return FAILURE;
          }

          gd->fWgchiFlag = TRUE;
          
        break;
      }
    }
}

#undef COSMOLIB_FREE_STRINGS_ON_FAILURE

#endif	// ! _class_lib_cosmo_lib_01_h
