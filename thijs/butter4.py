import numpy as np
from scipy import signal

import scipy
print(scipy.__file__)

"""
Desired Cutoff Frequency: fc = 0.2 (normalized between 0 and 1 in SciPy).

C++ Cutoff Frequency: cutoff_freq = fc / 2 = 0.1 (normalized between 0 and 0.5).

"""
t = np.arange(1000) * 0.02
x = np.sin(t * t)


cutoff_freq = 0.1

print('=============== python =============== cutoff_freq =',cutoff_freq)
for N in [7,8]:
    print('N =',N)
    b, a = filter = signal.butter(N, cutoff_freq, 'low', analog=False)
    print('b', b)
    print('a', a)

"""
N = 2

print('=============== python ===============')
for cutoff_freq in [0.1]:
    print(f'N = {N}, cutroff_freq = {cutoff_freq:.4f}')

    if 1==1:
        print('buttap')
        z, p, k = signal.buttap(N)
        print('z', z)
        print('p', p)
        print('k', k)


        Wn = cutoff_freq
        fs = 2.0
        warped = 2 * fs * np.tan(np.pi * Wn / fs)

        #warped = cutoff_freq
        #fs = 2.0
        print('transform to lowpass. warped =', warped)
        z, p, k = signal.lp2lp_zpk(z, p, k, wo=warped)
        print('z', z)
        print('p', p)
        print('k', k)

        print('discrete equivalent')
        z, p, k = signal.bilinear_zpk(z, p, k, fs=fs)
        print('z', z)
        print('p', p)
        print('k', k)

        print('digital')
        z, p, k = signal.butter(N, cutoff_freq, output='zpk', fs=fs, analog=False)
        print('z', z)
        print('p', p)
        print('k', k)

        print('converted to b,a')
        print('poly poles', np.poly(p))
        b, a = signal.zpk2tf(z, p, k)
        print('b', b)
        print('a', a)
        print()

    print('BUTTEN DIRECT CALL')
    b, a = filter = signal.butter(N, cutoff_freq, 'low', analog=False)
    print('b', b)
    print('a', a)
    print()


y = signal.lfilter(b, a, x, axis=-1, zi=None)

for i in range(0):
    print(f'{i} {x[i]:.8f} {y[i]:.8f}')


"""