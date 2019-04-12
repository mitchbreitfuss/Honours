import serial

ser1 = serial.Serial('/dev/ttyUSB0',115200)
va1 = "test"
ser1.write('sde\n'.encode())

check = 1
temp = ""

while check == 1:
    var = (ser1.readline())
    print(var)#decode('utf-8'))
    if var == b'html-start\r\n':
        check = 2
        var = ""
        
    
while check == 2:
    temp = ser1.readline()
    if temp == b'html-stop\r\n':
        check = 3
        print("VAR =:")
        print(var)
        break
    
    var = var + temp.decode('utf-8')
    #print(var)


outfile = open("web.html", "w")
outfile.write(var)
outfile.close()