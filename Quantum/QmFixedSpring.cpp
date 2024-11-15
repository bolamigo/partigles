#include "stdafx.h"
#include "QmFixedSpring.h"
#include "QmParticle.h"

namespace Quantum {
    float QmFixedSpring::spring_constant = 10.f;
    float QmFixedSpring::rest_length = 1.f;

    QmFixedSpring::QmFixedSpring(const glm::vec3& fixedPos) : fixedPosition(fixedPos) {}

    void QmFixedSpring::update(QmParticle* particle) {
        glm::vec3 distance = particle->getPosition() - fixedPosition;
        float length = glm::length(distance);
        float coefficient = -(length - rest_length) * spring_constant;
        particle->addForce(glm::normalize(distance) * coefficient);
    }
}
