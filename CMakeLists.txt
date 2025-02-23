cmake_minimum_required(VERSION 3.22)
set(BOARD teensy41)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nxpcup)

set(ENV{ZEPHYR_BASE} ${ZEPHYR_BASE})

set(CMAKE_GENERATOR "Ninja")

include_directories(inc)

target_sources(app PRIVATE 
                src/main.c
                src/blink.c
                src/adc.c
                src/usb.c
                src/camera.c
                src/led.c
                src/gpio.c
                src/servo.c
                src/algorithm.c
                src/pwm.c
                )

