import paho.mqtt.publish as publish

MQTT_SERVER = "192.168.137.52" 
MQTT_PATH = "motor_command"
while (1):
    state = input("Turn on or off:")
    if (state.isalpha() and state.lower() == "on"):
        state = "on"
    else:
        state = "off"
    print(state)
    publish.single(MQTT_PATH, state, hostname=MQTT_SERVER)
    print("done")
