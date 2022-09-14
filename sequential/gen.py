import random
import sys
import os

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: {} fname size (in MBs)".format(sys.argv[0]))
    size = int(sys.argv[2])
    length = random.randint(2, 50)
    buf = ''
    overall = 0
    lines_cnt = 0
    with open(sys.argv[1], 'w') as f:
        vec = ''
        for _ in range(length):
            vec += '{} '.format(random.randint(1, 100))
        f.write(vec[:-1] + '\n')
        while overall < size * 2**20:
            line = ''
            for _ in range(length):
                line += '{} '.format(random.randint(1, 100))
            buf += line[:-1] + '\n'
            lines_cnt += 1
            if len(buf) > 30000:
                f.write(buf)
                overall += len(buf)
                buf = ''
    os.system("sed -i '1i{0} {1}\n' {2}".format(length, lines_cnt, sys.argv[1]))
    print('Successfully created')

