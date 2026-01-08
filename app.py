from flask import Flask, jsonify, render_template
import serial

app=Flask(__name__)

ser=None

latest_data={
    "temperature":"N/A",
    "humidity":"N/A",
    "status":"UNKNOWN"
}

def init_serial():
    global ser
    try:
        ser=serial.Serial("COM3",9600,timeout=1)
        print("Serial connected")
    except Exception as e:
        print("Serial error:",e)
        ser=None

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/data")
def get_data():
    global ser

    if ser is None:
        init_serial()

    if ser and ser.in_waiting:

        try:
            line=ser.readline().decode().strip()
            print("RAW",line)

            if line!="ERROR":
                temp,hum,status=line.split(",")
                latest_data["temperature"]=temp
                latest_data["humidity"]=hum
                latest_data["status"]=status

        except:
            pass
            

        return jsonify(latest_data)

if __name__=="__main__":
    app.run(debug=True)