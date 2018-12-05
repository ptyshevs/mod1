import numpy as np


class Estimation:
    def __init__(self, datax, datay, dataz):
        self.x = datax
        self.y = datay
        self.v = dataz

    def estimate(self, x, y, using='ISD', sigma=.4):
        """
        Estimate point at coordinate x,y based on the input data for this
        class.
        """
        if using == 'ISD':  # inverse squared distance
            return self._isd(x, y, sigma=sigma)

    def _isd(self, x, y, p=-2, sigma=.4):
        d = np.sqrt((x - self.x) ** 2 + (y - self.y) ** 2 + sigma ** 2)
        if d.min() > 0:
            v = np.sum(self.v * d ** p / np.sum(d ** p))
            return v
        else:
            return self.v[d.argmin()]


def idw_mesh(points, xmin=0, xmax=1, ymin=0, ymax=1, n_points=50):
    """
    Generate mesh of interpolated height map
    :param points:
    :param xmin:
    :param xmax:
    :param ymin:
    :param ymax:
    :param n_points:
    :return: array of interpolated points
    """
    e = Estimation(points[:, 0], points[:, 1], points[:, 2])
    x = np.linspace(xmin, xmax, n_points)
    y = np.linspace(ymin, ymax, n_points)
    res = np.zeros((n_points ** 2, 3))
    for i in range(n_points):
        for j in range(n_points):
            res[i * n_points + j, :] = x[i], y[j], e.estimate(x[i], y[j], sigma=.2)
    return res

