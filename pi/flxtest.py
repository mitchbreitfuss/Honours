'''Flexx test file'''
from flexx import flx
import piclient


class testclass():
    pass

class PiMonitor(flx.HFix):

    def init(self):
        with flx.VBox():   
            self.server1_button = flx.Button(text="Server 1")
            self.server2_button = flx.Button(text="Server 2")

        with flx.VBox():
            message_label = flx.Label(text="Message")
            data_label = flx.Label(text="Data")
            command_label = flx.Label(text="Command")
        self.test_label = flx.Label(text="")

        

  
    @flx.reaction('server1_button.pointer_click')
    def server1_clicked(self,*events):
        self.test_label.set_text("Clicked")
        piclient.serverConnect1()

    @flx.reaction('server2_button.pointer_click')
    def server2_clicked(self,*events):
        piclient.serverConnect2()
        







app = flx.App(PiMonitor)
app.launch('app')
flx.run()

