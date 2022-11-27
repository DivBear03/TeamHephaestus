from flask import Flask, request
import socket

hostname = socket.gethostname()
ip_address = socket.gethostbyname(hostname)
print(ip_address)
#Create the Flask app
app = Flask(__name__)
@app.route("/")

def home():
    args = request.args
    for thing in args:
        print(args[thing])
    return "success"

app.run(debug = False, host = ip_address, port = 5000)