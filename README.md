# RT-Thread on CH32V208
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
