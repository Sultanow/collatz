#!/usr/bin/env python3
"""Exact finite return-occupation and second-moment identities.

Use only TERMINATED finite first-passage trajectories; never infer asymptotic
Collatz statements from these computations. All reported identities use integers.
Input state space is the narrow window (floor(X^(199/200)), X].
"""
from __future__ import annotations
import argparse, collections, json
from fractions import Fraction
from finite_return_graph import first_return, nthroot_floor
from induced_entrance_probe import next_step


def analyze(X:int,c:int,max_steps:int=20000,outside_stride:int=31, shell_kind:str="narrow"):
    if X < 1000 or (c%2==0) or c< -1: raise ValueError('Require X>=1000, odd c>=-1')
    if shell_kind == "narrow":
        lower = nthroot_floor(X**199,200)
        passage = nthroot_floor(X**99,100)
    elif shell_kind == "dyadic":
        lower = X // 2
        passage = X // 4
    else:
        raise ValueError("shell_kind must be narrow or dyadic")
    I = range(lower+1,X+1)
    f={}; status=collections.Counter(); max_steps_used=0
    for x in I:
        label, dest, nsteps = first_return(x,c,lower,passage,X,max_steps)
        status[label]+=1
        max_steps_used=max(max_steps_used,nsteps)
        f[x]=dest if label=='return' else None
    if status['unresolved'] or status['cycle_outside']:
        return dict(status='INCOMPLETE',X=X,c=c,event_counts=dict(status))
    # Independently test the finite functional graph has no return cycle.
    color={}; topo=[]
    def walk(z):
        if color.get(z)==1: raise RuntimeError('Return cycle: moment undefined')
        if color.get(z)==2: return
        color[z]=1
        if f[z] is not None: walk(f[z])
        color[z]=2
        topo.append(z)
    try:
        for y in I: walk(y)
    except RuntimeError as e:
        return dict(status='RETURN_CYCLE',X=X,c=c,message=str(e))
    depth={}
    for y in topo:
        depth[y]=1+(depth[f[y]] if f[y] is not None else 0)
    # Independent raw-trajectory spot check: counts must match the induced graph.
    raw_checked=0
    for start in range(lower+1,X+1,max(1,len(f)//256)):
        z=start; count=0; seen=set()
        for t in range(max_steps*(max(depth.values())+1)):
            if z<=passage: break
            if z in seen: raise AssertionError(('raw_path_cycle',start,z))
            seen.add(z)
            if lower<z<=X: count+=1
            z=next_step(z,c)
        else:
            raise AssertionError(('raw_path_unresolved',start))
        assert count==depth[start],('independent_raw_path',start,count,depth[start])
        raw_checked+=1
    # Arbitrary finite starting law represented by nonnegative integer counts.
    # First scenario: a single copy of each shell point.
    scenarios={'uniform_shell': collections.Counter({x:1 for x in I})}
    # Second scenario: start outside the shell, then extract the actual first-entry law.
    first_entry=collections.Counter(); excluded=0; reached_cutoff=0; no_visit=0
    for start in range(X+1, 2*X+1, outside_stride):
        z=start; seen=set(); hit=None
        for _ in range(max_steps):
            if z<=passage:
                reached_cutoff+=1
                break
            if lower<z<=X:
                hit=z
                break
            if z in seen:
                excluded+=1
                break
            seen.add(z)
            z=next_step(z,c)
        else:
            excluded+=1
        if hit is not None: first_entry[hit]+=1
        elif z<=passage: no_visit+=1
    scenarios['actual_first_entries_from_outside']=first_entry

    report={'status':'EXACT_FINITE_IDENTITY_ONLY','X':X,'c':c,'lower_exclusive':lower,
            'passage_cutoff':passage,'shell_states':len(f),'shell_kind':shell_kind,
            'first_return_status':dict(status), 'max_steps_to_event':max_steps_used,
            'maximum_return_chain_length':max(depth.values()),
            'independent_raw_trajectory_spotchecks_passed':raw_checked,
            'excluded_unresolved_outside_starts':excluded,
            'outside_starts_that_passed_without_shell_visit':no_visit,
            'scenarios':{}}
    for label, first in scenarios.items():
        occ=collections.Counter(); ordered=collections.Counter(); exact_sq=0
        exact_N=0; num_initial=sum(first.values()); npaths=0
        for y,count in first.items():
            if count<=0: continue
            npaths+=1
            z=y; k=1; seen=set()
            while z is not None:
                assert z not in seen
                seen.add(z)
                occ[z]+=count
                ordered[z]+=k*count
                z=f[z]
                k+=1
            n=k-1
            assert n==depth[y]
            exact_N+=count*n
            exact_sq+=count*n*n
        # The following identities are independent checks of the green operator.
        Qocc=collections.Counter(); Qordered=collections.Counter()
        for y,mass in occ.items():
            if f[y] is not None: Qocc[f[y]]+=mass
        for y,mass in ordered.items():
            if f[y] is not None: Qordered[f[y]]+=mass
        for y in I:
            assert occ[y]-Qocc[y]==first[y], ('GQ',y)
            assert ordered[y]-Qordered[y]==occ[y], ('G2Q',y)
        occ_total=sum(occ.values()); ordered_total=sum(ordered.values())
        assert occ_total==exact_N
        assert 2*ordered_total-occ_total==exact_sq
        # The promised deterministic predecessor-potential identity.
        potential_sq=sum(m*(1+2*(depth[y]-1)) for y,m in occ.items())
        assert potential_sq==exact_sq
        # The original occupation potential cannot be a strict one-step
        # supermeasure whenever an occupied state has no first-entry mass.
        later_only=sum(1 for y,m in occ.items() if m>0 and first[y]==0)
        depth_hist=collections.Counter()
        for y,count in first.items():
            depth_hist[depth[y]]+=count
        discounted={}
        for label_rho,rho in [('9/10',Fraction(9,10)),('19/20',Fraction(19,20))]:
            base=1/rho
            tot=sum(count*sum(base**j for j in range(d)) for d,count in depth_hist.items())
            discounted[label_rho]=round(float(tot / num_initial),9) if num_initial else None
        report['scenarios'][label]={
            'occupied_without_first_entry_states':later_only,
            'original_occupation_mass_fails_strict_sub_eigenmeasure_test':later_only>0,
            'discounted_occupation_mass_per_start':discounted,
            'number_of_first_entering_starts':num_initial,
            'distinct_first_entry_states':len(first),
            'distinct_occupation_states':len(occ),
            'unweighted_total_visits':exact_N,
            'unweighted_total_squared_visits':exact_sq,
            'green_mass_nu':occ_total,
            'second_green_mass_sigma':ordered_total,
            'identity_moment_equals_2sigma_minus_nu':2*ordered_total-occ_total==exact_sq,
            'identity_moment_equals_weighted_potential':potential_sq==exact_sq,
            'identity_nu_minus_Qnu_equals_q1':True,
            'identity_sigma_minus_Qsigma_equals_nu':True,
            'mean_visits_conditioned_on_first_entry':round(exact_N/num_initial,9) if num_initial else None,
            'second_moment_conditioned_on_first_entry':round(exact_sq/num_initial,9) if num_initial else None,
            'max_return_chain_on_scenario_support':max((depth[y] for y in first),default=0)
        }
    return report


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--X',type=int,default=100000)
    p.add_argument('--c',type=int,default=1)
    p.add_argument('--max-steps',type=int,default=20000)
    p.add_argument('--shell-kind',choices=['narrow','dyadic'],default='narrow')
    p.add_argument('--outside-stride',type=int,default=31)
    p.add_argument('--output',default='')
    args=p.parse_args()
    data=analyze(args.X,args.c,args.max_steps,args.outside_stride,args.shell_kind)
    s=json.dumps(data,indent=2,sort_keys=True)
    if args.output:
        from pathlib import Path
        Path(args.output).write_text(s+'\n',encoding='utf-8')
    else: print(s)

if __name__=='__main__': main()
