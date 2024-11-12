#pragma once
#ifndef QMMAGNETISM_H
#define QMMAGNETISM_H

#include "QmForceGenerator.h"

namespace Quantum {
	class QmMagnetism : public QmForceGenerator {
    public:
        QmMagnetism(bool* isMagnetic, glm::vec3* pointerPos, float* charge)
            : isPointerMagnetic(isMagnetic), pointerPosition(pointerPos), pointerCharge(charge) {}

        virtual void update(QmParticle*);

    private:
        bool* isPointerMagnetic;
        glm::vec3* pointerPosition;
        float* pointerCharge;
    };
}

#endif