#include "Network.hpp"

int main()
{
	srand(time(NULL));
	cout.precision(4);
	vector<int> layers = { 20, 24, 56, 76, 29 };
	Network network(layers, 0);
	float er = 0;
	const vector<float> in(20, 0.4);
	const vector<float> aim(29, 0.546);
	cout << "Training...\n";
	do
	{
		network.signal_passing(in);
		float er = network.back_propogation(aim);
	} while (er > 1e-07);
	network.store();
	cout << "Network trained and stored\n";
	return 0;
}