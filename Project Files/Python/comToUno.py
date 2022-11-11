import serial
import pyaudio
import speech_recognition as sr
import textdistance as td
# import numpy

r = sr.Recognizer()
mic = sr.Microphone()

for i in range(256):
    port = "COM" + str(i)
    try:
        serial_port = serial.Serial(port, 9600)
        print("port found: " + port)
        break
    except:
        continue

if (port == "COM255"):
    print("No serial port found")
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
    "M30 bolt":17, "M30 nut":18
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
        print("Sent &M" + str(hardware[bestMatch] + "|"))
        writeSerial("&M" + str(hardware[bestMatch] + "|"))    # match found

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

    return td.jaccard(token1,token2)

def writeSerial(input: str):
    serial_port.write((input+"\r\n").encode('utf-8'))

def Record():
    # record audio through microphone
    print("Recording")
    with mic as source:
        r.adjust_for_ambient_noise(source)
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

def main():
    print("Listening on serial")
    while (True):
        line = str(serial_port.readline())
        if (line.__contains__("P")):
            print(line)
            command = int(line[line.index("P")+1])
            print("Command:", command)
            if command == 1:
                Record()
            elif command  == 2:
                break
    main()
main()