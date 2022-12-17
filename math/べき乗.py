import numpy as np

print("2x2行列を入力してください")
print("例: 1 2 3 4")
a,b,c,d = map(int, input().split())

a = np.array([[a, b], [c, d]])
x = np.array([1., 0]) 

print("ループ回数 | 固有ベクトル | 固有値")              

for i in range(16):
    x1 = a @ x             
    k = np.linalg.norm(x1,np.inf) 
    print(i, x, k)      
    x = x1 / k     