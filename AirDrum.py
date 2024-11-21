import cv2
import serial
import tkinter as tk
from tkinter import Canvas, PhotoImage
from threading import Thread
import pygame  # Library for sound playback

# Initialize serial communication with ESP32
SERIAL_PORT = '/dev/ttyUSB0'  # Update port as needed
BAUD_RATE = 115200
esp32_serial = serial.Serial(SERIAL_PORT, baudrate=BAUD_RATE, timeout=1)

# Initialize pygame for sound
pygame.init()
sounds = {
    "Snare": "snare.wav",
    "Hi-hat": "hihat.wav",
    "Bass Drum": "bass.wav",
    "Cymbal": "cymbal.wav"
}

# Global variables
cap = None
running = False
current_zone = None

# Create GUI
root = tk.Tk()
root.title("Air Drum Kit")

canvas = Canvas(root, width=800, height=600, bg="white")
canvas.pack()

# Load drum images
drum_kit_img = PhotoImage(file="drum_kit.png")  # Background drum kit image
highlight_img = PhotoImage(file="highlight.png")  # Drum highlight overlay

# Draw initial drum kit
canvas.create_image(400, 300, image=drum_kit_img, anchor=tk.CENTER)

# Zones for drums (positions and sizes)
zones = {
    "Snare": (300, 400, 50),  # x, y, radius
    "Hi-hat": (200, 300, 50),
    "Bass Drum": (400, 500, 50),
    "Cymbal": (600, 200, 50),
}

def detect_drumstick(frame):
    """Detect the drumstick position and determine the detection zone."""
    # Placeholder for detection logic (replace with your model's logic)
    drumstick_position = (150, 100)  # Example coordinates
    if drumstick_position[0] < 100:
        return "Snare"
    elif drumstick_position[0] < 200:
        return "Hi-hat"
    elif drumstick_position[0] < 300:
        return "Bass Drum"
    else:
        return "Cymbal"

def update_gui(zone):
    """Update GUI to highlight the detected drum zone."""
    global current_zone

    # Clear previous highlight
    canvas.delete("highlight")

    # Highlight the current zone
    if zone in zones:
        x, y, r = zones[zone]
        canvas.create_image(x, y, image=highlight_img, tags="highlight", anchor=tk.CENTER)

    # Update the current zone
    current_zone = zone

    # Play corresponding sound
    if zone in sounds:
        pygame.mixer.Sound(sounds[zone]).play()

def camera_feed():
    """Capture and display camera feed."""
    global running, cap

    while running:
        ret, frame = cap.read()
        if not ret:
            break

        # Detect drumstick and send zone to ESP32
        zone = detect_drumstick(frame)
        esp32_serial.write(f"{zone}\n".encode())
        update_gui(zone)

        # Display the camera feed
        cv2.putText(frame, f"Zone: {zone}", (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.imshow("Air Drum Camera Feed", frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            stop_camera()
            break

def start_camera():
    """Start the camera and begin detection."""
    global cap, running

    if running:
        return  # Camera is already running

    try:
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            raise ValueError("Could not open camera.")
        running = True
        Thread(target=camera_feed, daemon=True).start()
    except Exception as e:
        print(f"Error: {e}")

def stop_camera():
    """Stop the camera and close the feed."""
    global running, cap

    running = False
    if cap is not None:
        cap.release()
    cv2.destroyAllWindows()

def on_closing():
    """Handle GUI window closing."""
    stop_camera()
    esp32_serial.close()
    root.destroy()

# GUI buttons
start_button = tk.Button(root, text="Start Camera", command=start_camera, width=15)
start_button.pack(pady=10)

stop_button = tk.Button(root, text="Stop Camera", command=stop_camera, width=15)
stop_button.pack(pady=10)

exit_button = tk.Button(root, text="Exit", command=on_closing, width=15)
exit_button.pack(pady=10)

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()
