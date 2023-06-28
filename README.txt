This is an autotamper system which includes a module that can be installed onto a module builder as well as remotes for long distance control.
The buttons on the module builder are in order STOP, EDIT, PROG1, PROG2, PROG3

NOTE:
Circuit board files are json files which are openable through the EasyEDA design software.
Both SPI and UART can be accomplished using an Arduino Uno board.
To use SPI, connect pins GND, 3.3V, 10-RESET, 11-MOSI, 12-MISO, 13-CLK.
Also use a voltage Bi-Directional Logic Level Converter so the 5V Arduino Uno signals will not harm the 3V circuit.
To use UART, remove the DIP chip from the Arduino Uno and connect GND, 5V, RESET-RESET, RX-RX, TX-TX.

Remote NOTES:
For remote_v3, install MiniCore on the Arduino IDE, then select the Board: ATMega328, BOD 2.7V, No Bootloader, Extrnal 8MHz Clock, LTO Enabled, Variant 328P/328PA, then to upload program, Sketch -> Upload Using Programmer
For remote_v3, the program can be uploaded using the 6 SPI pins, no bootloader required.
For remote_v2, it is necessary to burn the bootloader before soldering the jumper connector near the transmitter.
Afterwards, a program can be uploaded using the UART pins (5V, RESET, RX, TX, GND).

Module NOTES:
Chip A is located near the center of the board and chip B is located underneath the 12V to 5V voltage regulator.
For the module, it is necessary to burn the bootloader on both ATMega328p-au chips before soldering the jumper connector near the transmitter.
To program the chips, use the 5 UART pins and ensure the RX and TX pins between chips A and B are not connected.
After both chips A and B are programmed, you need to jumper or solder the RX pin of chip A to the TX pin of chip B for them to communicate.

Module Wiring:
12V and GND power the module.
There are 4 sensors, F1: the feeler near the cab, F2: the feeler far from the cab, PS: the pressure sensor, and OIL: the low oil switch.
For the four sensors there is a + and - wire, but it shouldn't matter which way it is connected.
There are also 4 coils which control fwd, back, up, down, each having +/- and the polarity doesn't matter.
