#!/usr/local/bin/python3
import sys

def get_y(entry):
    """ It's either number, or it has color specified after comma """
    try:
        return int(entry)
    except ValueError:
        return int(entry.split(',')[0])

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("usage: ./fdf_to_mod1.py <*.fdf> -r <n>")
        exit(1)
    repeat_zeros = 1
    if len(sys.argv) == 4 and sys.argv[2] == "-r":
        repeat_zeros = int(sys.argv[3])
    elif len(sys.argv) != 2:
        print("usage: ./fdf_to_mod1.py <*.fdf> -r <n>")
        exit(1)
    with open(sys.argv[1]) as fdf_fp:
        fdf = [list(map(get_y, line.split())) for line in fdf_fp.readlines()]
        with open(sys.argv[1][:-4] + '.mod1', 'w+') as mod1_fp:
            for y, line in enumerate(fdf):
                for x, z in enumerate(line):
                    if (z == 0):
                        for _ in range(repeat_zeros):
                            print("({},{},{})".format(x, z, y), file=mod1_fp)
                    else:
                            print("({},{},{})".format(x, z, y), file=mod1_fp)