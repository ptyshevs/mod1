import wireframe
import wireframe_np
import numpy as np
import contextlib
with contextlib.redirect_stdout(None):
    import pygame


class ProjectionViewer:
    """ Displays 3D objects on a Pygame screen"""

    def __init__(self, width, height):
        pygame.init()
        self.width = width
        self.ws = width / 2  # width scale
        self.height = height
        self.hs = height / 2  # height scale
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption("Wireframe Display")
        self.background = (10, 10, 50)

        self.wireframes = {}
        self.displayNodes = True
        self.displayEdges = True
        self.nodeColor = (255, 255, 255)
        self.edgeColor = (200, 200, 200)
        self.nodeRadius = 4

        self.key_to_function = {
            pygame.K_LEFT: (lambda x: x.translateAll('x', -10)),
            pygame.K_RIGHT: (lambda x: x.translateAll('x', 10)),
            pygame.K_DOWN: (lambda x: x.translateAll('y', 10)),
            pygame.K_UP: (lambda x: x.translateAll('y', -10)),
            pygame.K_EQUALS: (lambda x: x.scaleAll(1.25)),
            pygame.K_MINUS: (lambda x: x.scaleAll(0.8)),
            pygame.K_q: (lambda x: x.rotateAll('X', 0.1)),
            pygame.K_w: (lambda x: x.rotateAll('X', -0.1)),
            pygame.K_a: (lambda x: x.rotateAll('Y', 0.1)),
            pygame.K_s: (lambda x: x.rotateAll('Y', -0.1)),
            pygame.K_z: (lambda x: x.rotateAll('Z', 0.1)),
            pygame.K_x: (lambda x: x.rotateAll('Z', -0.1))
        }

    def run(self):
        pygame.key.set_repeat(80)
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key in self.key_to_function:
                        self.key_to_function[event.key](self)
            self.display()
            pygame.display.flip()

    def display(self):
        """ Draw the wireframes on the screen. """

        self.screen.fill(self.background)

        for wr in self.wireframes.values():
            if self.displayEdges:
                for edge in wr.edges:
                    start_pos = edge.start.x, edge.start.y
                    end_pos = edge.stop.x, edge.stop.y
                    pygame.draw.aaline(self.screen, self.edgeColor,
                                       start_pos,
                                       end_pos, 1)
            if self.displayNodes:
                for node in wr.nodes:
                    pygame.draw.circle(self.screen, self.nodeColor,
                                       (int(node.x), int(node.y)), self.nodeRadius, 0)

    def addWireframe(self, name, wireframe):
        self.wireframes[name] = wireframe

    def translateAll(self, axis, d):
        for wr in self.wireframes.values():
            wr.translate(axis, d)

    def rotateAll(self, axis, rad):
        rotFun = "rotate" + axis
        for wr in self.wireframes.values():
            centre = wr.findCentre()
            getattr(wr, rotFun)(centre, rad)

    def scaleAll(self, scale):

        centre = self.width / 2, self.height / 2

        for wr in self.wireframes.values():
            wr.scale(centre, scale)


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

        self.wireframes = {}
        self.displayNodes = True
        self.displayEdges = True
        self.nodeColor = (255, 255, 255)
        self.edgeColor = (200, 200, 200)
        self.nodeRadius = 1
        self.wireframes_colors = {}

        self.key_to_function = {
            pygame.K_LEFT: (lambda x: x.translateAll([-10, 0, 0])),
            pygame.K_RIGHT: (lambda x: x.translateAll([10, 0, 0])),
            pygame.K_DOWN: (lambda x: x.translateAll([0, 10, 0])),
            pygame.K_UP: (lambda x: x.translateAll([0, -10, 0])),
            pygame.K_EQUALS: (lambda x: x.scaleAll([1.25, 1.25, 1.25])),
            pygame.K_MINUS: (lambda x: x.scaleAll([0.8, 0.8, 0.8])),
            pygame.K_q: (lambda x: x.rotateAll('X', 0.1)),
            pygame.K_w: (lambda x: x.rotateAll('X', -0.1)),
            pygame.K_a: (lambda x: x.rotateAll('Y', 0.1)),
            pygame.K_s: (lambda x: x.rotateAll('Y', -0.1)),
            pygame.K_z: (lambda x: x.rotateAll('Z', 0.1)),
            pygame.K_x: (lambda x: x.rotateAll('Z', -0.1))
        }

    def node_colors(self, nodes):
        """ Calculate color gradient based on z coord of nodes """
        zmin, zmax = nodes[:, 2].min(), nodes[:, 2].max()
        start_color = np.array(self.background) + 5
        end_color = np.array(self.nodeColor)
        z = (nodes[:, 2] - zmin) / (zmax - zmin)
        # indexing [:, None] is used to explicitly state second axis
        c = (1 - z)[:, None] @ start_color[:, None].T + z[:, None] @ end_color[:, None].T
        return c

    def init_condition(self):
        for wr_name, wr in self.wireframes.items():
            colors = self.node_colors(wr.nodes)
            self.wireframes_colors[wr_name] = colors
        self.scaleAll([350, 350, 350])
        self.rotateAll('X', -.95)
        self.rotateAll('Y', -.35)

    def run(self):
        pygame.key.set_repeat(80)
        running = True
        self.init_condition()
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key in self.key_to_function:
                        self.key_to_function[event.key](self)
            self.display()
            pygame.display.flip()

    def display(self):
        """ Draw the wireframes on the screen. """

        self.screen.fill(self.background)

        for wr_name, wr in self.wireframes.items():
            if self.displayEdges:
                for n1, n2 in wr.edges:
                    start_pos = wr.nodes[n1][:2]
                    end_pos = wr.nodes[n2][:2]
                    pygame.draw.aaline(self.screen, self.edgeColor,
                                       start_pos,
                                       end_pos, 1)
            if self.displayNodes:
                for i, node in enumerate(wr.nodes):
                    pygame.draw.circle(self.screen, self.wireframes_colors[wr_name][i, :],
                                       (self.ws + int(node[0]), self.hs + int(node[1])), self.nodeRadius, 0)

    def addWireframe(self, name, wireframe):
        self.wireframes[name] = wireframe

    def translateAll(self, vals):
        for wr in self.wireframes.values():
            wr.transform(wireframe_np.translationMatrix(*vals))

    def rotateAll(self, axis, rad):
        if axis == 'X':
            rotFun = wireframe_np.rotateXMatrix
        elif axis == 'Y':
            rotFun = wireframe_np.rotateYMatrix
        else:
            rotFun = wireframe_np.rotateZMatrix
        for wr in self.wireframes.values():
            centre = wr.findCentre()[:-1]
            wr.transform(wireframe_np.translationMatrix(*-centre))
            wr.transform(rotFun(rad))
            wr.transform(wireframe_np.translationMatrix(*+centre))

    def scaleAll(self, scale):
        for wr in self.wireframes.values():
            centre = wr.findCentre()[:-1]
            wr.transform(wireframe_np.translationMatrix(*-centre))
            wr.transform(wireframe_np.scaleMatrix(*scale))
            wr.transform(wireframe_np.translationMatrix(*centre))


if __name__ == '__main__':
    # pv = ProjectionViewer(400, 300)
    # pv.addWireframe('cube', wireframe.Cube())
    # pv.run()
    pv = ProjectionViewerNP(400, 300)
    pv.addWireframe('cube', wireframe_np.Cube_np())
    cube2 = wireframe_np.Cube_np()
    cube2.transform(wireframe_np.translationMatrix(100, 100, 0))
    pv.addWireframe('cube2', cube2)
    pv.run()
