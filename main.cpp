#include "GameOfLife.h"
#include <iostream>

#include "cilktime.h"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned long long startTicks1, startTicks2;
	unsigned long long endTicks1, endTicks2;

	GameOfLife game(16, 16, PULSAR);

	// serial version
	startTicks1 = cilk_getticks();
	for (int i = 0; i < 10; i++)
	{
		cout << "Iteration: " << i << endl;

		game.printIteration();
		//system("pause");
		//system("cls"); 
		game.nextIterSerial();
	}
	endTicks1 = cilk_getticks();
	
	cout << "Serial time: \t\t\t" << endTicks1 - startTicks1 << " ticks\n";
	int serial = endTicks1 - startTicks1;
	// parallel version
	startTicks2 = cilk_getticks();
	for (int i = 0; i < 10; i++)
	{
		// TODO: IMPLEMENT PARALLEL FUNCTION
		cout << "Iteration: " << i << endl;

		game.printIteration();
		//system("pause");
		//system("cls"); 
		game.nextIterParallel();
	}
	endTicks2 = cilk_getticks();
	cout << "Parallel time: \t" << endTicks2 - startTicks2 << " ticks\n";
	int parallel = endTicks2 - startTicks2;
	cout << "Ubrzanje: "<< (double)serial/parallel<< endl;
	system("pause");

}
