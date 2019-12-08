# Pacman-Bots
There are two systems, the Game Master and the Robots. The Game Master controls the play field, a maze lit by leds. It also controls the position of Pacman and tells the Ghosts where to go to find him using A*. Lastly, it controls the win and lose conditions for the game. The player uses an N64 controller to move Pacman along the gridlines of the leds to try and get all the pellets before the ghosts get him.

### Watch this video to see it in action
[![Watch the video](https://i.imgur.com/N0LvHEk.jpg)](https://www.youtube.com/watch?v=sG0ARsvOe4A)

## Implementation
### Gamemaster
- Communicates with N64 controller to get player input
- Manages game states and play field
- Updates LEDs using hacky UART signal instead of bit banging
- Updates ghost AI location based on A* algorithm
- Manages win/lose conditions
- Changes LED colors to inform Pacman where to go

### Pacman Robot
- Uses light sensing on photodiodes to determine direction to move
- H Bridge controls for motor
- Follows certain intensity of light
- Photoresistors change resistance based on surrounding light
- Difference in resistance is detected via ADC
- Calculate appropriate direction to move based on this

## Technology
This project uses a Cortex-M3 microcontroller with an FPGA for each subsystem. 

Notable aspects of technology
* Serial communication with UART
* Wireless communication with XBee
* Unconventional N64 communication with UART
* LED interfacing with SPI and UART
* Full spectrum RGB, HSV library for LED
* A* Algorithm in C
* H Bridge Interfacing from software
* Hardware light following with photoresistors

![Block diagram of system](https://puu.sh/EE9v2/595842362b.png)



#### Made with Brandon Mosher, Rohan Mallya, and Charlie Light for University of Michigan EECS 373
