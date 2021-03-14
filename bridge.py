from serialControl.reader import Controller
import paho.mqtt.client as mqtt

serialHandler = Controller("COM7", 115200)

# GLOBALS
START_TELEMETRY = False
DEBUG_MODE = True
HOSTNAME = "localhost"
PORT = 1883

# The Topic to publish to after reading the serial port
PUBISH_TOPIC_CONTAINER = "cansat/container"
PUBISH_TOPIC_SP1 = "cansat/sp1"
PUBISH_TOPIC_SP2 = "cansat/sp2"

# The topic that this code will listen to for any event to write to serial port
SUBSCRIBE_TOPIC = "cansat/sendData"
TELEMETRY_ACTIVE_TOPIC = "cansat/telemetry"



def on_connect(client, userdata, flags, rc):
    if DEBUG_MODE:
        print("Connected")
    # Subscribing in on_connect() means that if we lose the connection and reconnect then subscriptions will be renewed.
    client.subscribe(SUBSCRIBE_TOPIC)
    if DEBUG_MODE:
        print("Successfully Subscribed to {}".format(SUBSCRIBE_TOPIC))
    client.subscribe(TELEMETRY_ACTIVE_TOPIC)
    if DEBUG_MODE:
        print("Successfully Subscribed to {}".format(TELEMETRY_ACTIVE_TOPIC))


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global START_TELEMETRY
    message = msg.payload.decode("utf-8")
    topic = msg.topic
    if DEBUG_MODE:
        print("Received Message\nTopic: {}\nMessage: {}".format(topic, message))
    telemetryMapping = {
        "ON":True,
        "OFF":False
    }
    if topic == SUBSCRIBE_TOPIC:
        if DEBUG_MODE:
            print("Writing to Serial Port:\nTopic: {}\nMessage {}".format(topic, message))
        serialHandler.writePort(message)
    elif topic == TELEMETRY_ACTIVE_TOPIC:
        START_TELEMETRY = telemetryMapping.get(message)
        command = "CMD,2176,CX,{}".format(message)
        serialHandler.writePort(command)
        if DEBUG_MODE:
            print("Writing Command: {}".format(command))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

if DEBUG_MODE:
    print("Connecting to {}:{}".format(HOSTNAME, PORT))
client.connect(HOSTNAME, PORT, 60)


def publish(data:str):
    PACKET_TYPE = data.split(",")[3]
    topicMap = {
        "C":PUBISH_TOPIC_CONTAINER,
        "S1":PUBISH_TOPIC_SP1,
        "S2":PUBISH_TOPIC_SP2
    }
    PUBISH_TOPIC = topicMap.get(PACKET_TYPE)
    if DEBUG_MODE:
        print("Publishing to\nTopic: {}\nMeaasge: {}".format(PUBISH_TOPIC, data))
    client.publish(PUBISH_TOPIC, data, qos=2, retain=False)


    


# MQTT loop starts in a separate therad
client.loop_start()
# Code below will continue executing

while True:
    try:
        # This runs in a separate therad
        # Read the serial port for available data
        # Execution will stop for as long as there's no return character i.e. "\n"
        # print("Got Serial Data: {}".format(readData), end="")
        if START_TELEMETRY:
            readData = serialHandler.readPort()
            if "2176" in readData:
                publish(readData)
    except:
        print("Error Occured..! Retrying..!")