/* tail_check.c -- distribution of Phi_X/X for actual orbits and for the random-walk model.
 * Usage: tail_check C LOG10X BETA S SEED MODE      (MODE = actual | model)
 * Starts: odd, logarithmically distributed in [X^(1+b), X^((1+b)^2)].
 * Phi_X(m): sum of the orbit points in W_X = (X^(1-b), X] in the Syracuse blocks 1..tau,
 * tau = first Syracuse iterate <= X_1 = X^(1-2b) (the odd endpoint of block tau is included).
 * Output: mean, second moment, max of Phi/X, number of overflows, and P(Phi/X > s) for s = 1..60. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
typedef unsigned __int128 u128;
static uint64_t st[2];
static uint64_t sm(uint64_t *x){uint64_t z=(*x+=0x9E3779B97F4A7C15ull);z=(z^(z>>30))*0xBF58476D1CE4E5B9ull;z=(z^(z>>27))*0x94D049BB133111EBull;return z^(z>>31);}
static uint64_t rnd(void){uint64_t a=st[0],b=st[1],r=a+b;b^=a;st[0]=((a<<24)|(a>>40))^b^(b<<16);st[1]=(b<<37)|(b>>27);return r;}
static double unif(void){return (rnd()>>11)*(1.0/9007199254740992.0);}
static void pr128(FILE*f,u128 x){char b[50];int i=49;b[i]=0;if(!x)b[--i]='0';while(x){b[--i]='0'+(int)(x%10);x/=10;}fputs(b+i,f);}
static long double u128ld(u128 x){return (long double)(uint64_t)(x>>64)*18446744073709551616.0L+(long double)(uint64_t)x;}
int main(int argc,char**argv){
  if(argc<7){fprintf(stderr,"usage: %s C LOG10X BETA S SEED actual|model\n",argv[0]);return 1;}
  long long c=atoll(argv[1]); double E=atof(argv[2]), b=atof(argv[3]); long long S=atoll(argv[4]);
  uint64_t seed=strtoull(argv[5],0,10); int model=!strcmp(argv[6],"model");
  st[0]=sm(&seed); st[1]=sm(&seed);
  long double lnX=E*logl(10.0L), lnTop=lnX, lnBot=(1-b)*lnX, lnX1=(1-2*b)*lnX;
  long double lnu=(1+b)*lnX, lnv=(1+b)*(1+b)*lnX;
  long double X1=expl(lnX1), Wbot=expl(lnBot), Wtop=expl(lnTop), X=Wtop;
  enum{NS=61}; static long long tail[NS]; double sum=0,sum2=0,mx=0; long long over=0,unres=0;
  const u128 LIM=((u128)1)<<125;
  for(long long it=0;it<S;it++){
    long double L=lnu+(lnv-lnu)*(long double)unif(); long double phi=0;
    if(!model){
      int sh=(int)floorl(L/logl(2.0L))-62; if(sh<0) sh=0;
      uint64_t hi=(uint64_t)expl(L-sh*logl(2.0L));
      u128 lo=(((u128)rnd()<<64)|rnd()); if(sh<128) lo&=((((u128)1)<<sh)-1); 
      u128 x=(((u128)hi)<<sh)|lo; x|=1; u128 x0=x;
      int ok=1, done=0; long steps=0;
      while(!done){
        /* x odd: one Syracuse step = T(x)=(3x+c)/2 then halvings; block points are T(x) ... odd end */
        __int128 y=(__int128)3*(__int128)x+c; if(y<=0){done=1;break;}
        u128 t=(u128)y/2;
        if(t>LIM){ok=0;break;}
        while(1){
          long double tv=u128ld(t);
          if(tv>Wbot && tv<=Wtop) phi+=tv/X;
          if(t&1) break;
          t>>=1;
        }
        x=t; steps++;
        if(u128ld(x)<=X1) done=1;
        if(steps>100000){ok=0;unres++;break;}
      }
      if(!ok){ over++; fprintf(stderr,"OVF "); pr128(stderr,x0); fprintf(stderr,"\n"); continue; }
    } else {
      long double D=0; const long double l2=logl(2.0L), l3=logl(3.0L);
      while(1){
        int g=1; while(unif()<0.5) g++;
        long double Dn=D+g*l2-l3;
        for(int i=0;i<g;i++){ long double z=L-Dn+i*l2; if(z>lnBot && z<=lnTop) phi+=expl(z-lnX); }
        D=Dn; if(L-D<=lnX1) break;
      }
    }
    double p=(double)phi; sum+=p; sum2+=p*p; if(p>mx) mx=p;
    for(int s=1;s<NS;s++) if(p>s) tail[s]++; else break;
  }
  printf("# c=%lld log10X=%.1f beta=%.4f S=%lld mode=%s overflow=%lld unresolved=%lld\n",c,E,b,S,argv[6],over,unres);
  printf("mean %.5f second %.5f max %.3f\n",sum/S,sum2/S,mx);
  printf("tail");
  for(int s=1;s<NS;s++) printf(" %lld",tail[s]);
  printf("\n");
  return 0;
}
