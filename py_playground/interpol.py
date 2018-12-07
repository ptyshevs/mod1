import numpy as np


class Estimation:
    def __init__(self, datax, datay, dataz):
        self.x = datax
        self.y = datay
        self.v = dataz

    def estimate(self, x, y, sigma=0.0, p=-2):
        """
        Estimate point at coordinate x,y based on the input data for this
        class.
        """
        d = np.sqrt((x - self.x) ** 2 + (y - self.y) ** 2) ** (1 + sigma)
        if d.min() > 0:
            return np.sum(self.v * d ** p / np.sum(d ** p))
        else:
            return self.v[d.argmin()]


def idw_mesh(points, n_points=50, sigma=0.6, map_points=True, xmin=0, xmax=2, ymin=0, ymax=2):
    """
        Generate mesh of interpolated height map using Inverse Distance Weighting

    :param points: map points to interpolate
    :param n_points: number of points per axis. 50 will give you 50x50 grid
    :param sigma: smoothing parameter
            Euclidean distance d(p1, p2) ^ (1 + sigma)
    :param map_points: Append map points if True
    :param xmin:
    :param xmax:
    :param ymin:
    :param ymax:
    :return: Array of interpolated points
    """
    e = Estimation(points[:, 0], points[:, 1], points[:, 2])
    x = np.linspace(xmin, xmax, n_points)
    y = np.linspace(ymin, ymax, n_points)
    res = np.zeros((n_points ** 2, 3))
    for i in range(n_points):
        for j in range(n_points):
            res[i * n_points + j, :] = x[i], y[j], e.estimate(x[i], y[j], sigma=sigma)
    if map_points:
        return np.vstack((res, points))
    return res

