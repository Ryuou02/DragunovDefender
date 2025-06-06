/****************************************************
*
* Code developed by Nikhil Tom Jose (Ryuou02) 
* Part of DragunovDefender
*
*****************************************************/
#pragma once
#include"perceptron.h"
#include<array>
//#define SINGLE_OUTPUT
#define SINGLE_HIDDEN


#ifdef SINGLE_HIDDEN
#ifdef SINGLE_OUTPUT
class neuralNetwork
{
	int hiddenSize;		/* size of hidden layers */
	int n_inputs;		/* number of inputs */
	perceptron *hiddenLayers, outputLayer;
	double* hiddenDel, *hiddenPrediction;	/* del values for hidden layer neurons */
public:
	neuralNetwork(int hiddenLayerSize, int inputSize):hiddenSize(hiddenLayerSize),n_inputs(inputSize){
		hiddenLayers = new perceptron[hiddenLayerSize];
		outputLayer = perceptron(sigmoid, hiddenLayerSize);
		for (int i = 0; i < hiddenSize; i++) {
			hiddenLayers[i] = perceptron(sigmoid, inputSize);
		}
		/* below variables are only used for training/back propogation but re-allocation of memory will take lot of time, hence making it a class variable*/
		hiddenPrediction = new double[hiddenSize];
		hiddenDel = new double[hiddenSize];
	}
	~neuralNetwork() {
		delete[] hiddenLayers;
		delete[] hiddenPrediction;
		delete[] hiddenDel;
	}
	double predict(double* inputs) {
		for (int i = 0; i < hiddenSize; i++)
			hiddenPrediction[i] = hiddenLayers[i].predict(inputs);		
		return outputLayer.predict(hiddenPrediction);
	}
	void backPropogate(double* input,double requiredOutput, float learningRate) {
		/* in the back propogation the predict function is not used directly, but the code for it is used in it. */
		double outputDel;

		double output = predict(input);
		
		// calculating del values for every neuron, in order to update the weights.
		outputDel = output * (1 - output) * (requiredOutput - output);
		outputLayer.setWeights(0, outputLayer[0] + learningRate * outputDel);

		for (int i = 0; i < hiddenSize; i++) {
			hiddenDel[i] = hiddenPrediction[i] * (1 - hiddenPrediction[i]) * (outputLayer[i] * outputDel);	// outputLayer[i] => ith weight of output layer
			// setting new weights on output layer.
			outputLayer.setWeights(i + 1, outputLayer[i + 1] + learningRate * outputDel * hiddenPrediction[i]);

			//setting new weights for hidden layers
			hiddenLayers[i].setWeights(0, hiddenLayers[i][0] + learningRate * hiddenDel[i]);
	//		cout << "\nhidden layer " << i << " weights : " << hiddenLayers[i][0] << " ";
			for (int j = 0; j < n_inputs; j++) {
		//		cout << hiddenLayers[i][j + 1] << " ";
				hiddenLayers[i].setWeights(j + 1, hiddenLayers[i][j + 1] + learningRate * hiddenDel[i] * input[j]);
			}
		}
	}
	void train(vector<double*> inputs, double* outputs, int nEpochs, float learningRate = 1) {
		for (int epoch = 0; epoch < nEpochs; epoch++) {
			for (int i = 0; i < inputs.size(); i++) {
				backPropogate(inputs[i], outputs[i], learningRate);
			}
			if (epoch % 500 == 0) {
				cout << "\nat epoch " << epoch << ", loss : ";
				double loss = 0;
				for (int i = 0; i < 4; i++) {
					double pred = predict(inputs[i]);
					loss += pow(pred - outputs[i], 2);

				}
				loss /= 4;
				cout << loss;
			}
		}
		std::cout << "\nthe final weights found for output layer is - \n";
		for (int i = 0; i < 3; i++) {
			std::cout << outputLayer[i] << '\n';
		}
		std::cout << "\n final weights for each perceptron - \n";
		for (int i = 0; i < hiddenSize; i++) {
			std::cout << "perceptron " << i << "\n";
			for (int j = 0; j < 3; j++) {
				std::cout << hiddenLayers[i][j] << '\n';
			}
		}
	}
};
#endif //SINGLE_OUTPUT
#ifndef SINGLE_OUTPUT
class neuralNetwork
{
	int hiddenSize;		/* size of hidden layers */
	int outputLayerSize;/* number of outputs */
	int n_inputs;		/* number of inputs */
	perceptron* hiddenLayers, * outputLayers;
	double* hiddenDel, * hiddenPrediction, *output, *outputPrediction, *outputDel;	/* del values for hidden layer and output layer neurons */
public:
	neuralNetwork(int hiddenLayerSize, int inputSize, int outputSize) :hiddenSize(hiddenLayerSize), n_inputs(inputSize), outputLayerSize(outputSize) {
		hiddenLayers = new perceptron[hiddenLayerSize];
		outputLayers = new perceptron[outputLayerSize];
		for (int i = 0; i < hiddenSize; i++) {
			hiddenLayers[i] = perceptron(sigmoid, inputSize);
		}
		for (int i = 0; i < outputLayerSize; i++) {
			outputLayers[i] = perceptron(sigmoid, hiddenLayerSize);
		}
		/* below variables are only used for training/back propogation but re-allocation of memory will take lot of time, hence making it a class variable*/
		
		hiddenPrediction = new double[hiddenSize];
		hiddenDel = new double[hiddenSize];
		outputPrediction = new double[outputLayerSize];
		outputDel = new double[outputLayerSize];
	}
	~neuralNetwork() {
		delete[] hiddenLayers;
		delete[] hiddenPrediction;
		delete[] hiddenDel;
		delete[] outputDel;
		delete[] outputPrediction;
	}
	double* predict(double* inputs) {
		double* pred = new double[outputLayerSize];
		for (int i = 0; i < hiddenSize; i++)
			hiddenPrediction[i] = hiddenLayers[i].predict(inputs);
		for (int i = 0; i < outputLayerSize; i++)
			 pred[i] = outputLayers[i].predict(hiddenPrediction);

		return pred;
	}
	void backPropogate(double* input, double* requiredOutput, float learningRate) {
		/* in the back propogation the predict function is not used directly, but the code for it is used in it. */
		
		for (int i = 0; i < hiddenSize; i++)
			hiddenPrediction[i] = hiddenLayers[i].predict(input);

		for (int i = 0; i < outputLayerSize; i++)
			outputPrediction[i] = outputLayers[i].predict(hiddenPrediction);

		// calculating del values for every neuron, in order to update the weights.
		for (int i = 0; i < outputLayerSize; i++) {
			outputDel[i] =  outputPrediction[i] * (1 - outputPrediction[i]) * (requiredOutput[i] - outputPrediction[i]);
			outputLayers[i].setWeights(0, outputLayers[i][0] + learningRate * outputDel[i]);
			for (int j = 0; j < hiddenSize; j++)
				outputLayers[i].setWeights(j + 1, outputLayers[i][j + 1] + learningRate* hiddenPrediction[j] * outputDel[i]);
		}

		for (int i = 0; i < hiddenSize; i++) {
			hiddenDel[i] = 0;
			for (int j = 0; j < outputLayerSize; j++)
				hiddenDel[i] +=  hiddenPrediction[i] * (1 - hiddenPrediction[i]) * (outputLayers[j][i + 1] * outputDel[j]);	// outputLayer[i] => ith weight of output layer
			//setting new weights for hidden layers
			hiddenLayers[i].setWeights(0, hiddenLayers[i][0] + learningRate * hiddenDel[i]);
			for (int j = 0; j < n_inputs; j++)
				hiddenLayers[i].setWeights(j + 1, hiddenLayers[i][j + 1] + learningRate * hiddenDel[i] * input[j]);
		}
	}
	
	double calcError(vector<double*> inputs, vector<double*> expectedOutputs) {	// calculation of MSE
		double loss = 0;
		for (int i = 0; i < inputs.size(); i++) {
			double* pred = predict(inputs[i]);
			for (int j = 0; j < outputLayerSize; j++) {
				loss += pow(pred[j] - expectedOutputs[i][j], 2) / outputLayerSize;
			}
		}
		loss /= inputs.size();
		return loss;
	}
	
	void train(vector<double*> inputs, vector<double*> outputs, int nEpochs, float learningRate = 1) {
		for (int epoch = 0; epoch < nEpochs; epoch++) {
			for (int i = 0; i < inputs.size(); i++) {
				backPropogate(inputs[i], outputs[i], learningRate);
			}
			if (epoch % 10 == 0) {
				cout << "\nat epoch " << epoch << ", loss : ";
				cout << calcError(inputs,outputs);

			}
		}
	}
};
#endif	// not SINGLE_OUTPUT
#endif	// SINGLE_HIDDEN
#ifndef SINGLE_HIDDEN
class neuralNetwork
{
	int *hiddenSize;	/* size of  each of the hidden layers */
	int outputLayerSize;/* number of outputs */
	int n_inputs;		/* number of inputs */
	perceptron** hiddenLayers, * outputLayers;
	double** hiddenDel, ** hiddenPrediction, * output, * outputPrediction, * outputDel;	/* del values for hidden layer and output layer neurons */
public:
	neuralNetwork(int hiddenLayerSize, int inputSize, int outputSize) :hiddenSize(hiddenLayerSize), n_inputs(inputSize), outputLayerSize(outputSize) {
		hiddenLayers = new perceptron[hiddenLayerSize];
		outputLayers = new perceptron[outputLayerSize];
		for (int i = 0; i < hiddenSize; i++) {
			hiddenLayers[i] = perceptron(sigmoid, inputSize);
		}
		for (int i = 0; i < outputLayerSize; i++) {
			outputLayers[i] = perceptron(sigmoid, hiddenLayerSize);
		}
		/* below variables are only used for training/back propogation but re-allocation of memory will take lot of time, hence making it a class variable*/

		hiddenPrediction = new double[hiddenSize];
		hiddenDel = new double[hiddenSize];
		outputPrediction = new double[outputLayerSize];
		outputDel = new double[outputLayerSize];
	}
	~neuralNetwork() {
		delete[] hiddenLayers;
		delete[] hiddenPrediction;
		delete[] hiddenDel;
		delete[] outputDel;
		delete[] outputPrediction;
	}
	double* predict(double* inputs) {
		double* pred = new double[outputLayerSize];
		for (int i = 0; i < hiddenSize; i++)
			hiddenPrediction[i] = hiddenLayers[i].predict(inputs);
		for (int i = 0; i < outputLayerSize; i++)
			pred[i] = outputLayers[i].predict(hiddenPrediction);

		return pred;
	}
	void backPropogate(double* input, double* requiredOutput, float learningRate) {
		/* in the back propogation the predict function is not used directly, but the code for it is used in it. */

		for (int i = 0; i < hiddenSize; i++)
			hiddenPrediction[i] = hiddenLayers[i].predict(input);

		for (int i = 0; i < outputLayerSize; i++)
			outputPrediction[i] = outputLayers[i].predict(hiddenPrediction);

		// calculating del values for every neuron, in order to update the weights.
		for (int i = 0; i < outputLayerSize; i++) {
			outputDel[i] = outputPrediction[i] * (1 - outputPrediction[i]) * (requiredOutput[i] - outputPrediction[i]);
			outputLayers[i].setWeights(0, outputLayers[i][0] + learningRate * outputDel[i]);
			for (int j = 0; j < hiddenSize; j++)
				outputLayers[i].setWeights(j + 1, outputLayers[i][j + 1] + learningRate * hiddenPrediction[j] * outputDel[i]);
		}

		for (int i = 0; i < hiddenSize; i++) {
			hiddenDel[i] = 0;
			for (int j = 0; j < outputLayerSize; j++)
				hiddenDel[i] += hiddenPrediction[i] * (1 - hiddenPrediction[i]) * (outputLayers[j][i + 1] * outputDel[j]);	// outputLayer[i] => ith weight of output layer
			//setting new weights for hidden layers
			hiddenLayers[i].setWeights(0, hiddenLayers[i][0] + learningRate * hiddenDel[i]);
			for (int j = 0; j < n_inputs; j++)
				hiddenLayers[i].setWeights(j + 1, hiddenLayers[i][j + 1] + learningRate * hiddenDel[i] * input[j]);
		}
	}
	void train(vector<double*> inputs, vector<double*> outputs, int nEpochs, float learningRate = 1) {
		for (int epoch = 0; epoch < nEpochs; epoch++) {
			for (int i = 0; i < inputs.size(); i++) {
				backPropogate(inputs[i], outputs[i], learningRate);
			}
			if (epoch % 500 == 0) {
				cout << "\nat epoch " << epoch << ", loss : ";
				double loss = 0;
				for (int i = 0; i < 4; i++) {
					double* pred = predict(inputs[i]);
					for (int j = 0; j < outputLayerSize; j++) {
						loss += pow(pred[j] - outputs[i][j], 2);
					}

				}
				loss /= 4;
				cout << loss;

			}
		}
	}
};

#endif
