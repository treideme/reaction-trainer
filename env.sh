#!/bin/bash
# FIXME: Change these to your target environment
# Then source it before invoking scons
# . env.sh

export RTT_CC=gcc
export RTT_EXEC_PATH=/opt/wch-riscv-embedded-gcc/bin/
export RTT_ROOT=$(pwd)/rt-thread/
export BSP_ROOT=$(pwd)
