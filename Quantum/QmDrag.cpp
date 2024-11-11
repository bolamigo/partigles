#include "stdafx.h"
#include "QmDrag.h"

using namespace Quantum;

bool QmDrag::state = true;

QmDrag::QmDrag(float K1, float K2)
{
	k1 = K1;
	k2 = K2;
}

void QmDrag::update(QmParticle* particle)
{
	glm::vec3 velocity = particle->getVelocity();
	float velocityLength = length(velocity);
	float coefficient = -(k1 * velocityLength + k2 * velocityLength * velocityLength);
	particle->addForce(normalize(velocity) * coefficient);
}
