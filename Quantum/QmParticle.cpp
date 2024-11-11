#include "stdafx.h"

#include "QmParticle.h"
#include "QmUpdater.h"

constexpr float DAMPING = .995;
constexpr float VELOCITY_MIN = .01;

using namespace Quantum;

QmParticle::QmParticle() : position(0, 0, 0), velocity(0, 0, 0), acceleration(0, 0, 0)
{
	type = TYPE_PARTICLE;
	invMass = 0.;
	damping = DAMPING;
	forceAccumulator = glm::vec3(0, 0, 0);
	updater = NULL;
}

QmParticle::QmParticle(glm::vec3 pos, glm::vec3 vel, glm::vec3 acc) : QmParticle()
{
	type = TYPE_PARTICLE;
	invMass = 0.;
	position = pos;
	velocity = vel;
	acceleration = acc;
	damping = DAMPING;
	forceAccumulator = glm::vec3(0, 0, 0);
	updater = NULL;
}

QmParticle::~QmParticle()
{
	delete updater;
}

glm::vec3 QmParticle::damp(glm::vec3 velocity) {
	if (length(velocity) <= VELOCITY_MIN)
		return velocity;

	return damping * velocity;
}

void QmParticle::integrate(float time)
{
	acceleration += forceAccumulator * invMass;
	velocity += time * acceleration;

	velocity = damp(velocity);

	position += time * velocity;

	if (updater != NULL)
		updater->update(position);
}

glm::vec3 QmParticle::getAcceleration()
{
	return this->acceleration;
}

glm::vec3 QmParticle::getVelocity()
{
	return this->velocity;
}

glm::vec3 QmParticle::getPosition()
{
	return this->position;
}

void Quantum::QmParticle::setMass(float mass)
{
	this->invMass = 1 / mass;
}

void QmParticle::setUpdater(QmUpdater* updater)
{
	this->updater = updater;
}

void QmParticle::addForce(glm::vec3 force)
{
	this->forceAccumulator += force;
}

void QmParticle::addAcceleration(glm::vec3 vector)
{
	this->acceleration += vector;
}

void QmParticle::resetForces()
{
	this->acceleration = glm::vec3(0, 0, 0);
	this->forceAccumulator = glm::vec3(0, 0, 0);
}

