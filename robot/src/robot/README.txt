High-level library for controlling the robot.

Initialize robot with RobotInit

Set/Get robot id with RobotSetId/RobotGetId

Set/Get color read by robot rgb sensor with RobotSetColor/RobotGetColor

Set/Get robot command with RobotSetCmd/RobotGetCmd

Send robot data to Game Master with RobotSend

Handle receipt of data from Game Master with RobotReceive

Set global robot direction based on current direction and photosensor input with RobotSetDirection

Get global robot direction with RobotGetDirection

Calibrate robot photosensor array with RobotCalibrate

Update robot motors based on photosensor input with RobotMotorUpdate

Stop robot with RobotStop

Cleanup robot's dynamically allocated memory with RobotDestroy

Run the robot state machine with RobotRun