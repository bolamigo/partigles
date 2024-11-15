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

		static float getSpringConstant() { return spring_constant; }
		static void setSpringConstant(float new_spring_constant) { spring_constant = new_spring_constant; }
		static float getRestLength() { return rest_length; }
		static void setRestLength(float new_rest_length) { rest_length = new_rest_length; }
	private:
		static float spring_constant;
		static float rest_length;
		QmParticle* other;
	};
}

#endif