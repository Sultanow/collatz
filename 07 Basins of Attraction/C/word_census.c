/* word_census.c -- Run J. For every type (l,k), l<=LMAX, q=3: histogram of reduced denominators e<=CMAX over ALL words.
   e = |D|/gcd(z(B),|D|), z(B)=sum_j 3^{k-j} 2^{p_j}, D=2^l-3^k. Output: l k |D| e count */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef unsigned __int128 u128;
static int L,K; static uint64_t A; static int nbig; static uint64_t big[4096]; static uint64_t pw2[64];
static uint64_t hist[4096];
static void dfs(int pos,int ones,uint64_t z){
  if(ones==K){ /* remaining positions are zeros */
    for(int i=0;i<nbig;i++) if(z%big[i]==0){hist[i]++;return;}
    return; }
  if(L-pos < K-ones) return;
  /* bit 0 */ dfs(pos+1,ones,z);
  /* bit 1 */ dfs(pos+1,ones+1,(uint64_t)(((u128)3*z+pw2[pos])%A));
}
int main(int argc,char**argv){
  int LMAX=atoi(argv[1]); uint64_t CMAX=strtoull(argv[2],0,10);
  for(int l=1;l<=LMAX;l++){
    for(int k=1;k<=l;k++){
      __int128 D=((__int128)1<<l); __int128 p3=1; for(int i=0;i<k;i++)p3*=3; D-=p3;
      if(D==0) continue; A=(uint64_t)(D<0?-D:D);
      for(int i=0;i<64;i++) pw2[i]=(uint64_t)(((u128)1<<i)%A);
      /* big divisors d of A with A/d <= CMAX, sorted descending (e ascending) */
      nbig=0; for(uint64_t e=1;e<=CMAX && e<=A;e++) if(A%e==0) big[nbig++]=A/e;
      for(int i=0;i<nbig;i++) hist[i]=0;
      L=l;K=k; dfs(0,0,0);
      for(int i=0;i<nbig;i++) if(hist[i]) printf("%d %d %llu %llu %llu\n",l,k,(unsigned long long)A,(unsigned long long)(A/big[i]),(unsigned long long)hist[i]);
    }
    fflush(stdout);
  }
  return 0;
}
