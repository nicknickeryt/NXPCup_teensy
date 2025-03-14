#include "NXP_Algorithm.hpp"
#include "NXP_Camera.hpp"

#include <stdlib.h>

NXP_Algorithm::NXP_Algorithm(NXP_Camera& camera, NXP_Servo& servo)
    : algorithmCamera(camera), algorithmServo(servo) {}

// TODO optimize this pls
int32_t NXP_Algorithm::calculatePosition() {
    uint32_t* cameraBuffer = algorithmCamera.getCameraBufArr();
    // uint32_t cameraBuffer[CAMERA_ADC_SAMPLES];

    // k_msgq_get(&camera_msgq, &cameraBuffer, K_NO_WAIT);

    uint32_t smoothing[CAMERA_ADC_SAMPLES];
    uint32_t sobel[CAMERA_ADC_SAMPLES];
    uint32_t suppression[CAMERA_ADC_SAMPLES];

    smoothing[0] = ((cameraBuffer[0] * 2) + cameraBuffer[1]) / 3;
    smoothing[CAMERA_ADC_SAMPLES - 1] =
        ((cameraBuffer[CAMERA_ADC_SAMPLES - 1] * 2) +
         cameraBuffer[CAMERA_ADC_SAMPLES - 2]) /
        3;

    for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++) {
        sobel[i] = 0;
        suppression[i] = 0;
    }

    for (unsigned int i = 1; i < CAMERA_ADC_SAMPLES - 1; i++)
        smoothing[i] =
            (cameraBuffer[i - 1] + cameraBuffer[i] + cameraBuffer[i + 1]) / 3;

    for (unsigned int i = 2; i < CAMERA_ADC_SAMPLES - 2; i++)
        sobel[i] = abs(smoothing[i - 2] - smoothing[i + 2]);

    for (unsigned int i = 3; i < CAMERA_ADC_SAMPLES - 3; i++)
        suppression[i] = (sobel[i] > sobel[i - 1]) && (sobel[i] > sobel[i + 1])
                             ? sobel[i]
                             : 0;

    int max1_idx = -1, max2_idx = -1;
    uint32_t max1_val = 0, max2_val = 0;

    for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++) {
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
    int steeringError = mean - ((CAMERA_ADC_SAMPLES / 2) - 1);

    return steeringError;
}

void NXP_Algorithm::proc() {
    while (1) {
        int steeringError = this->calculatePosition();
        algorithmServo.setDegrees(steeringError);

        k_msleep(1);
    }
}

void NXP_Algorithm::algorithmThreadWrapper(void* arg1, void* arg2, void* arg3) {
    static_cast<NXP_Algorithm*>(arg1)->proc();
}