import numpy as np
import wireframe
import ProjectionViewer as pview
import tools
from interpol import idw_mesh

if __name__ == '__main__':
    points = tools.rescale(tools.parse_file('../resources/demo1.mod1'), 0.1, 0.9)
    points[:, :-1] += 1  # to bias from the origin
    if len(points) == 1:
        points[:, 2] += 1
    points = np.vstack([points, tools.gen_borders(0, 2, 0, 2)])
    res = idw_mesh(points, 0, 2, 0, 2)

    wr = wireframe.Wireframe_np()
    wr.addNodes(res)

    pv = pview.ProjectionViewerNP(800, 800)
    pv.addWireframe('test', wr)
    pv.run()
