from serialControl.reader import Controller
import paho.mqtt.client as mqtt

serialHandler = Controller("COM8", 115200)

# The Topic that this code will publish to after reading the serial port
PUBISH_TOPIC_CONTAINER = "cansat/container"
PUBISH_TOPIC_SP1 = "cansat/sp1"
PUBISH_TOPIC_SP2 = "cansat/sp2"

# The topic that this code will listen to for any event to write to serial port
SUBSCRIBE_TOPIC = "cansat/sendData"


def on_connect(client, userdata, flags, rc):
    print("Connected")
    # Subscribing in on_connect() means that if we lose the connection and reconnect then subscriptions will be renewed.
    client.subscribe(SUBSCRIBE_TOPIC)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Writing to Serial Port: {}".format(msg.payload.decode("utf-8")))
    serialHandler.writePort(msg.payload.decode("utf-8"))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

# MQTT loop starts in a separate therad
client.loop_start()
# Code below will continue executing

def publish(data:str):
    PACKET_TYPE = data.split(",")[3]
    topics = {
        "C":PUBISH_TOPIC_CONTAINER,
        "S1":PUBISH_TOPIC_SP1,
        "S2":PUBISH_TOPIC_SP2
    }
    PUBISH_TOPIC = topics.get(PACKET_TYPE)
    client.publish(PUBISH_TOPIC, readData, qos=2, retain=True)

while True:
    # Read the serial port for available data
    # Execution will stop for as long as there's no return character i.e. "\n"
    readData = serialHandler.readPort()
    print("Got Serial Data: {}".format(readData))
    publish(readData)
