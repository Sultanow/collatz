/******************************************************************************
*									      *
*  FIND CURVES AND SUB-CURVES OF DIRICHLET PRODUCTS OF ZETA FUNCTION ZEROS    *
*  10/22/18 (dkc)							      *
*									      *
*  For 32-bit words and N<=100000, the products of primes either don't        *
*  overflow or still give correct results if they do.			      *
*									      *
******************************************************************************/
#include <math.h>
unsigned int nucheck(unsigned int N, unsigned int power, unsigned int extra,
  unsigned int subcur, unsigned int tsize, unsigned int *table,
  unsigned int *skip, unsigned int nope) {
unsigned int i,j,k,l,m,n;
unsigned int p,q,r,s,t,u,pth,qth,rth; // use "unsigned long long" for 64-bit words
unsigned int sth,tth;
unsigned int N2,N3,N5,N7,N11;
N2=N/2+1;
N3=N/6+1;
N5=N/30+1;
N7=N/210+1;
N11=N/2310;
if ((power==27)&&(extra!=0))
   goto fjskip;
if ((power==25)&&(extra!=0))
   goto fzskip;
if ((power==23)&&(extra!=0))
   goto caskip;
if ((power==21)&&(extra!=0))
   goto bmskip;
if ((power==19)&&(extra!=0))
   goto baskip;
if ((power==17)&&(extra!=0))
   goto acskip;
if ((power==15)&&(extra!=0))
   goto gskip;
if ((power==13)&&(extra!=0))
   goto fskip;
if ((power==11)&&(extra!=0))
   goto eskip;
if ((power==9)&&(extra!=0))
   goto dskip;
if ((power==7)&&(extra!=0))
   goto cskip;
if ((power==5)&&(extra!=0))
   goto bskip;
if ((power==3)&&(extra!=0))
   goto askip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p;
   for (j=1; j<power; j++)
     pth=pth*p;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
return(0);
//
// 3
//
askip:
if (subcur==2)
   goto agskip;
if (subcur==3)
   goto zjskip;
if (subcur==4)
   goto zoskip;
if ((subcur==0)&&(skip[0]!=0))
   goto agskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
  return(0);
agskip:
if ((subcur==0)&&(skip[1]!=0))
   goto zjskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   for (j=i+1; j<tsize; j++) {
     q=table[j];
     if ((p*q)==N)
	return(1);
     if (N<(p*q))
	break;
     }
   }
if (subcur==2)
   return(0);
zjskip:
if ((subcur==0)&&(skip[2]!=0))
   goto zoskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==3)
   return(0);
zoskip:
if ((subcur==0)&&(skip[3]!=0))
   goto zpskip;
if (nope!=0)
   goto zpskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (p==N)
      return(1);
   if (N<p)
      break;
   }
zpskip:
if (skip[19]!=0)
   return(0);
//
// 5
//
bskip:
if (subcur==2)
   goto ahskip;
if (subcur==3)
   goto zkskip;
if ((subcur==0)&&(skip[0]!=0))
   goto ahskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
   return(0);
ahskip:
if ((subcur==0)&&(skip[1]!=0))
   goto zkskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
   return(0);
zkskip:
if ((subcur==0)&&(skip[2]!=0))
   goto zqskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
zqskip:
if (skip[19]!=0)
   return(0);
//
// 7
//
cskip:
if (subcur==2)
   goto afskip;
if (subcur==3)
   goto aiskip;
if (subcur==4)
   goto zlskip;
if ((subcur==0)&&(skip[0]!=0))
   goto afskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
  return(0);
afskip:
if ((subcur==0)&&(skip[1]!=0))
   goto aiskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
  return(0);
aiskip:
if ((subcur==0)&&(skip[2]!=0))
   goto zlskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   for (j=i+1; j<tsize; j++) {
     q=table[j];
     qth=p*q;
     if (N<qth)
	break;
     for (k=j+1; k<tsize; k++) {
       r=table[k];
       if ((qth*r)==N)
	  return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==3)
   return(0);
zlskip:
if ((subcur==0)&&(skip[3]!=0))
   goto zrskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
zrskip:
if (skip[19]!=0)
   return(0);
//
// 9
//
dskip:
if (subcur==2)
  goto nskip;
if (subcur==3)
  goto ajskip;
if (subcur==4)
   goto ziskip;
if ((subcur==0)&&(skip[0]!=0))
   goto nskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
  return(0);
nskip:
if ((subcur==0)&&(skip[1]!=0))
   goto ajskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
  return(0);
ajskip:
if ((subcur==0)&&(skip[2]!=0))
   goto ziskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==3)
   return(0);
ziskip:
if ((subcur==0)&&(skip[3]!=0))
   goto ztskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
ztskip:
if (skip[19]!=0)
   return(0);
//
// 11
//
eskip:
if (subcur==2)
   goto oskip;
if (subcur==3)
   goto pskip;
if (subcur==4)
   goto akskip;
if (subcur==5)
   goto zmskip;
if ((subcur==0)&&(skip[0]!=0))
   goto oskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*p*pth;
   if (N<pth)
     break;
   p=p*p*p*pth;
   if (p==N)
      return(1);
   if (N<p)
      break;
   }
if (subcur==1)
  return(0);
oskip:
if ((subcur==0)&&(skip[1]!=0))
   goto pskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
  return(0);
pskip:
if ((subcur==0)&&(skip[2]!=0))
   goto akskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==3)
  return(0);
akskip:
if ((subcur==0)&&(skip[3]!=0))
   goto zmskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=pth*q;
     if (N<qth)
	break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       if ((qth*r)==N)
	  return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==4)
   return(0);
zmskip:
if ((subcur==0)&&(skip[4]!=0))
   goto yaskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
yaskip:
if (skip[18]!=0)
   return(0);
//
// 13
//
fskip:
if (subcur==2)
  goto qskip;
if (subcur==3)
  goto rskip;
if (subcur==4)
  goto tskip;
if (subcur==5)
  goto uskip;
if (subcur==6)
  goto alskip;
if (subcur==7)
  goto zbskip;
if (subcur==8)
   goto znskip;
if ((subcur==0)&&(skip[0]!=0))
   goto qskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
  return(0);
qskip:
if ((subcur==0)&&(skip[1]!=0))
   goto rskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
  return(0);
rskip:
if ((subcur==0)&&(skip[2]!=0))
   goto tskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==3)
  return(0);
tskip:
if ((subcur==0)&&(skip[3]!=0))
   goto uskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=pth*q;
     if (N<qth)
       break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       if ((qth*r)==N)
	 return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==4)
  return(0);
uskip:
if ((subcur==0)&&(skip[4]!=0))
   goto alskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   for (j=i+1; j<tsize; j++) {
     q=table[j];
     qth=p*q;
     if (N<qth)
	break;
     for (k=j+1; k<tsize; k++) {
       r=table[k];
       rth=qth*r;
       if (N<rth)
	  break;
       for (l=k+1; l<tsize; l++) {
	 s=table[l];
	 if ((rth*s)==N)
	    return(1);
	 if (N<(rth*s))
	    break;
	 }
       }
     }
   }
if (subcur==5)
  return(0);
alskip:
if ((subcur==0)&&(skip[5]!=0))
   return(0);
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
     q=table[j];
     qth=q*q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==6)
   return(0);
zbskip:
if ((subcur==0)&&(skip[6]!=0))
   goto znskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==7)
   return(0);
znskip:
if ((subcur==0)&&(skip[7]!=0))
   goto ybskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
ybskip:
if (skip[18]!=0)
   return(0);
//
// 15
//
gskip:
if (subcur==8)
   goto yfskip;
if (subcur==7)
   goto zeskip;
if (subcur==6)
   goto abskip;
if (subcur==5)
   goto lskip;
if (subcur==4)
   goto mskip;
if (subcur==3)
   goto kskip;
if (subcur==2)
   goto amskip;
if ((subcur==0)&&(skip[0]!=0))
   goto amskip;
for (i=0; i<tsize; i++) {     // p^15
   p=table[i];
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
   return(0);
amskip:
if ((subcur==0)&&(skip[1]!=0))
   goto kskip;
for (i=0; i<tsize; i++) {     // p^8q
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==2)
  return(0);
kskip:
if ((subcur==0)&&(skip[2]!=0))
   goto mskip;
for (i=0; i<tsize; i++) {     // p^5q^2
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   p=p*p*pth;
   if (N<p)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     if ((p*qth)==N)
	return(1);
     if (N<(p*qth))
	break;
     }
   }
if (subcur==3)
  return(0);
mskip:
if ((subcur==0)&&(skip[3]!=0))
   goto lskip;
for (i=0; i<tsize; i++) {     // p^2q^2r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     qth=pth*qth;
     if (N<qth)
	break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       if ((qth*r)==N)
	  return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==4)
  return(0);
lskip:
if ((subcur==0)&&(skip[4]!=0))
   goto abskip;
for (i=0; i<tsize; i++) {     // p^4qr
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*pth;
     if (N<qth)
	break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       if ((qth*r)==N)
	  return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==5)
  return(0);
abskip:
if ((subcur==0)&&(skip[5]!=0))
   goto zeskip;
for (i=0; i<tsize; i++) {     // p^4q^3
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
       return(1);
     if (N<(pth*qth))
       break;
     }
   }
if (subcur==6)
   return(0);
zeskip:
if ((subcur==0)&&(skip[6]!=0))
   goto yfskip;
for (i=0; i<tsize; i++) {     // p^9q
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     if ((pth*q)==N)
	return(1);
     if (N<(pth*q))
	break;
     }
   }
if (subcur==7)
   return(0);
yfskip:
if ((subcur==0)&&(skip[7]!=0))
   goto ycskip;
for (i=0; i<tsize; i++) {     // p^14
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
ycskip:
if (skip[18]!=0)
   return(0);
//
// 17
//
acskip:
if (subcur==2)
   goto adskip;
if (subcur==3)
   goto sskip;
if (subcur==4)
   goto aaskip;
if (subcur==5)
   goto aeskip;
if (subcur==6)
   goto zcskip;
if (subcur==7)
   goto zdskip;
if (subcur==8)
   goto zgskip;
if (subcur==9)
   goto zhskip;
if (subcur==10)
   goto ygskip;
if ((subcur==0)&&(skip[0]!=0))
   goto adskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*pth;
   if (pth==N)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==1)
  return(0);
adskip:
if ((subcur==0)&&(skip[1]!=0))
   goto sskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==2)
  return(0);
sskip:
if ((subcur==0)&&(skip[2]!=0))
   goto aaskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q;
     if (N<qth)
	break;
     qth=pth*qth;
     if (N<qth)
       break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       if ((qth*r)==N)
	 return(1);
       if (N<(qth*r))
	  break;
	}
     }
   }
if (subcur==3)
  return(0);
aaskip:
if ((subcur==0)&&(skip[3]!=0))
   goto aeskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*pth;
     if (N<qth)
	break;
     for (k=0; k<tsize; k++) {
       if (i==k)
	  continue;
       if (j==k)
	  continue;
       r=table[k];
       rth=r*qth;
       if (N<rth)
	  break;
       for (l=0; l<tsize; l++) {
	 if (i==l)
	    continue;
	 if (j==l)
	    continue;
	 if (k==l)
	    continue;
	 s=table[l];
	 if ((rth*s)==N)
	    return(1);
	 if (N<(rth*s))
	    break;
	 }
       }
     }
   }
if (subcur==4)
  return(0);
aeskip:
if ((subcur==0)&&(skip[4]!=0))
   goto zcskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*pth;
   if (N<pth)
     break;
   for (j=0; j<tsize; j++) {
     if (i==j)
	continue;
     q=table[j];
     qth=q*q*q;
     if (N<qth)
	break;
     if ((pth*qth)==N)
	return(1);
     if (N<(pth*qth))
	break;
     }
   }
if (subcur==5)
   return(0);
zcskip:
if ((subcur==0)&&(skip[5]!=0))
   goto zdskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==6)
   return(0);
zdskip:
if ((subcur==0)&&(skip[6]!=0))
   goto zgskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==7)
   return(0);
zgskip:
if ((subcur==0)&&(skip[7]!=0))
   goto zhskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==8)
   return(0);
zhskip:
if ((subcur==0)&&(skip[8]!=0))
   goto ygskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==9)
   return(0);
ygskip:
if ((subcur==0)&&(skip[9]!=0))
   goto ydskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
ydskip:
if (skip[18]!=0)
   return(0);
//
//  p=19
//
baskip:
if (subcur==2)
   goto bbskip;
if (subcur==3)
   goto bcskip;
if (subcur==4)
   goto bdskip;
if (subcur==5)
   goto beskip;
if (subcur==6)
   goto bfskip;
if (subcur==7)
   goto bgskip;
if (subcur==8)
   goto bhskip;
if (subcur==9)
   goto biskip;
if (subcur==10)
   goto bjskip;
if (subcur==11)
   goto bkskip;
if (subcur==12)
   goto blskip;
if (subcur==13)
   goto zsskip;
if (subcur==14)
   goto buskip;
if (subcur==15)
   goto bvskip;
if (subcur==16)
   goto bwskip;
if (subcur==17)
   goto bxskip;
if (subcur==18)
   goto yhskip;
if ((subcur==0)&&(skip[0]!=0))
   goto bbskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   pth=p*p*p*pth;
   if (N<pth)
     break;
   p=p*p*p*p*pth;
   if (p==N)
      return(1);
   if (N<p)
      break;
   }
if (subcur==1)
   return(0);
bbskip:
if ((subcur==0)&&(skip[1]!=0))
   goto bcskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==2)
   return(0);
bcskip:
if ((subcur==0)&&(skip[2]!=0))
   goto bdskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==3)
   return(0);
bdskip:
if ((subcur==0)&&(skip[3]!=0))
   goto beskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==4)
   return(0);
beskip:
if ((subcur==0)&&(skip[4]!=0))
   goto bfskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==5)
   return(0);
bfskip:
if ((subcur==0)&&(skip[5]!=0))
   goto bgskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=j+1; k<tsize; k++) {
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(rth*qth))
	    return(1);
	 if (N<(rth*qth))
	    break;
	 }
      }
   }
if (subcur==6)
   return(0);
bgskip:
if ((subcur==0)&&(skip[6]!=0))
   goto bhskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==7)
   return(0);
bhskip:
if ((subcur==0)&&(skip[7]!=0))
   goto biskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==8)
   return(0);
biskip:
if ((subcur==0)&&(skip[8]!=0))
   goto bjskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N==sth)
	       return(1);
	    if (N<sth)
	       break;
	    }
	 }
      }
   }
if (subcur==9)
   return(0);
bjskip:
if ((subcur==0)&&(skip[9]!=0))
   goto bkskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N7<p)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=p*q;
      if (N<qth)
	break;
      for (k=j+1; k<tsize; k++) {
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	   break;
	 for (l=k+1; l<tsize; l++) {
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=l+1; m<tsize; m++) {
	       t=table[m];
	       tth=t*sth;
	       if (N==tth)
		  return(1);
	       if (N<tth)
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==10)
   return(0);
bkskip:
if ((subcur==0)&&(skip[10]!=0))
   goto blskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==11)
   return(0);
blskip:
if ((subcur==0)&&(skip[11]!=0))
   goto zsskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==12)
   return(0);
zsskip:
if ((subcur==0)&&(skip[12]!=0))
   goto buskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==13)
   return(0);
buskip:
if ((subcur==0)&&(skip[13]!=0))
   goto bvskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==14)
   return(0);
bvskip:
if ((subcur==0)&&(skip[14]!=0))
   goto bwskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==15)
   return(0);
bwskip:
if ((subcur==0)&&(skip[15]!=0))
   goto bxskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==16)
   return(0);
bxskip:
if ((subcur==0)&&(skip[16]!=0))
   goto yhskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==17)
   return(0);
yhskip:
if ((subcur==0)&&(skip[17]!=0))
   goto yeskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
yeskip:
if (skip[18]!=0)
   return(0);
//
// 21
//
bmskip:
if (subcur==2)
   goto bnskip;
if (subcur==3)
   goto boskip;
if (subcur==4)
   goto bpskip;
if (subcur==5)
   goto bqskip;
if (subcur==6)
   goto brskip;
if (subcur==7)
   goto bsskip;
if (subcur==8)
   goto btskip;
if (subcur==9)
   goto byskip;
if (subcur==10)
   goto bzskip;
if (subcur==11)
   goto buzskip;
if (subcur==12)
   goto yiskip;
if ((subcur==0)&&(skip[0]!=0))
   goto bnskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(pth*rth))
	    break;
	 }
      }
   }
if (subcur==1)
   return(0);
bnskip:
if ((subcur==0)&&(skip[1]!=0))
   goto boskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N==sth)
	       return(1);
	    if (N<sth)
	       break;
	    }
	 }
      }
   }
if (subcur==2)
   return(0);
boskip:
if ((subcur==0)&&(skip[2]!=0))
   goto bpskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==3)
   return(0);
bpskip:
if ((subcur==0)&&(skip[3]!=0))
   goto bqskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==4)
   return(0);
bqskip:
if ((subcur==0)&&(skip[4]!=0))
   goto brskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==5)
   return(0);
brskip:
if ((subcur==0)&&(skip[5]!=0))
   goto bsskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(q*pth))
	 return(1);
      if (N<(q*pth))
	 break;
      }
   }
if (subcur==6)
   return(0);
bsskip:
if ((subcur==0)&&(skip[6]!=0))
   goto btskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(rth*s))
	       return(1);
	    if (N<(rth*s))
	       break;
	    }
	 }
      }
   }
if (subcur==7)
   return(0);
btskip:
if ((subcur==0)&&(skip[7]!=0))
   goto byskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(q*pth))
	 return(1);
      if (N<(q*pth))
	 break;
      }
   }
if (subcur==8)
   return(0);
byskip:
if ((subcur==0)&&(skip[8]!=0))
   goto bzskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==9)
   return(0);
bzskip:
if ((subcur==0)&&(skip[9]!=0))
   goto buzskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==10)
   return(0);
buzskip:
if ((subcur==0)&&(skip[10]!=0))
   goto yiskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*pth;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
if (subcur==11)
   return(0);
yiskip:
if ((subcur==0)&&(skip[11]!=0))
   return(0);
for (i=0; i<tsize; i++) {
   p=table[i];
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N==pth)
      return(1);
   if (N<pth)
      break;
   }
return(0);
//
// 23
//
caskip:
if (subcur==22)
   goto duskip;
if (subcur==21)
   goto dtskip;
if (subcur==20)
   goto dsskip;
if (subcur==19)
   goto drskip;
if (subcur==18)
   goto dqskip;
if (subcur==17)
   goto dpskip;
if (subcur==16)
   goto doskip;
if (subcur==15)
   goto dnskip;
if (subcur==14)
   goto dmskip;
if (subcur==13)
   goto dlskip;
if (subcur==12)
   goto dkskip;
if (subcur==11)
   goto djskip;
if (subcur==10)
   goto diskip;
if (subcur==9)
   goto dhskip;
if (subcur==8)
   goto dgskip;
if (subcur==7)
   goto dfskip;
if (subcur==6)
   goto deskip;
if (subcur==5)
   goto ddskip;
if (subcur==4)
   goto dcskip;
if (subcur==3)
   goto dbskip;
if (subcur==2)
   goto daskip;
if ((subcur==0)&&(skip[0]!=0))
   goto daskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==1)
   return(0);
daskip:
if ((subcur==0)&&(skip[1]!=0))
   goto dbskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N7<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==2)
   return(0);
dbskip:
if ((subcur==0)&&(skip[2]!=0))
   goto dcskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==3)
   return(0);
dcskip:
if ((subcur==0)&&(skip[3]!=0))
   goto ddskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==4)
   return(0);
ddskip:
if ((subcur==0)&&(skip[4]!=0))
   goto deskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==5)
   return(0);
deskip:
if ((subcur==0)&&(skip[5]!=0))
   goto dfskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==6)
   return(0);
dfskip:
if ((subcur==0)&&(skip[6]!=0))
   goto dgskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==7)
   return(0);
dgskip:
if ((subcur==0)&&(skip[7]!=0))
   goto dhskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==8)
   return(0);
dhskip:
if ((subcur==0)&&(skip[8]!=0))
   goto diskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==9)
   return(0);
diskip:
if ((subcur==0)&&(skip[9]!=0))
   goto djskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==10)
   return(0);
djskip:
if ((subcur==0)&&(skip[10]!=0))
   goto dkskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==11)
   return(0);
dkskip:
if ((subcur==0)&&(skip[11]!=0))
   goto dlskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==12)
   return(0);
dlskip:
if ((subcur==0)&&(skip[12]!=0))
   goto dmskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==13)
   return(0);
dmskip:
if ((subcur==0)&&(skip[13]!=0))
   goto dnskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==14)
   return(0);
dnskip:
if ((subcur==0)&&(skip[14]!=0))
   goto doskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==15)
   return(0);
doskip:
if ((subcur==0)&&(skip[15]!=0))
   goto dpskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==16)
   return(0);
dpskip:
if ((subcur==0)&&(skip[16]!=0))
   goto dqskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==17)
   return(0);
dqskip:
if ((subcur==0)&&(skip[17]!=0))
   goto drskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==18)
   return(0);
drskip:
if ((subcur==0)&&(skip[18]!=0))
   goto dsskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==19)
   return(0);
dsskip:
if ((subcur==0)&&(skip[19]!=0))
   goto dtskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==20)
   return(0);
dtskip:
if ((subcur==0)&&(skip[20]!=0))
   goto duskip;
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==21)
   return(0);
duskip:
if ((subcur==0)&&(skip[21]!=0))
   return(0);
for (i=0; i<tsize; i++) {
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
return(0);
//
// 25
//
fzskip:
if (subcur==27)
   goto ezskip;
if (subcur==26)
   goto eyskip;
if (subcur==25)
   goto exskip;
if (subcur==24)
   goto ewskip;
if (subcur==23)
   goto evskip;
if (subcur==22)
   goto euskip;
if (subcur==21)
   goto etskip;
if (subcur==20)
   goto esskip;
if (subcur==19)
   goto erskip;
if (subcur==18)
   goto eqskip;
if (subcur==17)
   goto epskip;
if (subcur==16)
   goto eoskip;
if (subcur==15)
   goto enskip;
if (subcur==14)
   goto emskip;
if (subcur==13)
   goto elskip;
if (subcur==12)
   goto ekskip;
if (subcur==11)
   goto ejskip;
if (subcur==10)
   goto eiskip;
if (subcur==9)
   goto ehskip;
if (subcur==8)
   goto egskip;
if (subcur==7)
   goto efskip;
if (subcur==6)
   goto eeskip;
if (subcur==5)
   goto edskip;
if (subcur==4)
   goto ecskip;
if (subcur==3)
   goto ebskip;
if (subcur==2)
   goto easkip;
if ((subcur==0)&&(skip[0]!=0))
   goto easkip;
for (i=0; i<tsize; i++) {      // p^4q^2rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==1)
   return(0);
easkip:
if ((subcur==0)&&(skip[1]!=0))
   goto ebskip;
for (i=0; i<tsize; i++) {      // p^3qrst
   p=table[i];
   if (N7<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==2)
   return(0);
ebskip:
if ((subcur==0)&&(skip[2]!=0))
   goto ecskip;
for (i=0; i<tsize; i++) {      // p^4q^3r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==3)
   return(0);
ecskip:
if ((subcur==0)&&(skip[3]!=0))
   goto edskip;
for (i=0; i<tsize; i++) {      // p^6q^3r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==4)
   return(0);
edskip:
if ((subcur==0)&&(skip[4]!=0))
   goto eeskip;
for (i=0; i<tsize; i++) {      // p^5q^2r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==5)
   return(0);
eeskip:
if ((subcur==0)&&(skip[5]!=0))
   goto efskip;
for (i=0; i<tsize; i++) {      // p^12qr
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==6)
   return(0);
efskip:
if ((subcur==0)&&(skip[6]!=0))
   goto egskip;
for (i=0; i<tsize; i++) {      // p^13q^2
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==7)
   return(0);
egskip:
if ((subcur==0)&&(skip[7]!=0))
   goto ehskip;
for (i=0; i<tsize; i++) {      // p^10q^3
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==8)
   return(0);
ehskip:
if ((subcur==0)&&(skip[8]!=0))
   goto eiskip;
for (i=0; i<tsize; i++) {      // p^9q^4
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==9)
   return(0);
eiskip:
if ((subcur==0)&&(skip[9]!=0))
   goto ejskip;
for (i=0; i<tsize; i++) {      // p^8q^5
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==10)
   return(0);
ejskip:
if ((subcur==0)&&(skip[10]!=0))
   goto ekskip;
for (i=0; i<tsize; i++) {      // p^8q^2r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==11)
   return(0);
ekskip:
if ((subcur==0)&&(skip[11]!=0))
   goto elskip;
for (i=0; i<tsize; i++) {      // p^6qrs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=k+1; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==12)
   return(0);
elskip:
if ((subcur==0)&&(skip[12]!=0))
   goto emskip;
for (i=0; i<tsize; i++) {      // pqrstu
   p=table[i];
   if (N11<p)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*p;
      if (N<qth)
	 break;
      for (k=j+1; k<tsize; k++) {
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=k+1; l<tsize; l++) {
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=l+1; m<tsize; m++) {
	       t=table[m];
	       tth=t*sth;
	       if (N<tth)
		  break;
	       for (n=m+1; n<tsize; n++) {
		  u=table[n];
		  if (N==(u*tth))
		     return(1);
		  if (N<(u*tth))
		     break;
		  }
	       }
	    }
	 }
      }
   }
if (subcur==13)
   return(0);
emskip:
if ((subcur==0)&&(skip[13]!=0))
   goto enskip;
for (i=0; i<tsize; i++) {      // p^6q^6
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==14)
   return(0);
enskip:
if ((subcur==0)&&(skip[14]!=0))
   goto eoskip;
for (i=0; i<tsize; i++) {      // p^3q^3r^3
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=j+1; k<tsize; k++) {
	 r=table[k];
	 rth=r*r*r;
	 if (N<rth)
	    break;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==15)
   return(0);
eoskip:
if ((subcur==0)&&(skip[15]!=0))
   goto epskip;
for (i=0; i<tsize; i++) {      // p^2q^2r^2s
   p=table[i];
   if (N5<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==16)
   return(0);
epskip:
if ((subcur==0)&&(skip[16]!=0))
   goto eqskip;
for (i=0; i<tsize; i++) {      // p^19q
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N<q)
	 break;
      if (N==(q*pth))
	 return(1);
      if (N<(q*pth))
	 break;
      }
   }
if (subcur==17)
   return(0);
eqskip:
if ((subcur==0)&&(skip[17]!=0))
   goto erskip;
for (i=0; i<tsize; i++) {      // p^7q^3r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==18)
   return(0);
erskip:
if ((subcur==0)&&(skip[18]!=0))
   goto esskip;
for (i=0; i<tsize; i++) {      // p^6q^2r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(qth*rth))
	    return(1);
	 if (N<(qth*rth))
	    break;
	 }
      }
   }
if (subcur==19)
   return(0);
esskip:
if ((subcur==0)&&(skip[19]!=0))
   goto etskip;
for (i=0; i<tsize; i++) {      // p^9q^2r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=pth*qth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
if (subcur==20)
   return(0);
etskip:
if ((subcur==0)&&(skip[20]!=0))
   goto euskip;
for (i=0; i<tsize; i++) {      // p^3q^3rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==21)
   return(0);
euskip:
if ((subcur==0)&&(skip[21]!=0))
   goto evskip;
for (i=0; i<tsize; i++) {      // p^7qrs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==22)
   return(0);
evskip:
if ((subcur==0)&&(skip[22]!=0))
   goto ewskip;
for (i=0; i<tsize; i++) {      // p^7q^6
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*q*qth;
      if (N<qth)
	 break;
      if (N==(qth*pth))
	 return(1);
      if (N<(qth*pth))
	 break;
      }
   }
if (subcur==23)
   return(0);
ewskip:
if ((subcur==0)&&(skip[23]!=0))
   goto exskip;
for (i=0; i<tsize; i++) {      // p^10q^4
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(qth*pth))
	 return(1);
      if (N<(qth*pth))
	 break;
      }
   }
if (subcur==24)
   return(0);
exskip:
if ((subcur==0)&&(skip[24]!=0))
   goto eyskip;
for (i=0; i<tsize; i++) {      // p^11q^3
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(qth*pth))
	 return(1);
      if (N<(qth*pth))
	 break;
      }
   }
if (subcur==25)
   return(0);
eyskip:
if ((subcur==0)&&(skip[25]!=0))
   goto ezskip;
for (i=0; i<tsize; i++) {      // p^14q^2
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(qth*pth))
	 return(1);
      if (N<(qth*pth))
	 break;
      }
   }
if (subcur==26)
   return(0);
ezskip:
if ((subcur==0)&&(skip[26]!=0))
   return(0);
for (i=0; i<tsize; i++) {      // p^13qr
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(qth*r))
	    return(1);
	 if (N<(qth*r))
	    break;
	 }
      }
   }
return(0);
//
// 27
//
fjskip:
if (subcur==35)
   goto gsskip;
if (subcur==34)
   goto grskip;
if (subcur==33)
   goto gqskip;
if (subcur==32)
   goto gpskip;
if (subcur==31)
   goto goskip;
if (subcur==30)
   goto gnskip;
if (subcur==29)
   goto gmskip;
if (subcur==28)
   goto glskip;
if (subcur==27)
   goto gkskip;
if (subcur==26)
   goto gjskip;
if (subcur==25)
   goto giskip;
if (subcur==24)
   goto ghskip;
if (subcur==23)
   goto ggskip;
if (subcur==22)
   goto gfskip;
if (subcur==21)
   goto geskip;
if (subcur==20)
   goto gdskip;
if (subcur==19)
   goto gcskip;
if (subcur==18)
   goto gbskip;
if (subcur==17)
   goto gaskip;
if (subcur==16)
   goto fhskip;
if (subcur==15)
   goto fgskip;
if (subcur==14)
   goto ffskip;
if (subcur==13)
   goto feskip;
if (subcur==12)
   goto fdskip;
if (subcur==11)
   goto fcskip;
if (subcur==10)
   goto fbskip;
if (subcur==9)
   goto faskip;
if (subcur==8)
   goto fiskip;
if (subcur==7)
   goto fyskip;
if (subcur==6)
   goto fxskip;
if (subcur==5)
   goto fwskip;
if (subcur==4)
   goto fvskip;
if (subcur==3)
   goto fuskip;
if (subcur==2)
   goto ftskip;
if ((subcur==0)&&(skip[0]!=0))
   goto ftskip;
for (i=0; i<tsize; i++) {      // p^5q^2rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==1)
   return(0);
ftskip:
if ((subcur==0)&&(skip[1]!=0))
   goto fuskip;
for (i=0; i<tsize; i++) {      // p^3q^2r^2s
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==2)
   return(0);
fuskip:
if ((subcur==0)&&(skip[2]!=0))
   goto fvskip;
for (i=0; i<tsize; i++) {   // p^8qrs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==3)
   return(0);
fvskip:
if ((subcur==0)&&(skip[3]!=0))
   goto fwskip;
for (i=0; i<tsize; i++) {    // p^2q^2rst
   p=table[i];
   if (N7<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==4)
   return(0);
fwskip:
if ((subcur==0)&&(skip[4]!=0))
   goto fxskip;
for (i=0; i<tsize; i++) {    // p^10q^2r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==5)
   return(0);
fxskip:
if ((subcur==0)&&(skip[5]!=0))
   goto fyskip;
for (i=0; i<tsize; i++) {    // p^8q^3r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==6)
   return(0);
fyskip:
if ((subcur==0)&&(skip[6]!=0))
   goto fiskip;
for (i=0; i<tsize; i++) {     // p^5q^3r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(rth*qth))
	    return(1);
	 if (N<(rth*qth))
	    break;
	 }
      }
   }
if (subcur==7)
   return(0);
fiskip:
if ((subcur==0)&&(skip[7]!=0))
   goto faskip;
for (i=0; i<tsize; i++) {     // p^7q^2r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(rth*qth))
	    return(1);
	 if (N<(rth*qth))
	    break;
	 }
      }
   }
if (subcur==8)
   return(0);
faskip:
if ((subcur==0)&&(skip[8]!=0))
   goto fbskip;
for (i=0; i<tsize; i++) {     // p^14qr
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==9)
   return(0);
fbskip:
if ((subcur==0)&&(skip[9]!=0))
   goto fcskip;
for (i=0; i<tsize; i++) {     // p^12q^3
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==10)
   return(0);
fcskip:
if ((subcur==0)&&(skip[10]!=0))
   goto fdskip;
for (i=0; i<tsize; i++) {     // p^11q^4
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==11)
   return(0);
fdskip:
if ((subcur==0)&&(skip[11]!=0))
   goto feskip;
for (i=0; i<tsize; i++) {     // p^9q^5
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==12)
   return(0);
feskip:
if ((subcur==0)&&(skip[12]!=0))
   goto ffskip;
for (i=0; i<tsize; i++) {     // p^8q^6
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=q*q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==13)
   return(0);
ffskip:
if ((subcur==0)&&(skip[13]!=0))
   goto fgskip;
for (i=0; i<tsize; i++) {     // p^15q^2
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==14)
   return(0);
fgskip:
if ((subcur==0)&&(skip[14]!=0))
   goto fhskip;
for (i=0; i<tsize; i++) {      // p^19q
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N==(pth*q))
	 return(1);
      if (N<(pth*q))
	 break;
      }
   }
if (subcur==15)
   return(0);
fhskip:
if ((subcur==0)&&(skip[15]!=0))
   goto gaskip;
for (i=0; i<tsize; i++) {      // p^7q^7
   p=table[i];
   if (N2<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      qth=q*q*q*qth;
      if (N<qth)
	 break;
      if (N==(pth*qth))
	 return(1);
      if (N<(pth*qth))
	 break;
      }
   }
if (subcur==16)
   return(0);
gaskip:
if ((subcur==0)&&(skip[16]!=0))
   goto gbskip;
for (i=0; i<tsize; i++) {	// p^9qrs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==17)
   return(0);
gbskip:
if ((subcur==0)&&(skip[17]!=0))
   goto gcskip;
for (i=0; i<tsize; i++) {	// p^3q^2rst
   p=table[i];
   if (N7<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==18)
   return(0);
gcskip:
if ((subcur==0)&&(skip[18]!=0))
   goto gdskip;
for (i=0; i<tsize; i++) {      // p^11q^2r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==19)
   return(0);
gdskip:
if ((subcur==0)&&(skip[19]!=0))
   goto geskip;
for (i=0; i<tsize; i++) {	  // p^9q^3r
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==20)
   return(0);
geskip:
if ((subcur==0)&&(skip[20]!=0))
   goto gfskip;
for (i=0; i<tsize; i++) {      // p^8q^2r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(rth*qth))
	    return(1);
	 if (N<(rth*qth))
	    break;
	 }
      }
   }
if (subcur==21)
   return(0);
gfskip:
if ((subcur==0)&&(skip[21]!=0))
   goto ggskip;
for (i=0; i<tsize; i++) {      // p^7q^3r^2
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 if (N==(rth*qth))
	    return(1);
	 if (N<(rth*qth))
	    break;
	 }
      }
   }
if (subcur==22)
   return(0);
ggskip:
if ((subcur==0)&&(skip[22]!=0))
   goto ghskip;
for (i=0; i<tsize; i++) {	// p^6q^2rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==23)
   return(0);
ghskip:
if ((subcur==0)&&(skip[23]!=0))
   goto giskip;
for (i=0; i<tsize; i++) {	// p^4q^2r^2s
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==24)
   return(0);
giskip:
if ((subcur==0)&&(skip[24]!=0))
   goto gjskip;
for (i=0; i<tsize; i++) {	// p^2q^2r^2s^2
   p=table[i];
   if (N5<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=i+1; j<tsize; j++) {
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=j+1; k<tsize; k++) {
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=k+1; l<tsize; l++) {
	    s=table[l];
	    sth=s*s;
	    if (N<sth)
	       break;
	    if (N==(sth*rth))
	       return(1);
	    if (N<(sth*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==25)
   return(0);
gjskip:
if ((subcur==0)&&(skip[25]!=0))
   goto gkskip;
for (i=0; i<tsize; i++) {      // p^14qr
   p=table[i];
   if (N3<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      if (N<q)
	 break;
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 if (N==(r*qth))
	    return(1);
	 if (N<(r*qth))
	    break;
	 }
      }
   }
if (subcur==26)
   return(0);
gkskip:
if ((subcur==0)&&(skip[26]!=0))
   goto glskip;
for (i=0; i<tsize; i++) {   // p^4qrst
   p=table[i];
   if (N7<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==27)
   return(0);
glskip:
if ((subcur==0)&&(skip[27]!=0))
   goto gmskip;
for (i=0; i<tsize; i++) {   // p^2q^2r^2st
   p=table[i];
   if (N7<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       if (N==(t*sth))
		  return(1);
	       if (N<(t*sth))
		  break;
	       }
	    }
	 }
      }
   }
if (subcur==28)
   return(0);
gmskip:
if ((subcur==0)&&(skip[28]!=0))
   goto gnskip;
for (i=0; i<tsize; i++) {    // p^2qrstu
   p=table[i];
   if (N11<p)
      break;
   pth=p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    sth=s*rth;
	    if (N<sth)
	       break;
	    for (m=0; m<tsize; m++) {
	       if (i==m)
		  continue;
	       if (j==m)
		  continue;
	       if (k==m)
		  continue;
	       if (l==m)
		  continue;
	       t=table[m];
	       tth=t*sth;
	       if (N<tth)
		  break;
	       for (n=0; n<tsize; n++) {
		  if (i==n)
		     continue;
		  if (j==n)
		     continue;
		  if (k==n)
		     continue;
		  if (l==n)
		     continue;
		  if (m==n)
		     continue;
		  u=table[n];
		  if (N==(u*tth))
		     return(1);
		  if (N<(u*tth))
		     break;
		  }
	       }
	    }
	 }
      }
   }
if (subcur==29)
   return(0);
gnskip:
if ((subcur==0)&&(skip[29]!=0))
   goto goskip;
for (i=0; i<tsize; i++) {	// p^5q^2r^2s
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==30)
   return(0);
goskip:
if ((subcur==0)&&(skip[30]!=0))
   goto gpskip;
for (i=0; i<tsize; i++) {	// p^5q^3rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p;
   if (N<pth)
      break;
   pth=p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==31)
   return(0);
gpskip:
if ((subcur==0)&&(skip[31]!=0))
   goto gqskip;
for (i=0; i<tsize; i++) {	// p^4q^3rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==32)
   return(0);
gqskip:
if ((subcur==0)&&(skip[32]!=0))
   goto grskip;
for (i=0; i<tsize; i++) {	// p^4q^4rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q*q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==33)
   return(0);
grskip:
if ((subcur==0)&&(skip[33]!=0))
   goto gsskip;
for (i=0; i<tsize; i++) {	// p^4q^2r^2s
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*r;
	 if (N<rth)
	    break;
	 rth=rth*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
if (subcur==34)
   return(0);
gsskip:
if ((subcur==0)&&(skip[34]!=0))
   return(0);
for (i=0; i<tsize; i++) {	// p^7q^2rs
   p=table[i];
   if (N5<p)
      break;
   pth=p*p*p*p;
   if (N<pth)
      break;
   pth=p*p*p*pth;
   if (N<pth)
      break;
   for (j=0; j<tsize; j++) {
      if (i==j)
	 continue;
      q=table[j];
      qth=q*q;
      if (N<qth)
	 break;
      qth=qth*pth;
      if (N<qth)
	 break;
      for (k=0; k<tsize; k++) {
	 if (i==k)
	    continue;
	 if (j==k)
	    continue;
	 r=table[k];
	 rth=r*qth;
	 if (N<rth)
	    break;
	 for (l=0; l<tsize; l++) {
	    if (i==l)
	       continue;
	    if (j==l)
	       continue;
	    if (k==l)
	       continue;
	    s=table[l];
	    if (N==(s*rth))
	       return(1);
	    if (N<(s*rth))
	       break;
	    }
	 }
      }
   }
return(0);
}
