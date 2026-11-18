#include <bits/stdc++.h>
using namespace std;
int main(int argc, char**argv){
    const double beta = argc>=2 ? stod(argv[1]) : 0.1;
    cout<<fixed<<setprecision(7);
    cout<<"beta="<<beta<<"\n";
    for (int pow2: {20,24,28}) {
        double X = ldexp(1.0,pow2), Y=pow(X,1-beta);
        uint64_t B=(uint64_t)floor(pow(X,1-2*beta));
        int L=(int)floor(log(X));
        uint64_t cnt=0, window_hits=0; long double weighted=0;
        auto tic = chrono::steady_clock::now();
        for(uint64_t y=1;y<=B;y++){
           uint64_t n=y; bool escaping=false;
           for(int t=1;t<=L;t++) {
               n=(n&1)?(3*n+1)/2:n/2;
               if(n>Y){escaping=true; if(n<=X) {window_hits++;weighted+=n;}}
           }
           if(escaping)cnt++;
        }
        double seconds=chrono::duration<double>(chrono::steady_clock::now()-tic).count();
        cout << "X=2^"<<pow2 << " B="<<B<<" L="<<L<<" escapes="<<cnt<<" ratio="<<double(cnt)/B<<" hits="<<window_hits<<" meanWeighted/X="<<double(weighted/B/X)<<" X^-beta="<<pow(X,-beta)<<" seconds="<<seconds<<"\n";
    }
}
