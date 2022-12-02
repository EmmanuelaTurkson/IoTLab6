import paho.mqtt.client as mqtt
import urllib.request
 
MQTT_SERVER = "172.16.5.69" # copied from the arduino code
MQTT_PATHHUM = "#" # this seems like the topic # represents id of esp32
MQTT_PATHTEMP = "temperature/#" # this seems like the topic
 
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
 
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.


    # this is to subscribe to both temp and humidity
    #client.subscribe(MQTT_PATHHUM)
    client.subscribe(MQTT_PATHHUM)



temp1 = "0"
temp2 = "0"
dis1 = "0"
dis2 = "0"

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):

    #global temp1, temp2, dis1, dis2

    print(msg.topic+" "+ str(msg.payload))
    #print(msg.payload, type(msg.payload))


    
    # more callbacks, etc

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
 
client.connect(MQTT_SERVER, 1883, 60)
 
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()