1. Admin module 
sudo insmod admin_dev.ko
sudo mknod -m 666 /dev/lcd c 255 0
sudo mknod -m 666 /dev/motor c 255 1
sudo mknod -m 666 /dev/motor c 255 2
sudo mknod -m 666 /dev/motor c 255 3

2. Sound sensor module
sudo insmod sound_dev.ko
sudo mknod -m 666 /dev/sound_dec c 250 0
user application : ./user

3. Ultra Sonic module
sudo insmod hc-sro4.ko
sudo chmod 777 /sys/class/distance/configure
sudo echo 23 24 1000 > /sys/class/distance/configure
user application : ./user_application

4. motor
./motor

5. LCD
./LCD
