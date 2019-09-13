#include <vector>
#include <random>
#include <iostream>
#include <math.h>
using Matrix = std::vector<std::vector<double>>;
using MatrixContainer = std::vector<Matrix>;

class NeuralNetwork
{
public:
	NeuralNetwork() {}
	NeuralNetwork(int inputLayerSize, int hiddenLayerSize, int hiddenLayersCount);
	void initializeInputMatrix(double from, double to);
	void initializeHiddenMatrix(double from, double to, int countL, int countN);
	void initializeOuputMatrix(double from, double to);
	void setInput(const std::vector<double>& input);
	void calculateHidden();
	void calculateOutput();
	double weightInputSum(const std::vector<double>& input, const std::vector<double>& weights);
	double sumDeltaWeights(const std::vector<double>& input, Matrix& weights, int c);
	double sigmoidFunc(double x);
	void backProb(double eSignal, double learnRate);
	double deltaOutput(double eSignal);
	void calcDeltaWHmatrix(double learnRate, double deltaOutput);
	std::vector<double> calcDeltaWHOmatrix(double learnRate, double deltaOut);
	void calcDeltaWHImatrix(double learnRate, std::vector<std::vector<double>>& deltaWHI);
	void calcDeltaHidden(double learnRate);
	void newWeightsHidden(const std::vector<double>& deltaWOH);
	void newWeihgtsInput(const std::vector<std::vector<double>>& deltaWHI);
	void newWeightsHiddenLayer();
	void printWeightMatrix();
	void printInputLayer();
	void printOutputMatrix();
	void printOutput();
	double getOutput();
	double getValue(const std::vector<double>& input);
	void printStat(const std::vector<double>& input);
	std::vector<std::vector<double>> returnInputMatrix();
	std::vector<double> returnHiddenMatrix();
	void setInputMatrix(std::vector<std::vector<double>> matrix);
	void setHiddenMatrix(std::vector<double> matrix);


private:
	std::vector<double> inputLayer, outputLayerWeights;
	Matrix hiddenLayers;
	MatrixContainer hiddenLayerWeights;
	MatrixContainer deltaHiddenLayer;
	double outputNeuron;
	Matrix WinputMatrix;
	Matrix deltaWH;

};


#pragma once

