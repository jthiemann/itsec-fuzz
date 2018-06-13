#include <fstream>
#include <iostream>
#include "util.h"
#include <stdlib.h>





class Dmesg
{
public:
    static Dmesg* getInstance();

    void readDmesg();
    void setVCAN(bool b);

    int upSPI(int spi);
    int downSPI(int spi);
    int upSPIAll();
    int downSPIAll();

    int numSPI0_0() {return 0;}
    int numSPI0_1() {return 1;}
    int numSPI1_0() {return 2;}

    std::string nameSPI0_0() {return nameSPI(0);}
    std::string nameSPI0_1() {return nameSPI(1);}
    std::string nameSPI1_0() {return nameSPI(2);}
    std::string nameSPI(int spi);
    const char * nameSPIc_str(int spi) {return nameSPI(spi).c_str();}

    int getSPIXfromCANX(int iface);
    int getCANXfromSPIX(int spi);

private:
    static Dmesg* m_Instance;
    Dmesg();
    bool loaded;
    bool vcan;
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
