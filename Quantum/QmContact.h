#pragma once
#ifndef QMCONTACT_H
#define QMCONTACT_H

#include "QmBody.h"

namespace Quantum {
	class QmContact {
	public:
		QmContact(QmBody* firstBody, QmBody* secondBody, glm::vec3 normalVector, float depthValue);
	private:
		QmBody* body1;
		QmBody* body2;
		glm::vec3 normal;
		float depth;
	};
}

#endif