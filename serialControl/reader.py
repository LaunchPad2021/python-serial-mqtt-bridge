import serial

class Controller():
    def __init__(self, PORT:str="COM8", BAUD_RATE:int=115200):
        self.ser = serial.Serial(PORT, BAUD_RATE)

    def readPort(self) -> str:
        return self.ser.readline().decode("utf-8")

    def writePort(self,data:str) -> None:
        return self.ser.write(data.encode("utf-8"))

if __name__ == "__main__":
    reader = Controller("COM8")
    while True:
        print(reader.readPort())