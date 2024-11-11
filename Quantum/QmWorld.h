#ifndef QMWORLD_H
#define QMWORLD_H

#include <list>
#include <vector>
#include <glm/glm.hpp>

namespace Quantum {

	class QmBody;
	class QmForceRegistry;

	class QmWorld {
	public:
		QmWorld();
		~QmWorld();
		void simulate(float);
		void addBody(QmBody*);
		std::vector<QmBody*> getBodies();
		void addForceRegistry(QmForceRegistry*);
		void toggleGravity();
		bool isGravityOn() const { return gravityOn; };

		void clear();
	private:
		float time;
		std::vector<QmBody*> bodies;
		std::vector<QmForceRegistry*> forceRegistries;
		glm::vec3 gravity;
		bool gravityOn;
		void integrate(float);
		void resetBodies();
		void applyGravity();
		void updateForces();
	};

}

#endif