#include <math.h>

class Neuron
{
public:
	Neuron() {}

	double sigmoid(double x);

	void setInput(double x);
	void setOutput(int x);
	void setDelta(double d);
	double getInput();
	double getOutput();
	double getDelta();

private:
	double input;
	double output;
	double delta;
};




#pragma once
