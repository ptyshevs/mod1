import random
import time
from height_map import Cube
import numpy as np


class Particle:
    def __init__(self, x, y, z, dx=0.0, dy=0.0, dz=0.0):
        self.coords = np.array([x, y, z])
        self.grad = np.array([dx, dy, dz])

    def __repr__(self):
        return f"Particle [{self.x:.2f}, {self.y:.2f}, {self.z:.2f}" \
               f" -> {self.dx}, {self.dy}, {self.dz}]"

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
        x, y, z = self.coords + other
        return Particle(x, y, z, *self.grad)


class Emitter:
    def __init__(self, cmin, cmax, ppersec=1, type='rain'):
        """
        Rate - number of water particles per second
        """
        self.rate = 1 / ppersec
        self.time = -1
        self.emitted = False
        self.type = type
        self.cmin = cmin
        self.cmax = cmax

    def emit(self):
        """
        Emit cube of state "water", according to the initialization parameters
        :return:
        """
        if not self.emitted or time.clock() - self.time > self.rate:
            # initialize timer
            self.time = time.clock()
            self.emitted = True
            cx = random.uniform(self.cmin, self.cmax)
            cy = random.uniform(self.cmin, self.cmax)
            if self.type == 'rain':
                cz = self.cmax
                return Particle(cx, cy, cz)
            elif self.type == 'underground':  # underground source
                cz = 0
                return Particle(cx, cy, cz, dz=.1)
            else:  # pour it randomly
                cz = random.uniform(self.cmin, self.cmax)
            return Cube(cx, cy, cz, "water")


if __name__ == '__main__':
    e = Emitter(0, 2, 1)
    while True:
        r = e.emit()
        if r:
            print(r)
