n=[]
x=[]

num = int(input())
for i in range(num):
	line = input().split()
	temp = []
	for j in range(num):
		temp.append(int(line[j]))
	n.append(temp)
	x.append(int(line[num]))

result = []
for i in n:
	r = 0
	for k,m in zip(x,i):	
		r += k*m
	result.append(r)

print(result)