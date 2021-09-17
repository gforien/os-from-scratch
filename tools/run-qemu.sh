qemu-system-arm -M raspi2 -m 512 -serial mon:stdio -nographic -kernel ../build/kernel.elf -s -S #-singlestep -d exec,int -icount 0

#qemu-system-arm -kernel ../build/kernel.elf -cpu arm1176 -m 512 -M raspi2 -nographic -no-reboot -chardev stdio,mux=on,id=terminal -serial chardev:terminal -monitor chardev:terminal -append "rw earlyprintk loglevel=8 panic=120 keep_bootcon rootwait dma.dmachans=0x7f35 bcm2708_fb.fbwidth=1024 bcm2708_fb.fbheight=768 bcm2708.boardrev=0xf bcm2708.serial=0xcad0eedf smsc95xx.macaddr=B8:27:EB:D0:EE:DF sdhci-bcm2708.emmc_clock_freq=100000000 vc_mem.mem_base=0x1c000000 vc_mem.mem_size=0x20000000  dwc_otg.lpm_enable=0 kgdboc=ttyAMA0,115200 console=ttyS0 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait" -S -s 

#qemu-system-arm -d guest_errors -kernel ../build/kernel.elf -cpu arm1176 -m 1G -smp 4 -M raspi2 -nographic -no-reboot -chardev stdio,mux=on,id=terminal -serial chardev:terminal -monitor chardev:terminal  -dtb ./bcm2709-rpi-2-b.dtb -append "rw earlyprintk loglevel=8 panic=120 keep_bootcon rootwait  dwc_otg.lpm_enable=0 kgdboc=ttyAMA0,115200 console=ttyS0 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait" -S -s 


#qemu-system-arm -M raspi2 -kernel ../build/kernel.elf -nographic -no-reboot -append "rw earlyprintk loglevel=8 console=ttyAMA0 root=/dev/mmcblk0p2" -chardev stdio,mux=on,id=terminal -serial chardev:terminal -monitor chardev:terminal -S -s

#qemu-system-arm -M raspi2 -kernel ../build/kernel.elf -append "rw earlyprintk loglevel=8 console=ttyAMA0,115200 dwc_otg.lpm_enable=0 root=/dev/mmcblk0p2 kgdboc=ttyAMA0,115200 console=ttyS0" -dtb 2015-11-21-raspbian-boot/bcm2709-rpi-2-b.dtb -serial stdio

# alternative:
# qemu-system-arm -singlestep -d exec,int -icount 0 -kernel ../build/kernel.elf ... ...


stty sane
