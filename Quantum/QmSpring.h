#pragma once
#ifndef QMSPRING_H
#define QMSPRING_H

#include "QmForceGenerator.h"
#include "QmParticle.h"

namespace Quantum {
	class QmSpring : public QmForceGenerator {
	public:
		QmSpring(QmParticle* otherParticle);
		virtual void update(QmParticle* particle);

		QmParticle* getOtherParticle() const { return other; }

	private:
		QmParticle* other;
	};
}

#endif