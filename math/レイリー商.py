import numpy as np

print("2x2行列を入力してください")
print("例: 1 2 3 4")
a,b,c,d = map(int, input().split())

A = np.array([[a, b], [c, d]])

u=np.array([1,0])

rel_eps = 0.0001 
rel_delta_u=100.0

while rel_delta_u >= rel_eps  :          

    uu = u/np.linalg.norm(u)    
    print("固有ベクトル=",uu)                   

    u = A@uu.T                   

    eigen_value=np.dot(uu,u)/(np.dot(uu,uu.T))

    print("最大固有値=",eigen_value)        

    delta_u_vec = uu-u/np.linalg.norm(u)   
    abs_delta_u_value= np.linalg.norm(delta_u_vec)
    rel_delta_u=abs_delta_u_value/np.abs(eigen_value) 

#    print("rel_delta_u_vec = ",rel_delta_u)

