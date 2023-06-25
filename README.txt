This is an autotamper system which includes a module that can be installed onto a module builder as well as remotes for long distance control.

NOTE:
Circuit board files are json files which are openable through the EasyEDA design software.

Remote NOTES:
For remote_v3, the program can be uploaded using the 6 SPI pins, no bootloader required.
For remote_v2, it is necessary to burn the bootloader before soldering the jumper connector near the transmitter.
Afterwards, a program can be uploaded using the UART pins (5V, RESET, RX, TX, GND).

Module NOTES:
Chip A is located near the center of the board and chip B is located underneath the 12V to 5V voltage regulator.
For the module, it is necessary to burn the bootloader on both ATMega328p-au chips before soldering the jumper connector near the transmitter.
To program the chips, use the 5 UART pins and ensure the RX and TX pins between chips A and B are not connected.
After both chips A and B are programmed, you need to jumper or solder the RX pin of chip A to the TX pin of chip B for them to communicate.
