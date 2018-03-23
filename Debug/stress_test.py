import os
import random
def new_test():
    with open('test.txt', 'w+') as f:
        commands = []
        for i in range(1000):
            commands.append("insert " + str(i) + "\n")
            commands.append("delete " + str(i) + "\n")

        random.shuffle(commands)
        
        # print(commands)

        for i in range(1000):
            a = commands.index("insert " + str(i) + "\n")
            b = commands.index("delete " + str(i) + "\n")
            if a > b:
                commands[a] = "delete " + str(i) + "\n"
                commands[b] = "insert " + str(i) + "\n"

        for command in commands:
            f.write(command + "\n")

        f.write("print\n")
        f.write("quit\n")

while True:
    new_test()
    os.system('EECS484_Projects.exe < test.txt > output.txt')
    out = open('output.txt','r')
    if 'Size = 0' in out.read():
            print("Good!")
    else:
        print("Bad result. Sould end up an empty tree. Check test.txt to see the specific test!")
        break