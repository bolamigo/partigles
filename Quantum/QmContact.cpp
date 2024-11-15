#include "stdafx.h"
#include "QmContact.h"

using namespace Quantum;

Quantum::QmContact::QmContact(QmBody* firstBody, QmBody* secondBody, glm::vec3 normalVector, float depthValue)
{
	this->body1 = firstBody;
	this->body2 = secondBody;
	this->normal = normalVector;
	this->depth = depthValue;
}
