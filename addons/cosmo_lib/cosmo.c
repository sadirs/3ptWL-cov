/*==============================================================================
 NAME: cosmo.c                [wlcov]
 Written by: A. Aviles et al.
 Adapted by: Mario A. Rodriguez-Meza
 Starting date: february 2026
 Purpose: Main routine
 Language: C
 Major revision:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

#include "globaldefs.h"

#define eps 0.0001

#ifdef CLASSLIB
#define COSMO_FAIL(cmd, ...)                                      \
    do {                                                          \
        snprintf((cmd)->error_message, _ERRORMSGSIZE_, __VA_ARGS__); \
        return FAILURE;                                           \
    } while (0)
#else
#define COSMO_FAIL(cmd, ...) error(__VA_ARGS__)
#endif

//B local headers
//B Initial needed routines
double Dplusf(struct cmdline_data* cmd, struct  global_data* gd, double z);
double Dplusf_func(struct cmdline_data* cmd, struct  global_data* gd,
                   int j, double la, double y[2]);
int read_inputpk(struct cmdline_data* cmd, struct  global_data* gd);
double sigmaRTH(struct cmdline_data* cmd, struct  global_data* gd,
                double r, double kini, double kfin, int Nk);
double linear_pkz0_data(struct cmdline_data* cmd, struct  global_data* gd,
                        double k);
double linear_pkz0(struct cmdline_data* cmd, struct  global_data* gd,
                   double k);
//E

//B background needed routines
int chiArray_all(struct cmdline_data* cmd, struct  global_data* gd,
                 double chiOfzT[], double zT[]);
double HoverH0(struct cmdline_data* cmd, struct  global_data* gd, double z);
int compute_gL(struct cmdline_data* cmd, struct  global_data* gd);
//B  Interpolation...
double chiOfz_func(double z);
double zOfchi_func(double chi);
double aOfchi_func(double chi);
double DpOfchi_func(double chi);
double qOfchi_func(double chi);
double interpolation1(double x, double xT[], double yT[], int n_data);
//E
int chiMaxforInt(struct cmdline_data* cmd, struct  global_data* gd);
int ArraysforChiQuad(struct cmdline_data* cmd, struct  global_data* gd);
int read_inputWgchi(struct cmdline_data* cmd, struct  global_data* gd);
double Wg_func(struct cmdline_data* cmd, struct  global_data* gd, double chi);
double q(struct cmdline_data* cmd, struct  global_data* gd, double chi);
//E
double calcrsigma(struct cmdline_data* cmd, struct  global_data* gd,
                  double Dp, double kini, double kfin, int Nk);
double sigmaRGaussian(struct cmdline_data* cmd, struct  global_data* gd,
                      double r, double kini, double kfin, int Nk);
double n_eff_func(struct cmdline_data* cmd, struct  global_data* gd,
                  double r_sigma, double Dp, double kini, double kfin, int Nk);
double gL(struct cmdline_data* cmd, struct  global_data* gd, double chi);
double gLDiracDelta(double chi); // This is gL for Wg a Dirac delta
double gL_func(double chi);   // gL(chi) interpolated
double sigmaRGaussian1stDeriv(struct cmdline_data* cmd, struct  global_data* gd,
                              double r, double kini, double kfin, int Nk);
double power(struct cmdline_data* cmd, struct  global_data* gd,
             double ell, double chi);
//E local headers

/*
 Initial routine:

 To be called in main:
    Initial(&cmd, &gd);

 This routine is in charge of the initialization

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
 Return (the error status):
    int SUCCESS or FAILURE
 */
int Initial(struct cmdline_data* cmd, struct  global_data* gd)
{
    gd->Dpz0 = Dplusf(cmd, gd, 0.0);
    if (read_inputpk(cmd, gd) == FAILURE)           // Read linear PS
        return FAILURE;                             //  power spectrum from CAMB

    gd->sigma8 = sigmaRTH(cmd, gd, 8,0.001,8.,100); // Computation of sigma8

    return SUCCESS;
}

/*
 background routine:

 To be called in the MainLoop:
    background(&cmd, &gd, zBin);

 This routine is in charge of computing the background cosmology

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
    * `zBin`: Input: double, redshift of bin
 Return (the error status):
    int SUCCESS or FAILURE
 */
int background(struct cmdline_data* cmd, struct  global_data* gd,
               double zBin)
{
    string routineName = "background";

    int nwrite;

    double cpustart = CPUTIME;

    if(cmd->Wg==1) compute_gL(cmd, gd);

    // Make z Array.
    for (int i=0;i<gv.Nz;i++) gv.zT[i] = gv.zMin
                                         + i*(gv.zMax-gv.zMin)/(gv.Nz-1);
    
    // Make chi(z) Array
    chiArray_all(cmd, gd, gv.chiOfzT, gv.zT);

    // Make Dp Array.
    for(int i=0;i<gv.Nz;i++) gv.DpT[i] = Dplusf(cmd, gd, gv.zT[i])/ gd->Dpz0;

    gv.chiBin = chiOfz_func(zBin);

    chiMaxforInt(cmd, gd);

    ArraysforChiQuad(cmd, gd);

    if (scanopt(cmd->options, "export")) {
        FILE *fp;
        char str[BUFFERSIZE];

        nwrite = snprintf(str, sizeof(str), "%s/%sbackground_functions.txt",cmd->rootDir,cmd->prefix);
        if (nwrite < 0 || (size_t)nwrite >= sizeof(str)) {
            COSMO_FAIL(cmd, "%s: str too long\n", routineName);
        }

        if((fp=fopen(str,"w+"))) {
            fprintf(fp, "%15s  %15s  %15s  %15s  %15s  %15s  \n",
                    "chi[Mpc/h]", "z", "Dplus", "rsigma[Mpc/h]",
                    "neff", "q(chi)");

            for(int i=0;i<iv.chiQuadSteps;i++){
                fprintf(fp,
                        "%15.15f  %15.15f  %15.15f  %15.15f  %15.15f  %15.15f  \n",
                        iv.chiT_chiint[i], iv.zT_chiint[i],iv.DpT_chiint[i],
                        iv.rsigma_chiint[i], iv.neff_chiint[i], iv.q_chiint[i]);
            }
            fclose(fp);
        } else {
            COSMO_FAIL(cmd, "\n%s: can not open file: %s\n", routineName, str);
        }
    }

    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\n\t%s: time evaluating background: %lf s\n",
                           routineName, CPUTIME - cpustart);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\t%s: iv.chiMaxInt = %f Mpc/h\n",
                           routineName, iv.chiMaxInt);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\t%s: gv.chiBin    = %f Mpc/h\n",
                           routineName, gv.chiBin);

    return SUCCESS;
};

// convergence power spectrum
int Cls_kappa(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "Cls_kappa";
    
    int nwrite;

    double kMin;
    double kMax;
    double psMin;
    double psMax;
    int Nell=120;
    double ellMin;
    double ellMax;
    double delta;
    double ellT[Nell];
    int i;
    
    //B power spectrum info
    kMin = gd->k_data[0];
    kMax = gd->k_data[gd->n_ps_data-1];
    psMin = gd->pkz0_data[0];
    psMax = gd->pkz0_data[gd->n_ps_data-1];
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\t%s: kMin, kMax: %g %g\n",
                           routineName, kMin, kMax);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\t%s: psMin, psMax: %g %g\n",
                           routineName, psMin, psMax);
    //E
    
    ellMin = 1.0;
    ellMax = 100000.0;
    delta = log10(ellMax-ellMin) / ((double)(Nell-1));
    for(i=1; i<=Nell;i++) {
        ellT[i-1] = pow(10.0, (log10(ellMin) + delta*(double)(i - 1)));
    }
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\t%s: ellMin, ellMax: %g %g\n",
                           routineName, ellT[0], ellT[Nell-1]);

    double chiMax=3000;
    
    //B final integration
    int Nchi = 100; // 150
    double chiT[Nchi];
    double ell;
    double chiminell;
    double chimaxell;
    double deltachi;
    double pkappa;
    double pkappaA;
    double pkappaB;
    double chiA;
    double chiB;
    double CkappaT[Nell];
    
    for(int l=1; l<=Nell;l++) {
        ell = ellT[l-1];
        chiminell = ell / kMax;
        chimaxell = MIN(chiMax, ell / kMin);
        deltachi = (chimaxell - chiminell)/((double)(Nchi-1));
        for(i=1; i<=Nchi;i++) {
            chiT[i-1] =  chiminell + deltachi*((double)(i - 1));
        }
        pkappa = 0.0;
        pkappaB = 0.0;
        chiA = chiT[0];
        pkappaA = q(cmd, gd, chiA) * q(cmd, gd, chiA)
                    * power(cmd, gd, ell, chiA)/(chiA*chiA);

        for(i=2;i<=Nchi;i++) {
            chiB = chiT[i-1];
            pkappaB = q(cmd, gd, chiB) * q(cmd, gd, chiB)
                        * power(cmd, gd, ell, chiB)/(chiB*chiB);
            deltachi = chiB - chiA;
            pkappa = pkappa + 0.5*(pkappaA + pkappaB)  *deltachi;
            chiA = chiB;
            pkappaA = pkappaB;
        }
        CkappaT[l-1] = pkappa;
    }
    //E
    
    FILE *fp;
    char str[BUFFERSIZE];

    nwrite = snprintf(str, sizeof(str), "%s/CkappaT.txt",cmd->rootDir);
    if (nwrite < 0 || (size_t)nwrite >= sizeof(str)) {
        COSMO_FAIL(cmd, "%s: str too long\n", routineName);
    }

    if((fp=fopen(str,"w+"))) {
        for(int l=1; l<=Nell;l++) {
            fprintf(fp, "%15.15f  %15.15f\n", log10(ellT[l-1]), CkappaT[l-1]);
        }
        fclose(fp);
    } else {
        COSMO_FAIL(cmd, "\n%s: can not open file: %s\n", routineName, str);
    }

    return SUCCESS;
}

double power(struct cmdline_data* cmd, struct  global_data* gd,
                   double ell, double chi)
{
    double tmp;
    double ellochi;
    
    ellochi = ell/chi;
    tmp = DpOfchi_func(chi) * linear_pkz0(cmd, gd, ellochi);

    return tmp;
}

//B Initial needed routines

//B input power spectrum
int read_inputpk(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "read_inputpk";
    FILE *fp;
    double k_data, pkz0_data;
    gd->n_ps_data=0;
    fp=fopen(cmd->fnamePS,"r");                     // linear P(k) table

    //B input: k[h/Mpc] P(k)[(Mpc/h)^3]
    int line;
    if(fp!=NULL){
        while ((line = fscanf(fp, "%lf %lf", &k_data, &pkz0_data)) != EOF) {
            if (line != 2) {
                fclose(fp);
                COSMO_FAIL(cmd,
            "%s: linear power spectrum file must have two columns of values... exiting\n\n",
                           routineName);
            }

            if (gd->n_ps_data >= n_data_max) {
                fclose(fp);
                COSMO_FAIL(cmd, "%s: n_data_max should be larger than the number of data lines\n",
                           routineName);

            }

            gd->k_data[gd->n_ps_data] = k_data;
            gd->pkz0_data[gd->n_ps_data] = pkz0_data;
            gd->n_ps_data++;
        }
        
        fclose(fp);
    } else
        COSMO_FAIL(cmd, "\n%s: linear power spectrum can't be opened (%s) \n\n",
                   routineName, cmd->fnamePS);

    if (gd->n_ps_data < 2)
        COSMO_FAIL(cmd, "\n%s: LPS file must have at least two rows of values... exiting\n\n",
               routineName);

    for (int i=0; i<gd->n_ps_data; i++) {
        if (!isfinite(gd->k_data[i]))
            COSMO_FAIL(cmd, "\n%s: LPS file must have finite k values... exiting\n\n",
                       routineName);
        if (!isfinite(gd->pkz0_data[i]))
            COSMO_FAIL(cmd, "\n%s: LPS file must have finite pkz0 values... exiting\n\n",
                       routineName);
        if (gd->k_data[i] <= 0.)
            COSMO_FAIL(cmd, "\n%s: LPS file must have k values positive... exiting\n\n",
                       routineName);
        if (gd->pkz0_data[i] <= 0.)
            COSMO_FAIL(cmd, "\n%s: LPS file must have positive pkz0 values... exiting\n\n",
                       routineName);
    }

    for (int i=0; i<gd->n_ps_data-1; i++) {
        if (gd->k_data[i] >= gd->k_data[i+1])
        COSMO_FAIL(cmd,
                   "\n%s: LPS file must have k values in ascending order... exiting\n\n",
                   routineName);
    }

    //E

    return SUCCESS;
}

//   Linear growth function
// linear growth factor at z (not normalized at z=0)
double Dplusf(struct cmdline_data* cmd, struct  global_data* gd,
              double z)
{
    int i,j,n;
    double a,a0,x,h,yp;
    double k1[2],k2[2],k3[2],k4[2],y[2],y2[2],y3[2],y4[2];

  a=1./(1.+z);
  a0=1./1100.;

  yp=-1.;
  n=10;

  for(;;){
    n*=2;
    h=(log(a)-log(a0))/n;
  
    x=log(a0);
    y[0]=1., y[1]=0.;
    for(i=0;i<n;i++){                       // is needed this loop?
      for(j=0;j<2;j++) k1[j]=h*Dplusf_func(cmd, gd, j,x,y);

      for(j=0;j<2;j++) y2[j]=y[j]+0.5*k1[j];
      for(j=0;j<2;j++) k2[j]=h*Dplusf_func(cmd, gd, j,x+0.5*h,y2);

      for(j=0;j<2;j++) y3[j]=y[j]+0.5*k2[j];
      for(j=0;j<2;j++) k3[j]=h*Dplusf_func(cmd, gd, j,x+0.5*h,y3);

      for(j=0;j<2;j++) y4[j]=y[j]+k3[j];
      for(j=0;j<2;j++) k4[j]=h*Dplusf_func(cmd, gd, j,x+h,y4);
      
      for(j=0;j<2;j++) y[j]+=(k1[j]+k4[j])/6.+(k2[j]+k3[j])/3.;
      x+=h;
    }

    if(fabs(y[0]/yp-1.)<0.1*eps) break;
    yp=y[0];
  }

    return a*y[0];
}

double Dplusf_func(struct cmdline_data* cmd, struct  global_data* gd,
                   int j, double la, double y[2])
{
  if(j==0) return y[1];
  
    double om, w, ow;

    om=gd->Omega0_m;
    ow=gd->Omega0_de;
    w=cmd->w;

  double g,a;
  a=exp(la);
  g=-0.5*(5.*om+(5.-3*w)*ow*pow(a,-3.*w))*y[1]-1.5*(1.-w)*ow*pow(a,-3.*w)*y[0];
  g=g/(om+ow*pow(a,-3.*w));
  if(j==1) return g;
  else return g;                // will never ocurr!!
}

// in this code we use the following sigma (and below)
double sigmaRTH(struct cmdline_data* cmd, struct  global_data* gd,
                double r, double kini, double kfin, int Nk)
// r[Mpc/h]
{
    double xp,xA,xB,kr,window;
    double kv, deltak;
    double kT[Nk];

    for(int j=0; j<Nk;j++){
        kT[j] = exp(log(kini) + j*log(kfin/kini)/(Nk-1.0));
    };

    xp=0.0; xA=0.0; xB=0.0;
    kv=kT[0];
    kr = kv*r;
    window = 3./pow(kr,3.) * ( sin(kr) - kr*cos(kr) );
    xA = kv*kv*kv*linear_pkz0(cmd, gd, kv)*window*window;
    
    for(int i=1;i<Nk;i++){
        kv=kT[i];
        deltak=log(kv/kT[i-1]);
        kr=kv*r;
        window = 3./pow(kr,3.) * (sin(kr) - kr*cos(kr));
        xB = kv*kv*kv*linear_pkz0(cmd, gd, kv)*window*window;
        xp = xp + 0.5*(xA+xB)*deltak;
        xA=xB;
    }
    return sqrt(xp/(2.0*M_PI*M_PI));
}

// Input Power spectrum
double linear_pkz0_data(struct cmdline_data* cmd, struct  global_data* gd,
                        double k)
// linear P(k) interpolated from the given table,  k[h/Mpc]  P(k)[(Mpc/h)^3]
{
  int j,j1,j2,jm;
    double lk,f;
  double norm=1.;


  lk=log10(k);
  if(k<gd->k_data[0]) return 0.;
  if(k>gd->k_data[gd->n_ps_data-1]) return 0.;
  
  j1=0, j2=gd->n_ps_data-1, jm=(j1+j2)/2;
  for(;;){
    if(k>gd->k_data[jm]) j1=jm;
    else j2=jm;
    jm=(j1+j2)/2;

    if(j2-j1==1) break;
  }
  j=j1;

  f=(log10(gd->pkz0_data[j+1])-log10(gd->pkz0_data[j]))/(log10(gd->k_data[j+1])
      -log10(gd->k_data[j]))*(lk-log10(gd->k_data[j]))+log10(gd->pkz0_data[j]);
  
  return norm*norm*pow(10.,f);
}

double linear_pkz0(struct cmdline_data* cmd, struct  global_data* gd,
                   double k)
// linear P(k)   k[h/Mpc], P(k)[(Mpc/h)^3]
{
  //~ if(gd.n_data!=0) return linear_pkz0_data(k);
  //~ else return linear_pkz0_eh(k);
    return linear_pkz0_data(cmd, gd, k);
}

//E Initial needed routines


//B background needed routines

int compute_gL(struct cmdline_data* cmd, struct  global_data* gd)
{
    if (read_inputWgchi(cmd, gd) == FAILURE)
        return FAILURE;

    double chiminint, chimaxint;
    
    chiminint=gd->chi_data[0];
    chimaxint=gd->chi_data[gd->n_chi_data-1];

    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                    "\nInput file Wg with %d lines. chiMin=%f, chiMax=%f \n\n",
                    gd->n_chi_data,chiminint,chimaxint);

    int Nsteps=gv.NstepsforgL;
    
    for    (int i=0;i<Nsteps;i++) gv.chiforgLT[i] = chiminint + i*(chimaxint-chiminint)/(Nsteps-1);
    
    double gLA, gLB, gLp, chiv, chi, deltachi, chiprev;

    for (int i=0;i<Nsteps;i++){
        chi= gv.chiforgLT[i];
        gLp=0; gLA=0; gLB=0;
        
        chiprev=chi;
        
        for(int j=i; j<Nsteps; j++){
            chiv=gv.chiforgLT[j];
            deltachi=chiv-chiprev;
            gLB=(chiv-chi)/chiv * Wg_func(cmd, gd, chiv);
            gLp=gLp + (gLA+gLB)/2.*deltachi;
            gLA=gLB;
            chiprev=chiv;
        }
        
        gv.gLT[i]=gLp;
    }

    return SUCCESS;
}

int chiArray_all(struct cmdline_data* cmd, struct  global_data* gd,
                  double chiOfzT[], double zT[])
{
    double z,deltaz;
    // check if it is needed zprev... it seems does not...
    double H0 = 0.00033356409519815205; //H0 in h/Mpc
    
    deltaz=zT[1]-zT[0];
    
    double xp=0.0, xA=0.0, xB=0.0;
    
    z=zT[0];
    xA = 1./HoverH0(cmd, gd, z)/H0;
    chiOfzT[0]=xA*z;
    
    for (int i=1;i<gv.Nz;i++){
        z=zT[i];
        xB = 1./HoverH0(cmd, gd, z)/H0;
        xp += 0.5*(xA + xB) * deltaz;
        chiOfzT[i] = xp;
        xA=xB;
    }
    
    return SUCCESS;
}

double HoverH0(struct cmdline_data* cmd, struct  global_data* gd, double z)
{
    return sqrt( gd->Omega0_m*pow(1+z,3)
                + (1-gd->Omega0_m)*pow(1+z,3*(1+cmd->w)));
}

//B  Interpolation of several background functions
double chiOfz_func(double z)
{
    return interpolation1(z, gv.zT, gv.chiOfzT, gv.Nz);
}

double zOfchi_func(double chi)
{
    return interpolation1(chi, gv.chiOfzT, gv.zT, gv.Nz);
}

double aOfchi_func(double chi)
{
    return 1/(zOfchi_func(chi)+1.);
}

double DpOfchi_func(double chi)
{
    return interpolation1(chi, gv.chiOfzT, gv.DpT, gv.Nz);
}

double qOfchi_func(double chi)
{
    return interpolation1(chi, iv.chiT_chiint, iv.q_chiint, gv.Nz);
}

// interpolation order 1
double interpolation1(double x, double xT[], double yT[], int n_data)
{
  int j,j1,j2,jm;
  double f;

  if(x<xT[0]) return 0.;
  if(x>xT[n_data-1]) return 0.;
  
  j1=0, j2=n_data-1, jm=(j1+j2)/2;
  for(;;){
    if(x>xT[jm]) j1=jm;
    else j2=jm;
    jm=(j1+j2)/2;

    if(j2-j1==1) break;
  }
  j=j1;

  f=(yT[j+1]-yT[j])/(xT[j+1]-xT[j]) * (x -xT[j])+ yT[j];
  
  return f;
}
//E

int chiMaxforInt(struct cmdline_data* cmd, struct  global_data* gd)
{
    double qMV, verysmall;
    qMV = q(cmd, gd, gv.chiBin/2);
    verysmall = 0.0001;
    for (int i=0;i<gv.Nz;i++) {
        if ( gv.chiOfzT[i] > gv.chiBin/2
            && q(cmd, gd, gv.chiOfzT[i])< verysmall* qMV) break;
        iv.chiMaxInt = gv.chiOfzT[i];
    }
    
    return SUCCESS;
}

int ArraysforChiQuad(struct cmdline_data* cmd, struct  global_data* gd)
{
    double chiv,zv,Dpv,rsigmav;
    double Deltachi;
    
    Deltachi = (iv.chiMaxInt - iv.chiMinInt) / (iv.chiQuadSteps-1);
    
    for(int i=0;i<iv.chiQuadSteps;i++){
        chiv = iv.chiMinInt + i * Deltachi;
        zv = zOfchi_func (chiv);
        Dpv= Dplusf(cmd, gd, zv) / gd->Dpz0;
        rsigmav = calcrsigma(cmd, gd, Dpv, 0.001,8.,100);
        iv.chiT_chiint[i]   = chiv;
        iv.zT_chiint[i]     = zv;
        iv.DpT_chiint[i]    = Dpv;
        iv.rsigma_chiint[i] = rsigmav;
        iv.neff_chiint[i]   = n_eff_func(cmd, gd, rsigmav,Dpv,0.001,8.,100);
        iv.q_chiint[i]      = q(cmd, gd, chiv);
    }

    return SUCCESS;
}

// Read input Wg(chi)
int read_inputWgchi(struct cmdline_data* cmd, struct  global_data* gd)
{
    string routineName = "read_inputWgchi";

    FILE *fp;
    gd->n_chi_data=0;
    fp=fopen(cmd->fWgchi,"r");   // Wg(chi) table  c#1: chi, c#2: Wg

    double chi_data;
    double Wg_chi_data;
    int line;
    if(fp!=NULL){   // input: chi[Mpc/h]   Wg(chi)
        while ((line = fscanf(fp, "%lf %lf", &chi_data, &Wg_chi_data)) != EOF) {
            if (line != 2) {
                fclose(fp);
                COSMO_FAIL(cmd, "%s: Wgchi file must have two columns of values\n", routineName);
            }
            
            if (gd->n_chi_data >= n_chi_data_max) {
                fclose(fp);
                COSMO_FAIL(cmd,
                        "%s: n_chi_data_max should be larger than the number of data lines\n",
                        routineName);
            }

            gd->chi_data[gd->n_chi_data] = chi_data;
            gd->Wg_chi_data[gd->n_chi_data] = Wg_chi_data;
            gd->n_chi_data++;
        }
    fclose(fp);
    } else
        COSMO_FAIL(cmd, "\n%s: Wg file can't be opened (%s)\n\n",
                   routineName, cmd->fWgchi);

    if (gd->n_chi_data < 2)
        COSMO_FAIL(cmd, "%s: Wgchi file must have at least two rows of values... exiting\n\n",
                   routineName);

    for (int i=0; i<gd->n_chi_data; i++) {
        if (!isfinite(gd->chi_data[i]))
            COSMO_FAIL(cmd, "%s: Wgchi file must have finite chi values... exiting\n\n",
                       routineName);
        if (!isfinite(gd->Wg_chi_data[i]))
            COSMO_FAIL(cmd, "%s: Wgchi file must have finite Wg values... exiting\n\n",
                       routineName);
        if (gd->chi_data[i] <= 0.)
            COSMO_FAIL(cmd, "%s: Wgchi file must have chi values positive... exiting\n\n",
                       routineName);
    }

//B  if Wg_chi_data is allowed to be zero or negative physically,
//      leaving that positivity check commented is fine.
//      If it must be finite, add isfinite() checks for both columns
//
//    for (int i=0; i<gd->chi_data; i++) {
//        if (gd->Wg_chi_data[i] <= 0.)
//            error("\nWg file must have positive Wg values... %s\n\n",
//                  "exiting");
//    }
//E
    for (int i=0; i<gd->n_chi_data-1; i++) {
        if (gd->chi_data[i] >= gd->chi_data[i+1])
            COSMO_FAIL(cmd,
                   "%s: Wgchi file must have chi values in ascending order... exiting\n\n",
                    routineName);
    }

    return SUCCESS;
}

// Wg(chi) interpolated
double Wg_func(struct cmdline_data* cmd, struct  global_data* gd, double chi)
{
    return interpolation1(chi, gd->chi_data, gd->Wg_chi_data, gd->n_chi_data);
}


double calcrsigma(struct cmdline_data* cmd, struct  global_data* gd,
                  double Dp, double kini, double kfin, int Nk)
// return r_sigma[Mpc/h] (=1/k_nl)
{
    double k,k1,k2, sgG;

    k1=k2=1.;
    for(;;){
        sgG= sigmaRGaussian(cmd, gd, 1./k1, kini, kfin, Nk);
        if(Dp*sgG<1.) break;
        k1*=0.5;
    }
    for(;;){
        sgG= sigmaRGaussian(cmd, gd, 1./k2, kini, kfin, Nk);
        if(Dp*sgG>1.) break;
        k2*=2.;
    }
    
    for(;;){
        k=0.5*(k1+k2);
        sgG= sigmaRGaussian(cmd, gd, 1./k, kini, kfin, Nk);
        if(Dp*sgG<1.) k1=k;
        else if(Dp*sgG>1.) k2=k;
        if(Dp*sgG==1. || fabs(k2/k1-1.)<eps*0.1) break;
  }

  return 1./k;
}

double sigmaRGaussian(struct cmdline_data* cmd, struct  global_data* gd,
                      double r, double kini, double kfin, int Nk)
// r[Mpc/h]
{
    double xp,xA,xB,kr,window;
    double k, deltak;
    double kT[Nk];

    for(int j=0; j<Nk;j++){
        kT[j] = exp(log(kini) + j*log(kfin/kini)/(Nk-1.0));
    };
     
    xp=0.0; xA=0.0; xB=0.0;
    k=kT[0];
    kr = k*r;
    window = exp(-0.5*kr*kr);
    xA = k*k*k*linear_pkz0(cmd, gd, k)*window*window;

    for(int i=1;i<Nk;i++){
        k=kT[i];
        deltak=log(k/kT[i-1]);
        kr=k*r;
        window = exp(-0.5*kr*kr);
        xB = pow(k,3.)*linear_pkz0(cmd, gd, k)*window*window;
        xp += 0.5*(xA+xB)*deltak;
        xA=xB;
    }
    return sqrt(xp/(2.0*M_PI*M_PI));
}

// qDiracDelta if Wg=0
double q(struct cmdline_data* cmd, struct  global_data* gd, double chi)
{
    double result, a;
    double H0 = 0.00033356409519815205; //H0 in h/Mpc
    a = aOfchi_func(chi);
    result = (3./2.)*gd->Omega0_m*H0*H0 * gL(cmd, gd, chi) * chi / a;
    return result;
}

double n_eff_func(struct cmdline_data* cmd, struct  global_data* gd,
                  double r_sigma, double Dp, double kini, double kfin, int Nk)
{
    return -3.+2.*pow(Dp*sigmaRGaussian1stDeriv(cmd, gd, r_sigma, kini,kfin,Nk),2);
}

double gL(struct cmdline_data* cmd, struct  global_data* gd, double chi)
{
    
    if (cmd->Wg==0){
        return gLDiracDelta(chi);
    } else if (cmd->Wg==1) {
        return gL_func(chi);
    } else {
        return 0;
    }
}

double gLDiracDelta(double chi) // This is gL for Wg a Dirac delta:
{
    if (chi>gv.chiBin){
         return 0;
    } else {
        return 1.- chi/gv.chiBin;
    }
}

double gL_func(double chi)   // gL(chi) interpolated
{
    return interpolation1(chi, gv.chiforgLT, gv.gLT, gv.NstepsforgL);
}

double sigmaRGaussian1stDeriv(struct cmdline_data* cmd, struct  global_data* gd,
                              double r, double kini, double kfin, int Nk)
// r[Mpc/h]
{
    double xp,xA,xB,kr,window;
    double k, deltak;
    double kT[Nk];

    for(int j=0; j<Nk;j++){
        kT[j] = exp(log(kini) + j*log(kfin/kini)/(Nk-1.0));
    };
     
    xp=0.0; xA=0.0; xB=0.0;
    k=kT[0];
    kr = k*r;
    window = kr*exp(-0.5*kr*kr);
    xA = k*k*k*linear_pkz0(cmd, gd, k)*window*window;
    
    for(int i=1;i<Nk;i++){
        k=kT[i];
        deltak=log(k/kT[i-1]);
        kr=k*r;
        window = kr*exp(-0.5*kr*kr);
        xB = pow(k,3.)*linear_pkz0(cmd, gd, k)*window*window;
        xp += 0.5*(xA+xB)*deltak;
        xA=xB;
    }
    return sqrt(xp/(2.0*M_PI*M_PI));
}

//E background needed routines

#undef eps

