import cv2
import serial
import tkinter as tk
from tkinter import messagebox
from threading import Thread

# Initialize serial communication with ESP32
SERIAL_PORT = '/dev/ttyUSB0'  # Update port, this just an example
BAUD_RATE = 115200
esp32_serial = serial.Serial(SERIAL_PORT, baudrate=BAUD_RATE, timeout=1)

# Global variables
cap = None
running = False

def detect_drumstick(frame):
    """Detect the drumstick position and determine the detection zone."""
    # Placeholder for detection logic
    drumstick_position = (150, 100)  # Example coordinates
    if drumstick_position[0] < 100:
        return "Snare"
    elif drumstick_position[0] < 200:
        return "Hi-hat"
    elif drumstick_position[0] < 300:
        return "Bass Drum"
    else:
        return "Cymbal"

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
        messagebox.showwarning("Warning", "Camera is already running!")
        return

    try:
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            raise ValueError("Could not open camera.")
        running = True
        Thread(target=camera_feed, daemon=True).start()
    except Exception as e:
        messagebox.showerror("Error", str(e))

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

# GUI setup
root = tk.Tk()
root.title("Air Drum Control")

frame = tk.Frame(root, padx=20, pady=20)
frame.pack()

start_button = tk.Button(frame, text="Start Camera", command=start_camera, width=15)
start_button.pack(pady=5)

stop_button = tk.Button(frame, text="Stop Camera", command=stop_camera, width=15)
stop_button.pack(pady=5)

exit_button = tk.Button(frame, text="Exit", command=on_closing, width=15)
exit_button.pack(pady=5)

root.protocol("WM_DELETE_WINDOW", on_closing)

root.mainloop()
