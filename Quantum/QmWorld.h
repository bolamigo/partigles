#ifndef QMWORLD_H
#define QMWORLD_H

#include <list>
#include <vector>
#include <glm/glm.hpp>
#include "QmContact.h"

namespace Quantum {

	class QmBody;
	class QmForceRegistry;

	class QmWorld {
	public:
		QmWorld();
		~QmWorld();
		void simulate(float);
		float tick(float);
		void addBody(QmBody*);
		std::vector<QmBody*> getBodies();
		void addForceRegistry(QmForceRegistry*);
		void toggleGravity();
		bool isGravityOn() const { return gravityOn; };
		bool useDELTA = false;
		void clear();
		std::vector<QmForceRegistry*> getForceRegistries() { return forceRegistries; }
	private:
		float time;
		float ticktime;
		std::vector<QmBody*> bodies;
		std::vector<QmForceRegistry*> forceRegistries;
		glm::vec3 gravity;
		bool gravityOn;
		void integrate(float);
		void interpolate(float);
		void resetBodies();
		void applyGravity();
		void updateForces();
		std::vector<QmContact> broadphase();
		std::vector<QmContact> narrowphase(std::vector<QmContact> c);
		void resolve(std::vector<QmContact> c);
	};

}

#endif