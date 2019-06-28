import os
import time
import serial
from tkinter import *

try:
    print(os.environ['DISPLAY'])
except(KeyError):
    print("No X Display running")
    HEADLESS = True
    exit()
# Starts a serial port on the Raspberry Pi UART pins
ser = serial.Serial(
    port='/dev/ttyS0',  # Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0.01
)

# Correstponds to how often to check the serial port for incoming bits.
# This is useful as the way the code is implimented using Tkinter allows process scheduling
refreshRate = 4


# This block pauses the program execution until the arduino is ready.
startup = 1
while startup == 0:
    x = ser.readline()
    decodedMessage = x.decode('utf-8')
    if("ESP Ready" in decodedMessage):
        startup = 1


# Frontend User program class.
# This class allows us to create a GUI application using tkinter.
class Application(Frame):

    AUTO_MODE = True

    # This method is responsible for communicating over the serial port with the arduino.
    # It also handles the data based on what kind of communication it is.
    # Currently there are 3 dataTypes, $D (data), $M (message), $C (command).
    # Data is for numbers relating to sensor readings or other quantitative data.
    # Messages are for general communications between the two devices, for debugging purposes.
    # Commands are for changing parameters on either device to change how it operates.
    
    
    def readSerial(self):
        x = ser.readline()  # Reads in a line from the serial port.

        # This if statement will only run if the read line x has non-zero length.
        if(len(x) != 0):
            # Converts from byte data to a utf-8 formatted string.
            while True:
                try:
                    decodedMessage = x.decode('utf-8')
                    break
                except UnicodeDecodeError:
                    decodedMessage = ''
                    #print("[Error] Decode Error from serial")
                    break
            if("\n" in decodedMessage):
                decodedMessage = decodedMessage.replace("\n", "")

            dataType = 0

            # The following block of code sorts the incoming message based on it's purpose.
            # All aspects of this project utilise the same syntax of using a $ sign followed by a letter to denote a communication's purpose.
            if("$D" in decodedMessage):
                decodedMessage = decodedMessage.replace("$D", "")
                dataType = "data"
            if("$M" in decodedMessage):
                decodedMessage = decodedMessage.replace("$M", "")
                dataType = "message"
            if("$C" in decodedMessage):
                decodedMessage = decodedMessage.replace("$C", "")
                dataType = "command"
            if("$L" in decodedMessage):
                decodedMessage = decodedMessage.replace("$L", "")
                dataType = "log"

            if(decodedMessage != ''):
                if(dataType == "log"):
                    self.displayMessage.delete(1.0, END)
                    self.displayMessage.insert(END, decodedMessage)
                    print("[Message] " + decodedMessage)
                    outfile = open("log.txt", "a")
                    outfile.write(decodedMessage + "    ")
                if(dataType == "message"):
                    self.displayMessage.delete(1.0, END)
                    self.displayMessage.insert(END, decodedMessage)
                    print("[Message] " + decodedMessage)

                if(dataType == "data"):
                    self.displayData.delete(1.0, END)
                    self.displayData.insert(END, decodedMessage)
                    print("[Data] " + decodedMessage)
                    outfile = open("log.txt", "a")
                    outfile.write(decodedMessage + "\n")
                    outfile.close()

        self.after(refreshRate, self.readSerial)
    # Method for sending a command from the text box to the arduino
    # Works in conjunction with retrieveInput.

    def sendCommand(self, command=None):

        if command == None:
            command = self.retrieveInput()
        if("\n" in command):
            command = command.replace("\n", "")
        print("Sending Command: ")
        if("$S" not in command):
            command = command + '$C'
        print(command)
        ser.write(bytes(command, "utf-8"))

    # Method responsible for getting user input from the commandBox tkinter widget.
    def retrieveInput(self):
        input = self.commandBox.get(1.0, END)
        self.commandBox.delete(1.0, END)
        return input

    # Placeholder method for testing connections to multiple wireless access points.
    def serverConnect1(self):
        print("Server 1")
        command = "1$S\n"
        self.sendCommand(command)

    # Placeholder method for testing connections to multiple wireless access points.
    def serverConnect2(self):
        print("Server 2")
        command = "2$S\n"
        self.sendCommand(command)

    # This method is responsible for creating all of the GUI through tkinter.
    # This includes the GUI functionality.
    def createWidgets(self):

          
          
##
        #self.main_window = Frame(self,height=480,width=640,bg='white')
        #self.main_window.grid(row=0,sticky=N+S+E+W)

        

        
        self.ser1Button = Button(self,text="Server 1",command=self.serverConnect1)
        self.ser1Button.grid(row=0,column=0)

        self.ser2Button = Button(self,text="Server 2",command=self.serverConnect2)
        self.ser2Button.grid(row=1,column=0)

        self.messageLabel = Label(self,text="Message")
        self.messageLabel.grid(row=0,column=2)

        self.displayMessage = Text(self,height=1,width=20)
        self.displayMessage.grid(row=0,column=3)

        self.dataLabel = Label(self,text="Data")
        self.dataLabel.grid(row=1,column=2)

        self.displayData = Text(self,height=1,width=20)
        self.displayData.grid(row=1,column=3)

        self.QUIT = Button(self,text = "QUIT",command=self.quit)
        self.QUIT.grid(row=0,column=4)

        self.commandLabel = Label(self,text="Command")
        self.commandLabel.grid(row=6,column=2)

        self.commandBox=Text(self,height=1,width=20)
        self.commandBox.grid(row=6,column=3)

        self.commandButton = Button(self,text="Send",command=self.sendCommand)
        self.commandButton.grid(row=6,column=4)
         
       #Old pack based GUI code
##        self.QUIT.pack({"side": "right"})
##
##        self.ser1Button = Button(self)
##        self.ser1Button["text"] = "Server 1"
##        self.ser1Button["command"] = self.serverConnect1
##        self.ser1Button.pack({"side": "left"})
##
##        self.ser2Button = Button(self)
##        self.ser2Button["text"] = "Server 2"
##        self.ser2Button["command"] = self.serverConnect2
##        self.ser2Button.pack({"side": "left"})
##
##        self.messageLabel = Label(self, text="Message\n\nData\n\nCommand")
##        self.messageLabel.pack({"side":"left"})
##
##        self.displayMessage = Text(self, height=2, width=36)
##        self.displayMessage.pack()
##
##        self.displayData = Text(self, height=2, width=36)
##        self.displayData.pack()
##
##        self.commandBox = Text(self, height=1, width=36)
##        self.commandBox.bind("<Return>")
##        self.commandBox.pack()
##
##        self.commandButton = Button(self)
##        self.commandButton["text"] = "Send"
##        self.commandButton["command"] = self.sendCommand
##        self.commandButton.pack()

    # Initialise function, runs at the startup of the GUI.
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.grid()
        self.createWidgets()
        
        
        #self.readSerial()
        #self.pack()


master = Tk()


app = Application()


app.mainloop()

master.destroy()
