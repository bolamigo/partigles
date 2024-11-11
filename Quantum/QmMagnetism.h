#pragma once
#ifndef QMMAGNETISM_H
#define QMMAGNETISM_H

#include "QmForceGenerator.h"

namespace Quantum {
	class QmMagnetism : public QmForceGenerator {
	public:
		virtual void update(QmParticle*);
	};
}

#endif