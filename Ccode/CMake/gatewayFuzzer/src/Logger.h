#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/****
 * ERROR and INFO log into file and console
 * INFO can be turned off by switching the Loglevel to 1 (ERROR only)
 *
 * MESSAGE will write into the file regardless of Loglevel, it wont show up on console
 */

namespace FuzzLogging
{
   // Direct Interface for logging into log file or console using MACRO(s)
    #define LOG_ERROR(x, LOG_CHANNEL)       Logger::getInstance()->error(x, LOG_CHANNEL)
    #define LOG_INFO(x, LOG_CHANNEL)        Logger::getInstance()->info(x, LOG_CHANNEL)
    #define LOG_MESSAGE(x, LOG_CHANNEL)     Logger::getInstance()->message(x, LOG_CHANNEL)

   // enum for LOG_LEVEL
   typedef enum LOG_LEVEL
   {
        LOG_LEVEL_ERROR   = 1,
        LOG_ALL           = 2,
   }LogLevel;

    typedef enum LOG_CHANNEL
    {
        interface0        = 0,
        interface1        = 1,
        interface2        = 2,
        debugfile         = 3,
        resultfile        = 4,
    }LogChannel;

   class Logger
   {
      public:
         static Logger* getInstance() throw ();

         // Interface for Error Log
         void error(const char* text, std::ofstream& file, LogChannel channel) throw();
         void error(const char* text, LogChannel channel) throw();
         void error(std::string text, LogChannel channel) throw();
         void error(std::ostringstream& stream, LogChannel channel) throw();

         // Interface for State Log
         void message(const char* text, std::ofstream& file, LogChannel channel) throw();
         void message(const char* text, LogChannel channel) throw();
         void message(std::string text, LogChannel channel) throw();
         void message(std::ostringstream& stream, LogChannel channel) throw();

         // Interface for Info Log
         void info(const char* text, std::ofstream& file, LogChannel channel) throw();
         void info(const char* text, LogChannel channel) throw();
         void info(std::string text, LogChannel channel) throw();
         void info(std::ostringstream& stream, LogChannel channel) throw();

      protected:
         Logger();
         ~Logger();

         std::string getCurrentTime();

      private:
         void logIntoFile(std::string& data, std::ofstream& file);
         void logOnConsole(std::string& data);

      private:
         static Logger*          m_Instance;
         std::ofstream           m_File0;
         std::ofstream           m_File1;
         std::ofstream           m_File2;
         std::ofstream           m_File3;
         std::ofstream           m_File4;

         LogLevel                m_LogLevel;
   };

   LogChannel getChannelNameByNumber(int channel);
   static char* toString(LogChannel channel);

} // End of namespace

#endif // End of _LOGGER_H_
