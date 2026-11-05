#!/bin/sh
# Build and run Task 3 (sigdelay). Run from inside this folder: ./doit.sh

# re-attach Vbuddy (Windows/WSL) if the helper script exists, and give it time to boot
ATTACH=~/Documents/iac/lab0-devtools/tools/attach_usb.sh
if [ -x "$ATTACH" ]; then
    "$ATTACH"
    sleep 3
fi

# clean up old build and waveform
rm -rf obj_dir
rm -f sigdelay.vcd

# run Verilator: all .sv files, top module, testbench
verilator -Wall --cc --trace --top-module sigdelay sigdelay.sv counter.sv ram2ports.sv --exe sigdelay_tb.cpp

# build the executable
make -j -C obj_dir/ -f Vsigdelay.mk Vsigdelay

# run it
obj_dir/Vsigdelay
