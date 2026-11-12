# ESP32 Real-Time Object Detection System

## Group Information
- **Group No.:** 
- **Section:** 
- **Members:** 
  - 
  - 
  - 

## Project Description
A webcam captures live video, and Python with a YOLO model detects objects
in each frame. The detected object's name, bounding box, and confidence score
are shown on screen. Python then sends a letter over USB serial to an ESP32,
which lights up the LED that matches the detected object.

## How It Works
Webcam → Python (YOLO) → USB Serial → ESP32 → LED

## Detected Objects
| Object | Serial Letter | LED Color | ESP32 GPIO |
|---|---|---|---|
| Person | P | Blue | 25 |
| Bottle | B | Green | 26 |
| Cell phone | C | Red | 27 |
| None | N | All off | - |

## Hardware
- ESP32 development board
- 3 LEDs and 3 x 220Ω resistors
- Breadboard and jumper wires
- USB cable
- Laptop with webcam

## Software
- Arduino IDE
- Python, OpenCV, Ultralytics YOLO, PySerial

## Circuit Connections
- GPIO 25 → 220Ω → Blue LED → GND
- GPIO 26 → 220Ω → Green LED → GND
- GPIO 27 → 220Ω → Red LED → GND

## How to Run
1. Upload `arduino/esp32_led_control/esp32_led_control.ino` to the ESP32 using Arduino IDE.
2. Install the Python libraries: `pip install -r requirements.txt`
3. Set the correct COM port in `python/detect.py`.
4. Run: `python python/detect.py`
5. Hold a person, bottle, or cell phone in front of the webcam.
6. Press `q` to quit.

## Results
(Add your detection screenshot and notes here after testing.)