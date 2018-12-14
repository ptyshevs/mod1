import time
import wireframe
import numpy as np
import contextlib
with contextlib.redirect_stdout(None):
    import pygame


class WaterSimulator:
    """ Displays 3D objects on a Pygame screen"""

    def __init__(self, width, height):
        pygame.init()
        self.width = width
        self.ws = width // 2  # width scale
        self.height = height
        self.hs = height // 2  # height scale
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption("Wireframe Display")
        self.background = (10, 10, 50)

        self.wireframe = None  # original wireframe
        self.tf_wireframe = None
        self.wireframe_col = None

        self.hmap = None

        self.emitter = None
        self.particles = []

        # self.wireframes = {}
        self.displayNodes = True
        self.displayEdges = True
        self.displayParticles = True
        self.nodeColor = (255, 255, 255)
        self.edgeColor = (200, 200, 200)
        self.waterColor = (0, 0, 255)
        self.nodeRadius = 1

        self.view_angle = [0, 0, 0]  # view angle in 3 axis
        self.scale = [1, 1, 1]  # scale in 3 axis
        self.bias = [0, 0, 0]  # bias in 3 axis

        # default settings
        self.time_step = .0003
        self.scale = [350, 350, 350]
        self.view_angle = [-.95, -.35, 0]
        self.bias = [0, 0, 0]

        self.keys_supported = (pygame.K_LEFT, pygame.K_RIGHT, pygame.K_DOWN,
                               pygame.K_UP, pygame.K_EQUALS, pygame.K_MINUS,
                               pygame.K_q, pygame.K_w, pygame.K_a, pygame.K_s,
                               pygame.K_z, pygame.K_x)

    def addHeightMap(self, hmap):
        self.hmap = hmap

    def addEmitter(self, emitter):
        self.emitter = emitter

    def key_hooks(self, key):
        """ Map key to change in transformation matrix """
        if key == pygame.K_LEFT:
            self.bias[0] -= 10
        elif key == pygame.K_RIGHT:
            self.bias[0] += 10
        elif key == pygame.K_DOWN:
            self.bias[1] += 10
        elif key == pygame.K_UP:
            self.bias[1] -= 10
        elif key == pygame.K_EQUALS:
            self.scale[0] += 5
            self.scale[1] += 5
            self.scale[2] += 5
        elif key == pygame.K_MINUS:
            self.scale[0] -= 5
            self.scale[1] -= 5
            self.scale[2] -= 5
        elif key == pygame.K_q:
            self.view_angle[0] += .1
        elif key == pygame.K_w:
            self.view_angle[0] -= .1
        elif key == pygame.K_a:
            self.view_angle[1] += .1
        elif key == pygame.K_s:
            self.view_angle[1] -= .1
        elif key == pygame.K_z:
            self.view_angle[2] += .1
        elif key == pygame.K_x:
            self.view_angle[2] -= .1

    def node_colors(self, nodes):
        """ Calculate color gradient based on z coord of nodes """
        zmin, zmax = nodes[:, 2].min(), nodes[:, 2].max()
        start_color = np.array(self.background) + 5
        end_color = np.array(self.nodeColor)
        z = (nodes[:, 2] - zmin) / (zmax - zmin)
        # indexing [:, None] is used to explicitly state second axis
        c = (1 - z)[:, None] @ start_color[:, None].T + z[:, None] @ end_color[:, None].T
        self.wireframe_col = c
        # return c

    def cube_colors(self, cubes):
        """ Calculate color from cubes """
        n = cubes.shape[0]
        col = np.zeros((n ** 3, 3))
        terrain_col = (66, 244, 72)
        empty_col = self.background
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    c = cubes[i, j, k]
                    col[i * n ** 2 + j * n + k] = empty_col if c.state == 'empty' else terrain_col
        self.wireframe_col = col

    def init_condition(self):
        # self.wireframe_col = self.node_colors(self.wireframe.nodes)

        cmin = np.hstack((self.wireframe.nodes[:, :3].min(axis=0), [0]))
        cmax = np.hstack((self.wireframe.nodes[:, :3].max(axis=0), [0]))
        # move center of coord. system by subtracting half of distance wrt each axis
        self._center_half = (cmax - cmin) / 2
        self.time = time.clock()
        # self.scaleAll([350, 350, 350])
        # self.rotateAll('X', -.95)
        # self.rotateAll('Y', -.35)

    def recalculate_transform(self):
        """ Recalculate coordinates of nodes based on the cumulative transformation """
        bias_mat = wireframe.translationMatrix(*self.bias)
        rot_mat = wireframe.rotateMatrix(*self.view_angle)
        scale_mat = wireframe.scaleMatrix(*self.scale)

        self.tf_mat = rot_mat @ scale_mat @ bias_mat
        self.tf_wireframe.nodes = (self.wireframe.nodes - self._center_half) @ \
                                  self.tf_mat

    def transform_node(self, node):
        """ Transform single node (used for particle rendering) """
        return (node - self._center_half) @ self.tf_mat

    def run(self):
        pygame.key.set_repeat(80)
        running = True
        self.init_condition()
        self.recalculate_transform()
        while running:
            particle = self.emitter.emit()
            if particle:
                self.particles.append(particle)
            self.update_particles()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key in self.keys_supported:
                        self.key_hooks(event.key)
                        self.recalculate_transform()
            self.display()
            pygame.display.flip()

    def update_particles(self):
        """ """
        for particle in self.particles:
            x, y, z = particle.x, particle.y, particle.z
            old_location = self.discritize(particle, False)
            self.hmap.hmap[old_location].state = 'empty'
            # print(self.hmap.hmap[ijk])
            if x + particle.dx < self.hmap.cmin or x + particle.dx > self.hmap.cmax:
                particle.dx = 0  # maybe bounce? (multiplying by -1)
            if y + particle.dy < self.hmap.cmin or y + particle.dy > self.hmap.cmax:
                particle.dy = 0
            if z + particle.dz < self.hmap.cmin or z + particle.dz > self.hmap.cmax:
                particle.dz = 0
            i1, j1, k1 = self.discritize(particle + particle.grad, False)
            next_cube = self.hmap.hmap[i1, j1, k1]
            if not next_cube.is_empty():
                particle.grad = np.zeros_like(particle.grad)
                # calculate gradient
                # partial derivative in x axis
                # neighbour_cube = self.hmap.hmap[i1 + 1, j1, k1]
                # terrain_level = self.hmap.find_terrain(i1 + 1, j1, k1)
                # if terrain_level != -1:
                #     dzdx = k1 - terrain_level
                # print("WTF:", next_cube, "Terrain level:", terrain_level, "dz/dx:", dzdx)
                pass
            # print(self.hmap.hmap[i1, j1, k1])
            particle.coords += particle.grad
            particle.grad += (0, 0, -9.81 * self.time_step)
            new_location = self.discritize(particle, False)
            self.hmap.hmap[new_location].state = 'water'


    def display(self):
        """ Draw the wireframes on the screen. """

        self.screen.fill(self.background)

        if self.displayEdges:
            for n1, n2 in self.tf_wireframe.edges:
                start_pos = self.tf_wireframe.nodes[n1][:2] + [self.ws, self.hs]
                end_pos = self.tf_wireframe.nodes[n2][:2] + [self.ws, self.hs]
                pygame.draw.aaline(self.screen, self.edgeColor,
                                   start_pos,
                                   end_pos, 2)
        if self.displayNodes:
            for i, node in enumerate(self.tf_wireframe.nodes):
                c = self.wireframe_col[i, :]
                if np.all(c != self.background):
                    pygame.draw.circle(self.screen, c, (int(self.ws + node[0]), int(self.hs + node[1])), self.nodeRadius, 0)

        if self.displayParticles:
            for particle in self.particles:
                idx = self.discritize(particle)
                node = self.wireframe.nodes[idx] + self.hmap.step / 2  # move circle into the center of a cube
                node = self.transform_node(node)
                x, y = int(self.ws + node[0]), int(self.hs + node[1])
                pygame.draw.circle(self.screen, self.waterColor, (x, y), 4)


    def discritize(self, particle, D1=True):
        """ Find node where this particle belongs """
        step = self.hmap.step
        xi, yi, zi = [int(_ / step) for _ in particle.coords]
        if D1:
            return xi * self.hmap.n ** 2 + yi * self.hmap.n + zi
        else:
            return xi, yi, zi


    def addWireframe(self, wireframe):
        """ Add wireframe to ProjectionViewer """
        self.wireframe = wireframe
        self.tf_wireframe = wireframe.copy()


if __name__ == '__main__':
    # pv = ProjectionViewer(400, 300)
    # pv.addWireframe('cube', wireframe.Cube())
    # pv.run()
    pv = WaterSimulator(400, 300)
    pv.addWireframe(wireframe.Cube())
    pv.run()
