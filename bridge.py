from serialControl.reader import readPort, writePort
import paho.mqtt.client as mqtt


# The Topic that this code will publish to after reading the serial port
PUBISH_TOPIC = "cansat/gotData"

# The topic that this code will listen to for any event to write to serial port
SUBSCRIBE_TOPIC = "cansat/sendData"


def on_connect(client, userdata, flags, rc):
    print("Connected")
    # Subscribing in on_connect() means that if we lose the connection and reconnect then subscriptions will be renewed.
    client.subscribe(SUBSCRIBE_TOPIC)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Writing to Serial Port: {}".format(msg.payload.decode("utf-8")))
    writePort(msg.payload.decode("utf-8"))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

# MQTT loop starts in a separate therad
client.loop_start()
# Code below will continue executing


while True:
    # Read the serial port for available data
    # Execution will stop for as long as there's no return character i.e. "\n"
    readData = readPort()
    print("Got Serial Data: {}".format(readData))
    client.publish(PUBISH_TOPIC, readData, qos=0, retain=True)
