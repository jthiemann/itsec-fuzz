#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Logger.h"

using namespace std;
using namespace FuzzLogging;

Logger* Logger::m_Instance = 0;

// Log file name
const string logFileName = "fuzzing.log";

Logger::Logger()
{
   m_File.open(logFileName.c_str(), ios::out|ios::app);
   m_LogLevel	= LOG_ALL;
   m_LogType	= CONSOLE;
}

Logger::~Logger()
{
   m_File.close();
}

Logger* Logger::getInstance() throw ()
{
   if (m_Instance == 0)
   {
      m_Instance = new Logger();
   }
   return m_Instance;
}

void Logger::logIntoFile(std::string& data)
{
   m_File << getCurrentTime() << "  " << data << endl;
}

void Logger::logOnConsole(std::string& data)
{
   cout << getCurrentTime() << "  " << data << endl;
}

string Logger::getCurrentTime()
{
   string currTime;
   //Current date/time based on current time
   time_t now = time(0);
   // Convert current time to string
   currTime.assign(ctime(&now));

   // Last charactor of currentTime is "\n", so remove it
   string currentTime = currTime.substr(0, currTime.size()-1);
   return currentTime;
}

// Interface for Error Log
void Logger::error(const char* text) throw()
{
   string data;
   data.append("[ERROR]: ");
   data.append(text);

   if(m_LogType == FILE_LOG)
   {
      logIntoFile(data);
   }
   else if(m_LogType == CONSOLE)
   {
      logOnConsole(data);
   }
}

void Logger::error(std::string& text) throw()
{
   error(text.data());
}

void Logger::error(std::ostringstream& stream) throw()
{
   string text = stream.str();
   error(text.data());
}

// Interface for Info Log
void Logger::info(const char* text) throw()
{
   string data;
   data.append("[INFO]: ");
   data.append(text);

   if((m_LogType == FILE_LOG) && (m_LogLevel == LOG_ALL))
   {
      logIntoFile(data);
   }
   else if((m_LogType == CONSOLE) && (m_LogLevel == LOG_ALL))
   {
      logOnConsole(data);
   }
}

void Logger::info(std::string& text) throw()
{
   info(text.data());
}

void Logger::info(std::ostringstream& stream) throw()
{
   string text = stream.str();
   info(text.data());
}


// Interfaces to control log levels
void Logger::updateLogLevel(LogLevel logLevel)
{
   m_LogLevel = logLevel;
}

// Enable all log levels
void Logger::enableLog()
{
   m_LogLevel = LOG_ALL;
}

// Disable all log levels, except error and alarm
void Logger:: disableLog()
{
   m_LogLevel = LOG_LEVEL_ERROR;
}

// Interfaces to control log Types
void Logger::updateLogType(LogType logType)
{
   m_LogType = logType;
}

void Logger::enableConsoleLogging()
{
   m_LogType = CONSOLE;
}

void Logger::enableFileLogging()
{
   m_LogType = FILE_LOG;
}

