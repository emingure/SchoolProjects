import math
import matplotlib.pyplot as plt
from matplotlib import gridspec

def lofn(n):
    top = 1
    for j in range(2,n):
        top = top + lofn(j)*funcp(n,j)
    bottom = 1 - funcp(n,0) - funcp(n,n)
    l = top/bottom
    return l

def funcp(n,j):
    part1 = math.factorial(n)/(math.factorial(j)*math.factorial(n-j))
    part2 = n**(-j)
    part3 = (1-1/n)**(n-j)
    return part1*part2*part3


n = range(2,11)
l = []
for i in n:
    l.append(lofn(i))

plt.subplot(2,1,1)
plt.plot(n, l)
plt.axis([1, 11, 1, 3])
plt.xlabel("n")
plt.ylabel("l(n)")
plt.axhline(y=math.exp(1), color='r', linestyle='-', label="e")
plt.axhline(y=2.442, color='g', linestyle='-', label="limit(n->inf)")
plt.legend()
# plt.show()

for n in [5,10]:
    j = range(0,n+1)
    p = []
    for i in j:
        p.append(funcp(n,i))

    plt.subplot(2,2,n/5+2)
    plt.plot(j, p)
    plt.axis([0, n, 0, 0.5])
    plt.xlabel("j")
    plt.ylabel("p(n,j)")
    plt.legend()
    # plt.show()

plt.tight_layout()
# plt.show()
plt.savefig("fig.png")
