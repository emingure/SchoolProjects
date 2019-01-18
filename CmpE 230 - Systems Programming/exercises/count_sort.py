import sys
import re

filename = sys.argv[1]

file = open(filename,'r')

dict = {}

for l in file:
	found = re.findall(r"\w+",l)
	for word in found:
		word = word.lower()
		if not word in dict:
			dict[word] = 0
		dict[word] += 1

for key in sorted(dict):
	print (key,dict[key])