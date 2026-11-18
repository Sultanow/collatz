#include <bits/stdc++.h>
using namespace std;
using u64=uint64_t;
int main(int argc,char**argv){
 int k=argc>1?atoi(argv[1]):20;
 double beta=argc>2?atof(argv[2]):0.005;
 long double X=ldexpl(1.L,k),Breal=powl(X,1-2*beta),Y=powl(X,1-beta);
 u64 B=(u64)floorl(Breal);
 int L=(int)floorl(logl(X));u64 M=1ULL<<L,m=M/2;
 vector<u64> c(M,0); vector<u64> p5(M,0);vector<u64> p10(M,0);
 u64 nall=0,nhit=0,n5=0,n10=0,nbad=0,escape=0,nfail=0,maxDepth=0;
 for(u64 n=B+1+(B%2);n<=2*B;n+=2){
  nall++;
  u64 y=n;unsigned d=0;
  while(y>B&&d<10000){u64 t=3*y+1; y=t>>__builtin_ctzll(t);d++;}
  if(y>B){nfail++;continue;}
  nhit++; maxDepth=max(maxDepth,(u64)d);
  c[y&(M-1)]++;
  if(d<=5){p5[y&(M-1)]++;n5++;}
  if(d<=10){p10[y&(M-1)]++;n10++;}
  u64 z=y;bool yes=false;
  for(int j=1;j<=L;j++){z=(z&1)?(3*z+1)/2:z/2;if((long double)z>Y)yes=true;if(z>Y&&z<=X)escape++;}
  nbad+=yes;
 }
 auto quality=[&](vector<u64>&v,u64 N){long double sum=0; for(int r=1;r<M;r+=2){long double a=v[r];sum+=a*a;}return (long double)m*sum/((long double)N*N);};
 cout<<fixed<<setprecision(8)<<"X=2^"<<k<<" beta="<<beta<<" B="<<B<<" m="<<m<<" sources="<<nall<<" reached="<<nhit<<" failures="<<nfail<<" max_depth="<<maxDepth<<"\n";
 cout<<"first_pass_by5="<<double(n5)/nall<<" by10="<<double(n10)/nall<<" full_collision="<<(double)quality(c,nhit)<<" collision_t5="<<(double)quality(p5,n5)<<" collision_t10="<<(double)quality(p10,n10)<<" early_escapes="<<(double)nbad/nall<<" window_occupations_per_source="<<(double)escape/nall<<"\n";
}
