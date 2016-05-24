#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>

using namespace std;

//Singleton Logger que realiza la escritura de los msj del servidor y los clientes.
class Logger
{
    public:
        static Logger* instance();
        void error(string);
        void warn(string);
        void info(string);
    protected:
        Logger();
        Logger(const Logger & );
        Logger &operator= (const Logger & );
    private:
        static Logger* loggerInst;
        virtual ~Logger();
        string timeCurrent();
        void truncateFileLog();
        int lengthFileLog();
};

#endif // LOGGER_H
