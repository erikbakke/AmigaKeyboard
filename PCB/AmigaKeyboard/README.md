# AmigaKeyboard PCB files

This folder contains the schematics and PCB design for revision 0.2 of the Amiga replacement keyboard design as a KiCad project.  This was initially developed in KiCad 6, and is currently maintained in KiCad 7.

The keyboard works as is with my A4000, but I have not yet had a chance to test it with my A500.  The design should be electrically compatible, and I'm hoping it would be dimensionally compatible as well.
Some work will be required to fit the PCB to existing Amiga external keyboard housings.  The current design has mounting holes to fit a keyboard case I had lying around, so I would be very happy to receive positions and dimensions for mounting holes on the different Amiga keyboards for A2000, A3000 and A4000 models, and will update the design in any way I can to increase compatibility.

I am getting PCBs manufactured from time to time and still have some revision 0.1 PCBs left if anyone want one for experimenting.

## Change log
Revision 0.1:
- Initial PCB version for prototype board manufacture.
  
Revision 0.2:
- A missing trace between D180 and MX80 (numeric pad "4") has been fixed.  The bodge wire fix is no longer needed on revision 0.2.
- Added AVR ISP connector to assist in firmware upgrades and troubleshooting.
