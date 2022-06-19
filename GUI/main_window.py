from PyQt5 import QtWidgets
import mysql.connector

from serial_ports import serial_ports
from typing import Union
import serial
import design

SPEEDS = ['1200', '2400', '4800', '9600', '19200', '38400', '57600', '115200']


class MainWindow(QtWidgets.QMainWindow, design.Ui_Project):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.port: Union[serial.Serial, None] = None
        self.init_elements()
        self.refreshButton.clicked.connect(self.on_click_refresh)

    def on_click_refresh(self):
        data = str(self.port.readlines()[-1])
        temperature = data[2:7]
        humidity = data[7:12]
        next_watering_hours = data[12:14]
        next_watering_minutes = data[14:16]
        next_watering_secs = data[16:18]
        measurement_hours = data[18:20]
        measurement_minutes = data[20:22]
        measurement_secs = data[22:24]
        last_hours = data[24:26]
        last_minutes = data[26:28]
        last_secs = data[28:30]
        self.lastWateringTime.setText(last_hours + ":" + last_minutes + ":" + last_secs)
        self.timeOfMeasurement.setText(measurement_hours + ":" + measurement_minutes + ":" + measurement_secs)
        self.dispTemperature.setText(temperature + " C")
        self.dispHumidity.setText(humidity + " %")
        self.nextWateringTime.setText(next_watering_hours + ":" + next_watering_minutes + ":" + next_watering_secs)
        if 10 < int(temperature[:2]) < 25:
            self.advice1.setText("- Temperature is normal")
            self.advice1.setStyleSheet("color: green;""font: 16px;""font-family: Comic Sans MS")
        elif int(temperature[:2]) < 10:
            self.advice1.setText("- Too cold for plant, it may freeze. Increase the temperature!")
            self.advice1.setStyleSheet("color: red;""font: 16px;""font-family: Comic Sans MS")
        elif int(temperature[:2]) > 25:
            self.advice1.setText("- Too hot for plant, it may dry out. Decrease the temperature!")
            self.advice1.setStyleSheet("color: red;""font: 16px;""font-family: Comic Sans MS")

        if 50 < int(humidity[:2]) < 80:
            self.advice2.setText("- Humidity is normal")
            self.advice2.setStyleSheet("color: green;""font: 16px;""font-family: Comic Sans MS")
        elif int(humidity[:2]) < 50:
            self.advice2.setText("- Too low humidity! Plant needs water!")
            self.advice2.setStyleSheet("color: red;""font: 16px;""font-family: Comic Sans MS")
        elif int(humidity[:2]) > 80:
            self.advice2.setText("- Too high humidity!")
            self.advice2.setStyleSheet("color: red;""font: 16px;""font-family: Comic Sans MS")

        self.advice3.setText("- Plants need watering every day")
        try:
            connection = mysql.connector.connect(
                host="localhost",
                database="micro_project",
                user="root",
                password="rootpass"
            )
            if connection.is_connected():
                db_info = connection.get_server_info()
                print("Connected to MySQL Server version ", db_info)
                cursor = connection.cursor()

                sql = "INSERT INTO history (temperature, humidity, hour, minute, sec) VALUES (%s, %s, %s, %s, %s)"
                print(sql)
                val = (temperature, humidity, measurement_hours, measurement_minutes, measurement_secs)
                cursor.execute(sql, val)

                connection.commit()
                cursor.close()
            connection.close()
        except mysql.connector.Error:
            print("Error while connection to database")

    def init_elements(self):
        self.portcomboBox.addItems(serial_ports())
        self.speedcomboBox.addItems(SPEEDS)
        self.port = serial.Serial(self.portcomboBox.currentText(), int(self.speedcomboBox.currentText()),
                                  parity=serial.PARITY_NONE,
                                  stopbits=serial.STOPBITS_ONE,
                                  bytesize=serial.EIGHTBITS,
                                  timeout=2)
