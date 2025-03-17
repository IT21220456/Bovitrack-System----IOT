import time
import serial

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
        parsed_data['TempHumidity'] = parts[0]
        parsed_data['pH'] = parts[1]
        parsed_data['WaterTemp'] = parts[2]
        parsed_data['Turbidity'] = parts[3]
        parsed_data['TDS'] = parts[4]
    return parsed_data

def display_data(parsed_data):
    if parsed_data:
        print("----- Sensor Data -----")
        print(f"Temperature & Humidity: {parsed_data['TempHumidity']}")
        print(f"pH Value: {parsed_data['pH']}")
        print(f"Water Temperature: {parsed_data['WaterTemp']} °C")
        print(f"Turbidity: {parsed_data['Turbidity']}")
        print(f"TDS: {parsed_data['TDS']}")
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
