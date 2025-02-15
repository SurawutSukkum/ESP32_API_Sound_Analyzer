from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Allow requests from other devices

@app.route('/receive_data', methods=['POST'])
def receive_data():
    try:
        data = request.get_json()
        amplitude = data.get("amplitude")
        print(f"Received amplitude: {amplitude}")
        return jsonify({"status": "success", "received_amplitude": amplitude})

    except Exception as e:
        return jsonify({"error": str(e)}), 400

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
