import sys
import re

filename = sys.argv[1]
name = sys.argv[2]

file = open(filename,'r')

rehber = {}

for l in file:
	found = re.search(r"(\w+)\s+([0-9\-]+)",l)
	rehber[found.group(1)] = found.group(2)

if name in rehber:
	print(rehber[name])
else:
	print("not found")