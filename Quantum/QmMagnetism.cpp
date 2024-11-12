#include "stdafx.h"
#include "QmMagnetism.h"
#include "QmParticle.h"
#include "QmWorld.h"

constexpr float K = 100.f;
constexpr float EPSILON = .01f;

bool isPointerMagnetic = false;

using namespace Quantum;

void QmMagnetism::update(QmParticle* particle)
{
    QmWorld* world = particle->getWorld();
    if (!world) return;

    for (QmBody* body : world->getBodies()) {
        QmParticle* otherParticle = dynamic_cast<QmParticle*>(body);
        if (otherParticle && otherParticle != particle) {
            glm::vec3 distance = particle->getPosition() - otherParticle->getPosition();
            float distanceSquared = glm::dot(distance, distance) + EPSILON;
            float coefficient = K * (particle->getCharge() * otherParticle->getCharge()) / distanceSquared;
            particle->addForce(glm::normalize(distance) * coefficient);
        }
    }

    // Apply magnetic force from mouse pointer if it is magnetic
    if (*isPointerMagnetic && pointerPosition) {
        glm::vec3 distance = particle->getPosition() - *pointerPosition;
        float distanceSquared = glm::dot(distance, distance) + EPSILON;
        float coefficient = K * (particle->getCharge() * (*pointerCharge)) / distanceSquared;
        particle->addForce(glm::normalize(distance) * coefficient);
    }
}
