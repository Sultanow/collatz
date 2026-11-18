#include <bits/stdc++.h>
using namespace std;
using u128 = unsigned __int128;
int main(int argc,char**argv){
 int k=(argc>1)?atoi(argv[1]):20;
 double beta=(argc>2)?atof(argv[2]):0.1;
 long long N=(argc>3)?atoll(argv[3]):500000;
 double X=ldexp(1.0,k), Y=pow(X,1-beta);
 uint64_t B=(uint64_t)floor(pow(X,1-2*beta));
 int L=(int)floor(log(X));
 vector<uint64_t> counts(B+1,0);
 mt19937_64 gen(20261118);
 uniform_real_distribution<double> rng((k+5)*log(2.0), (k+20)*log(2.0));
 long long hits=0, early=0, nonhit=0; long double mass=0;
 for(long long j=0;j<N;j++) {
     uint64_t m=(uint64_t)exp(rng(gen)); m|=1;
     u128 a=m; int steps=0;
     while(a>B && steps<10000){u128 v=3*a+1; while(!(v&1)){v>>=1;} a=v;steps++; }
     if(a>B){nonhit++;continue;}
     uint64_t y=(uint64_t)a;counts[y]++;hits++;
     u128 b=y; bool escaped=false;
     for(int t=1;t<=L;t++){
        b=(b&1)?(3*b+1)/2:b/2;
        if((long double)b>Y){escaped=true; if((long double)b<=X)mass+=(long double)b;}
     }
     if(escaped) early++;
 }
 long double lpnorm_p=0; double p=1.5;
 for(uint64_t y=1;y<=B;y++){
    long double rho=(long double)B*counts[y]/N;
    lpnorm_p+=pow(rho,p);
 }
 double mnorm=pow(lpnorm_p/B,1/p);
 cout<<fixed<<setprecision(8)<<"X=2^"<<k<<" beta="<<beta<<" B="<<B<<" L="<<L<<" N="<<N<<"\n"
     <<"hitfraction="<<double(hits)/N<<" nohit="<<nonhit
     <<" uniform_escape_probability=(from earlier runs)"
     <<" entrance_early_escape_probability="<<double(early)/N
     <<" entrance_mean_early_window_weight_over_X="<<double(mass/N/X)
     <<" sampled_Lp_density_norm_p1.5="<<mnorm<<"\n";
}
