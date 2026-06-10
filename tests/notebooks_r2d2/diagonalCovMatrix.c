#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

clock_t start_t, end_t; 
#define n_data_max 100000
double ell_data[n_data_max],Cls_data[n_data_max];
int n_data;

double max_of_two(double a, double b);
double max_of_three(double a, double b, double c);

double Bessel_Jn(int n, double x);
double Cls(double ell);

double to_integrate(int m, int mp, double r, 
			double theta1, double thetap1, double theta2, double thetap2,
			int ppp, double ellmin, double ellmax);


double xi_integrad( double r, double ell);
double xi( double r, int points_per_period, double ellmin, double ellmax);

double fmmp_integrand(double ell, int m, int mp, double theta, double thetap, double r);
double fmmp(int m, int mp, double r, double theta, double thetap,
			int points_per_period, double ellmin, double ellmax);

double fm_integrand(double ell, int m, double theta, double r); 
double fm(int m, double r, double theta, int points_per_period, double ellmin, 
		double ellmax);

double fmp_integrand(double ell, int mp, double thetap, double r); 
double fmp(int mp, double r, double thetap, int points_per_period, double ellmin, 
			double ellmax);

void printMatrixToFile(int N, int M, int matrix[N][M], int m, int mp);


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
double r_int;
int Nt=10;
double tangleT[]={0.00232711, 0.00332769, 0.00475849, 0.0068045, 
	0.00973022, 0.0139139, 0.0198965, 0.0284513, 0.0406845, 0.0581776};
double CovMat[10][10];	

double getDoubleValue(char *str, double defaultValue) {
    char *token = strchr(str, '='); 
    if (token != NULL) {
        return atof(token + 1);}
    return defaultValue;
}

char *getStringValue(char *str, char *defaultValue) {
    char *token = strchr(str, '='); 
    if (token != NULL) {
        return token + 1;}
    return defaultValue;
}

int getIntValue(char *str, int defaultValue) {
    char *token = strchr(str, '=');
    if (token != NULL) {
        return atoi(token + 1); }
    return defaultValue;
}


int main (int argc, char *argv[]) 
{

    double r = 0.116355;                  // Default value for r
    double theta1 = 0.1;//                // Default value for theta1
    double theta2 = 0.1;//                // Default value for theta2
    double thetap1 = 0.1;//              // Default value for thetap1
    double thetap2 = 0.1;//              // Default value for thetap2
    double ellmin = 1;                 // Default value for ellmin
    double ellmax = 2000.0;              // Default value for ellmax
    char *inputfile = "CkappaT_ep.dat";   // Default value for inputfile
    int m = 0;                           // Default value for m
    int mp = 0;                        // Default value for mp
    int ppp = 20;                      // Default value for ppp
    
    int fsky=1;


    //Read parameters from command line
	//
    // Check if command-line arguments are provided
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (strncmp(argv[i], "r=", 2) == 0) { 
                r = getDoubleValue(argv[i], r);
            } else if (strncmp(argv[i], "theta1=", 7) == 0) { 
                theta1 = getDoubleValue(argv[i], theta1);
            } else if (strncmp(argv[i], "theta2=", 7) == 0) { 
                theta2 = getDoubleValue(argv[i], theta2);
            } else if (strncmp(argv[i], "thetap1=", 8) == 0) {
                thetap1 = getDoubleValue(argv[i], thetap1);
            } else if (strncmp(argv[i], "thetap2=", 8) == 0) {
                thetap2 = getDoubleValue(argv[i], thetap2);
            } else if (strncmp(argv[i], "ellmin=", 7) == 0) {
                ellmin = getDoubleValue(argv[i], ellmin);
            } else if (strncmp(argv[i], "ellmax=", 7) == 0) {
                ellmax = getDoubleValue(argv[i], ellmax);
            } else if (strncmp(argv[i], "inputfile=", 10) == 0) {
                inputfile = getStringValue(argv[i], inputfile);
            } else if (strncmp(argv[i], "m=", 2) == 0) {
                m = getIntValue(argv[i], m);
            } else if (strncmp(argv[i], "mp=", 3) == 0) {
                mp = getIntValue(argv[i], mp);
            } else if (strncmp(argv[i], "ppp=", 4) == 0) {
                ppp = getIntValue(argv[i], ppp);
            }
        }
    }
    double rArcMin,theta1ArcMin, theta2ArcMin, thetap1ArcMin, thetap2ArcMin,radToArcMin; 
    
    radToArcMin = 180.0*60.0 / M_PI; 
    rArcMin       = r *radToArcMin;               
    theta1ArcMin   = theta1*radToArcMin;  
    theta2ArcMin   = theta2*radToArcMin;           
    thetap1ArcMin  = thetap1*radToArcMin; 
    thetap2ArcMin  = thetap2*radToArcMin;

    	// Print the values
	//~ printf("r= %.7f  = %.2f arcmin\n", r, rArcMin);
	//~ printf("theta1= %.7f = %.2f arcmin\n", theta1, theta1ArcMin);
	//~ printf("theta2= %.7f = %.2f arcmin\n", theta2, theta2ArcMin);
	//~ printf("thetap1= %.7f = %.2f arcmin\n", thetap1, thetap1ArcMin);
	//~ printf("thetap2= %.7f = %.2f arcmin\n", thetap2, thetap2ArcMin);
	printf("ellmin= %.3f\n", ellmin);
	printf("ellmax- %.3f\n", ellmax);
	printf("inputfile: %s\n", inputfile);
	printf("m= %d\n", m);
	printf("mp= %d\n", mp);
	printf("ppp (points_per_period)= %d\n", ppp);

	FILE *fp;		
	n_data=0;
	fp=fopen(inputfile,"r");   
	if(fp!=NULL){
		while(fscanf(fp, "%lf %lf", &ell_data[n_data], &Cls_data[n_data])!=EOF){
			n_data++;
			if(n_data>n_data_max) printf("n_data_max should be larger than the number of data lines \n");
		}  // ell   Ckappa
		fclose(fp);
	}	

	//~ start_t = clock(); 
	//~ xiplus = xi(r, ppp, ellmin, ellmax);
	//~ fmmp11=fmmp(m, mp, r, theta1, thetap1, ppp, ellmin, ellmax);
	//~ fmmp22=fmmp(m, mp, r, theta2, thetap2, ppp, ellmin, ellmax);
	//~ fmmp12=fmmp(m, mp, r, theta1, thetap2, ppp, ellmin, ellmax);
	//~ fmmp21=fmmp(m, mp, r, theta2, thetap1, ppp, ellmin, ellmax);

	//~ fm_nmp12=fmmp(m, -mp, r, theta1, thetap2, ppp, ellmin, ellmax);
	//~ fm_nmp21=fmmp(m, -mp, r, theta2, thetap1, ppp, ellmin, ellmax);

	//~ fm1=fm(m, r, theta1, ppp, ellmin, ellmax);
	//~ fm2=fm(m, r, theta2, ppp, ellmin, ellmax);
	//~ fmp1=fmp(mp, r, thetap1, ppp, ellmin, ellmax);
	//~ fmp2=fmp(mp, r, thetap2, ppp, ellmin, ellmax);	


	//~ integrand = 0.5 / fsky * r * xiplus*(fmmp11*fmmp22 + fmmp12*fmmp21) + fm2*fmmp11*fmp2 
				//~ + fm2*fm_nmp12*fmp1 + fm1*fmmp22*fmp1 + fm1*fm_nmp21*fmp2;

	//~ end_t = clock(); 

	/*
	printf ("%g \n", xiplus);
	printf ("%g \n", fmmp11);
	printf ("%g \n", fmmp22);
	printf ("%g \n", fmmp12);
	printf ("%g \n", fmmp21);
	printf ("%g \n", fm_nmp12);
	printf ("%g \n", fm_nmp21);
	printf ("%g \n", fm1); 
	printf ("%g \n", fm2);
	printf ("%g \n", fmp1);
	printf ("%g \n", fmp2);
	*/
	//~ printf ("integrand(r=%g)=%g \n", r,integrand);
	//~ printf("Total time: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC  );


	//~ start_t = clock();	
	//~ integrand = to_integrate(m, mp, r, 
			//~ theta1, thetap1, theta2, thetap2,
			//~ ppp, ellmin, ellmax);
	//~ end_t = clock();
	//~ printf ("integrand(r=%g)=%g \n", r,integrand);		
	//~ printf("Total time: %f\n", (double)(end_t - start_t) / CLOCKS_PER_SEC);  
	
	
	
	
	double rmax, rmin, log10rmin,step,log10ri,ri;
	double intA, intB, deltar, intval, prevr;
	int Nr;
	
	Nr=50;
	rmax=0.349066;
	rmin=0.00232711;
	log10rmin=log10(rmin);	
	step=log10(rmax/rmin)/(Nr-1);

	printf ("rmin=%g , rmax=%g \n", rmin, rmax);
		
	printf ("integrating... \n");	


	
	start_t = clock();	


	for (int ti=0; ti<Nt; ti++){
		for(int tj=ti; tj<Nt; tj++){
			

			ri=rmin;
			intA = to_integrate(m, mp, ri,theta1, thetap1, theta2, thetap2, 
								ppp, ellmin, ellmax);
			intB   = 0.0;
			intval = 0.0;
			prevr  = ri;	
			
			theta1=tangleT[ti];
			thetap1=tangleT[tj];
			theta2=theta1;
			thetap2=thetap1;
			
			for(int i=1; i<Nr; i++){		
				log10ri = log10rmin + i*step;	
				ri=pow(10.,log10ri);

				intB = to_integrate(m, mp, ri, theta1, thetap1, theta2, thetap2, 
									ppp, ellmin, ellmax);
				intval = intval + 0.5 * (intA + intB) * (ri-prevr);
				intA = intB;
				prevr = ri;								
			};
			
			intval=intval/fsky;
			
			CovMat[ti][tj]=intval;
			if(ti!=tj) CovMat[tj][ti]=intval;
		}
	}
	
	end_t = clock(); 

    char filename[50]; // Adjust the array size as needed to hold the filename
    sprintf(filename, "Cov_m%d_mp%d.txt", m, mp); // Create the filename

    FILE* file = fopen(filename, "w"); // Open the file for writing ("w" mode)

    //if (file == NULL) {
        //printf("Error opening the file.\n");
        //return;
    //}

	printf("printing to file %s\n", filename);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fprintf(file, "%e ", CovMat[i][j]);
        }
        fprintf(file, "\n"); // Add a newline after each row
    }

    fclose(file); // Close the file

	
	
  return 0;

}


// Cell(ell) interpolated from the given table,  ell,  Cell
// change later for cubic spline if necessary 	
double Cls(double ell)   
{
	int j,j1,j2,jm;
	double lell,f;

	lell=log10(ell);
	if(ell<ell_data[0]) return 0.;
	if(ell>ell_data[n_data-1]) return 0.;
  
	j1=0, j2=n_data-1, jm=(j1+j2)/2;
	for(;;){
		if(ell>ell_data[jm]) j1=jm;
		else j2=jm;
		jm=(j1+j2)/2;

		if(j2-j1==1) break;
	}
	j=j1;

	f=(log10(Cls_data[j+1])-log10(Cls_data[j]))/(log10(ell_data[j+1])
		-log10(ell_data[j]))*(lell-log10(ell_data[j]))+log10(Cls_data[j]);
  
  return pow(10.,f);  
}

double xi_integrand( double r, double ell)
{										 
	return 	ell * Cls(ell) * gsl_sf_bessel_J0(r*ell);

}

double xi( double r, int points_per_period, double ellmin, double ellmax) 
{
 
	double period, ell, deltaell;
	int Ntotal;	
	double intA, intB,intval;
	
	period = 2*M_PI ;
	deltaell = period / points_per_period;
	
	Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
	
		
	intA = xi_integrand(r, ellmin);
	intB = 0.0;
	intval=0.0;

	for (int i=1; i<Ntotal; i++){
		ell = ellmin + i * deltaell; 
		intB = xi_integrand(r, ell);
		intval = intval + 0.5 * (intA + intB) * deltaell;
		intA = intB;	
	}
	
	return  intval / (2.*M_PI);
	
};




double to_integrate(int m, int mp, double r, 
			double theta1, double thetap1, double theta2, double thetap2,
			int ppp, double ellmin, double ellmax) 
{
 
	double integrand, xiplus, fmmp11,fmmp22,fmmp12,fmmp21;
	double fm_nmp12,fm_nmp21,fm1,fm2,fmp1,fmp2;
	
	xiplus = xi(r, ppp, ellmin, ellmax);
	fmmp11=fmmp(m, mp, r, theta1, thetap1, ppp, ellmin, ellmax);
	fmmp22=fmmp(m, mp, r, theta2, thetap2, ppp, ellmin, ellmax);
	fmmp12=fmmp(m, mp, r, theta1, thetap2, ppp, ellmin, ellmax);
	fmmp21=fmmp(m, mp, r, theta2, thetap1, ppp, ellmin, ellmax);

	fm_nmp12=fmmp(m, -mp, r, theta1, thetap2, ppp, ellmin, ellmax);
	fm_nmp21=fmmp(m, -mp, r, theta2, thetap1, ppp, ellmin, ellmax);

	fm1=fm(m, r, theta1, ppp, ellmin, ellmax);
	fm2=fm(m, r, theta2, ppp, ellmin, ellmax);
	fmp1=fmp(mp, r, thetap1, ppp, ellmin, ellmax);
	fmp2=fmp(mp, r, thetap2, ppp, ellmin, ellmax);	


	integrand = 0.5 * r * ( xiplus*(fmmp11*fmmp22 + fmmp12*fmmp21) + fm2*fmmp11*fmp2 
				+ fm2*fm_nmp12*fmp1 + fm1*fmmp22*fmp1 + fm1*fm_nmp21*fmp2 );
	
	return  integrand;
	
};




double fmmp_integrand(double ell, int m, int mp, double r,  
						double theta, double thetap)
{										 
	return 	ell * Cls(ell) 
				* Bessel_Jn(m, theta*ell) * Bessel_Jn(mp, thetap*ell)
				* Bessel_Jn(m+mp, r*ell);

}




double fmmp(int m, int mp, double r, double theta, double thetap,
			int points_per_period, double ellmin, double ellmax) /* Fmm'*/
{
 
	double period, ell, deltaell;
	int Ntotal;	
	double intA, intB,intval;
	
	period = 2*M_PI / max_of_three(theta,thetap,r);
	deltaell = period / points_per_period;
	
	Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
	
		
	intA = fmmp_integrand(ellmin, m, mp, r, theta, thetap);
	intB = 0.0;
	intval=0.0;

	for (int i=1; i<Ntotal; i++){
		ell = ellmin + i * deltaell; 
		intB = fmmp_integrand(ell, m, mp, r, theta, thetap);
		intval = intval + 0.5 * (intA + intB) * deltaell;
		intA = intB;	
	}
	
	return  intval / (2.*M_PI);
	
};

double fm_integrand(double ell, int m, double r,  double theta)
{										 
	return 	ell * Cls(ell) 
				* Bessel_Jn(m, theta*ell) * Bessel_Jn(m, r*ell);

}

double fm(int m, double r, double theta,
			int points_per_period, double ellmin, double ellmax) 
{
 

	double period, ell, deltaell;
	int Ntotal;	
	double intA, intB,intval;
	
	period = 2*M_PI / max_of_two(theta,r);
	deltaell = period / points_per_period;
	
	Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
	
		
	intA = fm_integrand(ellmin, m, r, theta);
	intB = 0.0;
	intval=0.0;
	for (int i=1; i<Ntotal; i++){
		ell = ellmin + i * deltaell; 
		intB = fm_integrand(ell, m, r, theta);
		intval = intval + 0.5 * (intA + intB) * deltaell;
		intA = intB;	
	}

	
	return  intval / (2.*M_PI);
	
};


double fmp_integrand(double ell, int mp, double r,  
						double thetap)
{										 
	return 	ell * Cls(ell) 
				* Bessel_Jn(mp, thetap*ell) * Bessel_Jn(mp, r*ell);

}

double fmp(int mp, double r, double thetap,
			int points_per_period, double ellmin, double ellmax) 
{
 

	double period, ell, deltaell;
	int Ntotal;	
	double intA, intB,intval;
	
	period = 2*M_PI / max_of_two(thetap,r);
	deltaell = period / points_per_period;
	
	Ntotal = (int)( (ellmax-ellmin) / deltaell + 1) +1;
	
		
	intA = fmp_integrand(ellmin, mp, r, thetap);
	intB = 0.0;
	intval=0.0;
	for (int i=1; i<Ntotal; i++){
		ell = ellmin + i * deltaell; 
		intB = fmp_integrand(ell, mp, r, thetap);
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

double Bessel_Jn(int n, double x){ /* va devolviendo el valor de la función de bessel 
según  sea la n introducida*/
	
	double Jn=0.0;
	double largenumber = 100000.0;
	
		
	if(n==0){		
//		if(x<20)/*para bessels con argumento mayor que 20 toma la función pero para menores 
//		hace una aproximación*/
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

















