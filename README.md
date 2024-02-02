# Hedgehog blinky_soc Reference Design

## Overview

- This is a "basic blinky", out-of-box reference design for the Hedgehog Development Board
- The design implements a soft RISC-V MC MCU running 90MHz
- The design also supports USB-UART output via the PMOD connector J8 - [Digilent USBUART Pmod](https://digilent.com/shop/pmod-usbuart-usb-to-uart-interface/)
- This design currently targets Lattice Propel/Radiant v2023.2
- Refer to the [Propel Design Summary](documentation/soc_html/SoC_Memory_Map.html) for details

**Follow the sections below, in order, to build and run the design on the Hedgehog board**

## 1. Getting Started - Propel Builder

There is a tcl script (blinky_soc_gen.tcl) in the propel folder that can be used to automatically generate the Propel Builder project for this reference design.  Startup Propel Builder v2023.2 and in the Tcl Console, first use the `% cd <path>` command to change directory to the location of the blinky_soc_gen.tcl file and then enter the command `% source blinky_soc_gen.tcl`.  After the Tcl script completes, right-click in the schematic window to re-draw and cleanup the layout.  The Propel Builder project output is automatically generated as the last step in the script.

If your Lattice install folder for Propel Builder is not in the default location, it may be necessary to edit the blinky_soc_gen.tcl script to update the location.  Open the script in your favorite editor and modify the following line that sets the install path for Lattice Propel. `set propelRegenLsccBase "C:/lscc/propel/2023.2"`

Propel Builder generates a Tcl script within the path of the Propel Builder project (in this case `propel/blinky_soc/radiant_setup_template.tcl`) that in turn will create a Lattice Radiant project to build the HDL design and ultimately generate an FPGA bitstream.  This Tcl script is automatically sourced in Radiant when clicking the "Run Radiant" button (or from the Design menu item) in Propel Builder.

## 2. Getting Started - Radiant

With the Propel design opened in Radiant, the next step would be to add the constraint files to the project.  There is a pre-synthesis constraint file (hedgehog.sdc) that specifies the input clock frequency as 125MHz and a post-synthesis constraint file that sets the IO and pin assignment constaints for the Hedgehog board. These files are found in the constraints folder of the repository.  Use the File->Add->Existing File... menu option with the appropriate file type filter selected (.sdc/.pdc) to add both constraint files to the project.  Click on the "Export Files" process button to run through synthesis, map, place & route and bitstream file generation.

## 3. Getting started - Propel SDK

## 4. Configuring the FPGA

Use the Lattice Programmer (accessible from Radiant) to program the FPGA via the J3 micro-USB connector.  Use the default "Fast Configuration" programming operation to write the bitstream directly to the FPGA configuration SRAM.  See the section [Storing Bitstream to Flash]() for directions on storing the configuration image to the internal configuration flash.

## 5. Debugging/Running the RISC-V Software
