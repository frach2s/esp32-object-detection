import time
import cv2
import serial
from ultralytics import YOLO
from collections import deque, Counter

# ---------- Settings ----------
USE_ESP = False          # False = print "Would send" instead of using serial
SERIAL_PORT = "COM3"     # change to your ESP32 port (Arduino IDE -> Tools -> Port)
BAUD_RATE = 115200       # Contract: must be 115200
CAMERA_INDEX = 0         # try 1 if the webcam does not open
CONF_THRESHOLD = {"person": 0.8, "bottle": 0.5, "cell phone": 0.5}   # per-class, tune during testing

# Contract: YOLO class name -> letter
LETTERS = {"person": "P", "bottle": "B", "cell phone": "C"}

model = YOLO("yolov8n.pt")

# ---------- Serial ----------
esp = None
if USE_ESP:
    try:
        esp = serial.Serial(SERIAL_PORT, BAUD_RATE)
        time.sleep(2)  # ESP32 resets when the port opens
    except serial.SerialException as e:
        print(f"Could not open {SERIAL_PORT}: {e}")
        print("Close the Arduino Serial Monitor, check the COM port and use a data USB cable.")
        exit(1)


def send(letter):
    if USE_ESP:
        esp.write(letter.encode())
    else:
        print(f"Would send: {letter}")


# ---------- Webcam ----------
cap = cv2.VideoCapture(CAMERA_INDEX)

last_letter = None

history = deque(maxlen=10)   # last 10 frame results
STABLE_COUNT = 6             # a letter must win 6 of the last 10 frames

try:
    if not cap.isOpened():
        print("Could not open webcam (try CAMERA_INDEX = 1)")
        raise SystemExit(1)

    while True:
        ok, frame = cap.read()
        if not ok:
            print("Could not read frame")
            break

        results = model(frame, verbose=False)

        best_name = None
        best_conf = 0.0

        for box in results[0].boxes:
            name = model.names[int(box.cls[0])]
            conf = float(box.conf[0])

            if name not in LETTERS or conf < CONF_THRESHOLD[name]:
                continue

            x1, y1, x2, y2 = map(int, box.xyxy[0])
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, f"{name} {conf:.2f}", (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

            # many objects -> keep the one with the highest confidence
            if conf > best_conf:
                best_conf, best_name = conf, name

        # none seen -> N
        letter = LETTERS[best_name] if best_name else "N"

        # smooth: only switch when one letter clearly dominates recent frames
        history.append(letter)
        top, count = Counter(history).most_common(1)[0]

        if count >= STABLE_COUNT and top != last_letter:
            send(top)
            last_letter = top

        cv2.imshow("Detection", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    # ---------- Cleanup (runs on q, Ctrl+C, or a crash) ----------
    if esp:
        try:
            esp.write(b'N')   # turn all LEDs off on exit
        except serial.SerialException:
            pass              # port already gone (cable unplugged)
        esp.close()
    cap.release()
    cv2.destroyAllWindows()