# TracioX

# Line Tracing Car

This repository contains the code for a line tracing car using an Arduino. The project uses infrared sensors for line detection and an ultrasonic sensor for obstacle detection. Some photos and media of the car can be found in the `media` folder.

## Features

- **Line Following:** Uses 5 IR sensors to detect and follow a line.
- **PID Control:** Implements a PID controller (with tunable parameters) to adjust motor speeds and maintain the line.
- **Obstacle Detection:** Uses an ultrasonic sensor to detect obstacles and stop the car when needed.
- **Branch Decision Making:** Decides on turns or corrections based on sensor readings at intersections.

## Getting Started

Follow these instructions to set up and run the project on your Arduino.

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software)
- Arduino board (e.g., Arduino Uno)
- Motor driver (compatible with the defined motor pins)
- IR sensors (5 total)
- Ultrasonic sensor (HC-SR04 or similar)
- Connecting wires and power supply

### Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/xEspix/TracioX.git
   cd TracioX
   ```

2. **Open the Sketch:**

   Open the `LineTracingCar.ino` file in the Arduino IDE.

3. **Connect Your Hardware:**

   - Connect the IR sensors to the analog pins (A0–A4).
   - Connect the ultrasonic sensor to pins 4 (trig) and 5 (echo).
   - Connect the motor driver inputs to the corresponding pins:
     - ENA to pin 9, IN1 to pin 8, IN2 to pin 7.
     - ENB to pin 10, IN3 to pin 11, IN4 to pin 12.
   - Ensure that the motors and sensor power are connected according to your board’s specifications.

4. **Upload the Code:**

   Select the appropriate board and port in the Arduino IDE and click **Upload**.

## Code Overview

- **Line Tracing:** The `followLine()` function uses a PID controller (with parameters `Kp`, `Ki`, and `Kd`) to adjust the motor speeds based on sensor readings.
- **Sensor Reading:** The function `getLineError()` calculates the error by reading the 5 IR sensors.
- **Motor Control:** The `setMotorSpeed()` function adjusts the motor speeds and directions.
- **Obstacle Detection:** The `distance()` function uses the ultrasonic sensor to stop the car if an obstacle is detected within 20 cm.
- **Intersection Handling:** The code includes functions like `decideBranch()` and `executeTurn()` to handle turns and junctions.

## Usage

Once the code is uploaded and the hardware is connected:

- The car will start following a line.
- If an obstacle is detected within 20 cm, the car stops.
- At intersections or junctions, the car makes decisions based on sensor inputs:
  - Turns left/right or goes straight depending on the sensor configuration.
- All actions are logged via the Serial Monitor for debugging and adjustments.

## Media

Photos and videos of the Line Tracing Car in action are available in the `media` folder. Check them out to see the project in operation!

## Contributing

Contributions are welcome. If you have suggestions, improvements, or fixes:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes.
4. Open a pull request.
