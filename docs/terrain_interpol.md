Terrain can be interpolated using various methods:

1) Sin/Cos, probably smth like truncated Fourier series
`ol` demo's are maid using this approach.

2) Bezier surface. Causion, control points are *not* map points.
Moreover, here is quote from wiki that is highly relevant:
"""
However, Bézier patch meshes are difficult to render directly.
One problem with Bézier patches is that calculating their intersections with lines is difficult,
making them awkward for pure ray tracing or other direct geometric techniques which
do not use subdivision or successive approximation techniques.
They are also difficult to combine directly with perspective projection algorithms.

For this reason, Bézier patch meshes are in general eventually decomposed into meshes of flat triangles
by 3D rendering pipelines. In high-quality rendering, the subdivision is adjusted to be so fine that
the individual triangle boundaries cannot be seen.
To avoid a "blobby" look, fine detail is usually applied to Bézier surfaces
at this stage using texture maps, bump maps and other pixel shader techniques.

The curve in general does not pass through any of the control points except the first and last.

3) Lagrange polynomial

[This](https://forum.intra.42.fr/topics/1070/messages/last) guy expains the shortages
of Bezier surface approach (control points need to be calculated from map points) and
gives examples of lagrange interpolation:

https://www.wolframalpha.com/input/?i=plot+z%3Dx(x-1)y(y-1),+x%3D0..1+y%3D0..1
https://www.wolframalpha.com/input/?i=plot+z%3Dx(x-1)y(y-1)(+-1)

Overview of surface interpolation methods is located [here](http://m.dressler.sweb.cz/AOSIM.pdf).
All methods mentioned depend on *gridding* - rectangular grid is constructed to envelope map points, and for each point $(x, y)$ interpolation is built using vertices of the enclosing grid cell.