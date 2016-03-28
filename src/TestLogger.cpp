#include "./logger/Logger.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>

using namespace std;

std::mutex theMutex;
static const int num_threads = 4;

void threadPruebaConMutex(int i){
	theMutex.lock();
	cout<< "Inicio Thread: "<<i<<endl;
	Logger* logger = Logger::instanceLogger();
	string msj = "Un msj de error escrito por:";
	msj +=static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
	logger->writeLog("Error", msj);
	cout << "Fin de Thread\n" << endl;
	theMutex.unlock();
}

int main()
{
	std::thread t[num_threads];
    for (int i = 0; i < num_threads; ++i) {
    	t[i] = std::thread(threadPruebaConMutex, i);
    }

    for (int i = 0; i < num_threads; ++i) {
	   	t[i].join();
	}
	return 0;
}
