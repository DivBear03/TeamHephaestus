from flask import Flask, request
import socket
import time
import serial
import speech_recognition as sr
from textdistance import ratcliff_obershelp as rat_ober
# import numpy

r = sr.Recognizer()
mic = sr.Microphone()
serial_port  = "COM_257"
COMfound = False
start_time = time.time()
while not COMfound and time.time() - start_time < 15:
    for i in range(256):
        port = "COM" + str(i)
        print(port)
        try:
            serial_port = serial.Serial(port, 9600, timeout = 1)
            
        # Copy-pasta for personal hotspot
        # ;SSID:esben,esbenhotspot][172.20.10.5;
            print(serial_port.portstr)
            COMfound = True
            time.sleep(1)
            break
        except:
            pass
        time.sleep(0.01)

if serial_port == "COM_257":
    print("No COM port found")
    quit()

hardware = {
    "M5 bolt":1, "M5 nut":2,
    "M6 bolt":3, "M6 nut":4,
    "M8 bolt":5, "M8 nut":6,
    "M10 bolt":7, "M10 nut":8,
    "M12 bolt":9, "M12 nut":10,
    "M16 bolt":11, "M16 nut":12,
    "M20 bolt":13, "M20 nut":14,
    "M24 bolt":15, "M24 nut":16,
}

commands = ["dispense", "return"]

def matchHardware(gtext) -> int:
    # list of tuples (catalog_hardware, similarity)
    print("Removed Command:", gtext)
    sims = [(key,sim(gtext,key)) for key in hardware.keys()]  
    print(sims)
    bestMatch = ""
    max_sim = -1
    for s in range(len(sims)):
        if (sims[s][1] > 0.5 and sims[s][1] > max_sim):
            max_sim = sims[s][1]
            bestMatch = sims[s][0]
    print("String Comparison Result:", max_sim, bestMatch)
    if max_sim < 0.5:
        print("Sent &M-1|")
        writeSerial("&M-1|")  # Invalid hardware
    else:
        print("Sent &M" + str(hardware[bestMatch]) + "|")
        writeSerial("&M" + str(hardware[bestMatch]) + "|")   # match found

def sim(s1:str,s2:str) -> float:  
    # remove all spaces and dashes  
    while s1.__contains__("-"):
        s1 = s1.replace("-", " ")
        print("replaced1")

    while s2.__contains__("-"):
        s2 = s2.replace("-", " ")
        print("replaced2")

    s1 = s1.lower()
    s2 = s2.lower()

    token1 = s1.split(" ")
    token2 = s2.split(" ")

    return rat_ober(s1,s2)

def writeSerial(input: str):
    serial_port.write((input+"\r\n").encode('utf-8'))

def Record():
    # record audio through microphone
    print("Recording")
    with mic as source:
        # r.adjust_for_ambient_noise(source)
        print("listening now")
        audio = r.listen(source)
    print("analyzing")
    
    # send to google API
    gtext = r.recognize_google(audio)
    print("Google Result:", gtext)
    
    # compare first word of resulting string to command list
    firstWord = gtext.split()[0]
    print(firstWord)
    if firstWord != "dispense" and firstWord != "return":
        print("Invalid command")
        writeSerial("&M0|") # invalid command
    else:
        gtext = gtext.replace(gtext.split()[0], "")    # replace first word
        matchHardware(gtext)

hostname = socket.gethostname()
ip_address = socket.gethostbyname(hostname)
print(ip_address)
#Create the Flask app
app = Flask(__name__)
@app.route("/")

def home():
    args = request.args
    command = args["P"]
    if command == 1:
        Record()
    return "success"

app.run(debug = False, host = ip_address, port = 5000)