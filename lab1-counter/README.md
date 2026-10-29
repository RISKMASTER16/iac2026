# Lab 1 – Counter with Verilator and Vbuddy

Lab instructions: [ELEC50010-IAC/Lab1-Counter](https://github.com/ELEC50010-IAC/Lab1-Counter)

An 8-bit binary counter written in SystemVerilog, simulated with Verilator, checked in GTKWave, and then connected to the Vbuddy board. It is extended into a direction-controlled counter, a loadable counter, a single-stepping counter, and a counter with a binary-to-BCD decoder.

## Contents

| Folder | What it is | Vbuddy? |
|--------|------------|:-------:|
| [`task1/`](task1/) | Basic 8-bit counter + first testbench, viewed in GTKWave | no |
| [`task1/challenge-pause-at-9/`](task1/challenge-pause-at-9/) | Testbench stops counting for 3 cycles when count reaches 9 | yes |
| [`task2/`](task2/) | Testbench drives Vbuddy's 7-seg displays; button enables counting | yes |
| [`task2/plot/`](task2/plot/) | Same, but plots count on the TFT with `vbdPlot()` | yes |
| [`task2/challenge-up-down/`](task2/challenge-up-down/) | `en` selects count direction (1 = up, 0 = down) | yes |
| [`task3/step1-loadable/`](task3/step1-loadable/) | Button loads the rotary-encoder value into the counter (one-shot flag) | yes |
| [`task3/step2-single-step/`](task3/step2-single-step/) | Counter advances by one per button press | yes |
| [`task4/`](task4/) | Top-level module: counter → `bin2bcd`, count shown in decimal | yes |

`vbuddy.cpp` (course-provided) and `vbuddy.cfg` live once in this folder. Each Vbuddy task folder has symlinks to them, so there is only one copy to update.

## vbuddy.cfg

A one-line file holding Vbuddy's USB serial port, e.g. `/dev/ttyUSB0` on WSL or `/dev/tty.usbserial-110` on a Mac. `vbdOpen()` reads it from the folder the simulation runs in. The line **must end with a newline**, because `vbdOpen()` strips the last character.

## Vbuddy setup / troubleshooting (WSL)

What went wrong during the lab, and the fixes:

| Symptom | Cause | Fix |
|---------|-------|-----|
| `** Error opening port: /dev/ttyUSB0` | Vbuddy not attached to WSL | `~/Documents/iac/lab0-devtools/tools/attach_usb.sh` (needed after every replug or WSL restart; `doit.sh` now does this) |
| Same error, device exists | User not in `dialout` group | `sudo usermod -aG dialout $USER`, then `wsl --shutdown` in PowerShell and reopen |
| `Cannot find file containing module: 'counter.sv'` | Ran `doit.sh` from the wrong folder | `doit.sh` now `cd`s into its own folder first |
| `** Connected` but screen blank, program hangs | Commands sent while Vbuddy was still booting after attach, so `ack()` waits forever | Ctrl+C, wait a few seconds, rerun (or unplug/replug + re-attach) |
| GTKWave won't open after `wsl --shutdown` | WSLg GUI support didn't come back | Restart WSL again; check with `xeyes` |
| Empty/broken `.vcd` | Run stopped with Ctrl+C before `tfp->close()` | Let the run finish |

Useful checks: `ls -l /dev/ttyU*` (device exists, group `dialout`) and `groups` (`dialout` listed).
