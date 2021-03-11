import serial
ser = serial.Serial("COM7", 115200)

def readPort() -> str:
    return ser.readline().decode("utf-8")

def writePort(data:str) -> None:
    return ser.write(data.encode("utf-8"))

if __name__ == "__main__":
    print(readPort())