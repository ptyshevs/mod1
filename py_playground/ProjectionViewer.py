import wireframe
import numpy as np
import contextlib
with contextlib.redirect_stdout(None):
    import pygame


class ProjectionViewerNP:
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

        # self.wireframes = {}
        self.displayNodes = True
        self.displayEdges = True
        self.nodeColor = (255, 255, 255)
        self.edgeColor = (200, 200, 200)
        self.nodeRadius = 1

        self.view_angle = [0, 0, 0]  # view angle in 3 axis
        self.scale = [1, 1, 1]  # scale in 3 axis
        self.bias = [0, 0, 0]  # bias in 3 axis

        # default settings
        self.scale = [350, 350, 350]
        self.view_angle = [-.95, -.35, 0]
        self.bias = [0, 0, 0]

        self.keys_supported = (pygame.K_LEFT, pygame.K_RIGHT, pygame.K_DOWN,
                               pygame.K_UP, pygame.K_EQUALS, pygame.K_MINUS,
                               pygame.K_q, pygame.K_w, pygame.K_a, pygame.K_s,
                               pygame.K_z, pygame.K_x)
        # self.key_to_function = {
        #     pygame.K_LEFT: (lambda x: x.translateAll([-10, 0, 0])),
        #     pygame.K_RIGHT: (lambda x: x.translateAll([10, 0, 0])),
        #     pygame.K_DOWN: (lambda x: x.translateAll([0, 10, 0])),
        #     pygame.K_UP: (lambda x: x.translateAll([0, -10, 0])),
        #     pygame.K_EQUALS: (lambda x: x.scaleAll([1.25, 1.25, 1.25])),
        #     pygame.K_MINUS: (lambda x: x.scaleAll([0.8, 0.8, 0.8])),
        #     pygame.K_q: (lambda x: x.rotateAll('X', 0.1)),
        #     pygame.K_w: (lambda x: x.rotateAll('X', -0.1)),
        #     pygame.K_a: (lambda x: x.rotateAll('Y', 0.1)),
        #     pygame.K_s: (lambda x: x.rotateAll('Y', -0.1)),
        #     pygame.K_z: (lambda x: x.rotateAll('Z', 0.1)),
        #     pygame.K_x: (lambda x: x.rotateAll('Z', -0.1))
        # }

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
        # self.scaleAll([350, 350, 350])
        # self.rotateAll('X', -.95)
        # self.rotateAll('Y', -.35)

    def recalculate_transform(self):
        """ Recalculate coordinates of nodes based on the cumulative transformation """
        bias_mat = wireframe.translationMatrix(*self.bias)
        rot_mat = wireframe.rotateMatrix(*self.view_angle)
        scale_mat = wireframe.scaleMatrix(*self.scale)

        self.tf_wireframe.nodes = (self.wireframe.nodes - self._center_half) @ \
                                   rot_mat @ scale_mat @ bias_mat

    def run(self):
        pygame.key.set_repeat(80)
        running = True
        self.init_condition()
        self.recalculate_transform()
        while running:
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

    def addWireframe(self, wireframe):
        """ Add wireframe to ProjectionViewer """
        self.wireframe = wireframe
        self.tf_wireframe = wireframe.copy()


if __name__ == '__main__':
    # pv = ProjectionViewer(400, 300)
    # pv.addWireframe('cube', wireframe.Cube())
    # pv.run()
    pv = ProjectionViewerNP(400, 300)
    pv.addWireframe(wireframe.Cube())
    pv.run()
