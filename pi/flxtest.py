'''Flexx test file'''
from flexx import flx
import piclient
import os
import time
try: import serial
except:
    pass

from tkinter import *


class PiMonitor(flx.HFix):

    def init(self):
        with flx.VBox():   
            self.server1_button = flx.Button(text="Server 1")
            self.server2_button = flx.Button(text="Server 2")


        message_label = flx.Label(text="Message")
        data_label = flx.Label(text="Data")
        command_label = flx.Label(text="Command")
        self.test_label = flx.Label(text="")

        

  
    @flx.reaction('server1_button.pointer_click')
    def server1_clicked(self,*events):
        self.test_label.set_text("Clicked")
        
        piclient.serverConnect1()
        
        







app = flx.App(PiMonitor)
app.launch('app')
flx.run()

