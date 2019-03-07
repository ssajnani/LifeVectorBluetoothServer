To setup the bluetooth server

1. Install GCC for C++ 17

2. System configuration

The following sections may help users with their system configurations.

BlueZ Version

My distribution didn't have the latest version of BlueZ with proper support for their D-Bus API (it was still considered experimental up until v5.42.) I had to download and build it for myself. I grabbed BlueZ 5.45.

Here's how I built it - this will probably work for most or all Ubuntu users:

./configure --prefix=/usr --libexecdir=/usr/lib \
--sysconfdir=/etc --localstatedir=/var --enable-test \
--enable-manpages --enable-testing --enable-library \
--enable-maintainer-mode --enable-experimental \
--enable-deprecated
make
service bluetooth stop
sudo make install           # Helpful for development
systemctl daemon-reload     # Helpful for development
service bluetooth start     # Helpful for development
If you're on a Raspberry Pi, you should probably also run sudo rpi-update followed by a reboot.

Configuring BlueZ

The configuration file is located at /etc/bluetooth/main.conf. However, you should be fine with defaults here because this server can configure the settings as needed.

Enable BlueZ debug logs

Edit the file /lib/systemd/system/bluetooth.service and set the ExecStart line to look like:

ExecStart=/usr/lib/bluetooth/bluetoothd -d --noplugin=hostname
-d - it's a good idea to leave this enabled while you are setting up your services in GGK (you can remove it later). This will allow you to tail -f /var/log/syslog | grep bluetoothd to get some helpful debug information.

--noplugin=hostname - this prevents BlueZ from running the 'hostname' plugin, which will rename your device to whatever its hostname is (ignoring your configurations.)

Once you've done this, run the following commands to reload these settings and restart the service:

sudo systemctl daemon-reload
sudo service bluetooth stop
sudo service bluetooth start
Enabling D-Bus Permissions

In order for our application to communicate over D-Bus, we'll need to ask D-Bus for an owned name, which will in effect be our address on D-Bus. D-Bus must be configured to grant us permissions to do this. We'll grant these permissions to user root.

You'll need to locate the D-Bus permissions on your box. Likely, you'll find a set of files for this in the directory /etc/dbus-1/system.d. Create the file /etc/dbus-1/system.d/gobbledegook.conf and give it the contents:

<!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
<busconfig>
  <policy user="root">
    <allow own="com.gobbledegook"/>
    <allow send_destination="com.gobbledegook"/>
    <allow send_destination="org.bluez"/>
  </policy>
  <policy at_console="true">
    <allow own="com.gobbledegook"/>
    <allow send_destination="com.gobbledegook"/>
    <allow send_destination="org.bluez"/>
  </policy>
  <policy context="default">
    <deny send_destination="com.gobbledegook"/>
  </policy>
</busconfig>
Note the com.gobbledegook entries in your new gobbledegook.conf file. This must match the service name (the first parameter sent to ggkStart() in standalone.cpp). If you change the service name from gobbledegook to clownface in that call to ggkStart(), then you'll need to edit the gobbledegook.conf file and change all occurrances of com.gobbledegook to com.clownface.

Enabling Bluetooth

You don't need to do anything. this server will automatically power on the adapter, enable LE with advertisement.

However, if you want to do this manually, here are a few helpful commands you might try:

sudo btmgmt -i 0 power off
sudo btmgmt -i 0 le on
sudo btmgmt -i 0 connectable on
sudo btmgmt -i 0 advertising on
sudo btmgmt -i 0 power on

3. Build & Launch

GGK uses the standard autotools build process:

./configure && make
This will build libggk.a then compile standalone.cpp into a program that links with libggk.a. There is no make install as there is nothing to install.

Then run with:

sudo src/standalone -d
GGK requires super-user privileges when run due to privileges required for D-Bus and HCI sockets. A system can be configured to allow a user to run a GGK server without sudo, but that's beyond the scope of this document.

During development, I tend to run these three commands, each in their own terminal:

sudo tail -f /var/log/syslog | grep bluetoothd
sudo dbus-monitor --system
sudo ./src/standalone -d
With no parameters, standalone will output only service level output (starting stopping errors, etc.) Additional output parameters are:

`-q`        Quiet - errors only
`-v`        Verbose - include info log levels
`-d`        Debug - include debug log levels
