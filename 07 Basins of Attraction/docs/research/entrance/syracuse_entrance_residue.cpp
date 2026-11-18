#include <bits/stdc++.h>
using namespace std;
using u128=unsigned __int128;
int main(int argc,char**argv){
 int k=argc>1?atoi(argv[1]):24;double beta=argc>2?atof(argv[2]):0.1;
 int N=argc>3?atoi(argv[3]):600000;int shift=argc>4?atoi(argv[4]):0;
 long double X=ldexpl(1.0L,k),Y=powl(X,1-beta);uint64_t B=(uint64_t)floorl(powl(X,1-2*beta));
 int L=(int)floor(logl(X)), K=1<<(L-1), M=1<<L;
 vector<uint32_t> c(M);long long hits=0,nohits=0,escape=0,parity_bad=0;
 long double windowmass=0, avg_ratio=0;uint64_t high=0,small=0,medium=0,mod31=0,mod32=0;
 mt19937_64 eng(20261118ULL+101*shift+7*k);uniform_real_distribution<double> rng((k+5+shift)*log(2.0),(k+20+shift)*log(2.0));
 for(int j=0;j<N;j++){
  long double z=expl(rng(eng));u128 a=(uint64_t)z;a|=1;
  int steps=0;while(a>B&&steps<10000){u128 v=3*a+1;while(!(v&1))v>>=1;a=v;steps++;}
  if(a>B){nohits++;continue;}
  uint64_t y=(uint64_t)a;c[y&(M-1)]++;hits++;mod31+=(y%3==1);mod32+=(y%3==2);
  high+=y>B/2;medium+=y>B/4 && y<=B/2;small+=y<=B/4;
  uint64_t n=y;bool yes=0;
  for(int t=1;t<=L;t++){n=(n&1)?(3*n+1)/2:n/2;if((long double)n>Y)yes=1; if((long double)n>Y && (long double)n<=X)windowmass+=n/X;}
  escape+=yes;
 }
 long double sumc2=0,sumdev=0,tv=0, m15=0;int nonzero=0;
 for(int r=1;r<M;r+=2){long double cr=c[r];sumc2+=cr*(cr-1);long double pr=cr/(long double)hits;tv+=fabsl(pr-1.0L/K);if(cr>0)nonzero++;}
 long double M2deb=(long double)K*sumc2/((long double)hits*(hits-1));
 // unconditional parity martingale bad-class sufficient condition, measured from residues
 int nbad=0;long long captured=0;
 for(int r=1;r<M;r+=2){int64_t n=r;int q=0;bool bad=false;for(int t=1;t<=L;t++){q+=(n&1);n=(n&1)?(3*n+1)/2:n/2;long double Z=powl(3.0L,q)/ldexpl(1.0L,t);if(Z>powl(X,beta)/2)bad=true;}if(bad){nbad++;captured+=c[r];}}
 cout<<fixed<<setprecision(6)<<"k="<<k<<" beta="<<beta<<" N="<<N<<" shift="<<shift<<" B="<<B<<" L="<<L<<" K="<<K<<"\n"
  <<"mod3_1="<<double(mod31)/hits<<" mod3_2="<<double(mod32)/hits<<" "<<"hits="<<hits<<" nohit="<<nohits<<" y> B/2="<<(double)high/hits<<" B/4<y<=B/2="<<(double)medium/hits<<" y<=B/4="<<(double)small/hits<<"\n"
  <<"early_escape="<<(double)escape/hits<<" early_weight_over_X="<<(double)windowmass/hits<<"\n"
  <<"M2_mod_odd_debiased="<<sqrtl(M2deb)<<" chi2_debiased="<<(double)(M2deb-1)<<" sample_TV_to_odd_uniform="<<(double)(tv/2)<<" occupied_odd_residues="<<nonzero<<"/"<<K<<"\n"
  <<"martingale_bad_odd_residue_fraction="<<(double)nbad/K<<" entrance_bad_parity_mass="<<(double)captured/hits<<" bound_doob_3X^-beta="<<3*powl(X,-beta)<<"\n";
}
