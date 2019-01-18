import re

string = "yeni york123"
pattern = r"(a|y)\w+\d{2,4}?."

found = re.search(pattern,string)
if found:
	print found.group()
else:
	print 'not found'
