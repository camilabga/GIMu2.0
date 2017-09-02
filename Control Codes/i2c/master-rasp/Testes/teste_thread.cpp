// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <unistd.h>   /* For open(), */

using namespace std;

long int x = 0;

void foo() 
{
	for(int i=0;i<1000;i++){
		x ++;
		usleep(10);
	}
}

void bar()
{
	for(int i=0;i<10;i++){
		cout << x << endl;
		usleep(10000);
	}
}

int main() 
{
  thread first (foo);     // spawn new thread that calls foo()
  thread second (bar);  // spawn new thread that calls bar(0)

  cout << "main, foo and bar now execute concurrently...\n";

  // synchronize threads:
  first.join();                // pauses until first finishes
  second.join();               // pauses until second finishes

  cout << "foo and bar completed.\n";

  return 0;
}
