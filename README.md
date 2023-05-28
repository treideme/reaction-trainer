# Next-Generation Reaction Trainer

![Concept Rendition](doc/img/reaction-trainer.jpg)

This repository contains the firmware sources for the RT-Thread IoT contest. 

See [my blog post](https://www.reidemeister.com/?p=624) for more information and build updates.

 * [Hardware Designs](https://github.com/treideme/reaction-trainer-hw)
 * [Android Application Sources](https://github.com/treideme/reaction-trainer-app)

This repository will contains.

## Build setup
 * Commandline setup
```
add rt-thread repo as dependency
```
 * Setup environment script
```
. env.sh
# First time before running menuconfig
touch rtconfig.h # <- needed for scons not to fail before menu config
# Configure environment
scons --menuconfig
```
 * Build with
```
scons --verbose
```
 * If chip protection is disabled, flash with [Wlink](https://github.com/ch32-rs/wlink)
```
./wlink -v flash --address 0x08000000
```


----
(C) 2023 Thomas Reidemeister, released under [Apache 2](LICENSE).
