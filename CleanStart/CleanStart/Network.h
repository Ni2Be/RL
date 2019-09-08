#include <vector>
#include <random>
#include <iostream>
#include <math.h>
#include "Neuron.h"

using Vector = std::vector<double>;
using Matrix = std::vector<std::vector<double>>;
using MatrixContainer = std::vector<Matrix>;
using Layer = std::vector<Neuron>;
using LayerContainer = std::vector<Layer>;


class NeuralNetwork
{
public:
	NeuralNetwork() {}
	// 1 = input Layer Neurons, 2 = hidden Layer neurons, 3 = hidden Layers, 4 = ouput layer neurons   
	NeuralNetwork(int input, int hiddenLayerSize, int hiddenLayer, int output);

	//initialize weights and bias neruons
	void initInputWeights(double from, double to);
	void initHiddenWeights(double from, double to);
	void initOutputWeights(double from, double to);
	void initWeights(double from, double to);
	void initBiasNeurons();
	void initDeltaWeights();

	//calculation
	double sumInput(Layer& layer, const Vector& weights);
	double sumDeltaOutput(Layer& layer, const Matrix & weights, int colum);
	double sumDeltaHidden(Layer& layer, const Matrix & weights, int colum);

	//forward pass
	void setInputLayer(const Vector& input);
	void setHiddenLayers();
	void setOutputLayer();
	void calculate(const Vector& input);

	//Backpropagation
	void setOutputLayerDelta(Vector& setpoint);
	void setOutputWeightsDelta(float learnRate);
	void setHiddenLayerDelta();
	void setHiddenWeightsDelta(float learnRate);
	void setInputWeightsDelta(float learnRate);
	void setNewOutputWeights();
	void setNewHiddenWeights();
	void setNewInputWeights();
	void backpropagation(Vector& setpoint, float learnRate);

	//get output, weights
	Vector getOutput();
	Matrix getInputWeights();
	Matrix getOutputWeights();
	MatrixContainer getHiddenWeights();

	//set weights from file
	void setInputWeights(const Matrix& weights);
	void setOutputWeights(const Matrix& weights);
	void setHiddenWeights(const MatrixContainer& weights);


	void saveRewardsInFile(const std::string& file_name);
	void loadRewardsFromFile(const std::string& file_name);
private:
	//Layer
	Layer inputLayer, outputLayer;
	LayerContainer hiddenLayers;
	//Weights
	Matrix inputWeights, outputWeights;
	MatrixContainer hiddenWeights;
	//Weights Delta
	Matrix inputWeightsDelta, outputWeightsDelta;
	MatrixContainer hiddenWeightsDelta;
};


#pragma once
