import os
a = os.popen('ls all_tests/').readlines()
command = './bin/loop-find all_tests/'

for b in a:
	cmd = command + b[0:-1]
	os.system(cmd + ' >> result.txt')
