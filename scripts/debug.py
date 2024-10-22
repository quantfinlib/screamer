import numpy as np
from screamer import SMA

sma = SMA(3)

x = np.arange(20).reshape(-1,2)
print(x)

output1 = sma.transform(x)
print(output1)

# ---------------------------
sma = SMA(3)

output2 = np.zeros_like(x, dtype=float)
output2[:,0] =  sma.transform(x[:,0])
output2[:,1] =  sma.transform(x[:,1])
print(output2)

output1 = output1.flatten()
output2 = output2.flatten()
print(output1)
print(output2)


assert np.all([np.isnan(o1) if np.isnan(o2) else o1 == o2 for o1, o2 in zip(output1, output2)])
