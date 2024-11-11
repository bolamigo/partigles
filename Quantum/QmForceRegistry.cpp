#include "stdafx.h"
#include "Quantum.h"
#include "QmForceRegistry.h"

using namespace Quantum;

QmForceRegistry::QmForceRegistry(QmParticle* qmParticle, QmForceGenerator* qmForceGenerator)
{
	particle = qmParticle;
	forceGenerator = qmForceGenerator;
}
