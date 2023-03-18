#
# Alec Felix
# ENEE 459v
# Runs a python TCP server that recieves data within messagepack format from B-L475_IOT01A1 client
# Parses data and updates Tkinter User interface which displays the values
#
# At home settings: "192.168.1.219" PORT = 7000
# At school settings: "10.106.60.243" PORT 7000
#
#
# ----------------- TO RUN PROGRAM ----------------------
# Set bind_ip to your wifi adapters lan ip address
# leave bind_port alone unless an application uses that port
# RUN using IDLE or command line
# ================================================================
import time, threading
from tkinter import *
from tkinter import messagebox
import select
import socket
from threading import *
import umsgpack

# ----- GLOBAL VARS -----------

bind_ip = "192.168.1.43" 
bind_port = 7000
degree_sign = u'\N{DEGREE SIGN}'

# ------------------
# server_process()
# 
# server_process handles
# server logic / parsing
# runs in a seperate thread from tkinter
# -----------------
def server_process():
    print("server started")
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((bind_ip, bind_port))
    server_socket.listen(5)
    print ("[*] Listening on %s:%d" % (bind_ip, bind_port))
    rlist= [server_socket]    # ref #2
    wlist= [server_socket]
    xlist= [server_socket]
    while True:
        #try:
            readable, writable, errored = select.select(rlist,wlist,xlist,1.0)              # wmh : this will wait 1.0 second each time it runs
            if not readable:
                print ("[*] No connection yet")                                             #  we'll go down and check for the keyboard interrupt
            else:
                client, addr = server_socket.accept()                                       # wmh: would block here except 'select()' has already checked
                print ("[*] Accepted connection from: %s:%d" % (addr[0], addr[1]))
                # activate the function to handle the data from client
                client_handler = threading.Thread(target = handle_client, args=(client,))   # wmh: probably forks new thread here to run handle_client()
                client_handler.start()                                                      # wmh: returns here after client_handler (née handle_client()) closes socket

# ------------------
# handle_client(client_socket)
# 
# server_process handles
# This is the thread
# we use to handle data from the B-L475_IOT01A1 client
# -----------------
def handle_client(client_socket):
    # send data immediately on connection (quirk in B-L475_IOT01A1 code requires trhis)
    client_socket.send(b'<HTML><BODY><H1>Simple TCP Server : sorry, nothing here</H1></BODY></HTML>\r\n')
    # show the data received back from the client
    request = client_socket.recv(1024)
    print ("[*] Received: %s" % request)
    try:
        obj = umsgpack.unpackb(request)
        parse_msgPack(obj)
        #print ("[*] Unpacked: %s" % obj)
    except Exception:
        print ("Could not unpack")
    client_socket.close()

# ------------------
# parse_msgPack(obj)
# 
# parses the msg pack and
# update the variables
# used with tkinter's GUI
# -----------------
def parse_msgPack(obj):
    print ("[*] Parsing: %s" % obj)
    if obj["lbl"] == "temp":
        v = obj["val"] # a float
        tempVal.set(str(v))
        tempValView.update()
    elif obj["lbl"] == "humid":
        v = obj["val"] # a float
        humidVal.set(str(v))
        humidValView.update()
    elif obj["lbl"] == "press":
        v = obj["val"] # a float
        pressVal.set(str(v))
        pressValView.update()
    elif obj["lbl"] == "mag":
        v1 = obj["val1"] # a int
        v2 = obj["val2"] # a int
        v3 = obj["val3"] # a int
        magVal1.set(str(v1))
        magVal2.set(str(v1))
        magVal3.set(str(v1))
        magVal1View.update()
        magVal2View.update()
        magVal3View.update()
    elif obj["lbl"] == "gyro":
        v1 = obj["val1"] # a int
        v2 = obj["val2"] # a int
        v3 = obj["val3"] # a int
        gyroVal1.set(str(v1))
        gyroVal2.set(str(v1))
        gyroVal3.set(str(v1))
        gyroVal1View.update()
        gyroVal2View.update()
        gyroVal3View.update()
    elif obj["lbl"] == "accel":
        v1 = obj["val1"] # a int
        v2 = obj["val2"] # a int
        v3 = obj["val3"] # a int
        accelVal1.set(str(v1))
        accelVal2.set(str(v1))
        accelVal3.set(str(v1))
        accelVal1View.update()
        accelVal2View.update()
        accelVal3View.update()
    else:
        print("Could not parse msgpack")

# -------------- Setup GUI --------------------
# define the main window
mainWindow = Tk()
mainWindow.geometry("800x600")
mainWindow.title("HW11 Sensor Display")

# identify variables being displayed
# label of each sensor
pressText=StringVar()
tempText=StringVar()
humidText=StringVar()
accelText=StringVar()
gyroText=StringVar()
magText=StringVar()

#units for each sensor
pressUnits=StringVar()
tempUnits=StringVar()
humidUnits=StringVar()
accelUnits=StringVar()
gyroUnits=StringVar()
magUnits=StringVar()

# value of each sensor
pressVal=StringVar()
tempVal=StringVar()
humidVal=StringVar()

magVal1=StringVar()
magVal2=StringVar()
magVal3=StringVar()

gyroVal1=StringVar()
gyroVal2=StringVar()
gyroVal3=StringVar()

accelVal1=StringVar()
accelVal2=StringVar()
accelVal3=StringVar()

# initialize display values (unnecessary?)
pressText.set("Press")
tempText.set("Temp")
humidText.set("Humid")
magText.set("Mag")
accelText.set("Accel")
gyroText.set("Gyro")


pressUnits.set("mBar")
tempUnits.set(degree_sign + "C")
humidUnits.set("%")
accelUnits.set("XYZ")
gyroUnits.set("XYZ")
magUnits.set("XYZ")

pressVal.set("NIL")
tempVal.set("NIL")
humidVal.set("NIL")
magVal1.set("NIL")
magVal2.set("NIL")
magVal3.set("NIL")
gyroVal1.set("NIL")
gyroVal2.set("NIL")
gyroVal3.set("NIL")
accelVal1.set("NIL")
accelVal2.set("NIL")
accelVal3.set("NIL")

# configure display locations in main window

#press label display
pressTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=pressText) 
pressTextView.place(x=80,y=20)
# press val display
pressValView = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=pressVal) 
pressValView.place(x=170,y=20)
# units view
pressUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=pressUnits) 
pressUnitsView.place(x=320,y=20)


#Temp label view
tempTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=tempText) 
tempTextView.place(x=80,y=70)
# temp val view
tempValView = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=tempVal) 
tempValView.place(x=170,y=70)
# units view
tempUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=tempUnits) 
tempUnitsView.place(x=320,y=70)

#humid text
humidTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=humidText) 
humidTextView.place(x=80,y=120)
#humid val
humidValView = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=humidVal)
humidValView.place(x=170,y=120)
# units view
humidUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=humidUnits) 
humidUnitsView.place(x=320,y=120)

#mag text
magTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=magText) 
magTextView.place(x=80,y=170)
#mag val1
magVal1View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=magVal1)
magVal1View.place(x=170,y=170)
#mag val2
magVal2View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=magVal2)
magVal2View.place(x=320,y=170)
#mag val3
magVal3View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=magVal3)
magVal3View.place(x=470,y=170)
# units view
magUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=magUnits) 
magUnitsView.place(x=620,y=170)

#gyro text
gyroTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=gyroText) 
gyroTextView.place(x=80,y=220)
#gyro val1
gyroVal1View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=gyroVal1)
gyroVal1View.place(x=170,y=220)
#gyro val2
gyroVal2View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=gyroVal2)
gyroVal2View.place(x=320,y=220)
#gyro val3
gyroVal3View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=gyroVal3)
gyroVal3View.place(x=470,y=220)
# units view
gyroUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=gyroUnits) 
gyroUnitsView.place(x=620,y=220)

#accel text
accelTextView = Entry(mainWindow, width=6, font=("Arial",18,""),textvariable=accelText) 
accelTextView.place(x=80,y=270)
#accel val1
accelVal1View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=accelVal1)
accelVal1View.place(x=170,y=270)
#accel val2
accelVal2View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=accelVal2)
accelVal2View.place(x=320,y=270)
#accel val3
accelVal3View = Entry(mainWindow, width=10, font=("Arial",18,""),textvariable=accelVal3)
accelVal3View.place(x=470,y=270)
# units view
accelUnitsView = Entry(mainWindow, width=5, font=("Arial",18,""),textvariable=accelUnits) 
accelUnitsView.place(x=620,y=270)

# start server thread
serverThread = Thread(target=server_process)
serverThread.start()

# start tkinter GUI loop
mainWindow.mainloop()


