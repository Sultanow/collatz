/******************************************************************************/
/*									      */
/* COMPUTE PROBIT FUNCTION						      */
/* 03/09/18 (dkc)							      */
/*									      */
/* 03/16/18 Changed initial p value from 0 to 1/(MAXN+1) (optional).	      */
/*									      */
/******************************************************************************/
#include <math.h>
#include <stdio.h>
unsigned int MAXN=6;
void main () {
long double a0=3.3871328727963666080e+0;
long double a1=1.3314166789178437745e+2;
long double a2=1.9715909503065514427e+3;
long double a3=1.3731693765509461125e+4;
long double a4=4.5921953931549871457e+4;
long double a5=6.7265770927008700853e+4;
long double a6=3.3430575583588128105e+4;
long double a7=2.5090809287301226727e+3;
long double b1=4.2313330701600911252e+1;
long double b2=6.8718700749205790830e+2;
long double b3=5.3941960214247511077e+3;
long double b4=2.1213794301586595867e+4;
long double b5=3.9307895800092710610e+4;
long double b6=2.8729085735721942674e+4;
long double b7=5.2264952788528545610e+3;
long double abhash=55.8831928806149014439;
long double c0=1.42343711074968357734e+0;
long double c1=4.63033784615654529590e+0;
long double c2=5.76949722146069140550e+0;
long double c3=3.64784832476320460504e+0;
long double c4=1.27045825245236838258e+0;
long double c5=2.41780725177450611770e-1;
long double c6=2.27238449892691845833e-2;
long double c7=7.74545014278341407640e-4;
long double d1=2.05319162663775882187e+0;
long double d2=1.67638483018380384940e+0;
long double d3=6.89767334985100004550e-1;
long double d4=1.48103976427480074590e-1;
long double d5=1.51986665636164571966e-2;
long double d6=5.47593808499534494600e-4;
long double d7=1.05075007164441684324e-9;
long double cdhash=49.33206503301610289036;
long double e0=6.65790464350110377720e+0;
long double e1=5.46378491116411436990e+0;
long double e2=1.78482653991729133580e+0;
long double e3=2.96560571828504891230e-1;
long double e4=2.65321895265761230930e-2;
long double e5=1.24266094738807843860e-3;
long double e6=2.71155556874348757815e-5;
long double e7=2.01033439929228813265e-7;
long double f1=5.99832206555887937690e-1;
long double f2=1.36929880922735805310e-1;
long double f3=1.48753612908506148525e-2;
long double f4=7.86869131145613259100e-4;
long double f5=1.84631831751005468180e-5;
long double f6=1.42151175831644588870e-7;
long double f7=2.04426310338993978564e-15;
long double efhash=47.52583317549289671629;
long double split1=0.425;
long double split2=5.0;
long double const1=0.180625;
long double const2=1.6;
unsigned int i;
long double q,r;
long double sum,p,temp,pinc;
FILE *Outfp;
Outfp = fopen("out5c.dat","w");
if (Outfp==NULL)
   return;
p=0.0;
pinc=1.0/(double)(MAXN+1);
p=p+pinc;
for (i=0; i<MAXN; i++) {
   q=p-0.5;
   temp=q;
   if (temp<0.0)
      temp=-temp;
   if (temp<=split1) {
      r=const1-q*q;
      sum=((((((a7*r+a6)*r+a5)*r+a4)*r+a3)*r+a2)*r+a1)*r+a0;
      sum=q*sum/(((((((b7*r+b6)*r+b5)*r+b4)*r+b3)*r+b2)*r+b1)*r+1.0);
      }
   else {
      if (q<0.0)
	 r=p;
      else
	 r=1.0-p;
      r=sqrt(-log(r));
      if (r<=split2) {
	 r=r-const2;
	 sum=((((((c7*r+c6)*r+c5)*r+c4)*r+c3)*r+c2)*r+c1)*r+c0;
	 sum=sum/(((((((d7*r+d6)*r+d5)*r+d4)*r+d3)*r+d2)*r+d1)*r+1.0);
	 }
      else {
	 r=r-split2;
	 sum=((((((e7*r+e6)*r+e5)*r+e4)*r+e3)*r+e2)*r+e1)*r+e0;
	 sum=sum/(((((((f7*r+f6)*r+f5)*r+f4)*r+f3)*r+f2)*r+f1)*r+1.0);
	 }
      if (q<0.0)
	 sum=-sum;
      }
   if (i==(i/100000)*100000)
      printf("p=%llf %llf \n",p,sum);
   fprintf(Outfp," %.16f, \n",sum);
   p=p+pinc;
   }
//
fclose(Outfp);
 return;
}
