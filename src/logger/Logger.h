#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

//Singleton Logger que realiza la escritura de los msj del servidor y los clientes.
class Logger
{
    public:
		int lengthFileLog();
        static Logger* instanceLogger();
        void writeLog(string typeMsj,string msj);
    protected:
        Logger();
        Logger(const Logger & );
        Logger &operator= (const Logger & );
    private:
        static Logger* loggerInst;
        virtual ~Logger();
        string timeCurrent();
        void truncateFileLog();
};

#endif // LOGGER_H
