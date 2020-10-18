#%%
import numpy as np

#%%
def calc_v_i_plus_1(v1, k, i, alpha_array):
    sum_alpha = np.sum(alpha_array)
    sum=0
    for j in range(1,i+1):
        alpha_j=sum_alpha-np.sum(alpha_array[i-j:])
        #print("alpha_j " + str(alpha_array[i-j:]))
        sum=sum+k**(j-1)*2**alpha_j
    
    nominator=k**i*v1+sum
    sum_alpha_i=np.sum(alpha_array[:i])
    denominator=2**sum_alpha_i
    return nominator/denominator

#%%
k=3
n=3
v1=17
alpha_array=[2,3,4]

sum=1/v1
for i in range(1,n+1):
    v_i_plus_1_=calc_v_i_plus_1(v1, k, i, alpha_array)
    print("v"+str(i+1)+"="+str(v_i_plus_1_))
    sum=sum+1/v_i_plus_1_

print(sum)
