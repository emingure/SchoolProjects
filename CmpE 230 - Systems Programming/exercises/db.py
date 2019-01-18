import sys

filename1 = sys.argv[1]
filename2 = sys.argv[2]

f1 = open(filename1,'r')
f2 = open(filename2,'r')

db = {}

for l in f1:
	line = l.split()
	db[line[0]] = []
	db[line[0]].append(line[1])
	db[line[0]].append(line[2])

for l in f2:
	line = l.split()
	db[line[0]].append(line[1])

dbnew = {}

for id in db:
	#print(db[id][2])
	if not db[id][2] in dbnew:
		dbnew[db[id][2]] = []
	dbnew[db[id][2]].append([db[id][1],db[id][0]])

#print(dbnew)

for city in sorted(dbnew):
	print(city + ':')
	for person in dbnew[city]:
		
		print('\t' + person[0] + ',' + person[1])