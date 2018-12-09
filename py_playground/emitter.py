import random
import time
from height_map import Cube


class Emitter:
    def __init__(self, cmin, cmax, n_steps, rate=1, type='rain'):
        """
        Rate - number of water particles per second
        """
        self.rate = rate
        self.time = -1
        self.n_steps = n_steps
        self.step = (cmax - cmin) / n_steps
        self.emitted = False
        self.type = type

    def emit(self):
        """
        Emit cube of state "water", according to the initialization parameters
        :return:
        """
        if not self.emitted:
            # initialize timer
            self.time = time.clock()
            self.emitted = True
            cx = random.randrange(0, self.n_steps)
            cy = random.randrange(0, self.n_steps)
            if self.type == 'rain':
                cz = self.n_steps - 1
            elif self.type == 'source':  # underground source
                cz = 0
            else:  # pour it randomly
                cz = random.randint(0, self.n_steps)
            return Cube(cx * self.step, cy * self.step, cz * self.step, "water")
        else:
            if time.clock() - self.time > self.rate:
                self.emitted = False
                return self.emit()
            else:
                return None


if __name__ == '__main__':
    e = Emitter(0, 2, 4)
    while True:
        r = e.emit()
        if r:
            print(r)
