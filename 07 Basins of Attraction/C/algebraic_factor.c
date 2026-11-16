/* algebraic_factor.c -- Run J. Even type (l,k): rate of P | z(B) among APERIODIC words, relative to 1/P, P=2^{l/2}+3^{k/2}. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef __int128 i128;
int main(int argc,char**argv){
  int l=atoi(argv[1]),k=atoi(argv[2]); int h=l/2;
  i128 p3=1; for(int i=0;i<k/2;i++) p3*=3; long long P=(long long)(((i128)1<<h)+p3);
  uint64_t full=(1ULL<<l)-1, w=(1ULL<<k)-1; i128 p3k=1; for(int i=0;i<k;i++) p3k*=3;
  double na=0, nP=0;
  while(1){
    int aper=1; uint64_t r=w; for(int s=1;s<l;s++){ r=((r>>1)|((r&1)<<(l-1)))&full; if(r==w){aper=0;break;} }
    if(aper){ i128 z=0,t3=p3k/3; for(int i=0;i<l;i++) if((w>>i)&1){ z+=t3*((i128)1<<i); t3/=3; }
      na++; if(z%P==0) nP++; }
    uint64_t c=w&-w, rr=w+c; if(rr>full) break; w=(((rr^w)>>2)/c)|rr; if(w>full) break;
  }
  printf("%d %d %s P=%lld aperiodic=%.0f P|z=%.0f ratio=%.3f\n",l,k,((i128)1<<l)>p3k?"D>0":"D<0",P,na,nP,nP/(na/P));
}
