#include <ADRC.h>

ADRC::ADRC(double amplification, double damping, double plant, double precisionModifier, PID pid) {
	this->amplification = amplification;
	this->damping = damping;
	this->plant = plant;
	this->precisionModifier = precisionModifier;
}

double ADRC::Calculate(double setpoint, double processVariable, double dT) {
	precision = dT * precisionModifier;

	double pdValue = pid.Calculate(setpoint, processVariable, dT);

	NonlinearCombiner::Output currentOutput = NonlinearCombiner::Output{
		pid.Calculate(setpoint, processVariable, dT),
		output.Previous
	};

	ExtendedStateObserver::State state = eso.ObserveState(dT, output.Current, plant, processVariable);//double u, double y, double b0

	output.Previous = output.Current;
	output.Current = nlc.Combine(currentOutput, plant, state, precision);

	return output.Current;
}
