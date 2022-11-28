#!/bin/bash

cd ~/source/obj.i386/destdir.i386/boot/minix/.temp && qemu-system-i386 -display none -serial stdio -kernel kernel -append "console=tty00 rootdevname=c0d0p1" -initrd "mod01_ds,mod02_rs,mod03_pm,mod04_sched,mod05_vfs,mod06_memory,mod07_tty,mod08_mfs,mod09_vm,mod10_pfs,mod11_init" -hda /home/jstahl/source/minix/minix_x86.img --enable-kvm && cd -
