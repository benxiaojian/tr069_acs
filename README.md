# hms_simulator

Implement Tr069 Server with Csoap And Nanohttp libraries.

First `./install` to download libraries and install it.

Then `make` in src.

If appear problem `./libs/bin/csoap/generic/i686-linux/include/nanohttp-1.1/nanohttp/nanohttp-logging.h:107:1: error: expected declaration before } token } `. Open the file, and remove this line

Run `./bin/hms net_device  port` like `./bin/hms eth0 8083`
