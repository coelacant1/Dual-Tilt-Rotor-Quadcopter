#include <NonlinearCombiner.h>


NonlinearCombiner::NonlinearCombiner(double amplification, double damping) {
	this->amplificationCoefficient = amplification;
	this->dampingCoefficient = damping;
}

double NonlinearCombiner::Combine(Output output, double b0, ExtendedStateObserver::State state, double precisionCoefficient) {
	double e1, e2, u0;

	e1 = output.Current - state.Z1;
	e2 = output.Previous - state.Z2;

	u0 = -SetPointJumpPrevention(e1, dampingCoefficient * e2, amplificationCoefficient, precisionCoefficient);

	//Contains disturbance rejection
	return (u0 + state.Z3) / b0;// b0 must be positive
}

double NonlinearCombiner::SetPointJumpPrevention(double target, double targetDerivative, double r0, double h) {
	double d, a, a0, a1, a2, y, sy, sa;

	d = pow(r0, 2) * h;
	a0 = h * targetDerivative;
	y = target + a0;

	a1 = sqrt(d * (d + 8 * abs(y)));
	a2 = a0 + Math::Sign(y) * (a1 - d) / 2;
	sy = (Math::Sign(y + d) - Math::Sign(y - d)) / 2;//returns 1, or -1

	a = (a0 + y - a2) * sy + a2;
	sa = (Math::Sign(a + d) - Math::Sign(a - d)) / 2;//returns 1, or -1

	return -r0 * ((a / d) - Math::Sign(a)) * sa - r0 * Math::Sign(a);
}