sudo apt-get install mysql-server --fix-missing -y
sudo apt-get install default-libmysqlclient-dev -y
sudo apt-get install libcurl4-openssl-dev -y
sudo apt-get install libboost-all-dev -y
sudo apt-get install libssl-dev -y
sudo mysql --verbose < ./DBScripts/database_init
sudo apt-get install libglib2.0-dev
wget https://www.kernel.org/pub/linux/bluetooth/bluez-5.45.tar.xz
tar xf bluez-5.45.tar.xz
cd bluez-5.45
./configure --prefix=/usr --libexecdir=/usr/lib \
	--sysconfdir=/etc --localstatedir=/var --enable-test \
	--enable-manpages --enable-testing --enable-library \
	--enable-maintainer-mode --enable-experimental \
	--enable-deprecated
make
service bluetooth stop
sudo make install           # Helpful for development
sudo systemctl daemon-reload     # Helpful for development
sudo service bluetooth start     # Helpful for development
sudo apt-get install -y libdbus-1-dev libudev-dev libical-dev libreadline-dev
sed -i 's/.*ExecStart.*/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd -d --noplugin=hostname/' /lib/systemd/system/bluetooth.service
sudo systemctl daemon-reload
sudo service bluetooth stop
sudo service bluetooth start
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y g++-6
sudo ln -f -s /usr/bin/g++-6 /usr/bin/g++
cd ..
sudo apt-get install -y automake
sudo apt-get install libbluetooth-dev
apt-get install libstdc++6-4.6-dev
cd gatt-server
sudo ./server_init.sh
