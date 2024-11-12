#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistry.h"
#include "QmDrag.h"
#include "QmMagnetism.h"

constexpr float GRAVITY_EARTH = 9.81f;

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

void QmWorld::simulate(float t)
{
	resetBodies();
	applyGravity();
	updateForces();
	integrate(t);
}

float QmWorld::tick(float t)
{
	resetBodies();
	applyGravity();
	updateForces();
	integrate(t);
	ticktime += t;
	return time - ticktime;
}

void QmWorld::interpolate(float dt)
{

}

/*
void QmWorld::simulate(float t)
{

}
*/

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

void QmWorld::integrate(float t)
{
	time += t;
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

