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
        LOG_LEVEL_STATE   = 2,
        LOG_ALL           = 3,
   }LogLevel;

    typedef enum LOG_CHANNEL
    {
        interface0        = 0,
        interface1        = 1,
        interface2        = 2,
        debugfile         = 3,
    }LogChannel;

   class Logger
   {
      public:
         static Logger* getInstance() throw ();

         // Interface for Error Log
         void error(const char* text, std::ofstream& file) throw();
         void error(const char* text, LogChannel channel) throw();
         void error(std::string text, LogChannel channel) throw();
         void error(std::ostringstream& stream, LogChannel channel) throw();

         // Interface for State Log
         void state(const char* text, std::ofstream& file) throw();
         void state(const char* text, LogChannel channel) throw();
         void state(std::string text, LogChannel channel) throw();
         void state(std::ostringstream& stream, LogChannel channel) throw();

         // Interface for Info Log
         void info(const char* text, std::ofstream& file) throw();
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

         LogLevel                m_LogLevel;
   };

   LogChannel getChannelNameByNumber(int channel);

} // End of namespace

#endif // End of _LOGGER_H_
