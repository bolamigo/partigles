#pragma once
#ifndef QMFORCEREGISTRY_H
#define QMFORCEREGISTRY_H

#include "QmParticle.h"
#include "QmForceGenerator.h"

namespace Quantum {
	class QmForceRegistry {
	public:
		QmParticle* particle;
		QmForceGenerator* forceGenerator;
		QmForceRegistry(QmParticle*, QmForceGenerator*);
	};
}

#endif