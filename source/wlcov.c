/*==============================================================================
 MODULE: wlcov.c				[wlcov]
 Written by: Safia Samario & Alejandro Aviles
 adapted by: Mario A. Rodriguez-Meza
 Starting date:	01.05.2026
 Purpose:
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


//B socket:
#ifdef ADDONS
#include "wlcov_include_00.h"
#endif
//E

//B from IntegralCovMatrix
clock_t start_t, end_t;

double max_of_two(double a, double b);
double max_of_three(double a, double b, double c);

double Bessel_Jn(int n, double x);
double Cls(struct  cmdline_data*, struct  global_data*, double ell);

double to_integrate(struct  cmdline_data* cmd, struct  global_data* gd,
                    int m, int mp, double r,
                    double theta1, double thetap1, double theta2, double thetap2,
                    int ppp, double ellmin, double ellmax);

double xi_integrad(struct  cmdline_data*, struct  global_data*,
                   double r, double ell);
double xi(struct  cmdline_data*, struct  global_data*,
          double r, int points_per_period, double ellmin, double ellmax);

double fmmp_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                      double ell, int m, int mp, double theta, double thetap, double r);
double fmmp(struct  cmdline_data* cmd, struct  global_data* gd,
            int m, int mp, double r, double theta, double thetap,
            int points_per_period, double ellmin, double ellmax);

double fm_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                    double ell, int m, double theta, double r);
double fm(struct  cmdline_data* cmd, struct  global_data* gd,
          int m, double r, double theta, int points_per_period, double ellmin,
        double ellmax);

double fmp_integrand(struct  cmdline_data*, struct  global_data*,
                     double ell, int mp, double thetap, double r);
double fmp(struct  cmdline_data*, struct  global_data*,
           int mp, double r, double thetap, int points_per_period, double ellmin,
           double ellmax);
//E

/*
 MainLoop routine:

 To be called in main:
    MainLoop(&cmd, &gd);

 This routine is in charge of making the tree
    and do the searching.

 Arguments:
    * `cmd`: Input: structure cmdline_data pointer
    * `gd`: Input: structure global_data pointer
 Return (the error status):
    int SUCCESS or FAILURE
 */
int MainLoop(struct  cmdline_data* cmd, struct  global_data* gd)
{
//    string routineName = "MainLoop";

//    debug_tracking_s("00", routineName);


//B socket:
#ifdef ADDONS
#include "wlcov_include_01.h"
#endif
//E
    double fmmp11;
    double fmmp22;
    double fmmp12;
    double fmmp21;
    double fm_nmp12;
    double fm_nmp21;
    double fm1;
    double fm2;
    double fmp1;
    double fmp2;
    double integrand;
    double xiplus;

    //B original computational flow
    start_t = clock();

    xiplus = xi(cmd, gd, cmd->r, cmd->ppp, cmd->ellmin, cmd->ellmax);

    fmmp11=fmmp(cmd, gd, cmd->m, cmd->mp, cmd->r, cmd->theta1, cmd->thetap1,
                cmd->ppp, cmd->ellmin, cmd->ellmax);
    fmmp22=fmmp(cmd, gd, cmd->m, cmd->mp, cmd->r, cmd->theta2, cmd->thetap2,
                cmd->ppp, cmd->ellmin, cmd->ellmax);
    fmmp12=fmmp(cmd, gd, cmd->m, cmd->mp, cmd->r, cmd->theta1, cmd->thetap2,
                cmd->ppp, cmd->ellmin, cmd->ellmax);
    fmmp21=fmmp(cmd, gd, cmd->m, cmd->mp, cmd->r, cmd->theta2, cmd->thetap1,
                cmd->ppp, cmd->ellmin, cmd->ellmax);

    fm_nmp12=fmmp(cmd, gd, cmd->m, -cmd->mp, cmd->r, cmd->theta1, cmd->thetap2,
                  cmd->ppp, cmd->ellmin, cmd->ellmax);
    fm_nmp21=fmmp(cmd, gd, cmd->m, -cmd->mp, cmd->r, cmd->theta2, cmd->thetap1,
                  cmd->ppp, cmd->ellmin, cmd->ellmax);

    fm1=fm(cmd, gd, cmd->m, cmd->r, cmd->theta1, cmd->ppp, cmd->ellmin,
           cmd->ellmax);
    fm2=fm(cmd, gd, cmd->m, cmd->r, cmd->theta2, cmd->ppp, cmd->ellmin,
           cmd->ellmax);
    fmp1=fmp(cmd, gd, cmd->mp, cmd->r, cmd->thetap1, cmd->ppp, cmd->ellmin,
             cmd->ellmax);
    fmp2=fmp(cmd, gd, cmd->mp, cmd->r, cmd->thetap2, cmd->ppp, cmd->ellmin,
             cmd->ellmax);

    integrand = 0.5 / gd->fsky * cmd->r * xiplus*(fmmp11*fmmp22 + fmmp12*fmmp21) + fm2*fmmp11*fmp2
                + fm2*fm_nmp12*fmp1 + fm1*fmmp22*fmp1 + fm1*fm_nmp21*fmp2;

    end_t = clock();

//    debug_tracking_s("01", routineName);

    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "\nintegrand(r=%g)=%g \n", cmd->r,integrand);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "Total time: %f\n",
                           (double)(end_t - start_t) / CLOCKS_PER_SEC);

    start_t = clock();
    integrand = to_integrate(cmd, gd, cmd->m, cmd->mp, cmd->r,
                             cmd->theta1, cmd->thetap1, cmd->theta2, cmd->thetap2,
                             cmd->ppp, cmd->ellmin, cmd->ellmax);
    end_t = clock();
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "integrand(r=%g)=%g \n", cmd->r,integrand);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "Total time: %f\n",
                           (double)(end_t - start_t) / CLOCKS_PER_SEC);

//    double rmax, rmin,
    double log10rmin,step,log10ri,ri;
    double intA, intB, intval, prevr;
//    double deltar;
//    int Nr;

    //B numerical parameters...
//    Nr=50;
//    rmax=0.349066;
//    rmin=0.00232711;
    //E
    log10rmin=log10(cmd->rmin);
    step=log10(cmd->rmax/cmd->rmin)/(cmd->Nr-1);

    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "rmin=%g , rmax=%g \n", cmd->rmin, cmd->rmax);
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "integrating... \n");

    start_t = clock();

    ri=cmd->rmin;
    intA = to_integrate(cmd, gd, cmd->m, cmd->mp, ri, cmd->theta1, cmd->thetap1,
                        cmd->theta2, cmd->thetap2,
                        cmd->ppp, cmd->ellmin, cmd->ellmax);

    intB   = 0.0;
    intval = 0.0;
    prevr  = ri;
    
    for(int i=1; i<cmd->Nr; i++){
        log10ri = log10rmin + i*step;
        ri=pow(10.,log10ri);

        intB = to_integrate(cmd, gd, cmd->m, cmd->mp, ri, cmd->theta1, cmd->thetap1,
                            cmd->theta2, cmd->thetap2,
                            cmd->ppp, cmd->ellmin, cmd->ellmax);
        intval = intval + 0.5 * (intA + intB) * (ri-prevr);
        intA = intB;
        prevr = ri;
    };
    
    end_t = clock();
    verb_print_normal_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "Total time: %f\n",
                           (double)(end_t - start_t) / CLOCKS_PER_SEC);

    gd->intval=intval/gd->fsky;
    verb_print_min_info(cmd->verbose, cmd->verbose_log, gd->outlog,
                           "integral=%g \n", gd->intval);
    //E original computational flow

    return SUCCESS;
}

//B from IntegralCovMatrix
// Cell(ell) interpolated from the given table,  ell,  Cell
// change later for cubic spline if necessary
double Cls(struct  cmdline_data* cmd, struct  global_data* gd,
           double ell)
{
    int j,j1,j2,jm;
    double lell,f;

    lell=log10(ell);
    if(ell<gd->ell_data[0]) return 0.;
    if(ell>gd->ell_data[gd->n_data-1]) return 0.;
  
    j1=0, j2=gd->n_data-1, jm=(j1+j2)/2;
    for(;;){
        if(ell>gd->ell_data[jm]) j1=jm;
        else j2=jm;
        jm=(j1+j2)/2;

        if(j2-j1==1) break;
    }
    j=j1;

    f=(log10(gd->Cls_data[j+1])-log10(gd->Cls_data[j]))/(log10(gd->ell_data[j+1])
        -log10(gd->ell_data[j]))*(lell-log10(gd->ell_data[j]))
        +log10(gd->Cls_data[j]);

  return pow(10.,f);
}

double xi_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                    double r, double ell)
{
    return     ell * Cls(cmd, gd, ell) * gsl_sf_bessel_J0(r*ell);

}

double xi(struct  cmdline_data* cmd, struct  global_data* gd,
          double r, int points_per_period, double ellmin, double ellmax)
{
 
    double period, ell, deltaell;
    int Ntotal;
    double intA, intB,intval;
    
    period = 2*M_PI ;
    deltaell = period / points_per_period;
    
    Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
    
        
    intA = xi_integrand(cmd, gd, r, ellmin);
    intB = 0.0;
    intval=0.0;

    for (int i=1; i<Ntotal; i++){
        ell = ellmin + i * deltaell;
        intB = xi_integrand(cmd, gd, r, ell);
        intval = intval + 0.5 * (intA + intB) * deltaell;
        intA = intB;
    }
    
    return  intval / (2.*M_PI);
    
};


double to_integrate(struct  cmdline_data* cmd, struct  global_data* gd,
                    int m, int mp, double r,
                    double theta1, double thetap1, double theta2, double thetap2,
                    int ppp, double ellmin, double ellmax)
{
    double integrand, xiplus, fmmp11,fmmp22,fmmp12,fmmp21;
    double fm_nmp12,fm_nmp21,fm1,fm2,fmp1,fmp2;
    
    xiplus = xi(cmd, gd, r, ppp, ellmin, ellmax);
    fmmp11=fmmp(cmd, gd, m, mp, r, theta1, thetap1, ppp, ellmin, ellmax);
    fmmp22=fmmp(cmd, gd, m, mp, r, theta2, thetap2, ppp, ellmin, ellmax);
    fmmp12=fmmp(cmd, gd, m, mp, r, theta1, thetap2, ppp, ellmin, ellmax);
    fmmp21=fmmp(cmd, gd, m, mp, r, theta2, thetap1, ppp, ellmin, ellmax);

    fm_nmp12=fmmp(cmd, gd, m, -mp, r, theta1, thetap2, ppp, ellmin, ellmax);
    fm_nmp21=fmmp(cmd, gd, m, -mp, r, theta2, thetap1, ppp, ellmin, ellmax);

    fm1=fm(cmd, gd, m, r, theta1, ppp, ellmin, ellmax);
    fm2=fm(cmd, gd, m, r, theta2, ppp, ellmin, ellmax);
    fmp1=fmp(cmd, gd, mp, r, thetap1, ppp, ellmin, ellmax);
    fmp2=fmp(cmd, gd, mp, r, thetap2, ppp, ellmin, ellmax);

    integrand = 0.5 * r * ( xiplus*(fmmp11*fmmp22 + fmmp12*fmmp21) + fm2*fmmp11*fmp2
                + fm2*fm_nmp12*fmp1 + fm1*fmmp22*fmp1 + fm1*fm_nmp21*fmp2 );
    
    return  integrand;
    
};

double fmmp_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                      double ell, int m, int mp, double r,
                      double theta, double thetap)
{
    return     ell * Cls(cmd, gd, ell)
                * Bessel_Jn(m, theta*ell) * Bessel_Jn(mp, thetap*ell)
                * Bessel_Jn(m+mp, r*ell);

}

double fmmp(struct  cmdline_data* cmd, struct  global_data* gd,
            int m, int mp, double r, double theta, double thetap,
            int points_per_period, double ellmin, double ellmax) /* Fmm'*/
{
    double period, ell, deltaell;
    int Ntotal;
    double intA, intB,intval;

    period = 2*M_PI / max_of_three(theta,thetap,r);
    deltaell = period / points_per_period;
    
    Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
    
    intA = fmmp_integrand(cmd, gd, ellmin, m, mp, r, theta, thetap);

    intB = 0.0;
    intval=0.0;

    for (int i=1; i<Ntotal; i++){
        ell = ellmin + i * deltaell;
        intB = fmmp_integrand(cmd, gd, ell, m, mp, r, theta, thetap);
        intval = intval + 0.5 * (intA + intB) * deltaell;
        intA = intB;
    }

    return  intval / (2.*M_PI);
    
};

double fm_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                    double ell, int m, double r,  double theta)
{
    return     ell * Cls(cmd, gd, ell)
                * Bessel_Jn(m, theta*ell) * Bessel_Jn(m, r*ell);

}

double fm(struct  cmdline_data* cmd, struct  global_data* gd,
          int m, double r, double theta,
          int points_per_period, double ellmin, double ellmax)
{
    double period, ell, deltaell;
    int Ntotal;
    double intA, intB,intval;
    
    period = 2*M_PI / max_of_two(theta,r);
    deltaell = period / points_per_period;
    
    Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
    
    intA = fm_integrand(cmd, gd, ellmin, m, r, theta);
    intB = 0.0;
    intval=0.0;
    for (int i=1; i<Ntotal; i++){
        ell = ellmin + i * deltaell;
        intB = fm_integrand(cmd, gd, ell, m, r, theta);
        intval = intval + 0.5 * (intA + intB) * deltaell;
        intA = intB;
    }

    return  intval / (2.*M_PI);
    
};


double fmp_integrand(struct  cmdline_data* cmd, struct  global_data* gd,
                     double ell, int mp, double r,
                     double thetap)
{
    return     ell * Cls(cmd, gd, ell)
                * Bessel_Jn(mp, thetap*ell) * Bessel_Jn(mp, r*ell);

}

double fmp(struct  cmdline_data* cmd, struct  global_data* gd,
           int mp, double r, double thetap,
           int points_per_period, double ellmin, double ellmax)
{
    double period, ell, deltaell;
    int Ntotal;
    double intA, intB,intval;
    
    period = 2*M_PI / max_of_two(thetap,r);
    deltaell = period / points_per_period;
    
    Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;

    intA = fmp_integrand(cmd, gd, ellmin, mp, r, thetap);
    intB = 0.0;
    intval=0.0;
    for (int i=1; i<Ntotal; i++){
        ell = ellmin + i * deltaell;
        intB = fmp_integrand(cmd, gd, ell, mp, r, thetap);
        intval = intval + 0.5 * (intA + intB) * deltaell;
        intA = intB;
    }

    return  intval / (2.*M_PI);
    
};

double max_of_three(double a, double b, double c) {
  double max = a;
  if (b > max) max = b;
  if (c > max) max = c;
  return max;
}

double max_of_two(double a, double b) {
  double max = a;
  if (b > max) max = b;
  return max;
}

// va devolviendo el valor de la función de bessel
//  según  sea la n introducida
double Bessel_Jn(int n, double x)
{
    double Jn=0.0;
    double largenumber = 100000.0;
    
        
    if(n==0){
//        if(x<20)/*para bessels con argumento mayor que 20 toma la función
//          pero para menores hace una aproximación
        if(x<largenumber){
             Jn = gsl_sf_bessel_J0(x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==1){
        if(x<largenumber){ //check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==2){
        if(x<largenumber){//check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==3){
        if(x<largenumber){//check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==4){
        if(x<largenumber){//check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==5){
        if(x<largenumber){//check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n==6){
        if(x<largenumber){//check
             Jn = gsl_sf_bessel_Jn(n,x);
         } else {
             Jn = sqrt(2./(x*M_PI)) * cos( x - M_PI*(2.*n + 1) /4.)
             - sin(x - M_PI*(2.*n + 1) /4.) * (4.*n*n -1)/8 *sqrt(2./(M_PI*x*x*x));
         }
    }

    if(n>6) Jn = gsl_sf_bessel_Jn(n,x);

    return Jn;
    
}
//E from IntegralCovMatrix

//B socket:
#ifdef ADDONS
#include "wlcov_include_02.h"
#endif
//E

