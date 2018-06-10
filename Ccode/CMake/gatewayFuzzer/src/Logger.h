#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace FuzzLogging
{
   // Direct Interface for logging into log file or console using MACRO(s)
   #define LOG_ERROR(x)    Logger::getInstance()->error(x)
   #define LOG_INFO(x)     Logger::getInstance()->info(x)

   // enum for LOG_LEVEL
   typedef enum LOG_LEVEL
   {
      LOG_LEVEL_ERROR   = 1,
      LOG_ALL           = 2,
   }LogLevel;

   // enum for LOG_TYPE
   typedef enum LOG_TYPE
   {
      NO_LOG            = 1,
      CONSOLE           = 2,
      FILE_LOG          = 3,
   }LogType;

   class Logger
   {
      public:
         static Logger* getInstance() throw ();

         // Interface for Error Log
         void error(const char* text) throw();
         void error(std::string& text) throw();
         void error(std::ostringstream& stream) throw();

         // Interface for Info Log
         void info(const char* text) throw();
         void info(std::string& text) throw();
         void info(std::ostringstream& stream) throw();

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
         void logIntoFile(std::string& data);
         void logOnConsole(std::string& data);
         Logger(const Logger& obj) {}
         void operator=(const Logger& obj) {}

      private:
         static Logger*          m_Instance;
         std::ofstream           m_File;

         LogLevel                m_LogLevel;
         LogType                 m_LogType;
   };

} // End of namespace

#endif // End of _LOGGER_H_
