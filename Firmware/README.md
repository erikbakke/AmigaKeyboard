# AmigaKeyboard firmware

This folder contains the firmware source code for the Amiga replacement keyboard project.

The firmware is written in C as an Atmel Studio project and maintained in the current version (Microchip Studio 7).
I have not found any protocol timing issues during the testing of the firmware, and I have used this with my A4000 for over a year without any problems.
However, that doesn't mean there are no problems when used with other Amiga models, so if any problems are found I am happy to rewrite the relevant sections in AVR assembly code in order to achieve better control over the timing.
I would prefer to keep parts of the code that are not timing sensitive in C, though, to keep the code readable and understandable even for users who do not have AVR experience.

The AVR ATMega32U4 was chosen for this project as it has enough IO pins to run the keyboard matrix and it has a USB port which should make firmware upgrades and debugging easier.
The current version of the firmware does not yet take advantage of the USB port, though.  This is at the top of my todo-list at the moment.

It should be noted here that the keyboard matrix is not laid out identically to Commodore's matrix and that some keys which had dedicated lines in the Commodore design form part of the matrix in this design.
The use of diodes to support n-key rollover should remove the need for dedicated lines, and I have not yet experienced any issues related to this.
Even though the matrix is laid out differently the resulting scan codes are, of course, the same as in the original design.

## Operating principle

The matrix is scanned repeatedly by setting each row output low in turn and then checking each column inputs go low.
If a low is detected, the row/column combination is looked up in a table of scancodes.
The firmware tracks the status of each key and at the end of the matrix scan it will send down/up codes to the Amiga for any keys with a changed status.
The Caps Lock key is tracked separately, as this key toggles state each time it is pressed.

The communication with the Amiga is a custom serial protocol described in the Amiga Hardware Reference Manual (See Appendix G in HRM, 3rd revision).

## Missing features

- Ability to act as a USB serial port for debugging purposes.
- Support firmware updates over USB.
- Ability to act as a USB keyboard, which would allow use with any USB-capable computer.

## Firmware flashing process

I am using an Atmel ICE programmer, using the AVR-ISP programming plug.
On revision 0.1 of the PCB, I have to solder Dupont M-M wires to the PCB for each line required for the programming, and I connect the other end of the wires to the AVR-ISP plug.
With revision 0.2, there is a footprint for an AVR-ISP socket on the PCB, and each programming signal is routed to this footprint to make programming easier.

## Change log

Revision 0.1:
- Initial version of the firmware
- Incomplete debugging support
