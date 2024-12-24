# DataLogger System

**Hardware Components:**
- **NXP KE16 Microcontroller:** Handles data acquisition and communication.
- **ESP01 Wi-Fi Module:** Provides wireless connectivity to AWS IoT.
- **AWS IoT Core:** Serves as the central hub for receiving and visualizing data.

---

## Features
1. **Data Acquisition:**
   - Real-time sensor data collection via KE16.
2. **Wireless Communication:**
   - Data is transmitted to AWS IoT through ESP01.
3. **Cloud Integration:**
   - Data is stored and visualized using AWS IoT Core and Grafana.

---

## AWS IoT Snapshot
- [Grafana Dashboard Snapshot](https://g-d0c87c1b6b.grafana-workspace.us-east-1.amazonaws.com/dashboard/snapshot/owJJjGZJ6tyY20A8B1FOgLVPXDIcvonT)
- Displays temperature, humidity

---

## Setup Instructions
### Hardware Setup
1. Connect KE16 to ESP01 via UART.
   - Configure baud rate at 115200.
2. Attach sensors to KE16 for data acquisition.
3. Power the setup using a 3.3V or 5V power source.

### Software Setup
1. Flash the KE16 with the firmware provided.
2. Configure ESP01 for Wi-Fi access:
   ```plaintext
   SSID: YourNetworkSSID
   Password: YourNetworkPassword
   ```
3. Configure AWS IoT:
   - Attach an IoT policy to your device.
   - Use a certificate for secure communication.

### Grafana Setup
1. Link AWS IoT Core data to Grafana.
2. Import the provided snapshot for pre-configured visuals.

---

## Communication Protocol
- **MQTT** is used for data transmission.
  - Topics:
    - `esp8266/device`
    - `esp8266/temperature`
    - `esp8266/humidity`
  - Payload Format:
    ```json
    {
      "device": "esp8266",
      "temperature": 25,
      "humidity": 60
    }
    ```

---

## Troubleshooting
1. **No Data in Grafana:**
   - Check AWS IoT logs for dropped connections.
   - Verify ESP01 connectivity.
2. **Wi-Fi Issues:**
   - Ensure SSID and password are correct.
   - Reduce distance to the router.
3. **Data Transmission Errors:**
   - Verify MQTT topic subscriptions.
   - Ensure certificates are active.

---

## License
This project is licensed under the MIT License. See the LICENSE file for details.

