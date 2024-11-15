#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistry.h"
#include "QmDrag.h"
#include "QmMagnetism.h"
#include "QmUpdater.h"

constexpr float GRAVITY_EARTH = 9.81f;
constexpr float DELTA = 1.f/120;

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	time = 0.f;
	ticktime = 0.f;
	gravity = glm::vec3(0, -GRAVITY_EARTH, 0);
	gravityOn = true;
}

QmWorld::~QmWorld()
{

}

/* DEPRECATED
void QmWorld::simulate(float t)
{
	resetBodies();
	applyGravity();
	updateForces();
	integrate(t);
}
*/

float QmWorld::tick(float t)
{
	resetBodies();
	applyGravity();
	updateForces();
	integrate(t);
	ticktime += t;
	resolve(narrowphase(broadphase()));
	return time - ticktime;
}

void QmWorld::interpolate(float dt)
{
	for (QmBody* body : bodies) {
		QmParticle* particle = dynamic_cast<QmParticle*>(body);
		if (particle && particle->getUpdater()) {
			glm::vec3 interpolatedPos = particle->getPosition() + dt * particle->getVelocity();
			particle->getUpdater()->update(interpolatedPos);
		}
	}
}

void QmWorld::simulate(float t)
{
	time += t;
	float dt = time - ticktime;

	if (useDELTA)
	{
		while (dt >= DELTA)
			dt = tick(DELTA);
		interpolate(dt);
	}
	else
	{
		tick(t);
		interpolate(0);
	}
}

void QmWorld::resetBodies()
{
	for (QmBody* body : bodies)
		body->resetForces();
}

void QmWorld::applyGravity()
{
	for (QmBody* body : bodies)
		if (body->getType() == TYPE_PARTICLE)
			body->addAcceleration(gravity);
}

void QmWorld::updateForces()
{
	for (QmForceRegistry* forceRegistry : forceRegistries)
	{
		bool isForceOn = true;

		QmDrag* dragForce = dynamic_cast<QmDrag*>(forceRegistry->forceGenerator);
		if (dragForce)
			isForceOn = dragForce->isOn();
		QmMagnetism* magnetismForce = dynamic_cast<QmMagnetism*>(forceRegistry->forceGenerator);
		if (magnetismForce)
			isForceOn = magnetismForce->isOn();

		if (isForceOn && forceRegistry->forceGenerator && forceRegistry->particle)
			forceRegistry->forceGenerator->update(forceRegistry->particle);
	}
}

std::vector<QmContact> Quantum::QmWorld::broadphase()
{
	return std::vector<QmContact>();
}

std::vector<QmContact> Quantum::QmWorld::narrowphase(std::vector<QmContact> c)
{
	return std::vector<QmContact>();
}

void Quantum::QmWorld::resolve(std::vector<QmContact> c)
{
}

void QmWorld::integrate(float t)
{
	for (QmBody* b : bodies)
		b->integrate(t);
}

void QmWorld::addBody(QmBody* body)
{
	bodies.push_back(body);
	QmParticle* particle = dynamic_cast<QmParticle*>(body);
	if (particle)
		particle->setWorld(this);
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
}

void QmWorld::toggleGravity()
{
	std::cout << "Gravity o" << ((gravityOn = !gravityOn) ? "n" : "ff") << std::endl;
	gravity.y = gravityOn * -GRAVITY_EARTH;
}

void Quantum::QmWorld::addForceRegistry(QmForceRegistry* forceRegistry)
{
	forceRegistries.push_back(forceRegistry);
}

void QmWorld::clear()
{
	for (QmForceRegistry* registry : forceRegistries)
	{
		delete registry->forceGenerator;
		delete registry;
	}
	forceRegistries.clear();

	for (QmBody* b : bodies)
		delete b;
	bodies.clear();
}

