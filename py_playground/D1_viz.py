import numpy as np
import ipywidgets as widgets
import bqplot
from bqplot import pyplot as plt
from bqplot import Scatter, Axis


class D1_viz:
    def __init__(self, simulation, emitter):
        self.simulation = simulation
        self.hmap = simulation.hmap
        self.emitter = emitter

        self.coords = np.array([self.hmap.hmap[i].coords for i in range(self.hmap.n ** 2)])
        self.colors = ['green' if self.hmap.hmap[i].state == 'terrain' else 'steelblue' for i in range(self.hmap.n ** 2)]

        ax_x = Axis(label="x", scale=bqplot.LinearScale(min=0, max=2))
        ax_y = Axis(label="y", scale=bqplot.LinearScale(min=0, max=2), orientation="vertical", side="left")

        self.grid_scat = plt.scatter(x=self.coords[:, 0], y=self.coords[:, 1], colors=self.colors,
                                default_size=3, default_opacities=[.3], marker='rectangle')
        self.particles_scat = plt.scatter(x=[p.x for p in simulation.particles], y=[p.y for p in simulation.particles])

        self.fig = plt.Figure(marks=[self.grid_scat, self.particles_scat],
                              axes=[ax_x, ax_y], animation_duration=100,
                              padding_x=.05, padding_y=.05)

        self.out = widgets.Output()

        def on_value_change(change):
            t = change['new']
            if t == 1:
                self.simulation.reset()
                self.emitter.reset()
                self.hmap.reset()
                self.out.clear_output()
            self.particles_scat.x = [p.x for p in self.simulation.particles]
            self.particles_scat.y = [p.y for p in self.simulation.particles]
            self.simulation.update_particles()
            p = self.emitter.emit()
            if p:
                self.simulation.add_particle(p)

        self.slider = widgets.IntSlider(min=0, max=1000, step=1, continuous_update=True)
        self.play = widgets.Play(min=1, max=1000, interval=150)

        self.slider.observe(on_value_change, 'value')
        widgets.jslink((self.play, 'value'), (self.slider, 'value'))

    def run(self):
        return widgets.VBox([widgets.HBox([self.play, self.slider]), self.fig, self.out])

    def reset(self):
        self.simulation.reset()
        self.hmap.reset()
        self.emitter.reset()
