#pragma once
#ifndef QMSPRING_H
#define QMSPRING_H

#include "QmForceGenerator.h"

namespace Quantum {
	class QmSpring : public QmForceGenerator {
	public:
		virtual void update(QmParticle*);
	};
}

#endif