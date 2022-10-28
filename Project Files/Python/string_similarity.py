import textdistance
import numpy

catalog = {
    "six thirty two" : 0,
    "eight thirty two" : 1,
    "ten twenty four" : 2,
    "ten thirty two" : 3,
    "quarter twenty" : 4,
    "half twenty" : 5
}

commands = ["dispense", "return", "confirm"]

def matchCommand(command:str):
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

    token1 = s1.split(" ")
    token2 = s2.split(" ")

    return textdistance.jaccard(token1,token2)

def matchHardware(speech:str) -> int:
    # list of tuples (catalog_hardware, similarity)
    sims = [(key,sim(speech,key)) for key in catalog.keys()]    
    bestMatch = ""
    max_sim = -1
    for s in range(len(sims)):
        if (sims[s][1] > 0.5 and sims[s][1] > max_sim):
            max_sim = sims[s][1]
            bestMatch = sims[s][0]

    # if valid result
    return catalog[bestMatch] if max_sim != -1 and max_sim > 0.5 else -1


end = False
while (not end):
    speech = input("Enter tool command: ")
    
    # Interpret the starting command
    match = matchCommand(speech)
    if match[0] > .5:
        print(commands[match[1]], "Similarity = ", round(match[0],2))
        end = True
    else:
        print("Command failed, try again")
        continue

    # Interpret the catalog selection
    # Remove the first command word
    speech = speech.replace(speech.split()[0], "")
    print(matchHardware(speech))