import numpy as np
from interpol import Estimation
from tools import parse_file, rescale, center, gen_borders
from ProjectionViewer import ProjectionViewerNP
from wireframe import Wireframe


class Cube:
    def __init__(self, x, y, z, state="empty"):
        """ Cube is defined by a down-bottom-left point """
        self.x = x
        self.y = y
        self.z = z
        self.state = state

    def __repr__(self):
        return f"[{self.x:.2f}, {self.y:.2f}, {self.z:.2f}] - {self.state}"

    @property
    def coords(self):
        return [self.x, self.y, self.z]

    @coords.setter
    def coords(self, val):
        self.x, self.y, self.z = val

class DynamicCube:
    def __init__(self, x, y, z, dx=0, dy=0, dz=0):
        pass


def height_map(points, n_points, cmin, cmax):
    c = np.linspace(cmin, cmax, n_points)
    hmap = np.empty((n_points, n_points, n_points), dtype=object)
    step = cmax / n_points

    e = Estimation(points[:, 0], points[:, 1], points[:, 2])
    for i in range(n_points):
        for j in range(n_points):
            z_int = e.estimate(c[i], c[j], sigma=0.6)
            z = int(z_int // step)  # index of interpolated terrain cube
            for k in range(n_points):
                if k != z or z == 0:
                    hmap[i, j, k] = Cube(c[i], c[j], c[k])
                else:
                    hmap[i, j, k] = Cube(c[i], c[j], c[k], 'terrain')
    return hmap


def height_to_nodes(hmap):
    """
    Convert 3D height map to numpy array of points
    :param hmap:
    :return:
    """
    n = hmap.shape[0]
    nodes = np.zeros((n ** 3, 3))
    for i in range(n):
        for j in range(n):
            for k in range(n):
                p = hmap[i, j, k]
                nodes[i * n ** 2 + j * n + k] = [p.x, p.y, p.z]
    return nodes


def height_to_edges(hmap):
    """
    Generate edges of terrain cubes with 'terrain' state
    :param hmap:
    :return:
    """
    n = hmap.shape[0]
    edges = []
    idx = lambda i, j, k: n ** 2 * i + n * j + k
    for i in range(n - 1):
        for j in range(n - 1):
            for k in range(n - 1):
                if hmap[i, j, k].state != 'empty':
                    ind = idx(i, j, k)
                    ind2 = idx(i + 1, j + 1, k + 1)
                    edges.append((ind, idx(i + 1, j, k)))
                    edges.append((ind, idx(i, j + 1, k)))
                    edges.append((ind, idx(i, j, k + 1)))
                    edges.append((ind2, idx(i + 1, j + 1, k)))
                    edges.append((ind2, idx(i + 1, j, k + 1)))
                    edges.append((ind2, idx(i, j + 1, k + 1)))
                    edges.append((idx(i, j, k + 1), idx(i, j + 1, k + 1)))
                    edges.append((idx(i, j + 1, k), idx(i + 1, j + 1, k)))
                    edges.append((idx(i + 1, j, k), idx(i + 1, j + 1, k)))
                    edges.append((idx(i + 1, j, k), idx(i + 1, j, k + 1)))
                    edges.append((idx(i, j + 1, k), idx(i, j + 1, k + 1)))
                    edges.append((idx(i, j, k + 1), idx(i + 1, j, k + 1)))
    return edges


if __name__ == '__main__':

    cmin = 0
    cmax = 2
    n_points = 100  # very slow
    n_points = 30

    points = center(rescale(parse_file('../resources/demo4.mod1')))
    points = np.vstack([points, gen_borders(cmin, cmax, cmin, cmax)])

    hmap = height_map(points, n_points, cmin, cmax)
    nodes = height_to_nodes(hmap)
    edges = height_to_edges(hmap)

    wr = Wireframe()
    wr.addNodes(nodes)
    wr.addEdges(edges)
    pv = ProjectionViewerNP(800, 800)
    pv.addWireframe(wr)
    pv.cube_colors(hmap)
    pv.run()
