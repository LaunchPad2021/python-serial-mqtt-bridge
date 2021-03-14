from serialControl.reader import Controller
import paho.mqtt.client as mqtt

serialHandler = Controller("COM7", 115200)

# GLOBALS
START_TELEMETRY = False

# The Topic that this code will publish to after reading the serial port
PUBISH_TOPIC_CONTAINER = "cansat/container"
PUBISH_TOPIC_SP1 = "cansat/sp1"
PUBISH_TOPIC_SP2 = "cansat/sp2"

# The topic that this code will listen to for any event to write to serial port
SUBSCRIBE_TOPIC = "cansat/sendData"
TELEMETRY_ACTIVE_TOPIC = "cansat/telemetry"



def on_connect(client, userdata, flags, rc):
    print("Connected")
    # Subscribing in on_connect() means that if we lose the connection and reconnect then subscriptions will be renewed.
    client.subscribe(SUBSCRIBE_TOPIC)
    client.subscribe(TELEMETRY_ACTIVE_TOPIC)


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global START_TELEMETRY
    message = msg.payload.decode("utf-8")
    topic = msg.topic
    telemetryMapping = {
        "ON":True,
        "OFF":False
    }
    if topic == SUBSCRIBE_TOPIC:
        print("Writing to Serial Port: {}".format(topic))
        serialHandler.writePort(message)
    elif topic == TELEMETRY_ACTIVE_TOPIC:
        START_TELEMETRY = telemetryMapping.get(message)
        serialHandler.writePort("CMD,2176,CX,{}".format(message))


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)



def publish(data:str):
    PACKET_TYPE = data.split(",")[3]
    topics = {
        "C":PUBISH_TOPIC_CONTAINER,
        "S1":PUBISH_TOPIC_SP1,
        "S2":PUBISH_TOPIC_SP2
    }
    PUBISH_TOPIC = topics.get(PACKET_TYPE)
    # print("Got Serial Data: {}".format(data), end="")
    # print("Publish Topic: {}".format(PUBISH_TOPIC))
    client.publish(PUBISH_TOPIC, data, qos=2, retain=False)


    


# MQTT loop starts in a separate therad
client.loop_start()
# Code below will continue executing

while True:
    try:
        # Read the serial port for available data
        # Execution will stop for as long as there's no return character i.e. "\n"
        # print("Got Serial Data: {}".format(readData), end="")
        if START_TELEMETRY:
            readData = serialHandler.readPort()
            if "1267" in readData:
                publish(readData)
    except:
        print("Error Occured..! Retrying..!")