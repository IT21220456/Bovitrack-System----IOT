import RPi.GPIO as GPIO
import spidev
import time
import serial
from SX127x.LoRa import *
from SX127x.board_config import BOARD

# Configure the LoRa board
BOARD.setup()

class LoRaRcvCont(LoRa):
    def __init__(self, verbose=False):
        super(LoRaRcvCont, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)

    def start(self):
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        while True:
            time.sleep(1)

    def on_rx_done(self):
        print("\nReceived packet: ")
        self.set_mode(MODE.SLEEP)
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        data = bytes(payload).decode("utf-8", 'ignore')
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        return data

lora = LoRaRcvCont(verbose=False)
lora.set_mode(MODE.STDBY)
lora.set_pa_config(pa_select=1)
lora.set_freq(433.0)
lora.set_spreading_factor(7)
lora.set_bw(125)
lora.set_coding_rate(CODING_RATE.CR4_5)
lora.set_rx_crc(True)
lora.set_preamble(8)
lora.set_sync_word(0x34)
lora.set_power(14)

# Initialize the serial port
ser = serial.Serial('/dev/ttyS0', 9600)

def read_lora():
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        return data
    return None

def parse_data(data):
    parsed_data = {}
    if data:
        parts = data.split(",")
        if len(parts) > 1:  # Check for comma-separated format
            parsed_data['TempHumidity'] = parts[0]
            parsed_data['pH'] = parts[1]
            parsed_data['WaterTemp'] = parts[2]
            parsed_data['Turbidity'] = parts[3]
            parsed_data['TDS'] = parts[4]
        else:  # Assume key-value pairs separated by ":"
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
        if 'TempHumidity' in parsed_data:
            print(f"Temperature & Humidity: {parsed_data['TempHumidity']}")
        if 'pH' in parsed_data:
            print(f"pH Value: {parsed_data['pH']}")
        if 'WaterTemp' in parsed_data:
            print(f"Water Temperature: {parsed_data['WaterTemp']} °C")
        if 'Turbidity' in parsed_data:
            print(f"Turbidity: {parsed_data['Turbidity']}")
        if 'TDS' in parsed_data:
            print(f"TDS: {parsed_data['TDS']}")
        print("------------------------")
    else:
        print("No valid data received.")

def main():
    lora.start()
    while True:
        data = read_lora()
        if data:
            parsed_data = parse_data(data)
            display_data(parsed_data)
        time.sleep(1)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
    finally:
        GPIO.cleanup()
