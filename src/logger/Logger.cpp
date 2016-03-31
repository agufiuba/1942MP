#include "Logger.h"

using namespace std;

const int MAX_LENGHT_FILE_LOG = 10000;
Logger* loggerInstance = NULL;
string fileLogPath = "logs/logger.log";
string fileLogName = "logger.log";
ofstream fileLog;

Logger::Logger() {

	ofstream file(fileLogPath.c_str(), ios::app | ios::binary);

	if (!file) {
		// si no se pudo crear el archivo arroja una excepción/
		throw std::ios_base::failure("El archivo log no pudo ser abierto");
	}

	fileLog.open(fileLogPath.c_str(), ios::app | ios::binary); // abro el logger.log

}

Logger::~Logger() {
	delete loggerInstance;
	fileLog.close();
}

Logger* Logger::instance() {
	if (!loggerInstance)
		loggerInstance = new Logger();
	return loggerInstance;
}

void Logger::warn(string msj) {
	if (this->lengthFileLog() > MAX_LENGHT_FILE_LOG)
		this->truncateFileLog();
	fileLog << this->timeCurrent() << "[WARN] - " << msj << endl;
	if (!fileLog)
		// si no se pudo crear el archivo arroja una excepción/
		throw std::ios_base::failure("El archivo log no pudo ser escrito");
}

void Logger::error(string msj) {
	if (this->lengthFileLog() > MAX_LENGHT_FILE_LOG)
		this->truncateFileLog();
	fileLog << this->timeCurrent() << "[ERROR] - " << msj << endl;
	if (!fileLog)
		// si no se pudo crear el archivo arroja una excepción/
		throw std::ios_base::failure("El archivo log no pudo ser escrito");
}

void Logger::info(string msj) {
	if (this->lengthFileLog() > MAX_LENGHT_FILE_LOG)
		this->truncateFileLog();
	fileLog << this->timeCurrent() << "[INFO] - " << msj << endl;
	if (!fileLog)
		// si no se pudo crear el archivo arroja una excepción/
		throw std::ios_base::failure("El archivo log no pudo ser escrito");
}


string Logger::timeCurrent() {
	time_t timeAc;
	time(&timeAc);
	struct tm* timelocal = localtime(&timeAc);

	char bufferTime[80];
	strftime(bufferTime, 80, "%d-%m-%y %I:%M:%S", timelocal);
	string timeCurrent(bufferTime);
	return timeCurrent;
}

int Logger::lengthFileLog() {
	fileLog.seekp(0, ios::end);
	int fileLogSize = static_cast<int>(fileLog.tellp());
	return fileLogSize;
}

void Logger::truncateFileLog() {
	string fileLogPathOld = "logs/" + this->timeCurrent() + " - " + fileLogName;
	cout << "Archivo Nuevo: " + fileLogPathOld << endl;
	rename(fileLogPath.c_str(), fileLogPathOld.c_str());

	fileLog.close();

	ofstream file(fileLogPath.c_str(), ios::app | ios::binary);

	if (!file) {
		// si no se pudo crear el archivo arroja una excepción/
		throw std::ios_base::failure("El archivo log no pudo ser abierto");
	}

	fileLog.open(fileLogPath.c_str(), ios::app | ios::binary); // abro el logger.log

}
