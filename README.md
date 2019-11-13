# Pacman-Bots

This project was created to learn about embedded systems and FPGAs. 

### What it is
There are two systems, the Game Master and the Robots. The Game Master controls the play field, a maze lit by leds. It also controls the position of Pacman and tells the Ghosts where to go to find him using A*. Lastly, it controls the win and lose conditions for the game. The player uses an N64 controller to move Pacman along the gridlines of the leds to try and get all the pellets before the ghosts get him.

### Technology
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
