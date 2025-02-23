#include "algorithm.h"

#include <assert.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

#include "adc.h"
#include "camera.h"
#include "servo.h"

int32_t algorithmCalculatePosition(uint32_t* cameraBuffer) {
    uint32_t smoothing[128];
    uint32_t sobel[128];
    uint32_t suppression[128];

    smoothing[0] = ((cameraBuffer[0] * 2) + cameraBuffer[1]) / 3;
    smoothing[127] = ((cameraBuffer[127] * 2) + cameraBuffer[126]) / 3;

    for (unsigned int i = 0; i < 128; i++) {
        sobel[i] = 0;
        suppression[i] = 0;
    }

    for (unsigned int i = 1; i < 127; i++)
        smoothing[i] =
            (cameraBuffer[i - 1] + cameraBuffer[i] + cameraBuffer[i + 1]) / 3;

    for (unsigned int i = 2; i < 126; i++)
        sobel[i] = abs(smoothing[i - 2] - smoothing[i + 2]);

    for (unsigned int i = 3; i < 125; i++)
        suppression[i] = (sobel[i] > sobel[i - 1]) && (sobel[i] > sobel[i + 1])
                             ? sobel[i]
                             : 0;

    int max1_idx = -1, max2_idx = -1;
    uint32_t max1_val = 0, max2_val = 0;

    for (unsigned int i = 0; i < 128; i++) {
        if (suppression[i] > max1_val) {
            max2_val = max1_val;
            max2_idx = max1_idx;
            max1_val = suppression[i];
            max1_idx = i;
        } else if (suppression[i] > max2_val) {
            max2_val = suppression[i];
            max2_idx = i;
        }
    }

    float mean = (max1_idx + max2_idx) / 2;
    int steeringError = mean - 63;

    return steeringError;
}