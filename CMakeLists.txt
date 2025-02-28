cmake_minimum_required(VERSION 3.22)
set(BOARD teensy41)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nxpcup)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENV{ZEPHYR_BASE} ${ZEPHYR_BASE})

set(CMAKE_GENERATOR "Ninja")

include_directories(NXP_hal)

target_sources(app PRIVATE 
                NXP_hal/NXP_GPIO.cpp
                NXP_hal/NXP_Blinky.cpp
                NXP_hal/NXP_USB.cpp
                NXP_hal/NXP_ADC.cpp
                NXP_hal/NXP_Camera.cpp
                NXP_hal/NXP_PWM.cpp
                NXP_hal/NXP_Servo.cpp
                NXP_hal/NXP_Algorithm.cpp

                src/main.cpp
            )
