#include "Neuron.h"



double Neuron::sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

void Neuron::setInput(double x)
{
	input = x;
}

void Neuron::setOutput(int x)
{
	switch (x)
	{
	case 0:
		output = input;
		break;
	case 1:
		output = sigmoid(input);
		break;
	default:
		break;
	}

}

void Neuron::setDelta(double d)
{
	delta = d;
}

double Neuron::getInput()
{
	return input;
}

double Neuron::getOutput()
{
	return output;
}

double Neuron::getDelta()
{
	return delta;
}
