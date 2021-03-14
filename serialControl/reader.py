import serial

class Controller():
    def __init__(self, PORT:str="COM8", BAUD_RATE:int=115200):
        self.port = PORT
        self.baudRate = BAUD_RATE
        self.ser = serial.Serial()
        self.ser.port = self.port
        self.ser.baudrate = self.baudRate

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