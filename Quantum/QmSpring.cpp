#include "stdafx.h"
#include "QmSpring.h"
#include "QmParticle.h"

constexpr float SPRING_CONSTANT = 10.f;
constexpr float REST_LENGTH = 1.f;

namespace Quantum {
    QmSpring::QmSpring(QmParticle* otherParticle) : other(otherParticle) {}

    void QmSpring::update(QmParticle* particle) {
        if (!other || !particle) return;

        glm::vec3 displacement = particle->getPosition() - other->getPosition();
        float distance = glm::length(displacement);
        glm::vec3 direction = glm::normalize(displacement);

        float forceMagnitude = -SPRING_CONSTANT * (distance - REST_LENGTH);
        glm::vec3 force = direction * forceMagnitude;

        particle->addForce(force);
        // other->addForce(-force); // This destroys everything
    }
}