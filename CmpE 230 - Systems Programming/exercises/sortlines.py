import sys

f = open(sys.argv[1],'r')

lines = []
for l in f:
	lines.append(l.replace('\n','').lower())

print(lines)

lines.sort()

for i in lines:
	print(i)