#include "stdafx.h"
#include "QmSpring.h"
#include "QmParticle.h"

namespace Quantum {
    float QmSpring::spring_constant = 10.f;
    float QmSpring::rest_length = 1.f;

    QmSpring::QmSpring(QmParticle* otherParticle) : other(otherParticle) {}

    void QmSpring::update(QmParticle* particle) {
        if (!other || !particle) return;

        glm::vec3 displacement = particle->getPosition() - other->getPosition();
        float distance = glm::length(displacement);
        glm::vec3 direction = glm::normalize(displacement);

        float forceMagnitude = -spring_constant * (distance - rest_length);
        glm::vec3 force = direction * forceMagnitude;

        particle->addForce(force);
        // other->addForce(-force); // This destroys everything, should be fixed with RK4
    }
}