import numpy as np


def parse_file(filename):
    with open(filename) as f:
        points = []
        for line in f.readlines():
            points += [[int(_) for _ in point.strip('()').split(',')] for point in line.split()]
        return points


def normalize(points):
    mx = max([max(point) for point in points])
    mn = min([min(point) for point in points])
    return [[(_ - mn) / (mx - mn) for _ in point] for point in points]


def lin_interpolate(point, xmin=0, xmax=1, ymin=0, ymax=1, zmin=0, zmax=1, steps=50):
    interpol = []  # funny name ;)
    px, py, pz = point
    for x in np.linspace(xmin, xmax, steps):
        for y in np.linspace(ymin, ymax, steps):
            d = (x - xmin) / (xmax - xmin) * (zmax - zmin) + \
                (y - ymin) / (ymax - ymin) * (zmax - zmin)
            interpol.append((x, y, d))
    return interpol


if __name__ == '__main__':
    points = parse_file('../resources/piramid.mod1')
    for point in points:
        print(point)
    h = points[0]
    for p in lin_interpolate(points[0], 0, 2, 0, 2, 0, 1, 5):
        print(p)

