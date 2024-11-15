#include "stdafx.h"
#include "QmAABB.h"

using namespace Quantum;

QmAABB::QmAABB(glm::vec3 minimum, glm::vec3 maximum)
{
	this->min = minimum;
	this->max = maximum;
}
