#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace FuzzLogging
{
   // Direct Interface for logging into log file or console using MACRO(s)
   #define LOG_ERROR(x, LOG_CHANNEL)    Logger::getInstance()->error(x, LOG_CHANNEL)
   #define LOG_INFO(x, LOG_CHANNEL)     Logger::getInstance()->info(x, LOG_CHANNEL)

   // enum for LOG_LEVEL
   typedef enum LOG_LEVEL
   {
        LOG_LEVEL_ERROR   = 1,
        LOG_ALL           = 2,
   }LogLevel;

   // enum for LOG_TYPE
   typedef enum LOG_TYPE
   {    NO_LOG            = 1,
        CONSOLE           = 2,
        FILE              = 3,
   }LogType;

    typedef enum LOG_CHANNEL
    {
        interface0        = 1,
        interface1        = 2,
        interface2        = 3,
        debugfile         = 4,
    }LogChannel;

   class Logger
   {
      public:
         static Logger* getInstance() throw ();

         // Interface for Error Log
         void error(const char* text, std::ofstream& file) throw();
         void error(const char* text, LogChannel channel) throw();
         void error(std::string text, LogChannel channel) throw();

         // Interface for Info Log
         void info(const char* text, std::ofstream& file) throw();
         void info(const char* text, LogChannel channel) throw();
         void info(std::string text, LogChannel channel) throw();

         // Interfaces to control log levels
         void updateLogLevel(LogLevel logLevel);
         void enableLog();  // Enable all log levels
         void disableLog(); // Disable all log levels, except error and alarm

         // Interfaces to control log Types
         void updateLogType(LogType logType);
         void enableConsoleLogging();
         void enableFileLogging();

      protected:
         Logger();
         ~Logger();

         std::string getCurrentTime();

      private:
         void logIntoFile(std::string& data, std::ofstream& file);
         void logOnConsole(std::string& data);
         Logger(const Logger& obj) {}
         void operator=(const Logger& obj) {}

      private:
         static Logger*          m_Instance;
         std::ofstream           m_File0;
         std::ofstream           m_File1;
         std::ofstream           m_File2;
         std::ofstream           m_File3;

         LogLevel                m_LogLevel;
         LogType                 m_LogType;
   };

} // End of namespace

#endif // End of _LOGGER_H_
