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
void Logger::error(const char* text, std::ofstream& file) throw()
{
   string data;
   data.append("[ERROR]: ");
   data.append(text);

   logIntoFile(data, file);
   logOnConsole(data);
}

void Logger::error(string text, LogChannel channel) throw()
{
    error(text.data(), channel);
}

void Logger::error(const char* text, LogChannel channel) throw()
{
   if (channel == 0) {
       error(text, m_File0);
   } else if (channel == 1) {
       error(text, m_File1);
   } else if ( channel == 2) {
       error(text, m_File2);
   } else if (channel == 3){
       error(text, m_File3);
   }
}
void Logger::error(std::ostringstream& stream, LogChannel channel) throw()
{
   string text = stream.str();
   error(text.data(), channel);
}

// Interface for State Log
void Logger::state(const char* text, std::ofstream& file) throw()
{
   if (m_LogLevel >= 2) {
       string data;
       data.append("[STATE]: ");
       data.append(text);

       logIntoFile(data, file);
       logOnConsole(data);
   }
}

void Logger::state(string text, LogChannel channel) throw()
{
    state(text.data(), channel);
}

void Logger::state(const char* text, LogChannel channel) throw()
{
   if (channel == 0) {
       state(text, m_File0);
   } else if (channel == 1) {
       state(text, m_File1);
   } else if ( channel == 2) {
       state(text, m_File2);
   } else if (channel == 3){
       state(text, m_File3);
   }
}
void Logger::state(std::ostringstream& stream, LogChannel channel) throw()
{
   string text = stream.str();
   state(text.data(), channel);
}

// Interface for Info Log
void Logger::info(const char* text, std::ofstream& file) throw()
{
   if (m_LogLevel == 3) {
       string data;
       data.append("[INFO]: ");
       data.append(text);

       logIntoFile(data, file);
       /*logOnConsole(data);*/
   }

}
void Logger::info(string text, LogChannel channel) throw()
{
    info(text.data(), channel);
}

void Logger::info(const char* text, LogChannel channel) throw()
{
    if (channel == 0) {
        info(text, m_File0);
    } else if (channel == 1) {
        info(text, m_File1);
    } else if ( channel == 2) {
        info(text, m_File2);
    } else if (channel == 3){
        info(text, m_File3);
    }
}

void Logger::info(std::ostringstream& stream, LogChannel channel) throw()
{
   string text = stream.str();
   info(text.data(), channel);
}

LogChannel FuzzLogging::getChannelNameByNumber(int channel) {
    if (channel == 0) {
        return interface0;
    } else if (channel == 1) {
        return interface1;
    } else if (channel == 2) {
        return interface2;
    } else if (channel == 3){
        return debugfile;
    }
}
