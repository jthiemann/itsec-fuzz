#include <fstream>
#include <iostream>
#include "util.h"
#include <stdlib.h>





class Dmesg
{
public:
    static Dmesg* getInstance();

    void readDmesg();

    int upSPI(int iface);
    int downSPI(int iface);
    int upSPIAll();
    int downSPIAll();

    int numSPI0_0() {return 0;}
    int numSPI0_1() {return 1;}
    int numSPI1_0() {return 2;}

    int getSPIXfromCANX(int iface);
    int getCANXfromSPIX(int spi);

private:
    static Dmesg* m_Instance;
    Dmesg();
    bool loaded;
    int canX[3];
    std::ofstream File;
};

class Config
{
public:

    Config(const std::string path);
    ~Config();
    const char *NameComfort();
    const char *NameDiagnose();
    const char *NameEngine();


private:
    std::string Name(int type);
    bool loaded;
    int canX[3];
    bool v[3];
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
