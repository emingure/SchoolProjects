import sys

filename1 = sys.argv[1]
filename2 = sys.argv[2]

f = open('names.txt','r')
f1 = open(filename1,'r')
f2 = open(filename2,'r')

names = {}

for l in f:
	line = l.lower().split()
	names[line[0]] = line[1]

#print(f1.read().split())

result = {}

def func(f):
	data = f.read().lower().replace('.','').split()

	for i in range(len(data)):
		if data[i] in names:
			if(data[i+1] == names[data[i]]):
				ns = data[i]+'-'+data[i+1]
				if not ns in result:
					result[ns] = 0
				result[ns] += 1

func(f1)
func(f2)

for k in sorted(result):
	print(k,'=',result[k])