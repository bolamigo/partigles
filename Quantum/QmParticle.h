#ifndef QMPARTICLE_H
#define QMPARTICLE_H

#include <glm/glm.hpp>
#include "QmBody.h"
#include "QmWorld.h"

namespace Quantum {
	class QmUpdater;

	class QmParticle : public QmBody {
	public:
		QmParticle();
		QmParticle(glm::vec3, glm::vec3, glm::vec3);
		~QmParticle();
		virtual void integrate(float);
		virtual void resetForces();
		
		glm::vec3 getAcceleration();
		glm::vec3 getVelocity();
		glm::vec3 getPosition();
		float getInvMass() const { return invMass; };

		void setMass(float);
		void setUpdater(QmUpdater*);
		void addForce(glm::vec3);
		void addAcceleration(glm::vec3);

		void setCharge(float charge) { this->charge = charge; }
		float getCharge() const { return charge; }
		void setWorld(QmWorld* world) { this->world = world; }
		QmWorld* getWorld() const { return world; }
	private:
		QmUpdater* updater;
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float invMass;
		glm::vec3 forceAccumulator;

		float damping;
		glm::vec3 damp(glm::vec3);

		float charge;
		QmWorld* world;
	};
}

#endif