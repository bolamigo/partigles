#pragma once
#ifndef QMFORCEGENERATOR_H
#define QMFORCEGENERATOR_H

#include "QmParticle.h"

namespace Quantum {
	class QmForceGenerator {
	public:
		virtual void update(QmParticle*) {};
	};
}

#endif