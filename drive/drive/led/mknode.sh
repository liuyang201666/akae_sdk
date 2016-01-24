#!/bin/sh
device="led"

rm -f /dev/${device}
major=$(awk "\$2==\"led\" {print \$1}" /proc/devices)
mknod /dev/${device} c ${major} 0

