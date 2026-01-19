from flask import Flask, jsonify, render_template
import serial

app = Flask(__name__)

ser = serial.Serial("COM3", 115200, timeout=1)

latest_data = {
    "temperature": "-",
    "humidity": "-",
    "light": "-",
    "mode": "-",
    "level": "-"
}

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/data")
def get_data():
    global latest_data

    if ser.in_waiting:
        line = ser.readline().decode().strip()
        try:
            temp, hum, light, mode, level = line.split(",")
            latest_data["temperature"] = temp
            latest_data["humidity"] = hum
            latest_data["light"] = light
            latest_data["mode"] = mode
            latest_data["level"] = level
        except:
            pass

    return jsonify(latest_data)

@app.route("/control/<cmd>")
def control(cmd):
    if cmd == "auto":
        ser.write(b"MODE:AUTO\n")
    elif cmd == "manual":
        ser.write(b"MODE:MANUAL\n")
    elif cmd == "light_on":
        ser.write(b"LIGHT:ON\n")
    elif cmd == "light_off":
        ser.write(b"LIGHT:OFF\n")
    return "OK"

if __name__ == "__main__":
    app.run(debug=False)
