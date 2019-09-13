#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int inputLayerSize, int hiddenLayerSize, int hiddenLayersCount) : inputLayer(inputLayerSize + 1), hiddenLayers(hiddenLayersCount, std::vector<double>(hiddenLayerSize + 1)), deltaWH(hiddenLayersCount, std::vector<double>(hiddenLayerSize))
{
	initializeInputMatrix(-0.1, 0.1);
	initializeHiddenMatrix(-0.1, 0.1, hiddenLayersCount, hiddenLayerSize);
	initializeOuputMatrix(-0.1, 0.1);

	inputLayer[0] = 1.0;
	for (int i = 0; i < hiddenLayers.size(); i++)
		hiddenLayers[i][0] = 1.0;
}

void NeuralNetwork::initializeInputMatrix(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);

	for (int i = 0; i < hiddenLayers[0].size() - 1; i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < inputLayer.size(); j++)
		{
			temp.push_back(dis(gen));
		}
		WinputMatrix.push_back(temp);
	}


}

void NeuralNetwork::initializeHiddenMatrix(double from, double to, int countLayer, int countNeurons)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);
	for (int i = 0; i < countLayer - 1; i++)
	{
		Matrix weights;
		for (int k = 0; k < countNeurons; k++)
		{
			std::vector<double> temp;
			for (int j = 0; j < countNeurons + 1; j++)
			{
				temp.push_back(dis(gen));
			}
			weights.push_back(temp);
		}
		hiddenLayerWeights.push_back(weights);
	}
}

void NeuralNetwork::initializeOuputMatrix(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);

	for (int i = 0; i < hiddenLayers[0].size(); i++)
		outputLayerWeights.push_back(dis(gen));
}

void NeuralNetwork::setInput(const std::vector<double>& input)
{
//	std::cout << input.size();
	for (int i = 1; i < inputLayer.size(); i++)
		inputLayer[i] = input[i - 1];
}

void NeuralNetwork::calculateHidden()
{
	for (int i = 1; i < hiddenLayers[0].size(); i++)
	{
		hiddenLayers[0][i] = sigmoidFunc(weightInputSum(inputLayer, WinputMatrix[i - 1]));
	}

	for (int i = 1; i < hiddenLayers.size(); i++)
	{
		for (int j = 1; j < hiddenLayers[0].size(); j++)
		{
			hiddenLayers[i][j] = sigmoidFunc(weightInputSum(hiddenLayers[i - 1], hiddenLayerWeights[i - 1][j - 1]));
		}
	}
	/*for (int i = 1; i < hiddenLayer.size(); i++)
	{
		hiddenLayer[i] = sigmoidFunc(weightInputSum(inputLayer, WinputMatrix[i - 1]));
	}*/
}

void NeuralNetwork::calculateOutput()
{
	outputNeuron = sigmoidFunc(weightInputSum(hiddenLayers[hiddenLayers.size() - 1], outputLayerWeights));
}

double NeuralNetwork::weightInputSum(const std::vector<double>& input, const std::vector<double>& weights)
{
	double sum = 0.0;
	for (int i = 0; i < input.size(); i++)
	{
		sum += input[i] * weights[i];
	}
	return sum;
}

double NeuralNetwork::sumDeltaWeights(const std::vector<double>& input, Matrix& weights, int c)
{
	double sum = 0.0;
	for (int i = 0; i < input.size(); i++)
	{
		sum += input[i] * weights[i][c];
	}
	return sum;
}

double NeuralNetwork::sigmoidFunc(double x)
{
	return 1 / (1 + exp(-x));
}


// eSignal = soll - ist
void NeuralNetwork::backProb(double eSignal, double learnRate)
{
	double deltOutput = deltaOutput(eSignal);
	std::vector<double> deltaWOH = calcDeltaWHOmatrix(learnRate, deltOutput);
	calcDeltaWHmatrix(learnRate, deltOutput);
	calcDeltaHidden(learnRate);
	std::vector<std::vector<double>> deltaWHI;
	calcDeltaWHImatrix(learnRate, deltaWHI);
	newWeightsHidden(deltaWOH);
	newWeightsHiddenLayer();
	newWeihgtsInput(deltaWHI);
}

double NeuralNetwork::deltaOutput(double eSignal)
{
	return (outputNeuron * (1 - outputNeuron)) * eSignal;
}

void NeuralNetwork::calcDeltaWHmatrix(double learnRate, double deltaOut)
{
	for (int i = 1; i < hiddenLayers[0].size(); i++)
	{
		deltaWH[deltaWH.size() - 1][i - 1] = (hiddenLayers[hiddenLayers.size() - 1][i] * (1 - hiddenLayers[hiddenLayers.size() - 1][i])) * (deltaOut * outputLayerWeights[i]);
	}

	for (int i = deltaWH.size() - 2; i >= 0; i--)
	{
		for (int j = 1; j < hiddenLayers[0].size(); j++)
		{
			deltaWH[i][j - 1] = (hiddenLayers[i][j] * (1 - hiddenLayers[i][j]) * sumDeltaWeights(deltaWH[i + 1], hiddenLayerWeights[i], j));
		}
	}

}

std::vector<double> NeuralNetwork::calcDeltaWHOmatrix(double learnRate, double deltaOut)
{
	std::vector<double> deltaWHOmatrix;
	for (int i = 0; i < hiddenLayers[0].size(); i++)
		deltaWHOmatrix.push_back(hiddenLayers[hiddenLayers.size() - 1][i] * deltaOut * learnRate);
	return deltaWHOmatrix;
}

void NeuralNetwork::calcDeltaWHImatrix(double learnRate, std::vector<std::vector<double>>& deltaWHI)
{
	for (int i = 0; i < deltaWH[0].size(); i++)
	{
		std::vector<double> delWHIrow;
		for (int j = 0; j < inputLayer.size(); j++)
		{
			delWHIrow.push_back(deltaWH[0][i] * inputLayer[j] * learnRate);
		}
		deltaWHI.push_back(delWHIrow);
	}
}

void NeuralNetwork::calcDeltaHidden(double learnRate)
{
	deltaHiddenLayer.clear();
	for (int i = 0; i < hiddenLayerWeights.size(); i++)
	{
		Matrix weights;
		for (int k = 0; k < deltaWH[0].size(); k++)
		{
			std::vector<double> temp;
			for (int j = 0; j < hiddenLayers[0].size(); j++)
			{
				temp.push_back(deltaWH[i + 1][k] * hiddenLayers[i][j] * learnRate);
			}
			weights.push_back(temp);
		}
		deltaHiddenLayer.push_back(weights);
	}
}

void NeuralNetwork::newWeightsHidden(const std::vector<double>& deltaWOH)
{
	for (int i = 0; i < outputLayerWeights.size(); i++)
		outputLayerWeights[i] += deltaWOH[i];
}

void NeuralNetwork::newWeihgtsInput(const std::vector<std::vector<double>>& deltaWHI)
{
	for (int i = 0; i < WinputMatrix.size(); i++)
		for (int j = 0; j < WinputMatrix[0].size(); j++)
		{
			WinputMatrix[i][j] += deltaWHI[i][j];
		}
}

void NeuralNetwork::newWeightsHiddenLayer()
{
	for (int i = 0; i < hiddenLayerWeights.size(); i++)
	{
		for (int k = 0; k < hiddenLayerWeights[0].size(); k++)
		{
			for (int j = 0; j < hiddenLayerWeights[0][0].size(); j++)
			{
				hiddenLayerWeights[i][k][j] += deltaHiddenLayer[i][k][j];
			}
		}
	}
}

void NeuralNetwork::printWeightMatrix()
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



void NeuralNetwork::printInputLayer()
{
	for (int i = 0; i < inputLayer.size(); i++)
		std::cout << inputLayer[i] << " , ";
	std::cout << "\n";
}
void NeuralNetwork::printOutputMatrix()
{
	for (int i = 0; i < outputLayerWeights.size(); i++)
		std::cout << outputLayerWeights[i] << " , ";
	std::cout << "\n";
}
void NeuralNetwork::printOutput()
{
	std::cout << outputNeuron << "\n";
}

double NeuralNetwork::getOutput()
{
	return outputNeuron;
}

double NeuralNetwork::getValue(const std::vector<double>& input)
{
	setInput(input);
	calculateHidden();
	calculateOutput();

	return outputNeuron;
}

void NeuralNetwork::printStat(const std::vector<double>& input)
{
	for (int i = 0; i < input.size(); i++)
		std::cout << input[i] << ",";
	std::cout << "\n";
}

std::vector<std::vector<double>> NeuralNetwork::returnInputMatrix()
{
	return WinputMatrix;
}

std::vector<double> NeuralNetwork::returnHiddenMatrix()
{
	return outputLayerWeights;
}

void NeuralNetwork::setInputMatrix(std::vector<std::vector<double>> matrix)
{
	WinputMatrix = matrix;
}

void NeuralNetwork::setHiddenMatrix(std::vector<double> matrix)
{
	outputLayerWeights = matrix;
}
