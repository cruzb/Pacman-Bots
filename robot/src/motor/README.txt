Library for use with MOTOR.v verilog module. The motor instance consists of an hbridge, pwm and encoder instance and uses their respective libraries.

Allowed motor commandsa are
MOTOR_CMD_COAST
MOTOR_CMD_FORWARD
MOTOR_CMD_BACKWARD
MOTOR_CMD_BRAKE

Initialize the motor instance with motor_init;

Set the motor command with motor_set_cmd.

Set the motor speed with motor_set_speed. Acceptable values are between 0.0 and 1.0. Values out of this range will be silently "clamped" to the correct value.

Get current motor speed with motor_get_speed;

Set motor command and motor speed simultaneously with motor_set;

Test all motor commands with motor_test.