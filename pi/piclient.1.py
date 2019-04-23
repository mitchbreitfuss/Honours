"""
Demo server.
"""

from flexx import flx
from flexx import app

import monitor


class Demo(flx.Widget):

    def init(self):
        with flx.TabLayout():
            monitor()
            

if __name__ == '__main__':
    # This example is setup as a server app
    # app.serve(Monitor)
    # app.serve(ChatRoom)
    # app.serve(ColabPainting)
    # app.serve(CollisionDemo)
    # # app.serve(PlotlyGeoDemo)  # CORS fail?
    # app.serve(Demo)
    app = flx.App(Demo)
    app.launch('app')
    flx.start()