#pragma once
#ifndef QMAABB_H
#define QMAABB_H

#include <glm/glm.hpp>

namespace Quantum {
	class QmAABB {
	public:
		QmAABB(glm::vec3 minimum, glm::vec3 maximum);
	private:
		glm::vec3 min;
		glm::vec3 max;
	};
}

#endif