Some information about the eCos development environment
-------------------------------------------------------
-------------------------------------------------------


Virtual machine Ubuntu
----------------------

eCos sources are installed at:
 /opt/ecos/ecos3.0

gnutools are installed at:
 /opt/ecos/gnutools

/opt/ecos/ecosenv.sh  --- shell script file to define environment variables



eCos target configuration was created at:
 /home/sce/ecos-work

using "configtool" or "ecosconfig" and the configuration file
 "pc.ecc"

Library target files were created at:
 /home/sce/ecos-work/pc_install


Application examples available at:
 /home/sce/ecos-work/work/examples
 /home/sce/ecos-work/work/cmdecosx



Execution environment
---------------------

If the virtual machine was shutdown, you may need to redefine environment variables:

  setxkbmap pt                 --- portuguese keyboard
  source /opt/ecos/ecosenv.sh  --- to define PATH to ecos tools


to compile an application:
  make INSTALL_DIR=/home/sce/ecos-work/pc_install

to generate image file (<app> is the name of the executable file that was produced):
  i386-elf-objcopy -O binary <app> <app>.bin


If you are using a virtual machine for eCos, you need to write the "floppy":
  dd conv=sync if=<app>.bin of=/dev/fd0

Make sure that the "floppy" is connected (in removable devices),
and you have read/write permission. If needed do:
  sudo chmod 666 /dev/fd0


If you are using "qemu", you don't need to write the "floppy". Do:
  qemu-system-i386 -fda <app>.bin -serial /dev/ttyUSB0

Make sure that the serial converter is connected (in removable devices),
and you have read/write permission. If needed do:
  sudo chmod 666 /dev/ttyUSB0

(if you have other serial devices the number may be different from 0)



Virtual machine eCos
--------------------

empty virtual machine that boots from a "floppy" (use the same host file that is used in the Ubuntu virtual machine for the "floppy").

We also need to have a serial port (usually can be configured to be "auto detect") (if needed use control panel to check the number of the COM port).
