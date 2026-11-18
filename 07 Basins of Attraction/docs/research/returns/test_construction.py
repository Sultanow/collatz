import math, json
from pathlib import Path

beta=1/200
zeta=math.log(2)/math.log(3)
zeta0=.55

def build(t):
    s=math.ceil((math.log(t)+3)**2)
    r=math.ceil(t/45)
    modulus=1<<(r+1)
    g=pow(3,t+s,modulus)
    v0=((1+(1<<r))*pow(g,-1,modulus))%modulus
    assert v0%2
    v=next(v0+k*modulus for k in (0,1) if (pow(2,t+s,3)*(v0+k*modulus)-1)%3)
    n=(1<<(t+s))*v-1
    a=(1<<s)*v*pow(3,t)-1
    m=(v*pow(3,t+s)-1)//(1<<r)
    assert (v*pow(3,t+s)-1)%(1<<r)==0
    assert m%2==1
    assert n%3!=0
    assert ((3*a+1)//2) == ((1<<(s-1))*v*pow(3,t+1)-1) # first odd
    assert m == (v*pow(3,t+s)-1)//(1<<r)
    la=math.log(a)
    tx=math.ceil(3*beta*la/((zeta-zeta0)*math.log(3)))
    b=math.ceil(math.log(la)**2)
    assert s>=b and s<=tx
    lm=math.log(m)
    assert lm <(1-2*beta)*la, (t,la,lm,s,r, (la-lm)/la)
    la_div_n=la-math.log(n)
    return {'t':t,'s':s,'r':r,'t_X':tx,'b_X':b,'log_X':la,'log_m_over_log_X':lm/la,'log_a_over_n':la_div_n,'log_g_lower_over_log_X':la_div_n/la,'digits_X':int(la/math.log(10))+1,'m':int(m) if t<=100 else None, 'v':v if t<=100 else None}

if __name__=='__main__':
    out=[]
    for t in [25000,30000,40000,50000,75000,100000,125000]:
        d=build(t)
        out.append(d)
        print({k:(round(v,6) if isinstance(v,float) else v) for k,v in d.items() if k not in ('m','v')},flush=True)
    Path('test_results.json').write_text(json.dumps(out,indent=2))

def trajectory_check(t):
    s=math.ceil((math.log(t)+3)**2); r=math.ceil(t/45)
    modulus=1<<(r+1)
    v0=((1+(1<<r))*pow(pow(3,t+s,modulus),-1,modulus))%modulus
    v=next(v0+k*modulus for k in (0,1) if (pow(2,t+s,3)*(v0+k*modulus)-1)%3)
    a=(1<<s)*v*pow(3,t)-1
    X1_log=(1-2*beta)*math.log(a)
    x=a
    visits=0
    parity=[]
    for idx in range(s+r+2):
        if math.log(x)<=X1_log: break
        if 2*x>a: visits+= (x<=a)
        parity.append(x&1)
        x=(3*x+1)//2 if x&1 else x//2
    assert s < idx <= s+r, (idx,s,r)
    assert visits==2,visits
    assert parity[:s]==[1]*s
    assert parity[s:]==[0]*(idx-s)
    assert math.log(x)<=X1_log
    return {'t':t,'visits':visits,'odd_streak':s,'even_descent':r,'steps_until_cutoff':idx}

if __name__=='__main__':
    print('Orbit checks:', [trajectory_check(t) for t in (25000,30000,50000)])
