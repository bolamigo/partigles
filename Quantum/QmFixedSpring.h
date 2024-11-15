#pragma once
#ifndef QMFIXEDSPRING_H
#define QMFIXEDSPRING_H

#include "QmForceGenerator.h"
#include <glm/glm.hpp>

namespace Quantum {
    class QmFixedSpring : public QmForceGenerator {
    public:
        QmFixedSpring(const glm::vec3& fixedPos);
        virtual void update(QmParticle* particle);
        void setFixedPosition(const glm::vec3& newFixedPosition) { fixedPosition = newFixedPosition; }
    private:
        glm::vec3 fixedPosition;
    };
}

#endif