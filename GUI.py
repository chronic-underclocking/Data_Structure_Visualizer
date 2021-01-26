import subprocess
import tkinter as tk
from tkinter import * 


def compileStructures():
    cmd = "g++ dataStructures/RedBlackTree.cpp -o RedBlackTree.exe"
    subprocess.run(cmd)

def rbtree():
    print
    subprocess.run('RedBlackTree.exe')
    print


def getRadioButtonValue():
    buttonSelected = rbVariable.get()
    return buttonSelected


def btnClickFunction():
    choice = getRadioButtonValue()
    if choice == 1:
        rbtree()

root = Tk()

rbVariable = tk.IntVar()

root.geometry('852x509')
root.configure(background='#8B7355')
root.title('Data Structure Visualizer')


Label(root, text='Choose a data structure:', bg='#8B7355', font=('courier', 22, 'bold',)).place(x=24, y=29)

frame=Frame(root, width=0, height=0, bg='#C1CDCD')
frame.place(x=27, y=80)
ARBEES=[
    ('Red Black Tree', 1), 
    ]
for text, mode in ARBEES:
    rbGroupOne=Radiobutton(frame, text=text, variable=rbVariable, value=mode, bg='#8B7355', font=('courier', 15, 'bold')).pack(side='top', anchor = 'w')

Button(root, text='Begin!', bg='#C1CDCD', font=('courier', 18, 'bold'), command=btnClickFunction).place(x=370, y=396)

def main():
    compileStructures()
    root.mainloop()

if __name__ == "__main__":
    main()


