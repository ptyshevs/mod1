import time
import numpy as np


class Particle:
    def __init__(self, coords, grad=np.zeros((3,))):
        self.coords = coords
        self.grad = grad

    def __repr__(self):
        return f"Particle [{self.coords.round(2)}] -> {self.grad.round(2)}"

    @property
    def x(self):
        return self.coords[0]

    @x.setter
    def x(self, val):
        self.coords[0] = val

    @property
    def y(self):
        return self.coords[1]

    @y.setter
    def y(self, val):
        self.coords[1] = val

    @property
    def z(self):
        return self.coords[2]

    @z.setter
    def z(self, val):
        self.coords[2] = val

    @property
    def dx(self):
        return self.grad[0]

    @dx.setter
    def dx(self, val):
        self.grad[0] = val

    @property
    def dy(self):
        return self.grad[1]

    @dy.setter
    def dy(self, val):
        self.grad[1] = val

    @property
    def dz(self):
        return self.grad[2]

    @dz.setter
    def dz(self, val):
        self.grad[2] = val

    def __add__(self, other):
        return Particle(self.coords + other, self.grad)


class Emitter:
    def __init__(self, cmin, cmax, ppersec=1, n_coords=3, pmax=np.inf, mode='rain'):
        """
                Rate - number of water particles per second
        :param cmin: min coordinate value
        :param cmax: max coordinate value
        :param ppersec: particles per second of real time
        :param pmax: max number of particles emitted
        :param mode: if "rain", the last coordinate is <cmax>
                     if "groundwater", the last coordinate is <cmin>
                     otherwise choose it uniformly at random
        """
        self.rate = 1 / ppersec
        self.n_coords = n_coords
        self.time = -1
        self.mode = mode
        self.cmin = cmin
        self.cmax = cmax
        self.range = cmax - cmin
        self.n_emitted = 0
        self.n_max = pmax

    def emit(self):
        """
        Emit cube of state "water", according to the initialization parameters
        :return:
        """
        if (time.clock() - self.time > self.rate) \
                and self.n_emitted < self.n_max:
            # initialize timer
            self.time = time.clock()
            self.n_emitted += 1
            coords = np.random.rand(self.n_coords)
            if self.mode == 'rain':
                coords[-1] = 1
            elif self.mode == 'groundwater':  # underground source
                coords[-1] = 0
            return Particle(self.cmin + coords * self.range, np.zeros(self.n_coords))

    def can_emit(self):
        return self.n_emitted < self.n_max


if __name__ == '__main__':
    e = Emitter(0, 2, 1, n_coords=2, pmax=10)
    while e.can_emit():
        r = e.emit()
        if r:
            print(r)
