from flask import Flask, jsonify, render_template
import serial

app=Flask(__name__)

ser=serial.Serial("COM3",9600,timeout=1)


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
        line=ser.readline().decode().strip()

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

if __name__=="__main__":
    app.run(debug=False)