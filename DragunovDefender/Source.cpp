#include"AudioFile.h"
#include"neuralNetwork.h"

#define THRESHOLD_PRED 4e-4
#define THRESHOLD_LOUD 0.05

//#define TRY_RANDOM

#ifndef TRY_RANDOM
int main() {
	AudioFile<double> audio;
	vector<double*> inputs;
	vector<double*> outputs;
	double* ip;
	double* op;
	/*
	* load all the audio files, they are all like gs<number>bp.wav
	* take 10 samples each into ip, create double array of size 10 for each observation.
	* take next 2 samples into op, create double array of size 2 for each observation.
	*/
	
	/*
	int numSamples = audio.getNumSamplesPerChannel();
	for (int i = 0; i < numSamples; i++)
		double currentSample = audio.samples[0][i];
	*/
	for (int aud = 1; aud < 8; aud++) {
		std::string audioFilePath = "D:\\Dragunov Defender\\gunshot\\gss";
		audioFilePath += std::to_string(aud);
		audioFilePath += ".wav";
		audio.load(audioFilePath);
		audio.printSummary();
		int numSamples = audio.getNumSamplesPerChannel();
		for (int i = 0; i < numSamples - 13; i ++) {
			ip = new double[10];
			op = new double[3];

			/* each of the sample needs to be in range [0,1] so that it would be valid with the sigmoid activation function*/
			for (int j = i; j < i + 10; j++) {
				ip[j - i] = (audio.samples[0][j] + 1) / 2;
			}
			op[0] = (audio.samples[0][i] + 1) / 2;
			op[1] = (audio.samples[0][i + 1] + 1) / 2;
			op[2] = (audio.samples[0][i + 2] + 1) / 2;
			inputs.push_back(ip);
			outputs.push_back(op);
		}
	}

	neuralNetwork n1(130, 10, 3);
	n1.train(inputs, outputs, 101, 0.4);

	vector<double*> testInputs, testOutputs;
	std::string audioFilePath = "D:\\Dragunov Defender\\gunshot\\gs6.wav";
	int numSamples = audio.getNumSamplesPerChannel();
	for (int i = 0; i < numSamples - 13; i++) {
		ip = new double[10];
		op = new double[3];

		/* each of the sample needs to be in range [0,1] so that it would be valid with the sigmoid activation function*/
		for (int j = i; j < i + 10; j++) {
			ip[j - i] = (audio.samples[0][j] + 1) / 2;
		}
		op[0] = (audio.samples[0][i] + 1) / 2;
		op[1] = (audio.samples[0][i + 1] + 1) / 2;
		op[2] = (audio.samples[0][i + 2] + 1) / 2;
		testInputs.push_back(ip);
		testOutputs.push_back(op);
	}
	std::cout << "\nerror when testing with external = " << n1.calcError(testInputs, testOutputs);


	for (double* temp : testInputs)
		delete[] temp;
	for (double* temp : testOutputs)
		delete[] temp;

	
	do {
		int n;
		std::cout << "\nenter test audio number : ";
		cin >> n;
		if (n == 0)
			break;
		std::string testAudioPath = "D:\\Dragunov Defender\\gunshot\\testAudio" + std::to_string(n) + "bp.wav";
		audio.load(testAudioPath);
		/* ensure the heap memory is cleared */
		for (double* temp : inputs)
			delete[] temp;
		for (double* temp : outputs)
			delete[] temp;

		inputs.clear();
		outputs.clear();

		std::cout << "\nbeginning test\n";

		audio.load(testAudioPath);
		audio.printSummary();
		int numSamples = audio.getNumSamplesPerChannel();
		for (int i = 0; i < numSamples - 13; i++) {
			ip = new double[10];
			op = new double[3];

			/* each of the sample needs to be in range [0,1] so that it would be valid with the sigmoid activation function*/
			for (int j = i; j < i + 10; j++) {
				ip[j - i] = (audio.samples[0][j] + 1) / 2;
			}
			op[0] = (audio.samples[0][i] + 1) / 2;
			op[1] = (audio.samples[0][i + 1] + 1) / 2;
			op[2] = (audio.samples[0][i + 2] + 1) / 2;
			inputs.push_back(ip);
			outputs.push_back(op);
		}
		double* prediction;
		std::cout << "loudness threshold = " << THRESHOLD_LOUD << '\n';
		std::cout << "prediction threshold = " << THRESHOLD_PRED << '\n';
		for (int i = 0; i < inputs.size(); i++) {
			prediction = n1.predict(inputs[i]);
			if (((outputs[i][0]) - 0.5 > THRESHOLD_LOUD ) && (abs(prediction[0] - outputs[i][0]) + abs(prediction[1] - outputs[i][1]) + abs(prediction[2] - outputs[i][2]) < THRESHOLD_PRED)) {
				std::cout << "\033[32m gunshot detected at " << (i / double(22050)) << "\033[0m\n";
				//			}else if ((outputs[i][0]) - 0.5 > THRESHOLD_LOUD){
				std::cout << "time : " << (i / double(22050)) << "=>" << prediction[0] - outputs[i][0] << " " << prediction[1] - outputs[i][1] << " " << prediction[2] - outputs[i][2] << '\n';
			}
			delete[] prediction;
		}
	} while (true);

	/* ensure the heap memory is cleared */
	for (double* temp : inputs)
		delete[] temp;
	for (double* temp : outputs)
		delete[] temp;

}
#endif // TRY_RANDOM
#ifdef TRY_RANDOM
int main() {
	AudioFile<double> audio;
	vector<double*> inputs;
	vector<double*> outputs;
	double* ip;
	double* op;
	std::string audioFilePath = "C:\\Users\\Nikhil Jose\\Music\\01ringtone.wav";
	audio.load(audioFilePath);
	audio.printSummary();
	int numSamples = audio.getNumSamplesPerChannel();
	for (int i = 0; i < numSamples - 13; i++) {
		ip = new double[10];
		op = new double[3];

		/* each of the sample needs to be in range [0,1] so that it would be valid with the sigmoid activation function*/
		for (int j = i; j < i + 10; j++) {
			ip[j - i] = (audio.samples[0][j] + 1) / 2;
		}
		op[0] = (audio.samples[0][i] + 1) / 2;
		op[1] = (audio.samples[0][i + 1] + 1) / 2;
		op[2] = (audio.samples[0][i + 2] + 1) / 2;
		inputs.push_back(ip);
		outputs.push_back(op);
	}

	neuralNetwork n1(70, 10, 2);
	n1.train(inputs, outputs, 1000, 1);
}
#endif