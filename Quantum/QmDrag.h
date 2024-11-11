#pragma once
#ifndef QMDRAG_H
#define QMDRAG_H

#include "QmForceGenerator.h"
#include <iostream>

namespace Quantum {
	class QmDrag : public QmForceGenerator {
	public:
		QmDrag(float, float);
		virtual void update(QmParticle*);
		static void switchState() {
			std::cout << "Drag o" << ((state = !state) ? "n" : "ff") << std::endl;
		};
		static bool isOn() { return state; };
	private:
		static bool state;
		float k1, k2;
	};
}

#endif