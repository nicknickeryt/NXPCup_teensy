#ifndef NXP_ALGORITHM_HPP
#define NXP_ALGORITHM_HPP

#include "NXP_Camera.hpp"

// TODO proc method & thread or sth...
class NXP_Algorithm {
   public:
    NXP_Algorithm(NXP_Camera &camera);
    int32_t calculatePosition();

   private:
    NXP_Camera &algorithmCamera;
};

#endif  // NXP_ALGORITHM_HPP
