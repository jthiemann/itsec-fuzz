#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Logger.h"

using namespace std;
using namespace FuzzLogging;

Logger* Logger::m_Instance = 0;

// Path to logfiles
const string path = "";

// Log file name
const string file0 = "../logfiles/interface0.log";
const string file1 = "../logfiles/interface1.log";
const string file2 = "../logfiles/interface2.log";
const string debug = "../logfiles/debug.log";

Logger::Logger()
{
   m_File0.open(file0.c_str(), ios::out|ios::app);
   m_File1.open(file1.c_str(), ios::out|ios::app);
   m_File2.open(file2.c_str(), ios::out|ios::app);
   m_File3.open(debug.c_str(), ios::out|ios::app);
   m_LogLevel	= LOG_ALL;
   m_LogType	= FILE;
}

Logger::~Logger()
{
   m_File0.close();
   m_File1.close();
   m_File2.close();
   m_File3.close();
}

Logger* Logger::getInstance() throw ()
{
   if (m_Instance == 0)
   {
      m_Instance = new Logger();
   }
   return m_Instance;
}

void Logger::logIntoFile(std::string& data, std::ofstream& file)
{
   file << getCurrentTime() << "  " << data << endl;
   cout << getCurrentTime() << "  " << data << endl;

}

void Logger::logOnConsole(std::string& data)
{
    //TODO: Change or remove "only console" functionality
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
void Logger::error(const char* text, std::ofstream& file) throw()
{
   string data;
   data.append("[ERROR]: ");
   data.append(text);

   if(m_LogType == FILE)
   {
      logIntoFile(data, file);
   }
   else if(m_LogType == CONSOLE)
   {
      logOnConsole(data);
   }
}

void Logger::error(string text, LogChannel channel) throw()
{
    if (channel == 1) {
        error(text.data(), m_File0);
    } else if (channel == 2) {
        error(text.data(), m_File1);
    } else if ( channel == 3) {
        error(text.data(), m_File2);
    } else if (channel == 4){
        error(text.data(), m_File3);
    }
}

void Logger::error(const char* text, LogChannel channel) throw()
{
   if (channel == 1) {
       error(text, m_File0);
   } else if (channel == 2) {
       error(text, m_File1);
   } else if ( channel == 3) {
       error(text, m_File2);
   } else if (channel == 4){
       error(text, m_File3);
   }
}


// Interface for Info Log
void Logger::info(const char* text, std::ofstream& file) throw()
{
   string data;
   data.append("[INFO]: ");
   data.append(text);

   if((m_LogType == FILE) && (m_LogLevel == LOG_ALL))
   {
      logIntoFile(data, file);
   }
   else if((m_LogType == CONSOLE) && (m_LogLevel == LOG_ALL))
   {
      logOnConsole(data);
   }
}
void Logger::info(string text, LogChannel channel) throw()
{
    if (channel == 1) {
        info(text.data(), m_File0);
    } else if (channel == 2) {
        info(text.data(), m_File1);
    } else if ( channel == 3) {
        info(text.data(), m_File2);
    } else if (channel == 4){
        info(text.data(), m_File3);
    }
}
void Logger::info(const char* text, LOG_CHANNEL channel) throw()
{
    if (channel == 1) {
        info(text, m_File0);
    } else if (channel == 2) {
        info(text, m_File1);
    } else if ( channel == 3) {
        info(text, m_File2);
    } else if (channel == 4){
        info(text, m_File3);
    }
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
   m_LogType = FILE;
}

