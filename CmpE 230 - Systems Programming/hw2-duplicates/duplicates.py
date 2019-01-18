#!/usr/bin/python

import os
import pwd
import sys
import subprocess
import hashlib
import re


try:
 if(sys.argv[1] == '-h' or sys.argv[1] == '--help'):
  print('usage: duplicates < -c <command> | -p > [-f | -d] [“...”] [<dir1> <dir2> ..]')
  exit()
except Exception:
 pass

next_arg = 1
_p = True #default
try:
 first_arg =  sys.argv[next_arg] 
 #print(first_arg)
 if(first_arg == "-c"):
  _p = False
  next_arg+=1
  command = sys.argv[next_arg]
  #print(command)
 elif(first_arg == "-p"):
  pass
 else:
  next_arg-=1
 next_arg+=1
except Exception:
    pass
 
_f = True #default
try:
 second_arg =  sys.argv[next_arg]
 #print(second_arg)
 if(second_arg == "-d"):
  _f = False
 elif(second_arg == "-f"):
  pass
 else:
  next_arg-=1
 next_arg+=1
except Exception:
    pass
 
_pattern = r''
try:
 if(sys.argv[next_arg][0] == sys.argv[next_arg][-1] == '"'):
  _pattern = r'' + sys.argv[next_arg][1:-1]
  next_arg+=1
 else:
  pass
 #print(_pattern)
except Exception:
    pass
#print('sa',_pattern)
pattern = re.compile(_pattern)


dirlist = []
while True:
 try:
  pth = sys.argv[next_arg]
  if(pth[-1] == '/'):
   pth.remove(-1)
  dirlist.append(pth)
  next_arg+=1
 except Exception:
  break

if(dirlist == []):
 dirlist.append('.')

#print(dirlist)

hash_dict = {}
new_dict = {}
duplicates = {}



def dirs(dirlist,pattern):
 global hash_dict
 while dirlist:
  fullpathname = dirlist.pop()
  #print('!!!',fullpathname)
  hashes = []
  
  curlist = os.listdir(fullpathname)
  for fdname in curlist:

   #print('asasasa')
   newpath = fullpathname + "/" + fdname
   
   
   if os.path.isdir(newpath):#and not fullpathname in hash_dict:
    dirlist.append(newpath)
    #print('dd',dirlist)
    dirs(dirlist,pattern)
    #print('##',newpath, hash_dict[newpath])
    try:
     hashes.append(hash_dict[newpath])
    except Exception as e:
     print('!!!!!!!!!!!!!!!!!!!!!!!!')
     print(e)
   else:
    #print(newpath)
    out = subprocess.check_output("shasum -a 256 '"+newpath+"'",shell=True).decode("utf-8")[0:64]
    hashes.append(out)
 #print('@@',hashes)
 hashes.sort()
 s = ''.join(hashes)
 hash_dict[fullpathname] = hashlib.sha256(s.encode('utf-8')).hexdigest()
 #print(fullpathname, hash_dict[fullpathname])
def files(dirlist,pattern):
 while dirlist:
  fullpathname = dirlist.pop()
  curlist = os.listdir(fullpathname)
  for fdname in curlist:
   newpath = fullpathname + "/" + fdname
   if os.path.isdir(newpath):
    dirlist.append(newpath)
   else:
    out = subprocess.check_output("shasum -a 256 '"+newpath+"'",shell=True).decode("utf-8")[0:64]
    hash_dict[newpath] = out


if(_f):
 # -f
 files(dirlist,pattern)

else:
 # -d
 dirs(dirlist,pattern)

for key,value in hash_dict.items():

 new_dict[value] = new_dict.get(value,[])
 new_dict[value].append(key)
for key,value in new_dict.items():
 if(len(value) > 1):
  pss = True
  #print('bb',value)
  valueNew = []
  for path in value:
   #print(os.path.basename(path), pattern.search(os.path.basename(path)))
   if(pattern.search(os.path.basename(path)) != None):
    valueNew.append(path)
    #print('cc',os.path.basename(path))
    #print('aa',value)
  if(len(valueNew) > 1):  
   if(_p):
    if(key != 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855'):
     #print(key)
     print()
     for path in valueNew:
      print (path+'\t',end="")
      
   else:
    for path in valueNew:
     cmmd = subprocess.check_output(command + ' "' + path + '"',shell=True).decode("utf-8")
     print(cmmd)
   print() 

#print(new_dict)