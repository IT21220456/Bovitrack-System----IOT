import RPi.GPIO as GPIO
import spidev
from time import sleep
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
            sleep(1)

    def on_rx_done(self):
        print("\nReceived packet: ")
        self.set_mode(MODE.SLEEP)
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        print(bytes(payload).decode("utf-8", 'ignore'))
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)

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

try:
    lora.start()
except KeyboardInterrupt:
    print("KeyboardInterrupt")
finally:
    GPIO.cleanup()
