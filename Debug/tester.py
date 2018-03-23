import os
from random import *

def new_test():
    with open('rando_test.txt', 'w+') as f:
        nums = []
        for i in range(20):
            num = 0
            if i < 15:
                command = 'insert'
                num = randint(0, 200)
                nums.append(num)
            else:
                command = 'delete'
                j = randint(0, len(nums)-1)
                num = nums[j]
                del nums[j]

            f.write(command + ' ' + str(num) + '\n')
            f.write('print\n')

        f.write('quit\n')

testNum = 0
new_test()
while testNum < 10 and os.system('EECS484_Projects.exe < rando_test.txt > out/test' + str(testNum) + '.out') == 0:
    print("Test: " + str(testNum))
    testNum += 1
    new_test()