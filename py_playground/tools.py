import numpy as np


def parse_file(filename):
  """ Parse files structured in the same way as demos are """
  with open(filename) as f:
    points = []
    for line in f.readlines():
        points += [[int(_) for _ in point.strip('()').split(',')] for point in line.split()]
    return np.array(points)

def rescale(points, new_min=0.1, new_max=1):
    """ Rescale each axis to be within the specified range """
    if len(points) == 1:
      return np.full_like(points, new_max)
    pmin = points.min(axis=0)
    pmax = points.max(axis=0) 
    return (points - pmin) / (pmax - pmin) * (new_max - new_min) + new_min

def gen_borders(xmin=0, xmax=1, ymin=0, ymax=1, n_points=50):
    """ Generate border points """
    border = []
    border += [[_, ymin, 0] for _ in np.linspace(xmin, xmax, n_points)]
    border += [[xmin, _, 0] for _ in np.linspace(ymin, ymax, n_points)]
    border += [[xmax, _, 0] for _ in np.linspace(ymin, ymax, n_points)]
    border += [[_, ymax, 0] for _ in np.linspace(xmin, xmax, n_points)]
    return np.array(border)

def gen_grid(xmin=0, xmax=1, ymin=0, ymax=1, n_points=50):
  """ Given two vertices, generate grid between them """
  x = np.linspace(xmin, xmax, n_points)
  y = np.linspace(ymin, ymax, n_points)
  return np.meshgrid(x, y)


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

