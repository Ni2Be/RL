#include "Network.h"

#include <iostream>
#include <fstream>

NeuralNetwork::NeuralNetwork(int input, int hiddenLayerSize, int hiddenLayer, int output) :
	inputLayer(Layer(input + 1)),
	hiddenLayers(LayerContainer(hiddenLayer, Layer(hiddenLayerSize + 1))),
	outputLayer(Layer(output))
{
	initWeights(-0.01, 0.01);
	initDeltaWeights();
	initBiasNeurons();
}

void NeuralNetwork::initInputWeights(double from, double to)
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
		inputWeights.push_back(temp);
	}

}

void NeuralNetwork::initHiddenWeights(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);

	for (int i = 0; i < hiddenLayers.size() - 1; i++)
	{
		Matrix weights;
		for (int j = 0; j < hiddenLayers[0].size() - 1; j++)
		{
			std::vector<double> temp;
			for (int k = 0; k < hiddenLayers[0].size(); k++)
			{
				temp.push_back(dis(gen));
			}
			weights.push_back(temp);
		}
		hiddenWeights.push_back(weights);
	}
}

void NeuralNetwork::initOutputWeights(double from, double to)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(from, to);
	//dis(gen)
	for (int i = 0; i < outputLayer.size(); i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < hiddenLayers[0].size(); j++)
		{
			temp.push_back(dis(gen));
		}
		outputWeights.push_back(temp);
	}
}

void NeuralNetwork::initWeights(double from, double to)
{
	initInputWeights(from, to);
	initHiddenWeights(from, to);
	initOutputWeights(from, to);
}

void NeuralNetwork::initBiasNeurons()
{
	inputLayer[0].setInput(1.0);
	inputLayer[0].setOutput(0);

	for (int i = 0; i < hiddenLayers.size(); i++)
	{
		hiddenLayers[i][0].setInput(1.0);
		hiddenLayers[i][0].setOutput(0);
	}
}

void NeuralNetwork::initDeltaWeights()
{
	inputWeightsDelta = Matrix(inputWeights.size(), std::vector<double>(inputWeights[0].size()));
	outputWeightsDelta = Matrix(outputWeights.size(), std::vector<double>(outputWeights[0].size()));
	if (hiddenLayers.size() > 1)
		hiddenWeightsDelta = MatrixContainer(hiddenWeights.size(), Matrix(hiddenWeights[0].size(), std::vector<double>(hiddenWeights[0][0].size())));
}

double NeuralNetwork::sumInput(Layer& layer, const Vector& weights)
{
	double sum = 0.0;
	for (int i = 0; i < layer.size(); i++)
		sum += layer[i].getOutput() * weights[i];
	return sum;
}

double NeuralNetwork::sumDeltaOutput(Layer & layer, const Matrix & weights, int colum)
{
	double sum = 0.0;
	for (int i = 0; i < layer.size(); i++)
		sum += layer[i].getDelta() * weights[i][colum];
	return sum;
}

double NeuralNetwork::sumDeltaHidden(Layer & layer, const Matrix & weights, int colum)
{
	double sum = 0.0;
	for (int i = 1; i < layer.size(); i++)
		sum += layer[i].getDelta() * weights[i - 1][colum];
	return sum;
}

void NeuralNetwork::setInputLayer(const Vector& input)
{
	for (int i = 1; i < inputLayer.size(); i++)
	{
		inputLayer[i].setInput(input[i - 1]);
		inputLayer[i].setOutput(0);
	}
}

void NeuralNetwork::setHiddenLayers()
{
	//set first hiddenLayer
	int hlSize = hiddenLayers[0].size();
	for (int i = 1; i < hlSize; i++)
	{
		hiddenLayers[0][i].setInput(sumInput(inputLayer, inputWeights[i - 1]));
		hiddenLayers[0][i].setOutput(1);
	}

	//set if hiddenLayer size > 1
	for (int i = 1; i < hiddenLayers.size(); i++)
		for (int j = 1; j < hlSize; j++)
		{
			hiddenLayers[i][j].setInput(sumInput(hiddenLayers[i - 1], hiddenWeights[i - 1][j - 1]));
			hiddenLayers[i][j].setOutput(1);
		}
}

void NeuralNetwork::setOutputLayer()
{
	for (int i = 0; i < outputLayer.size(); i++)
	{
		outputLayer[i].setInput(sumInput(hiddenLayers[hiddenLayers.size() - 1], outputWeights[i]));
		outputLayer[i].setOutput(1);
	}
}

void NeuralNetwork::calculate(const Vector & input)
{
	setInputLayer(input);
	setHiddenLayers();
	setOutputLayer();
}

void NeuralNetwork::setOutputLayerDelta(Vector & setpoint)
{
	for (int i = 0; i < outputLayer.size(); i++)
		outputLayer[i].setDelta((outputLayer[i].getOutput() * (1 - outputLayer[i].getOutput())) * (setpoint[i] - outputLayer[i].getOutput()));
}

void NeuralNetwork::setOutputWeightsDelta(float learnRate)
{
	int row = outputWeightsDelta.size(),
		colum = outputWeightsDelta[0].size(), lastHl = hiddenLayers.size() - 1;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < colum; j++)
			outputWeightsDelta[i][j] = outputLayer[i].getDelta() * hiddenLayers[lastHl][j].getOutput() * learnRate;
}

void NeuralNetwork::setHiddenLayerDelta()
{
	//set last Hiddenlayer
	int hlSize = hiddenLayers[0].size(),
		lastLayer = hiddenLayers.size() - 1;
	for (int i = 1; i < hlSize; i++)
		hiddenLayers[lastLayer][i].setDelta((hiddenLayers[lastLayer][i].getOutput() * (1 - hiddenLayers[lastLayer][i].getOutput())) * sumDeltaOutput(outputLayer, outputWeights, i));

	//if hiddenLayers > 1
	int layers = hiddenLayers.size();
	for (int i = layers - 2; i >= 0; i--)
		for (int j = 1; j < hlSize; j++)
			hiddenLayers[i][j].setDelta((hiddenLayers[i][j].getOutput() * (1 - hiddenLayers[i][j].getOutput())) * sumDeltaHidden(hiddenLayers[i + 1], hiddenWeights[i], j));

}

void NeuralNetwork::setHiddenWeightsDelta(float learnRate)
{
	int row = hiddenWeightsDelta[0].size(),
		colum = hiddenWeightsDelta[0][0].size();

	for (int i = 0; i < hiddenWeightsDelta.size(); i++)
	{
		for (int j = 0; j < row; j++)
			for (int k = 0; k < colum; k++)
				hiddenWeightsDelta[i][j][k] = hiddenLayers[i + 1][j + 1].getDelta() * hiddenLayers[i][k].getOutput() * learnRate;
	}

}

void NeuralNetwork::setInputWeightsDelta(float learnRate)
{
	int row = inputWeightsDelta.size(),
		colum = inputWeightsDelta[0].size();

	for (int i = 0; i < row; i++)
		for (int j = 0; j < colum; j++)
			inputWeightsDelta[i][j] = hiddenLayers[0][i + 1].getDelta() * inputLayer[j].getOutput() * learnRate;
}

void NeuralNetwork::setNewOutputWeights()
{
	int row = outputWeights.size(),
		colum = outputWeights[0].size();
	for (int i = 0; i < row; i++)
		for (int j = 0; j < colum; j++)
			outputWeights[i][j] += outputWeightsDelta[i][j];
}

void NeuralNetwork::setNewHiddenWeights()
{
	int row = hiddenWeights[0].size(),
		colum = hiddenWeights[0][0].size();
	for (int i = 0; i < hiddenWeights.size(); i++)
		for (int j = 0; j < row; j++)
			for (int k = 0; k < colum; k++)
				hiddenWeights[i][j][k] += hiddenWeightsDelta[i][j][k];
}

void NeuralNetwork::setNewInputWeights()
{
	int row = inputWeights.size(),
		colum = inputWeights[0].size();
	for (int i = 0; i < row; i++)
		for (int j = 0; j < colum; j++)
			inputWeights[i][j] += inputWeightsDelta[i][j];
}

void NeuralNetwork::backpropagation(Vector & setpoint, float learnRate)
{
	setOutputLayerDelta(setpoint);
	setOutputWeightsDelta(learnRate);
	setHiddenLayerDelta();
	if (hiddenLayers.size() > 1)
	{
		setHiddenWeightsDelta(learnRate);
		setNewHiddenWeights();
	}
	setInputWeightsDelta(learnRate);
	setNewOutputWeights();
	setNewInputWeights();
}

Vector NeuralNetwork::getOutput()
{
	Vector output;
	for (int i = 0; i < outputLayer.size(); i++)
		output.push_back(outputLayer[i].getOutput());

	return output;
}

Matrix NeuralNetwork::getInputWeights()
{
	return inputWeights;
}

Matrix NeuralNetwork::getOutputWeights()
{
	return outputWeights;
}

MatrixContainer NeuralNetwork::getHiddenWeights()
{
	return hiddenWeights;
}

void NeuralNetwork::setInputWeights(const Matrix & weights)
{
	inputWeights = weights;
}

void NeuralNetwork::setOutputWeights(const Matrix & weights)
{
	outputWeights = weights;
}

void NeuralNetwork::setHiddenWeights(const MatrixContainer & weights)
{
	hiddenWeights = weights;
}


void NeuralNetwork::saveRewardsInFile(const std::string& file_name)
{
	std::ofstream myfile(file_name.c_str());
	if (myfile.is_open())
	{
		for (int i = 0; i < inputWeights.size(); i++)
			for (int j = 0; j < inputWeights[i].size(); j++)
				myfile << inputWeights[i][j] << " , ";

		for (int i = 0; i < hiddenWeights.size(); i++)
			for (int j = 0; j < hiddenWeights[i].size(); j++)
				for (int k = 0; k < hiddenWeights[i][j].size(); k++)
					myfile << hiddenWeights[i][j][k] << " , ";

		for (int i = 0; i < outputWeights.size(); i++)
			for (int j = 0; j < outputWeights[i].size(); j++)
				myfile << outputWeights[i][j] << " , ";

		myfile.close();
	}
	else std::cout << "Unable to open file";
}

void NeuralNetwork::loadRewardsFromFile(const std::string& file_name)
{
	std::ifstream myfile(file_name.c_str());
	NeuralNetwork& nn = *this;
	Matrix inputMatrix = nn.getInputWeights();
	MatrixContainer hiddenMatrix = nn.getHiddenWeights();
	Matrix outputMatrix = nn.getOutputWeights();
	char sign;
	if (myfile.is_open())
	{
		for (int i = 0; i < inputMatrix.size(); i++)
			for (int j = 0; j < inputMatrix[i].size(); j++)
			{
				myfile >> inputMatrix[i][j];
				myfile >> sign;
			}

		for (int i = 0; i < hiddenMatrix.size(); i++)
			for (int j = 0; j < hiddenMatrix[i].size(); j++)
				for (int k = 0; k < hiddenMatrix[i][j].size(); k++)
				{
					myfile >> hiddenMatrix[i][j][k];
					myfile >> sign;
				}

		for (int i = 0; i < outputMatrix.size(); i++)
			for (int j = 0; j < outputMatrix[i].size(); j++)
			{
				myfile >> outputMatrix[i][j];
				myfile >> sign;
			}
		setInputWeights(inputMatrix);
		setOutputWeights(outputMatrix);
		setHiddenWeights(hiddenMatrix);
		myfile.close();
	}
	else std::cout << "Unable to open file";
}