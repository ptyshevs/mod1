import numpy as np
from interpol import Estimation
from tools import parse_file, rescale, center, gen_borders
from height_map import height_to_nodes, height_to_edges, height_map, HeightMap
from WaterSimulator import WaterSimulator
from wireframe import Wireframe
from emitter import Emitter

if __name__ == '__main__':

    cmin = 0
    cmax = 2
    n_points = 100  # very slow
    n_points = 40

    points = center(rescale(parse_file('../resources/demo1.mod1')))
    points = np.vstack([points, gen_borders(cmin, cmax, cmin, cmax)])

    hmap = HeightMap(points, n_points, cmin, cmax)
    nodes = height_to_nodes(hmap.hmap)
    edges = height_to_edges(hmap.hmap)

    wr = Wireframe()
    wr.addNodes(nodes)
    wr.addEdges(edges)
    ws = WaterSimulator(800, 800)
    ws.addEmitter(Emitter(cmin, cmax, 1))
    ws.addWireframe(wr)
    ws.addHeightMap(hmap)
    ws.cube_colors(hmap.hmap)
    ws.run()
