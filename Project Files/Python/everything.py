import textdistance as td
import speech_recognition as sr

r = sr.Recognizer()
mic = sr.Microphone()

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

input()

with mic as source:
    r.adjust_for_ambient_noise(source)
    audio = r.listen(source)

text = r.recognize_google(audio)

trigger = "toolbox"

start = text.index(trigger)

