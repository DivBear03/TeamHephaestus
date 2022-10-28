import textdistance

def sim(s1,s2):     
    while s1.__contains__("-"):
        s1 = s1.replace("-", " ")
        print("replaced1")
    while s2.__contains__("-"):
        s2 = s2.replace("-", " ")
        print("replaced2")
    token1 = s1.split(" ")
    token2 = s2.split(" ")

    print("Similarity: ", textdistance.jaccard(token1,token2))

s1 = "M4 half inch screw"
s2 = "M4 screw half-inch"
# print(s2.__contains__("-"))

sim(s1,s2)