import numpy as np
import wireframe
import ProjectionViewer as pview
import tools
from interpol import idw_mesh

if __name__ == '__main__':
    points = tools.center(tools.rescale(tools.parse_file('../resources/demo4.mod1')))
    print(points)
    points = np.vstack([points, tools.gen_borders(0, 2, 0, 2)])
    res = idw_mesh(points, 0, 2, 0, 2, 100)

    wr = wireframe.Wireframe()
    wr.addNodes(res)

    pv = pview.ProjectionViewerNP(800, 800)
    pv.addWireframe(wr)
    pv.run()
