import time
import serial
from lora import LoRa

# Initialize LoRa
lora = LoRa('/dev/ttyS0', 433E6)
lora.set_tx_power(20, 0)

# Read data from LoRa
def read_lora():
    if lora.received():
        data = lora.receive()
        return data
    return None

def parse_data(data):
    parsed_data = {}
    if data:
        parts = data.split(", ")
        for part in parts:
            key, value = part.split(":")
            parsed_data[key] = value
    return parsed_data

def display_data(parsed_data):
    if parsed_data:
        print("----- Sensor Data -----")
        if "HR" in parsed_data and "SpO2" in parsed_data:
            print(f"Heart Rate: {parsed_data['HR']} bpm")
            print(f"SpO2: {parsed_data['SpO2']} %")
        if "Temp" in parsed_data:
            print(f"Temperature: {parsed_data['Temp']} C")
        if "ECG" in parsed_data:
            print(f"ECG Signal: {parsed_data['ECG']}")
        if "Lead Off Detected" in parsed_data:
            print("Lead Off Detected!")
        print("------------------------")
    else:
        print("No valid data received.")

def main():
    while True:
        data = read_lora()
        if data:
            parsed_data = parse_data(data)
            display_data(parsed_data)
        time.sleep(1)

if __name__ == "__main__":
    main()
