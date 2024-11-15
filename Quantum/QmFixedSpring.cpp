#include "stdafx.h"
#include "QmFixedSpring.h"
#include "QmParticle.h"

constexpr float SPRING_CONSTANT = 10.f;
constexpr float REST_LENGTH = 1.f;

namespace Quantum {
    QmFixedSpring::QmFixedSpring(const glm::vec3& fixedPos) : fixedPosition(fixedPos) {}

    void QmFixedSpring::update(QmParticle* particle) {
        glm::vec3 distance = particle->getPosition() - fixedPosition;
        float length = glm::length(distance);
        float coefficient = -(length - REST_LENGTH) * SPRING_CONSTANT;
        particle->addForce(glm::normalize(distance) * coefficient);
    }
}
