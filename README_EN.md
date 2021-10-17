# About this package
## Overview
This is a library to control AK series motors (made by T-motor) from ROS node.

## How it works
The STM microcontroller is connected between the PC and the motor to bridge the ROS and CAN communication.
It also implements target value, response value, and service communication for initialization.
rosserial_mbed package is used to connect the PC to the ROS.

# Details
## How it works
1. Connect the microcontroller to the ROS network
`$ roslaunch AKROS_bridge_ros connect.launch`

2. Initialize the motor. Settings and parameters are in AKROS_bridge_controller/config/robot_config.yaml.
`$ roslaunch AKROS_bridge_controller initialize_robot.launch`

3. Set the origin of the robot.
`$ rosservice call /set_position_to_zero <CAN_ID of the corresponding motor>` 

4. Set the target command and PD gain in the message and publish from the node (=run the controller)
`$ rosrun AKROS_bridge_controller <controller_node>`

# Package list
- AKROS_bridge = metapackage
- AKROS_bridge_controller = package related to control of robot
- AKROS_bridge_converter = Converts between ROS messages and CAN messages
- AKROS_bridge_msgs = Package that summarizes messages and services used for communication
- AKROS_bridge_ros = Package for other communication
- (mbed = Folder for firmware of microcontroller. NOT ROS PACKAGE!)

# List of topics
- /motor_cmd ... Motor command topic (Angle, Velocity, Torque, P-gain, D-gain)
- /motor_reply ... Motor response (Angle, Velocity, Torque)

# List of service
- /set_position_to_zero ... Set the current position to zero
- /servo_setting ... Change servo stiffness of the motor
- /current_state ... Receive the current joint state
