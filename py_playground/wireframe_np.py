import numpy as np
import math


class Wireframe_np:
    def __init__(self):
        self.nodes = np.zeros((0, 4))
        self.edges = []

    def addEdges(self, edgeList):
        self.edges += edgeList

    def addNodes(self, node_array):
        ones_col = np.ones((len(node_array), 1))

        ones_added = np.hstack((node_array, ones_col))
        self.nodes = np.vstack((self.nodes, ones_added))

    def outputNodes(self):
        print("---- Nodes ----")
        for i, (x, y, z, _) in enumerate(self.nodes):
            print(f"{i}: ({x}, {y}, {z})")

    def outputEdges(self):
        print("---- Edges ----")
        for i, (frm, to) in enumerate(self.edges):
            print(f"{i}: {frm} -> {to}")

    def transform(self, matrix):
        self.nodes = self.nodes @ matrix

    def translate(self, axis, d):
        """ Translate each node of a wireframe by d along a given axis """
        if axis in ['x', 'y', 'z']:
            col = 0 if axis == 'x' else (1 if axis == 'y' else 2)
            self.nodes[:, col] += d

    def scale(self, centre, scale):
        """ Scale the wireframe from the centre of the screen. """
        x, y = centre
        for node in self.nodes:
            node.x = x + scale * (node.x - x)
            node.y = y + scale * (node.y - y)
            node.z *= scale

    def findCentre(self):
        return np.mean(self.nodes, axis=0)

    def rotateZ(self, centre, radians):
        cx, cy, cz = centre
        for node in self.nodes:
            x = node.x - cx
            y = node.y - cy
            d = math.hypot(x, y)
            theta = math.atan2(y, x) + radians
            node.x = cx + d * math.cos(theta)
            node.y = cy + d * math.sin(theta)

    def rotateX(self, centre, radians):
        cx, cy, cz = centre
        for node in self.nodes:
            y = node.y - cy
            z = node.z - cz
            d = math.hypot(z, y)
            theta = math.atan2(y, z) + radians
            node.z = cz + d * math.cos(theta)
            node.y = cy + d * math.sin(theta)

    def rotateY(self, centre, radians):
        cx, cy, cz = centre
        for node in self.nodes:
            x = node.x - cx
            z = node.z - cz
            d = math.hypot(x, z)
            theta = math.atan2(x, z) + radians
            node.z = cz + d * math.cos(theta)
            node.x = cx + d * math.sin(theta)

def translationMatrix(dx=0, dy=0, dz=0):
    r = np.eye(4)
    r[3, :-1] = dx, dy, dz
    return r

def scaleMatrix(sx=0, sy=0, sz=0):
    r = np.eye(4)
    r[0, 0] = sx
    r[1, 1] = sy
    r[2, 2] = sz
    return r

def rotateXMatrix(radians):
    c = np.cos(radians)
    s = np.sin(radians)
    return np.array([[1, 0, 0, 0],
                     [0, c, -s, 0],
                     [0, s, c, 0],
                     [0, 0, 0, 1]])

def rotateYMatrix(radians):
    c = np.cos(radians)
    s = np.sin(radians)
    return np.array([[c, 0, s, 0],
                     [0, 1, 0, 0],
                     [-s, 0, c, 0],
                     [0, 0, 0, 1]])

def rotateZMatrix(radians):
    c = np.cos(radians)
    s = np.sin(radians)
    return np.array([[c, -s, 0, 0],
                     [s, c, 0, 0],
                     [0, 0, 1, 0],
                     [0, 0, 0, 1]])


class Cube_np(Wireframe_np):
    def __init__(self):
        super().__init__()
        nodes = [(x, y, z) for x in (0, 1) for y in (0, 1) for z in (0, 1)]
        self.addNodes(np.array(nodes))
        self.addEdges([(n, n + 4) for n in range(4)])
        self.addEdges([(n, n + 1) for n in range(0, 8, 2)])
        self.addEdges([(n, n + 2) for n in (0, 1, 4, 5)])
