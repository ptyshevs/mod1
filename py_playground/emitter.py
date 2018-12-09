import random
import time
from height_map import Cube


class Particle:
    def __init__(self, x, y, z, dx=0.0, dy=0.0, dz=0.0):
        self.x, self.y, self.z = x, y, z
        self.dx, self.dy, self.dz = dx, dy, dz

    def step(self):
        self.x += self.dx
        self.y += self.dy
        self.z += self.dz
    def __repr__(self):
        return f"Particle [{self.x:.2f}, {self.y:.2f}, {self.z:.2f}" \
               f" -> {self.dx}, {self.dy}, {self.dz}]"


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
                return Particle(cx, cy, cz, dz=-.01)
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
