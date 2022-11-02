import textdistance as td
import speech_recognition as sr
import serial
import numpy
import textdistance

# global variable declarations
global r, mic, hardware, commands, audio
r = sr.Recognizer()
mic = sr.Microphone()
# cataliog
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
commands = ["dispense", "return", "confirm"]

def matchCommand(command:str):
    global commands
    sims = [sim(command.split()[0],word) for word in commands]
    return [max(sims), numpy.argmax(sims)]

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

    return textdistance.jaccard(token1,token2)

def matchHardware(speech) -> int:
    global hardware
    # list of tuples (catalog_hardware, similarity)
    sims = [(key,sim(speech,key)) for key in hardware.keys()]  
    bestMatch = ""
    max_sim = -1
    for s in range(len(sims)):
        if (sims[s][1] > 0.5 and sims[s][1] > max_sim):
            max_sim = sims[s][1]
            bestMatch = sims[s][0]

    # if valid result
    return hardware[bestMatch] if max_sim != -1 and max_sim > 0.5 else -1

def analyze(command):
    match = matchCommand(command)
    if match[0] > .5:
        print("Command Found:", commands[match[1]], "Similarity = ", round(match[0],2))
        command = command.replace(command.split()[0], "")
        command = command.rstrip().lstrip()
        print(matchHardware(command))
    else:
        print("Command failed, try again")

def Record():
    global audio, r, mic
    with mic as source:
        r.adjust_for_ambient_noise(source)
        print("adjusted, listening now")
        audio = r.listen(source, 2)
    print("done listening")
    analyze(r.recognize_google(audio))

def listenStart():
    # portvalue = serial.Serial("COM2", 9600, timeout = 1)
    # *********
    portvalue = 1
    # *********
    if portvalue == 1:
        print("recording")
        Record()

def main():
    listenStart()

main()