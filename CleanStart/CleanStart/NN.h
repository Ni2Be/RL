#include <vector>
#include <random>
#include <iostream>
#include <math.h>


	class NNetwork
	{
	public:
		NNetwork() {}
		NNetwork(int inputLayerSize, int hiddenLayerSize);
		void initializeInputMatrix(double from, double to);
		void initializeHiddenMatrix(double from, double to);
		void setInput(const std::vector<double>& input);
		void calculateHidden();
		void calculateOutput();
		double weightInputSum(const std::vector<double>& input, const std::vector<double>& weights);
		double sigmoidFunc(double x);
		void backProb(double eSignal, double learnRate);
		double deltaOutput(double eSignal);
		std::vector<double> calcDeltaWHOmatrix(double learnRate, double deltaOut);
		std::vector<double> deltaHidden(double deltaOut);
		void calcDeltaWHImatrix(double learnRate, const std::vector<double>& deltaHidden, std::vector<std::vector<double>>& deltaWHI);
		void newWeightsHidden(const std::vector<double>& deltaWOH);
		void newWeihgtsInput(const std::vector<std::vector<double>>& deltaWHI);
		void printWeightMatrix();
		void printHiddenLayer();
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
		std::vector<double> inputLayer, hiddenLayer, WhiddenMatrix;
		double outputNeuron;
		std::vector<std::vector<double>> WinputMatrix;

	};


