import board
from digitalio import DigitalInOut, Direction
import time

# import adafruit_dotstar
# led = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)
# led.brightness = 0.1
# led[0] = (0, 0, 0)

# Requires agc_alarm_replica circuitypython board definition

# fifteen lamp positions are defined:
# 7 left column (top to bottom), 7 right column, red LED

lamp = [DigitalInOut(board.D1),
        DigitalInOut(board.D5),
        DigitalInOut(board.D6),
        DigitalInOut(board.D7),
        None,
        None,
        None,
        DigitalInOut(board.D8),
        DigitalInOut(board.D9),
        DigitalInOut(board.D10),
        DigitalInOut(board.D11),
        DigitalInOut(board.D12),
        None,
        None,
        DigitalInOut(board.D13)]

for l in lamp:
    if l is not None:
        l.direction = Direction.OUTPUT

# led[0] = (0, 64, 0)

state = 0
last = lamp[14]
last.value = True
cur = last
time.sleep(1.0)

# iterate through the lamps, turning
# each one on for one second
while True:
    cur = lamp[state]

    if last is not None:
        last.value = False

    if cur is not None:
        cur.value = True

    state = (state+1) % 15
    time.sleep(0.5)
    # led[0] = (0, 255, 0)
    time.sleep(0.5)
    # led[0] = (0, 255, 255)