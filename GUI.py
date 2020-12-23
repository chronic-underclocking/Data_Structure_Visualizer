import subprocess

cmd = "g++ dataStructures\\RedBlackTree.cpp -o RedBlackTree.exe"
subprocess.run(cmd)
print
subprocess.run('RedBlackTree.exe')
print

