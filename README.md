# EmbeddedSystems

### PROJECT TITLE 
Smart Alarm Clock

### DESCRIPTION
The alarm clock with a display and a buzzer playing a melody which requires a sequence of actions to be turned off.

### DEVICE 
Arduino Uno Rev3 
 
### FUNCTIONALITIES

- DHT11 Temperature and humidity sensor
- IR Remote Control
- HC-SR04 Ultrasonic distance sensor
- Red, yellow, and green LEDs
- Passive buzzer
- RTC DS1302
- I2C
- LCD1602 display
- Water level sensor

![image](https://user-images.githubusercontent.com/50404753/170697609-ccaf6f42-b311-4c2b-a52e-ec1e35d38078.png)

## General description

Device is a smart alarm clock. It can be set up to play an alarm melody at specified hour. To switch off the melody, user must interact with a set of sensors and perform a set of actions: increase the humidity around humidity sensor, wet the water level sensor and close the distance in front of the distance sensor. Completion of each operation is signaled by separate diodes of different colors. Additionally, values recorded by the sensors can be displayed on the LCD screen, which content is operated by IR remote control.

![image](https://user-images.githubusercontent.com/50404753/170697645-2d30135c-a4f7-4358-b9e7-43f2959321a1.png)

![image](https://user-images.githubusercontent.com/50404753/170697682-dd75e77b-3433-49fb-a025-bcda0f605917.png)

## Setting up the device

After powering on the device, all sensors start working. Main screen displays current time and time of the alarm, while side screen displays values recorded by sensors. There is only one LCD Display, so switching screens is made possible with left and right ‘arrow buttons’ on a remote control. To set new or change existing alarm time, user must press asterisk (\*) button on the remote. Then they can input desired hour and minute by pressing 4 consecutive digits (buttons 0 – 9 on the controller), hour is stored in format HH:MM. If user enters invalid data (other buttons or non-existing time, example: 12:61), then the process will be stopped, and user must press (\*) again to start over. After the alarm is set, it will automatically go off at the desired time.

![image](https://user-images.githubusercontent.com/50404753/170697722-29cc6e0d-3bbd-46c2-8f9f-11f78c4a6f99.png)

## Using the alarm

When the alarm goes off, functions of the device (such as setting the alarm or switching screens) are paused until the user turns off the alarm and melody which is played indefinitely. To accomplish that, they must perform a set of 3 tasks:

1. Wet the surface of water level sensor (for example put the end of it in a glass of water).
2. Short a distance measured by a distance sensor to below 10 cm (for example cover it with a hand).
3. Increase humidity around temperature and humidity sensor to over 60% (for example by breathing very close to it).

Completion of each task is signaled by separate diode:

- Yellow diode signals if humidity condition is met.
- Red diode signals if there is an object closer than 10 cm to the distance sensor.
- Green diode signals if water level sensor is wet.

After completing all three tasks simultaneously, the melody will stop playing and the alarm will be ready to be set up again.

![image](https://user-images.githubusercontent.com/50404753/170697759-67ccd50e-a770-487b-8906-ee2e54715ffe.png)

![image](https://user-images.githubusercontent.com/50404753/170697782-026a9fe6-4aa2-4066-a7f9-034271bcc3f6.png)
