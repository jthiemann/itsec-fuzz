#include <fstream>
#include "util.h";


class Config
{
public:

    Config(const std::string& path);
    ~Config();
    std::string read();
    char * interfaceNameTranslation(std::string bustype);

private:
    bool loaded;
    std::string can0,can1,can2;
    std::ofstream File;
};

class LogFile
{
public:

    LogFile(const std::string& name);
    ~LogFile();
    void close();
    void timestamp();
    void logCANframeAsOutput(can_frame * frame);
    void logCANframeAsInput(can_frame * frame);
    void logError(std::string msg);



private:
    std::ofstream File;
};
