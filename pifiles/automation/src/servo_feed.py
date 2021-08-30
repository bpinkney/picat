from RPIO import PWM
import sys

servo_pin = 18

servo = PWM.Servo()

# Set servo on GPIOservo_pin to 1200us (1.2ms)
servo.set_servo(servo_pin, int(sys.argv))

# Set servo on GPIOservo_pin to 2000us (2.0ms)
#servo.set_servo(servo_pin, 2000)

# Clear servo on GPIOservo_pin
servo.stop_servo(servo_pin)