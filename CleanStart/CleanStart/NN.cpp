#include "NN.h"

NNetwork::NNetwork(int inputLayerSize, int hiddenLayerSize) : inputLayer(inputLayerSize + 1), hiddenLayer(hiddenLayerSize + 1)
{
	initializeInputMatrix(-0.02, 0.02);
	initializeHiddenMatrix(-0.02, 0.02);

	inputLayer[0] = 1.0;
	hiddenLayer[0] = 1.0;
}

void NNetwork::initializeInputMatrix(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);

	for (int i = 0; i < hiddenLayer.size() - 1; i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < inputLayer.size(); j++)
		{
			temp.push_back(dis(gen));
		}
		WinputMatrix.push_back(temp);
	}


}

void NNetwork::initializeHiddenMatrix(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);

	for (int i = 0; i < hiddenLayer.size(); i++)
	{
		WhiddenMatrix.push_back(dis(gen));
	}
}

void NNetwork::setInput(const std::vector<double>& input)
{
	for (int i = 1; i < inputLayer.size(); i++)
		inputLayer[i] = input[i - 1];
}

void NNetwork::calculateHidden()
{
	for (int i = 1; i < hiddenLayer.size(); i++)
	{
		hiddenLayer[i] = sigmoidFunc(weightInputSum(inputLayer, WinputMatrix[i - 1]));
	}
}

void NNetwork::calculateOutput()
{
	outputNeuron = sigmoidFunc(weightInputSum(hiddenLayer, WhiddenMatrix));
}

double NNetwork::weightInputSum(const std::vector<double>& input, const std::vector<double>& weights)
{
	double sum = 0.0;
	for (int i = 0; i < input.size(); i++)
	{
		sum += input[i] * weights[i];
	}
	return sum;
}

double NNetwork::sigmoidFunc(double x)
{
	return 1 / (1 + exp(-x));
}


// eSignal = soll - ist
void NNetwork::backProb(double eSignal, double learnRate)
{
	double deltOutput = deltaOutput(eSignal);
	std::vector<double> deltaWOH = calcDeltaWHOmatrix(learnRate, deltOutput);
	std::vector<double> deltaHidd = deltaHidden(deltOutput);
	std::vector<std::vector<double>> deltaWHI;
	calcDeltaWHImatrix(learnRate, deltaHidd, deltaWHI);
	newWeightsHidden(deltaWOH);
	newWeihgtsInput(deltaWHI);
}

double NNetwork::deltaOutput(double eSignal)
{
	return (outputNeuron * (1 - outputNeuron)) * eSignal;
}

std::vector<double> NNetwork::calcDeltaWHOmatrix(double learnRate, double deltaOut)
{
	std::vector<double> deltaWHOmatrix;
	for (int i = 0; i < hiddenLayer.size(); i++)
		deltaWHOmatrix.push_back(deltaOut * hiddenLayer[i] * learnRate);
	return deltaWHOmatrix;
}

std::vector<double> NNetwork::deltaHidden(double deltaOut)
{
	std::vector<double> deltaHidden;
	for (int i = 1; i < hiddenLayer.size(); i++)
		deltaHidden.push_back((hiddenLayer[i] * (1 - hiddenLayer[i])) * (deltaOut * WhiddenMatrix[i]));

	return deltaHidden;
}

void NNetwork::calcDeltaWHImatrix(double learnRate, const std::vector<double>& deltaHidden, std::vector<std::vector<double>>& deltaWHI)
{
	for (int i = 0; i < deltaHidden.size(); i++)
	{
		std::vector<double> delWHIrow;
		for (int j = 0; j < inputLayer.size(); j++)
		{
			delWHIrow.push_back(deltaHidden[i] * inputLayer[j] * learnRate);
		}
		deltaWHI.push_back(delWHIrow);
	}
}

void NNetwork::newWeightsHidden(const std::vector<double>& deltaWOH)
{
	for (int i = 0; i < WhiddenMatrix.size(); i++)
		WhiddenMatrix[i] += deltaWOH[i];
}

void NNetwork::newWeihgtsInput(const std::vector<std::vector<double>>& deltaWHI)
{
	for (int i = 0; i < WinputMatrix.size(); i++)
		for (int j = 0; j < WinputMatrix[0].size(); j++)
			WinputMatrix[i][j] += deltaWHI[i][j];
}

void NNetwork::printWeightMatrix()
{

	for (int i = 0; i < WinputMatrix.size(); i++)
	{
		for (int j = 0; j < WinputMatrix[0].size(); j++)
		{
			std::cout << WinputMatrix[i][j] << " , ";
		}
		std::cout << "\n";
	}


}

void NNetwork::printHiddenLayer()
{
	for (int i = 0; i < hiddenLayer.size(); i++)
		std::cout << hiddenLayer[i] << " , ";
	std::cout << "\n";
}

void NNetwork::printInputLayer()
{
	for (int i = 0; i < inputLayer.size(); i++)
		std::cout << inputLayer[i] << " , ";
	std::cout << "\n";
}
void NNetwork::printOutputMatrix()
{
	for (int i = 0; i < WhiddenMatrix.size(); i++)
		std::cout << WhiddenMatrix[i] << " , ";
	std::cout << "\n";
}
void NNetwork::printOutput()
{
	std::cout << outputNeuron << "\n";
}

double NNetwork::getOutput()
{
	return outputNeuron;
}

double NNetwork::getValue(const std::vector<double>& input)
{
	setInput(input);
	calculateHidden();
	calculateOutput();

	return outputNeuron;
}

void NNetwork::printStat(const std::vector<double>& input)
{
	for (int i = 0; i < input.size(); i++)
		std::cout << input[i] << ",";
	std::cout << "\n";
}

std::vector<std::vector<double>> NNetwork::returnInputMatrix()
{
	return WinputMatrix;
}

std::vector<double> NNetwork::returnHiddenMatrix()
{
	return WhiddenMatrix;
}

void NNetwork::setInputMatrix(std::vector<std::vector<double>> matrix)
{
	WinputMatrix = matrix;
}

void NNetwork::setHiddenMatrix(std::vector<double> matrix)
{
	WhiddenMatrix = matrix;
}
