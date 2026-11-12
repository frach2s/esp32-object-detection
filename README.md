# ESP32 Real-Time Object Detection System

> **This README is the single source of truth for the project.**
> If anything in the code, chat, or notes disagrees with this file, this file wins.
> Update it first, then change the code.

---

## 1. Group Information

- **Group No.:**
- **Section:**
- **Course:** Microprocessor Systems / Machine Learning (Activity 4)
- **Project Title:** ESP32 Real-Time Object Detection System Using YOLO and USB Serial

| Member | Team | Branch |
|---|---|---|
|  | Python |  |
|  | Python |  |
|  | Python |  |
|  | ESP32 |  |
|  | ESP32 |  |

---

## 2. What the Project Does

A laptop webcam captures live video. Python runs a pretrained YOLO model on each frame and shows the **object name, bounding box, and confidence score** on screen. Python then sends a **single letter** over **USB serial** to a real ESP32. The ESP32 reads the letter and lights the **LED assigned to that object**.

```
Webcam -> Python (YOLO) -> USB serial -> ESP32 -> LED
 sees      detects          sends a      reads     lights up
 object    the object       letter       letter    matching LED
```

**Key idea:** the ESP32 does **not** run AI. The laptop does the detection. The ESP32 only receives a letter and controls hardware. The webcam is connected to the laptop, not the ESP32.

### Assignment requirements (checklist)

- [ ] Real ESP32 connected by USB (simulation-only is **not** accepted)
- [ ] LED circuit on a breadboard
- [ ] Webcam live video through Python and OpenCV
- [ ] Trained YOLO model (pretrained COCO model is fine)
- [ ] At least **3 object categories**
- [ ] Name, bounding box, and confidence score shown on the live video
- [ ] Python to ESP32 communication (USB serial)
- [ ] ESP32 lights a **different LED per object**
- [ ] Tested with real physical objects
- [ ] 4 to 5 minute group presentation with live demo (all members speak)
- [ ] Each student submits **one individual .docx reflection** on Google Classroom

---

## 3. The Contract (both teams must follow this exactly)

This is the **only** thing the ESP32 team and the Python team must agree on. Do not change it without updating this table and telling the group.

### Serial settings

| Setting | Value |
|---|---|
| Connection | USB serial (UART over the USB cable) |
| Baud rate | **115200** |
| Data format | **1 byte per message**, a single ASCII letter, no newline needed |
| Direction | Python -> ESP32 only |

### Objects, letters, and LEDs

| Detected object (YOLO class name) | Letter | LED color | ESP32 GPIO |
|---|---|---|---|
| `person` | `P` | Blue | 25 |
| `bottle` | `B` | Green | 26 |
| `cell phone` | `C` | Red | 27 |
| nothing detected | `N` | All LEDs off | - |

> Use the exact YOLO class name, including the space in `cell phone`.
> If the group changes the objects, update this table **first**.

### ESP32 behavior (what the sketch must do)

1. `Serial.begin(115200)` and set GPIO 25, 26, 27 as outputs.
2. Read one character at a time from Serial.
3. On `P`, `B`, or `C`: turn **all LEDs off**, then turn on only the matching LED.
4. On `N`: turn all LEDs off.
5. Ignore any other character.

### Python behavior (what the script must do)

1. Open the webcam with OpenCV and load a YOLO model (`yolov8n.pt`, downloaded automatically by Ultralytics).
2. For each frame, draw the **bounding box, class name, and confidence** for detections.
3. Only consider the three target classes: `person`, `bottle`, `cell phone`.
4. Ignore detections below a confidence threshold (suggested: **0.5**).
5. **Multiple objects rule (decision):** if more than one target object is visible, send the letter of the one with the **highest confidence**. *(Edit this line if the group decides differently.)*
6. If no target object is detected, send `N`.
7. Send a letter only when it **changes** from the previous one, to avoid flooding the serial port.
8. After opening the serial port, wait **2 seconds** (`time.sleep(2)`), because the ESP32 resets when the port opens.
9. Send **bytes**, not strings: `esp.write(b'B')` or `esp.write(letter.encode())`.
10. Include a switch: `USE_ESP = True/False`. When `False`, print `Would send: <letter>` instead of using the serial port, so the Python team can test without hardware.
11. Press `q` to quit.

---

## 4. Hardware

### Parts

- ESP32 development board
- 3 LEDs (blue, green, red) and 3 x 220 ohm resistors
- Breadboard and jumper wires
- USB cable (**data** cable, not charge-only)
- Laptop or PC with a working webcam

### Circuit connections

```
GPIO 25 -> 220 ohm -> Blue LED  (long leg toward resistor) -> GND
GPIO 26 -> 220 ohm -> Green LED (long leg toward resistor) -> GND
GPIO 27 -> 220 ohm -> Red LED   (long leg toward resistor) -> GND
```

The long leg of an LED is the anode (+) and faces the resistor side. The short leg goes to GND.

---

## 5. Software and Setup

| Tool | Used for |
|---|---|
| Arduino IDE | Writing and uploading the ESP32 sketch |
| Python 3 | Detection script |
| OpenCV, Ultralytics (YOLO), PySerial | Webcam, detection, serial communication |
| VS Code + Git | Code and version control |

Install Python libraries:

```
pip install -r requirements.txt
```

`requirements.txt`:

```
opencv-python
ultralytics
pyserial
```

---

## 6. Repository Structure

```
esp32-object-detection/
├── README.md                  <- this file (source of truth)
├── .gitignore
├── requirements.txt
├── arduino/
│   └── esp32_led_control/
│       └── esp32_led_control.ino
├── python/
│   ├── detect.py              <- main detection + serial script
│   └── serial_test.py         <- sends P, B, C, N to test the ESP32 without YOLO
├── images/                    <- circuit diagram, photos, screenshots
├── docs/                      <- presentation
└── video/
    └── demo_link.md           <- link to demo video (do not push large videos)
```

The `.ino` file **must** be inside a folder with the **same name** (`esp32_led_control`).

---

## 7. Teams, Branches, and Git Workflow

### Branch structure

```
main                    final, working version only
├── esp32-staging       ESP32 team (2 members)
└── python-staging      Python team (3 members)
```

| Team | Owns this folder | Branch |
|---|---|---|
| ESP32 team | `arduino/` | `esp32-staging` |
| Python team | `python/`, `requirements.txt` | `python-staging` |

### Rules

- Edit **only your team's folder**. This avoids merge conflicts.
- Tell the group before changing `README.md`, `requirements.txt`, or the Contract in section 3.
- Never push directly to `main`. Merge through pull requests.
- Always `git pull` before you start working.
- Use clear commit messages, for example `Add confidence threshold`.
- Commit under **your own** account. Commit history shows individual contribution.

### Personal branches (optional)

A personal branch (for example `francisco-dev`) is fine, even if it was created from `main`. Merge into your team's staging branch when a piece of work is done, and bring teammates' changes in with:

```
git checkout your-branch
git merge python-staging       # or esp32-staging
```

### Create the staging branches (one person, once)

```
git checkout main
git pull
git checkout -b esp32-staging
git push -u origin esp32-staging

git checkout main
git checkout -b python-staging
git push -u origin python-staging
```

### Join a staging branch (each member)

```
git fetch
git checkout python-staging      # or esp32-staging
```

- `git fetch` downloads the branch list from GitHub without changing your files.
- `git checkout <branch>` switches your files to that branch.

### Daily workflow

```
git pull
git add .
git commit -m "Describe your change"
git push
```

### Merge into main (when both teams' parts work)

1. On github.com, open a pull request `esp32-staging` -> `main` and merge it.
2. Open a pull request `python-staging` -> `main` and merge it.
3. If `README.md` conflicts, choose **Accept Both Changes**, save, and commit.
4. Locally: `git checkout main` then `git pull`.

### Common Git errors

| Error | Fix |
|---|---|
| `Can't push refs to remote. Try running "Pull" first` | Run `git pull --no-rebase`, then `git push` |
| `GH007: Your push would publish a private email address` | Use your GitHub noreply email: `git config --global user.email "ID+username@users.noreply.github.com"`, then `git commit --amend --reset-author --no-edit`, then `git push` |
| `src refspec ... does not match any` | Branch name typo. Run `git branch` and copy the exact name |
| `Git: Clone` missing in VS Code | Turn off Restricted Mode (Trust the folder) and make sure Git is installed (`git --version`) |

---

## 8. How to Test

The two teams do **not** need each other's code to test their own part.

### ESP32 team (no Python needed)

**Option 1: Arduino Serial Monitor**
1. Upload the sketch.
2. Open Serial Monitor at **115200** baud.
3. Type `P`, `B`, `C`, `N` and press Send.
4. The correct LED should light each time. You are acting as Python.

**Option 2: `python/serial_test.py`** (also tests the cable, port, and baud rate)

Close the Serial Monitor first.

```python
import serial, time

esp = serial.Serial('COM3', 115200)   # change to your port
time.sleep(2)

for letter in [b'P', b'B', b'C', b'N']:
    esp.write(letter)
    print("Sent:", letter)
    time.sleep(2)
```

### Python team (no ESP32 needed)

Set `USE_ESP = False` in `detect.py`. Check that:
- the webcam opens,
- boxes, names, and confidence scores appear on the live video,
- the printed `Would send: <letter>` matches what is held in front of the camera,
- `N` is printed when no target object is visible.

### Integration test (both together, one laptop)

1. The person with the ESP32 pulls the latest code from **both** staging branches (or from `main` after merging).
2. Plug in the ESP32 and check the port in **Arduino IDE -> Tools -> Port** (for example `COM3`).
3. Set the same port in `detect.py` and set `USE_ESP = True`.
4. **Close the Arduino Serial Monitor.** Only one program can use the port at a time.
5. Run:
   ```
   python python/detect.py
   ```
6. Hold up a person, bottle, and cell phone one at a time. The matching LED should light. Remove the object and all LEDs should turn off.

### Test log (fill in during testing)

| Object | Distance / lighting | Detected? | Confidence | LED correct? | Notes |
|---|---|---|---|---|---|
| person |  |  |  |  |  |
| bottle |  |  |  |  |  |
| cell phone |  |  |  |  |  |
| none |  |  |  |  |  |

---

## 9. Troubleshooting

| Problem | Likely cause and fix |
|---|---|
| `Access is denied` or `could not open port` | Arduino Serial Monitor is still open, or wrong COM port. Close it and recheck the port |
| LED does nothing | Baud rates differ (must be 115200), letters differ from the Contract, or wiring is wrong |
| LED works in Serial Monitor but not from Python | Python sent a string, not bytes. Use `esp.write(b'B')` |
| Works once, then stops | Missing `time.sleep(2)` after opening the port (ESP32 resets on connect) |
| Port does not appear in Arduino IDE | Charge-only USB cable, or missing USB driver (CP210x or CH340) |
| Webcam does not open | Another app is using it, or try camera index `1` instead of `0` |
| Object not detected | Poor lighting, object too small or too far. Hold it 30 to 60 cm away |
| Two similar objects confuse the model | Keep them apart, or choose different objects |
| LED flickers | Python is sending every frame. Send only when the letter changes |
| `analogWrite`/pin errors on ESP32 | Use plain `digitalWrite` for this project |

---

## 10. Deliverables and Submission

### Group

- Working demo with real ESP32 hardware
- 4 to 5 minute presentation, all members speak:
  1. Project overview and objectives
  2. ESP32 hardware and circuit connections
  3. Machine learning model and detection process
  4. **Live demo** (webcam detecting, ESP32 responding)
  5. Results and challenges
- Record a **backup demo video** in case the live demo fails.

### Individual (each student, one .docx on Google Classroom)

- Full name, section, group number, project title
- Your specific responsibilities and contributions
- What you learned about ESP32, machine learning, object detection, and hardware-software integration
- Technical problems you faced and how the group solved them
- How the activity improved your understanding of Microprocessor Systems and Machine Learning
- Real photos or screenshots of you participating and of the finished project

**Reflections must be written individually and must not be identical across members.**
This README and the GitHub repo are for organization only. The required submission is the individual .docx.

---

## 11. Results

*(Fill in after testing.)*

- Model used: YOLOv8n (pretrained on COCO)
- Detection screenshot: `![Detection](images/detection_screenshot.png)`
- Circuit photo: `![Circuit](images/circuit_photo.jpg)`
- Observations:

## 12. Demo Video

[Add link here]

---

## 13. Context for AI Assistants

If you paste this repo or README into an AI assistant for help, use this summary so every member gets consistent answers:

- This is a **school activity** (Activity 4) for a Microprocessor Systems / Machine Learning course.
- The system is: **webcam -> Python + YOLO (pretrained, COCO) on a laptop -> single-letter USB serial at 115200 baud -> real ESP32 -> one LED per object.**
- The ESP32 does **not** run machine learning. It only reads a letter (`P`, `B`, `C`, `N`) and controls GPIO 25, 26, 27.
- The Python script must display name, bounding box, and confidence, and must have a `USE_ESP` switch.
- The **Contract in section 3 is fixed.** Do not suggest changing letters, baud rate, pins, or protocol unless the README is updated first.
- Real ESP32 hardware is required. Do not suggest simulation-only solutions.
- Team split: **ESP32 team (2)** owns `arduino/` on `esp32-staging`; **Python team (3)** owns `python/` on `python-staging`; both merge into `main`.
- Do not write reflections for students. Each student's reflection must be original.
- Activity 5 (LDR + regression model on ESP32) is a **separate project** and is not part of this repo.