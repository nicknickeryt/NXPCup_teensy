#ifndef NXP_ALGORITHM_HPP
#define NXP_ALGORITHM_HPP

#include "NXP_Camera.hpp"
#include "NXP_Servo.hpp"

// TODO proc method & thread or sth...
class NXP_Algorithm {
   public:
    NXP_Algorithm(NXP_Camera &camera, NXP_Servo &servo);
    int32_t calculatePosition();
    void proc();

    static void algorithmThreadWrapper(void *arg1, void *arg2, void *arg3);

   private:
    NXP_Camera &algorithmCamera;
    NXP_Servo &algorithmServo;
};

#endif  // NXP_ALGORITHM_HPP
