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
const string file3 = "../logfiles/debug.log";
const string file4 = "../logfiles/results.log";
const string file5 = "../logfiles/inputmsg.log";

Logger::Logger()
{
   m_File0.open(file0.c_str(), ios::out|ios::app);
   m_File1.open(file1.c_str(), ios::out|ios::app);
   m_File2.open(file2.c_str(), ios::out|ios::app);
   m_File3.open(file3.c_str(), ios::out|ios::app);
   m_File4.open(file4.c_str(), ios::out|ios::app);
   m_File5.open(file5.c_str(), ios::out|ios::app);
   m_LogLevel	= LOG_ALL;
}

Logger::~Logger()
{
   m_File0.close();
   m_File1.close();
   m_File2.close();
   m_File3.close();
   m_File4.close();
   m_File5.close();
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
void Logger::error(const char* text, std::ofstream& file, LogChannel channel) throw()
{
   string data;
   data.append(toString(channel));
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
       error(text, m_File0, channel);
   } else if (channel == 1) {
       error(text, m_File1, channel);
   } else if (channel == 2) {
       error(text, m_File2, channel);
   } else if (channel == 3){
       error(text, m_File3, channel);
   } else if (channel == 4){
       error(text, m_File4, channel);
   }else if (channel == 5){
       error(text, m_File5, channel);
   }
}

void Logger::error(std::ostringstream& stream, LogChannel channel) throw()
{
   string text = stream.str();
   error(text.data(), channel);
}

// Interface for message Log
void Logger::message(const char* text, std::ofstream& file, LogChannel channel) throw()
{
    string data;
    data.append(toString(channel));
    data.append("[MESSAGE]: ");
    data.append(text);

    logIntoFile(data, file);
}

void Logger::message(string text, LogChannel channel) throw()
{
    message(text.data(), channel);
}

void Logger::message(const char* text, LogChannel channel) throw()
{
   if (channel == 0) {
       message(text, m_File0, channel);
   } else if (channel == 1) {
       message(text, m_File1, channel);
   } else if (channel == 2) {
       message(text, m_File2, channel);
   } else if (channel == 3){
       message(text, m_File3, channel);
   } else if (channel == 4){
       message(text, m_File4, channel);
   } else if (channel == 5){
       message(text, m_File5, channel);
   }
}

void Logger::message(std::ostringstream& stream, LogChannel channel) throw()
{
   string text = stream.str();
   message(text.data(), channel);
}

// Interface for Info Log
void Logger::info(const char* text, std::ofstream& file, LogChannel channel) throw()
{
   if (m_LogLevel == 2) {
       string data;
       data.append(toString(channel));
       data.append("[INFO]: ");
       data.append(text);

       logIntoFile(data, file);
       logOnConsole(data);
   }

}
void Logger::info(string text, LogChannel channel) throw()
{
    info(text.data(), channel);
}

void Logger::info(const char* text, LogChannel channel) throw()
{
    if (channel == 0) {
        info(text, m_File0, channel);
    } else if (channel == 1) {
        info(text, m_File1, channel);
    } else if (channel == 2) {
        info(text, m_File2, channel);
    } else if (channel == 3){
        info(text, m_File3, channel);
    } else if (channel == 4){
        info(text, m_File4, channel);
    } else if (channel == 5){
        info(text, m_File5, channel);
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
    } else if (channel == 4){
        return resultfile;
    } else if (channel == 5){
        return inputmsgfile;
    }
}

static char* FuzzLogging::toString(LogChannel channel) {
    switch (channel)
    {
        case 0: return "[interface0]  ";
        case 1: return "[interface1]  ";
        case 2: return "[interface2]  ";
        case 3: return "[debugfile ]  ";
        case 4: return "[resultfile]  ";
        case 5: return "[inputmsgs ]  ";
    default: return "UNKNOWN CHANNEL  ";
    }
}
