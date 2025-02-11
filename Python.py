from flask import Flask, request

app = Flask(__name__)

@app.route('/audio', methods=['POST'])
def receive_audio():
    with open("received_audio.raw", "ab") as f:
        f.write(request.data)
    return "Received", 200

if __name__ == "__main__":
        app.run(host='127.0.0.1', port=3001)
