// Reproducible finite-source diagnostic for first-passage incoming regularity.
// Not an estimator of the infinite-source logarithmic limiting entrance law.
#include <bits/stdc++.h>
using namespace std;
using u128=__uint128_t; using u64=uint64_t;
static uint64_t rng_state=0x9e3779b97f4a7c15ULL;
static uint64_t rng(){ uint64_t z=(rng_state+=0x9e3779b97f4a7c15ULL); z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL; z=(z^(z>>27))*0x94d049bb133111ebULL; return z^(z>>31);}
static u128 syr(u128 n){u128 v=3*n+1; while(!(v&1))v>>=1; return v;}
static u128 shortcut(u128 n){return (n&1)?(3*n+1)/2:n/2;}
static bool hit_below(u128 n,uint64_t B,u128 &y){
 for(int i=0;i<20000;i++){
   if(n<=B){y=n;return true;}
   if(n>(u128(1)<<115)) return false;
   n=syr(n);
 } return false;
}
static bool dangerous(uint64_t y,int L,long double Y){
 u128 z=y;
 for(int t=1;t<=L;t++){
  z=shortcut(z);
  if((long double)z>Y) return true;
 }return false;
}
int main(){
 const long double beta=.08L;
 const int N=200000;
 cout<<"beta=0.08 (stress-test; paper beta=0.005), samples="<<N<<" per shell\n";
 cout<<"B,j,L,lambda_E,incoming_rate,relative_L2,collision_Q,weighted_E,weighted_over_uniform,failed\n";
 for(uint64_t B:{2048ULL,8192ULL,32768ULL}){
  long double logX=logl((long double)B)/(1-2*beta);
  int L=(int)floorl(logX);long double Y=expl((1-beta)*logX);
  vector<char> E(B+1,false);
  uint64_t R=0, eN=0;
  for(uint64_t z=B+1;z<=2*B;z++) if(z&1){
    R++;u128 y=0; if(hit_below(z,B,y)&&dangerous((u64)y,L,Y)){E[z-B]=1;eN++;}
  }
  for(int j:{2,5,8}){
   uint64_t base=(2*B)<<j; // source shell (base,2base]
   vector<uint32_t> c(B+1,0);uint64_t incoming=0,success=0,bad=0;
   for(int t=0;t<N;t++){
     uint64_t n=base+1+2*(rng()% (base/2));
     u128 y=0;
     if(!hit_below(n,2*B,y)){bad++;continue;}
     success++;
     if(y>B){ c[(u64)y-B]++;incoming++; }
   }
   long double sq=0,ews=0;vector<u64> res(1<<(L-1),0);
   for(uint64_t z=B+1;z<=2*B;z++)if(z&1){
       long double v=c[z-B];sq+=v*v;
       if(E[z-B]) ews+=v;
       res[(z% (1ULL<<L))/2]+=c[z-B];
   }
   long double Q=0;for(auto v:res)Q+=(long double)v*v;
   if(incoming){sq=sqrtl((long double)R*sq/(incoming*incoming));Q=(1LL<<(L-1))*Q/(incoming*incoming);}
   cout<<B<<','<<j<<','<<L<<','<<fixed<<setprecision(6)<<(long double)eN/R<<','<<(long double)incoming/N<<','<<sq<<','<<Q<<','<<(incoming?ews/incoming:0)<<','<<(eN&&incoming?(ews/incoming)/((long double)eN/R):0)<<','<<bad<<'\n';
  }
 }
}
