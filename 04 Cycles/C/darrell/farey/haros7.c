/*CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                             C
C  GENERATE FAREY SERIES                                                      C
C  06/11/07 (DKC)							      C
C                                                                             C
C  The Farey series Fn of order n is the ascending series of irreducible      C
C  fractions between 0 and 1 whose denominators do not exceed n.  The         C
C  fractions in the series are generated using the theorem that if h/k,       C
C  h'/k', and h''/k'' are three successive fractions in a Farey series, then  C
C  h'/k' = (h + h'')/(k + k'').  The fraction after two successive fractions  C
C  h/k and h'/k' in the series is then (j*h' - h)/(j*k' - k) where j is some  C
C  positive integer.  Using the theorem that the sum of the denominators of   C
C  successive fractions in a Farey series is greater than the order of the    C
C  series gives j = [(n + k)/k'].                                             C
C                                                                             C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC*/
unsigned int haros7(unsigned int N, unsigned int M, unsigned int H,
		    unsigned int K, unsigned int HP, unsigned int KP,
		    unsigned int D, unsigned int *ND) {
      unsigned int HPP,KPP,J,OLDM;
      OLDM=M;
      M=M+1;
//
// FIND FRACTIONS IN FAREY SERIES FOLLOWING H/K, HP/KP
//
L100: J=(N+K)/KP;
      HPP=J*HP-H;
      KPP=J*KP-K;
      H=HP;
      K=KP;
      HP=HPP;
      KP=KPP;
      M=M+1;
      if(KP!=D) goto L100;
//
      J=(N+K)/KP;
      HPP=J*HP-H;
      KPP=J*KP-K;
      ND[0]=HPP;
      ND[1]=KPP;
      return(M-OLDM+1);
      }

