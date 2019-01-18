import random
import math
import matplotlib.pyplot as plt

n = [10**2,10**3,10**4,10**5,10**6]
r = 2
results = []

for i in n:
	in_count = 0
	for j in range(0,i):
		x = random.uniform(0,r)
		y = random.uniform(0,r)
		if (x**2 + y**2 <= r**2):
			in_count = in_count+1
	results.append(in_count/i)

plt.xscale('log')
plt.plot(n, results, 'ro')
plt.axis([10**1, 10**7, 0.5, 1])
plt.xlabel("Sample")
plt.ylabel("Probability")
plt.axhline(y=math.pi/4, color='b', linestyle='-', label="PI/4")
plt.legend()
plt.savefig("fig2.png")
