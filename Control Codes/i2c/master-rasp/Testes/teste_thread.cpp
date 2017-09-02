#include <iostream>       // std::cout
#include <thread>         // std::thread
using namespace std;
 

void foo() 
{
	for(int i=0;i<100000;i++){
		cout << i << endl;
	}
}

void bar(int x)
{
	// do stuff...
}

int main() 
{
	thread first (foo);     // spawn new thread that calls foo()
	thread second (bar,0);  // spawn new thread that calls bar(0)

	cout << "main, foo and bar now execute concurrently...\n";
	
	second.join();
	cout << "Voltou2" << endl;
	first.join();
	cout << "Voltou1" << endl;
	
	cout << "foo and bar completed.\n";

	return 0;
}
