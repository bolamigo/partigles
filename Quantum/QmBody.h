#pragma once
#ifndef QMBODY_H
#define QMBODY_H

#include <glm/glm.hpp>

namespace Quantum {

	const int TYPE_PARTICLE = 0;
	
	class QmBody {
		public:
			virtual void integrate(float) {};
			virtual void resetForces() {};
			virtual void addForce(glm::vec3) {};
			virtual void addAcceleration(glm::vec3) {};
			int getType() const { return type; }

		protected:
			int type;
		private:
	};
}

#endif