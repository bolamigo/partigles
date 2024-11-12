#pragma once
#ifndef QMMAGNETISM_H
#define QMMAGNETISM_H

#include "QmForceGenerator.h"
#include <iostream>

namespace Quantum {
	class QmMagnetism : public QmForceGenerator {
    public:
        QmMagnetism(bool* isMagnetic, glm::vec3* pointerPos, float* charge)
            : isPointerMagnetic(isMagnetic), pointerPosition(pointerPos), pointerCharge(charge) {}

        virtual void update(QmParticle*);
        static void switchState() {
            std::cout << "Magnetism o" << ((state = !state) ? "n" : "ff") << std::endl;
        };
        static bool isOn() { return state; };
    private:
        static bool state;
        bool* isPointerMagnetic;
        glm::vec3* pointerPosition;
        float* pointerCharge;
    };
}

#endif