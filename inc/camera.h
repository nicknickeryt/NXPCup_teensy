#ifndef __CAMERA_H
#define __CAMERA_H

#include <stdarg.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define CAMERA_CLK_GPIO_PORT GPIO_PORT_6
#define CAMERA_CLK_GPIO_PIN 3

#define DELAY_US 36
#define DELAY_US_HALF DELAY_US / 2

#define CAMERA_ADC_SAMPLES 128
#define CAMERA_ADC_CHANNEL 0
#define CAMERA_ADC_DEVICE adc_channels[CAMERA_ADC_CHANNEL].dev

#define CAMERA_MSG_LEN sizeof(cameraBufArr)
#define CAMERA_MSG_ARRAY_SIZE 4

#define GPIO_SI_INPUT_PIN 0

extern bool cameraInterruptState;

extern uint32_t cameraBufArr[CAMERA_ADC_SAMPLES];
extern struct k_msgq camera_dat_msgq;

void setupCamera();
void cameraSetCLKPin();
void cameraResetCLKPin();

void cameraProc();

void cameraDebugPrintk();
void cameraDebugPrintkLoop();

#endif /* __CAMERA_H */