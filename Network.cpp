#include "Network.hpp"

void Neuron::activation() { value = 1 / (1 + pow(e, -value)); }

float Neuron::der() { return value * (1 - value); }

Network::Network(vector<int> layers, bool b) : neurons(layers.size()), connections(layers.size() - 1), bios(b)
{
	srand(time(NULL));
	NoL = layers.size();
	for (int i = 0; i < layers.size(); i++)
	{
		neurons[i] = vector<Neuron>(layers[i]);
		for (int j = 0; j < layers[i]; j++)
		{
			neurons[i][j] = Neuron();
		}
	}
	for (int n = 0; n < NoL - 1; n++)
	{
		for (int i = 0; i < layers[n]; i++)
		{
			connections[n].emplace_back(vector<float>());
			for (int j = 0; j < layers[n + 1]; j++)
			{
				connections[n][i].push_back(rand() / 16384.0 - 1);
			}
		}
	}
	if (bios)
	{
		for (int n = 0; n < NoL - 1; n++)
		{
			neurons[n].emplace_back(Neuron());
			neurons[n][neurons[n].size() - 1].value = 1;
			connections[n].emplace_back(vector<float>());
			for (int i = 0; i < neurons[n + 1].size(); i++)
			{
				connections[n][neurons[n].size() - 1].push_back(rand() / 16384.0 - 1);
			}
		}
	}
}

vector<float> Network::signal_passing(vector<float> in)
{
	for (int i = 0; i < neurons[0].size() - bios; i++)
	{ 
		neurons[0][i].value = in[i]; 
	}
	for (int n = 1; n < NoL; n++)
	{
		int d = (n == (NoL - 1)) ? neurons[n].size() : neurons[n].size() - bios;
		for (int i = 0; i < d; i++)
		{
			neurons[n][i].value = 0;
			for (int j = 0; j < neurons[n-1].size(); j++)
			{
				neurons[n][i].value += neurons[n-1][j].value * connections[n-1][j][i];
			}
			neurons[n][i].activation();
		}
	}
	vector<float> res(neurons[NoL-1].size());
	for (int i = 0; i < neurons[NoL - 1].size(); i++) { res[i] = neurons[NoL - 1][i].value; }
	return res;
}

float Network::back_propogation(vector<float> correct)
{
	float tot_error = 0;
	for (int i = 0; i < correct.size(); i++)
	{
		neurons[NoL - 1][i].error = correct[i] - neurons[NoL - 1][i].value;
		tot_error += pow(neurons[NoL - 1][i].error, 2);
	}
	for (int n = NoL - 2; n > 0; n--)
	{
		for (int i = 0; i < neurons[n].size() - bios; i++)
		{
			neurons[n][i].error = 0;
			int d = (n + 1 == (NoL - 1)) ? neurons[n + 1].size() : neurons[n + 1].size() - bios;
			for (int j = 0; j < d; j++)
			{
				neurons[n][i].error += neurons[n + 1][j].error * connections[n][i][j];
				tot_error += pow(neurons[n][i].error, 2);
			}
			neurons[n][i].error *= neurons[n][i].der();
		}
	}
		
	for (int n = 0; n < NoL - 1; n++)
	{
		for (int i = 0; i < neurons[n].size(); i++)
		{
			int d = (n + 1 == (NoL - 1)) ? neurons[n + 1].size() : neurons[n + 1].size() - bios;
			for (int j = 0; j < d; j++)
			{
				float q = r * neurons[n + 1][j].error * neurons[n][i].value;
				connections[n][i][j] += q;
			}
		}
	}
	return tot_error;
}

void Network::store()
{
	ofstream ofile("network.dat", ios::binary | ios::trunc);
	ofile.write((char*)&bios, sizeof(bios));
	ofile.write((char*)&NoL, sizeof(NoL));
	for (int i = 0; i < NoL; i++)
	{
		int n = (i == (NoL - 1)) ? neurons[i].size() : neurons[i].size() - bios;
		ofile.write((char*)&n, sizeof(n));
	}
	for (int n = 0; n < NoL - 1; n++)
		for (int i = 0; i < connections[n].size(); i++)
			for (int j = 0; j < connections[n][i].size(); j++)
				ofile.write((char*)&connections[n][i][j], sizeof(float));
}

void Network::load()
{
	ifstream infile("network.dat", ios::binary);
	bool b;	infile.read((char*)&b, sizeof(b));
	int x; infile.read((char*)&x, sizeof(x));
	vector<int> layers(x);
	for (int i = 0; i < x; i++)
	{
		infile.read((char*)&layers[i], sizeof(int));
	}
	*this = Network(layers, b);
	for (int n = 0; n < NoL - 1; n++)
		for (int i = 0; i < connections[n].size(); i++)
			for (int j = 0; j < connections[n][i].size(); j++)
				infile.read((char*)&connections[n][i][j], sizeof(float));
}
