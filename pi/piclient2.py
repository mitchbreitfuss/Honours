import os
import time
import datetime
import csv
SERIAL_AVAILABLE = True
try: import serial
except:
    SERIAL_AVAILABLE = False
from tkinter import *
from flexx import flx



# try:
#     print(os.environ['DISPLAY'])
# except(KeyError):
#     print("No X Display running")
#     HEADLESS = True
#     exit()
# Starts a serial port on the Raspberry Pi UART pins
if(SERIAL_AVAILABLE == True):
    ser = serial.Serial(
        port='/dev/ttyS0',  # Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate=9600,
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


def sendCommand(command=None):
# Method for sending a command from the text box to the arduino
# Works in conjunction with retrieveInput.
    if command == None:
        command = retrieveInput()
    if("\n" in command):
        command = command.replace("\n", "")
    print("Sending Command: ")
    if("$S" not in command):
        command = command + '$C'
    print(command)
    ser.write(bytes(command, "utf-8"))

# Method responsible for getting user input from the commandBox tkinter widget.
def retrieveInput():
    input = "input text"
    return input

# Placeholder method for testing connections to multiple wireless access points.
def serverConnect1():
    print("Server 1")
    command = "1$S\n"
    sendCommand(command)

# Placeholder method for testing connections to multiple wireless access points.
def serverConnect2():
    print("Server 2")
    command = "2$S\n"
    sendCommand(command)

# Function to recieve commands when running the program in headless mode. (WIP)
def CLI():
    while True:
        input = input("Enter command to run: ")


# Insert a command dictionary here to allow running commands from the commandline.
# Considder using a shared header file between the Pi and Arduinos so when new functionality is added it is simpler to impliment.
# COMMAND   CODE
# Connect1  1$S
# Connect2  2$S
# Log Data  100$C


def logData():
    sendCommand(command='100$S')



# GUI Object, using Tkinter.
class Application(Frame):

    AUTO_MODE = True

        # Initialise function, runs at the startup of the GUI.
    def __init__(self, parent=None):
        
        Frame.__init__(self, parent)
        self.grid()
        self.createWidgets()
        if(SERIAL_AVAILABLE==True):
            self.readSerial()
    def flexxcall(self):
        import flxtest as graphApp
        graphApp.readCSVData()
        app = flx.App(graphApp.TempGraph)
        app.launch('app')
        flx.run()
    
    if(SERIAL_AVAILABLE == True):
        def readSerial(self):
            
            now = datetime.datetime.now()
            if(now.hour % 1 == 0 and now.minute == 0 and now.second == 0):
                logData()
                time.sleep(1)

                # This method is responsible for communicating over the serial port with the arduino.
            # It also handles the data based on what kind of communication it is.
            # Currently there are 3 dataTypes, $D (data), $M (message), $C (command).
            # Data is for numbers relating to sensor readings or other quantitative data.
            # Messages are for general communications between the two devices, for debugging purposes.
            # Commands are for changing parameters on either device to change how it operates.
            x = ser.readline()  # Reads in a line from the serial port.
            print(x)
            WRITE_DATETIME=0
            
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
                if("$L1" in decodedMessage):
                    
                    decodedMessage = decodedMessage.replace("$L1", "")
                    dataType = "INVERTERLOG"
                if("$L2" in decodedMessage):
                    decodedMessage = decodedMessage.replace("$L2", "")
                    dataType = "ELECTROLYSERLOG"
                print(decodedMessage)

                if(decodedMessage=="101.00"):
                    print("Writing Date and Time")
                    WRITE_DATETIME = 1
                    Frame.RECIEVING = 1
                    
                if(decodedMessage=="201.00"):
                    print("recieving electrolyser")
                    
                    WRITE_DATETIME = 1
                    Frame.RECIEVING = 1

                if(WRITE_DATETIME == 1):
                    if(decodedMessage == "101.00"):
                        outfile = open("inverter.csv", "a")
                    if(decodedMessage == "201.00"):
                        print("Opening Electrolyser CSV")
                        outfile = open("electrolyser.csv","a")
                    now = datetime.datetime.now()
                    date = str(now.year) + "_" + str(now.month) + "_" + str(now.day)
                    if(len(str(now.minute))== 1):
                        time = str(now.hour) + "0" + str(now.minute)
                    else:
                        time = str(now.hour) + str(now.minute)
                    outfile.write("\n"+date+ ",")
                    outfile.write(time + ",")
                    WRITE_DATETIME = 0
                    outfile.close()
                    decodedMessage = ""

                if(decodedMessage=="102.00" or decodedMessage == "202.00"):
                    Frame.RECIEVING = 0

                if(decodedMessage != ''):
                    if(dataType == "INVERTERLOG"):
                        print("RECIEVING_INVERTER = " + str(Frame.RECIEVING))
                        if(Frame.RECIEVING ==1):
                            print("Writing to CSV")
                            outfile = open("inverter.csv", "a")
                            outfile.write(decodedMessage + ",")


                        
                    if(dataType == "ELECTROLYSERLOG"):
                        print("RECIEVING_ELECTROLYSER = " + str(Frame.RECIEVING))
                        if(Frame.RECIEVING ==1):
                            print("Writing to CSV")
                            outfile = open("electrolyser.csv", "a")
                            outfile.write(decodedMessage + ",")

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

    # This method is responsible for creating all of the GUI through tkinter.
    # This includes the GUI functionality.
    def createWidgets(self):
        # Below the GUI elements are created  
        self.winfo_toplevel().title("Pi Client")
        # Server 1 Connect Button  
        self.ser1Button = Button(self,text="Server 1",command=serverConnect1)
        self.ser1Button.grid(row=0,column=0)
        
        # Server 2 Connect Button
        self.ser2Button = Button(self,text="Server 2",command=serverConnect2)
        self.ser2Button.grid(row=1,column=0)

        # Message Box Label
        self.messageLabel = Label(self,text="Message")
        self.messageLabel.grid(row=0,column=2)

        # Message Text Box
        self.displayMessage = Text(self,height=1,width=20)
        self.displayMessage.grid(row=0,column=3)

        # Data Box Label
        self.dataLabel = Label(self,text="Data")
        self.dataLabel.grid(row=1,column=2)

        # Data Text Box
        self.displayData = Text(self,height=1,width=20)
        self.displayData.grid(row=1,column=3)

        # Quit Button
        self.QUIT = Button(self,text = "QUIT",command=self.quit,fg='red')
        self.QUIT.grid(row=0,column=4)

        # Command Box Label
        self.commandLabel = Label(self,text="Command")
        self.commandLabel.grid(row=6,column=2)

        # Command Text Box
        self.commandBox=Text(self,height=1,width=20)
        self.commandBox.grid(row=6,column=3)

        # Command Send Button
        self.commandButton = Button(self,text="Send",command=sendCommand)
        self.commandButton.grid(row=6,column=4)
        self.master.bind("<Return>",sendCommand) # Bind return key to send the command for better UX
        
        # Flexx Button
        self.flxButton = Button(self,text="Flexx",command=self.flexxcall)
        self.flxButton.grid(row=6,column=6)
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


def start_app():
    # Initialise tkinter.
    master = Tk()

    # Set Application object to variable app.
    app = Application(master)



    # Runs the application loop. (Tkinter)
    master.mainloop()

    # Garbage collection for when the mainloop is stopped.
    #master.destroy()





start_app()
