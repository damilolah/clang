import os

os.system('\cp LoopFind.cpp ~/llvm/llvm/tools/clang/tools/extra/loop-find/')
os.system('ninja loop-find')

a = os.popen('ls all_tests/').readlines()
command = './bin/loop-find all_tests/'

for b in a:
	cmd = command + b[0:-1]
	os.system(cmd + ' >> result.txt')
