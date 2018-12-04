import math


class Node:
    def __init__(self, coordinates):
        self.x = coordinates[0]
        self.y = coordinates[1]
        self.z = coordinates[2]


class Edge:
    def __init__(self, start, stop):
        self.start = start
        self.stop = stop


class Wireframe:
    def __init__(self):
        self.nodes = []
        self.edges = []

    def addNodes(self, nodeList):
        for node in nodeList:
            self.nodes.append(Node(node))

    def addEdges(self, edgeList):
        for (start, stop) in edgeList:
            self.edges.append(Edge(self.nodes[start], self.nodes[stop]))

    def outputNodes(self):
        print("------- Nodes --------")
        for i, node in enumerate(self.nodes):
            print(f"{i}: ({node.x:.2f}, {node.y:.2f}, {node.z:.2f})")

    def outputEdges(self):
        print("------- Edges --------")
        for i, edge in enumerate(self.edges):
            start, stop = edge.start, edge.stop
            print(f"{i}: ({start.x:.2f}, {start.y:.2f}, {start.z:.2f}) ->"
                  f" ({stop.x:.2f}, {stop.y:.2f}, {stop.z:.2f})")

    def translate(self, axis, d):
        """ Translate each node of a wireframe by d along a given axis """
        if axis in ['x', 'y', 'z']:
            for node in self.nodes:
                setattr(node, axis, getattr(node, axis) + d)

    def scale(self, centre, scale):
        """ Scale the wireframe from the centre of the screen. """
        x, y = centre
        for node in self.nodes:
            node.x = x + scale * (node.x - x)
            node.y = y + scale * (node.y - y)
            node.z *= scale

    def findCentre(self):
        num_nodes = len(self.nodes)
        meanX = sum([node.x for node in self.nodes]) / num_nodes
        meanY = sum([node.y for node in self.nodes]) / num_nodes
        meanZ = sum([node.z for node in self.nodes]) / num_nodes
        return meanX, meanY, meanZ

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


class Cube(Wireframe):
    def __init__(self):
        super().__init__()
        self.addNodes([(x, y, z) for x in (0, 1) for y in (0, 1) for z in (0, 1)])
        self.addEdges([(n, n + 4) for n in range(0, 4)])
        self.addEdges([(n, n + 1) for n in range(0, 8, 2)])
        self.addEdges([(n, n + 2) for n in (0, 1, 4, 5)])
