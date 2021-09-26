# Tutorial 1: https://linuxconfig.org/bash-scripting-tutorial
# Tutorial 2: https://tldp.org/LDP/Bash-Beginners-Guide/html/

sudo insmod proj2.ko int_str="1,2,3,4,5"
cat /proc/proj2
sudo rmmod proj2.ko
dmesg | tail -10