import logging
from awscrt import mqtt, http
from awsiot import mqtt_connection_builder
import sys
import threading
import time
import json
import os
import base64

# AWS region and topic for MQTT
AWS_REGION = "us-east-1"
MQTT_TOPIC_PUBLISH = "esp8266/ota"
MQTT_TOPIC_SUBSCRIBE = "esp8266/status"
AWS_IOT_ENDPOINT = "aeuqcio2h8teq-ats.iot.us-east-1.amazonaws.com" 
CLIENT_ID = "iotconsole-11ccf3af-7ae1-4fbe-9bef-a23a5a7487fc" 

CA_CERT = "root-CA.crt"
CLIENT_CERT = "certificate.pem.crt"
PRIVATE_KEY = "private.pem.key"

srec_file_path = "Datalogger_v0.srec" 
#srec_file_path = "Datalogger_v1.srec" 
# srec_file_path = "blink_gpio_led_output.srec" 



#################### JUST FOR DEBUG ####################
# Setup logging to file
LOG_FILE = "LOG.log"
if os.path.exists(LOG_FILE):
    os.remove(LOG_FILE) 

logging.basicConfig(
    filename=LOG_FILE,
    level=logging.INFO,
    format='%(asctime)s - %(message)s'
)

# Callback when connection is accidentally lost.
def on_connection_interrupted(connection, error, **kwargs):
    logging.error("> Connection interrupted. error: {}".format(error))

# Callback when an interrupted connection is re-established.
def on_connection_resumed(connection, return_code, session_present, **kwargs):
    logging.info("> Connection resumed. return_code: {} session_present: {}".format(return_code, session_present))

    if return_code == mqtt.ConnectReturnCode.ACCEPTED and not session_present:
        logging.info("> Session did not persist. Resubscribing to existing topics...")
        resubscribe_future, _ = connection.resubscribe_existing_topics()
        resubscribe_future.add_done_callback(on_resubscribe_complete)

def on_resubscribe_complete(resubscribe_future):
    resubscribe_results = resubscribe_future.result()
    logging.info("> Resubscribe results: {}".format(resubscribe_results))

    for topic, qos in resubscribe_results['topics']:
        if qos is None:
            sys.exit("> Server rejected resubscribe to topic: {}".format(topic))

# Callback when the connection successfully connects
def on_connection_success(connection, callback_data):
    assert isinstance(callback_data, mqtt.OnConnectionSuccessData)
    logging.info("> Connection Successful with return code: {} session present: {}".format(callback_data.return_code, callback_data.session_present))

# Callback when a connection attempt fails
def on_connection_failure(connection, callback_data):
    assert isinstance(callback_data, mqtt.OnConnectionFailureData)
    logging.error("> Connection failed with error code: {}".format(callback_data.error))

# Callback when a connection has been disconnected or shutdown successfully
def on_connection_closed(connection, callback_data):
    logging.info("> Connection closed")
    
def disconnect():
    logging.info("Disconnecting...")
    disconnect_future = mqtt_connection.disconnect()
    disconnect_future.result()
    logging.info("Disconnected!")
    
#################### MAIN FUNCTION I CARE ####################
current_line = -1
end_of_file = False
mqtt_connection = None
line_lock = threading.Lock() 

def read_srec_line(line_number):
    global end_of_file
    if line_number < 0  :
        return None
    
    try:
        with open(srec_file_path, "r") as srec_file:
            for current_line_number, line in enumerate(srec_file):
                if current_line_number == line_number:
                    return line.strip()
        end_of_file = True
        return None
    except FileNotFoundError:
        logging.error("> SREC file not found!")
        end_of_file = True
        return None

def count_lines_in_file():
    with open(srec_file_path, "r") as srec_file:
        line_count = sum(1 for line in srec_file)
        return line_count

# Callback when the subscribed topic receives a message
def on_message_received(topic, payload, dup, qos, retain, **kwargs):
    global current_line
    logging.info("> Received message from topic '{}': {}".format(topic, payload))

    try:
        message = json.loads(payload)
        print(message)

        with line_lock: 
            if message.get("data") == "ok":                
                current_line += 1
            elif message.get("data") == "error":
                current_line = message.get("line")

        publish_to_mqtt(current_line)

    except Exception as e:
        logging.error(f"Error processing message: {e}")
    

# Function to send the payload using AWS IoT Core
def publish_to_mqtt(line):
    global end_of_file
    with line_lock:
        data = read_srec_line(line)

    if data is None:
        if end_of_file:
            line = -1
            data = "bye"
        else:
            data = "hello"

    payload = {
        "line": line,
        "data": data
    }
    json_payload = json.dumps(payload)
    print(f"> Send {line}")
    try:
        mqtt_connection.publish(
            topic=MQTT_TOPIC_PUBLISH,
            payload=json_payload,
            qos=mqtt.QoS.AT_LEAST_ONCE)
        logging.info(f"Sent to MQTT {json_payload}.")
    except Exception as e:
        logging.error(f"Failed to publish message: {e}")




if __name__ == '__main__':
    # Create the proxy options if the data is present in cmdData
    proxy_options = None

    # Create a MQTT connection using hardcoded values
    mqtt_connection = mqtt_connection_builder.mtls_from_path(
        endpoint=AWS_IOT_ENDPOINT,
        port=8883,
        cert_filepath=CLIENT_CERT,
        pri_key_filepath=PRIVATE_KEY,
        ca_filepath=CA_CERT,
        on_connection_interrupted=on_connection_interrupted,
        on_connection_resumed=on_connection_resumed,
        client_id=CLIENT_ID,
        clean_session=False,
        keep_alive_secs=30,
        http_proxy_options=proxy_options,
        on_connection_success=on_connection_success,
        on_connection_failure=on_connection_failure,
        on_connection_closed=on_connection_closed)

    logging.info(f"> Connecting to {AWS_IOT_ENDPOINT} with client ID '{CLIENT_ID}'...")
    connect_future = mqtt_connection.connect()

    # Future.result() waits until a result is available
    connect_future.result()
    logging.info("> Connected!")

    # Subscribe
    logging.info("> Subscribing to topic '{}'...".format(MQTT_TOPIC_SUBSCRIBE))
    subscribe_future, packet_id = mqtt_connection.subscribe(
        topic=MQTT_TOPIC_SUBSCRIBE,
        qos=mqtt.QoS.AT_LEAST_ONCE,
        callback=on_message_received)
    subscribe_result = subscribe_future.result()
    logging.info("> Subscribed with {}".format(str(subscribe_result['qos'])))
    
    while current_line < 0 :
        publish_to_mqtt(current_line)
        time.sleep(5)
    


    # Keep running 
    try:
        while not end_of_file:
            time.sleep(1)
        disconnect()
    except KeyboardInterrupt: 
        #Disconnect
        disconnect()
