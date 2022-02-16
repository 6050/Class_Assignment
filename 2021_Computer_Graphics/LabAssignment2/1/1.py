import numpy as np

M = np.arange(2,27,1)
print(M)
print()

M = M.reshape(5,5)
print(M)
print()

M[1:4, 1:4] = 0
print(M)
print()

M = M @ M
print(M)
print()

print(np.sqrt(np.sum(M[0] * M[0])))
