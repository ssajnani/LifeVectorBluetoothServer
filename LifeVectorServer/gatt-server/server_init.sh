#!/bin/bash
id="$(cat /proc/cpuinfo | grep Serial | cut -d ' ' -f 2)"
sed -i "s/lifepi.*\"/lifepi$id\"/g" ./init/gobbledegook.conf 
sed -i "s/fareconnect[^\"]*\"/lifepi$id\"/g" ./src/standalone.cpp
sed -i "s/FareConnect[^\"]*\"/lifepi$id\"/g" ./src/standalone.cpp
sed -i "s/lifepi[^\"]*\"/lifepi$id\"/g" ./src/standalone.cpp
cp ./init/gobbledegook.conf /etc/dbus-1/system.d/gobbledegook.conf
sudo ./configure && sudo make
sudo src/standalone -d
