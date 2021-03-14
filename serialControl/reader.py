import serial
from serial.tools import list_ports

class Controller():
    def __init__(self, PORT:str=list_ports.comports()[0].name, BAUD_RATE:int=115200):
        self.ser = serial.Serial(PORT, BAUD_RATE)

    def readPort(self) -> str:
        if not self.ser.is_open:
            self.ser.open()
        return self.ser.readline().decode("utf-8")

    def writePort(self,data:str) -> None:
        if not self.ser.is_open:
            self.ser.open()
        return self.ser.write(data.encode("utf-8"))

if __name__ == "__main__":
    reader = Controller("COM7", 115200)
    reader.ser.is_open
    while True:
        print(reader.readPort())