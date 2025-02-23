cmake_minimum_required(VERSION 3.22)
set(BOARD teensy41)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nxpcup)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENV{ZEPHYR_BASE} ${ZEPHYR_BASE})

set(CMAKE_GENERATOR "Ninja")

include_directories(inc)

target_sources(app PRIVATE 
                src/main.cpp
                src/blink.cpp
                src/adc.cpp
                src/usb.cpp
                src/camera.cpp
                src/led.cpp
                src/gpio.cpp
                src/servo.cpp
                src/algorithm.cpp
                src/pwm.cpp
                )

