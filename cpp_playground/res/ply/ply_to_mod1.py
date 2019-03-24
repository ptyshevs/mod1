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
        print("usage: ./ply_to_mod1.py <*.ply>")
        exit(1)
    with open(sys.argv[1]) as ply_fp:
        ply_header = ply_fp.readline()
        if ply_header.rstrip() != "ply":
            print("File provided is not a ply format")
            exit(1)
        format = ply_fp.readline()
        comment = ply_fp.readline()
        n_vertices = int(ply_fp.readline().split()[2])
        for _ in range(5):
            ply_fp.readline()
        n_elements = int(ply_fp.readline().split()[2])
        ply_fp.readline()
        end_header = ply_fp.readline()
        if end_header.rstrip() != "end_header":
            print("File doesn't have end_header flag")
            exit(1)

        with open(sys.argv[1][:-4] + '.mod1', 'w+') as mod1_fp:
            for _ in range(n_vertices):
                x, y, z = list(map(float, ply_fp.readline().split()))[:3]
                print("({},{},{})".format(int(100000 * (1 + x)), int(100000 * (y + 1)),  int(100000 * (z + 1))), file=mod1_fp)