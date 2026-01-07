from flask import Flask, jsonify
import serial

app=Falsk(__name__)

ser=serial.Serial("COM3",9600,timeout=1)

latest_data={
    "temperature":None,
    "humidity":None,
    "status":None
}

@app.route("/data")
def get_data():
    if ser.in_waiting:
        line=ser.readline().decode().strip()

        if line!="ERROR":
            temp,hum,status=line.split(",")
            latest_data["temperature"]=temp
            latest_data["humidity"]=hum
            latest_data["status"]=status

        return jsonify(latest_data)

    if __name__=="__main":
        app.run(debug=True)