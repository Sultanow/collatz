#include <bits/stdc++.h>
using namespace std;
using u128=__uint128_t;
static uint64_t seed0=202611161932ULL;
int main(int argc,char**argv){
 int samples=20000; if(argc>1)samples=atoi(argv[1]);
 int X=8192, low=2538, cut=786, limit=40000, L=3;
 mt19937_64 rng(seed0);
 uniform_real_distribution<long double> uni(0.0L,1.0L);
 vector<int> cs={-1,1,5,13};
 vector<int> K={4,5};
 for(int c:cs){
   for(int power: {1,2,3,4}){
     // m in [4X, 16X) for power=1, m in [X^power, 4X^power) otherwise
     long double base=(power==1?4.0L*X:powl((long double)X,power));
     u128 B[9]={}; long long successful=0,censored=0,excess=0,maxvisit=0;
     for(int s=0;s<samples;s++){
       long double z=base*expl(uni(rng)*logl(4.0L));
       u128 x=((u128)z)|1; vector<int> hits; hits.reserve(100);
       bool ok=false;
       for(int step=0;step<limit;step++){
         if(x <=(u128)cut){ok=true;break;}
         if(!(x & 1)) x/=2;
         else x=(c>=0 ? (3*x+(unsigned long long)c)/2 : (3*x-(unsigned long long)(-c))/2);
         if(x>(u128)low && x<=(u128)X)hits.push_back((int)x);
       }
       if(!ok){censored++;continue;}
       successful++;
       int sum=0,pos=-1;
       for(int i=0;i<(int)hits.size();i++){
         sum+=hits[i]; if(sum>L*X){pos=i;break;}
       }
       if(pos<0)continue;
       excess++;
       int N[2]={0,0};
       for(int i=pos+1;i<(int)hits.size();i++){
          int y=hits[i]; int j=(y> X/2)?0:1; N[j]++;
       }
       maxvisit=max(maxvisit,(long long)max(N[0],N[1]));
       for(int ell=0;ell<9;ell++){
         unsigned long long term=(unsigned long long)8*(N[0]>=((1<<ell)*K[0]))+ (unsigned long long)5*(N[1]>=((1<<ell)*K[1]));
         // weights: scaled by 2: 2*K0=8, K1=5
         B[ell]+=term;
       }
     }
     cout<<"c="<<c<<" power="<<power<<" samples="<<samples<<" successful="<<successful<<" censored="<<censored<<" excess="<<excess<<" maxN="<<maxvisit;
     for(int ell=0;ell<4;ell++){
       if(B[ell]>0)cout<<" q"<<ell<<"="<<fixed<<setprecision(4)<<(long double)B[ell+1]/(long double)B[ell];
     }
     cout<<"\n";
   }
 }
}
