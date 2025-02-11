from flask import Flask, jsonify
from flask_cors import CORS
import numpy as np
import time

app = Flask(__name__)
CORS(app)  # Allow cross-origin requests

@app.route('/audio', methods=['GET'])
def generate_sine_wave():
    frequency = 5  # Hz
    duration = 1   # seconds
    sample_rate = 100  # samples per second

    t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)
    sine_wave = (100 * np.sin(2 * np.pi * frequency * t)).tolist()

    return jsonify({"time": t.tolist(), "amplitude": sine_wave})

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
