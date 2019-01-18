import re
import sys

n = int(sys.argv[1])

for i in range(2,n+1):
	str = "1" * i
	found = re.search(r"^(11+?)\1+$",str)
	if not found:
		print len(str),str
