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
        static float getSpringConstant() { return spring_constant; }
        static void setSpringConstant(float new_spring_constant) { spring_constant = new_spring_constant; }
        static float getRestLength() { return rest_length; }
        static void setRestLength(float new_rest_length) { rest_length = new_rest_length; }
    private:
        static float spring_constant;
        static float rest_length;
        glm::vec3 fixedPosition;
    };
}

#endif