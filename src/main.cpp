#include <iostream>
#include "gauge972.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include "esp_32_serial_read.h"
using namespace std;

int parse_arg(int argc, char *argv[], int *dev, double *timeout, std::string *logfile, int *usb);

void logs(std::string txt,std::string logfile,double timestamp);
void header(std::string logfile);
int main(int argc, char *argv[])
{
    int d=0;
    double max=0,cumul=0;
    int usb=0;
    std::string logfile = "test.csv",buff="",buff_esp="";
    //set begin and end
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    //devices
    gauge972 ls;
    esp_32_serial_read esp;

    //parse the command line options
    if (parse_arg(argc,argv,&d,&max,&logfile,&usb)!=0)
    {
        cout<<"Usage: pressure --usb <usb number> --dev <device number> --timeout <time out in min> --logfile <full path+logfile name>"<<endl;
        return 0;
    }
    //create csv file header
    header(logfile);
    //convert seconds to ms
    max*=60.0;

    esp.ouvrirport(usb,115200);
    ls.ouvrirport(d,115200);//test

    while (1) {
        begin = std::chrono::high_resolution_clock::now();
        try {
            ls.ecrireport("@253PR4?;FF\f");//test
            ls.lireport(&buff,18);//test
            esp.lireport(&buff_esp,1024);
        } catch (...) {
            end = std::chrono::high_resolution_clock::now();
            cumul+=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() /1e9;
            continue;
        }
        //if successfull, log the stuff
        end = std::chrono::high_resolution_clock::now();
        cumul+=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() /1e9;
        if (buff.length()>7 && buff_esp.length()>9)
        {//log only if the buffers contain something
            logs(buff.substr(7,8)+","+buff_esp.substr(9,8),logfile,cumul);
        }
        buff.clear();
        buff_esp.clear();
        if (cumul>=max)
        {
            break;
        }
    }//while

    esp.fermerport();
    ls.fermerport();
    return 0;
}

int parse_arg(int argc, char *argv[], int *dev,double *timeout,std::string *logfile,int *usb)
{
    *dev=0;
    if (argc%2==0 || argc==1)
    {
        return -1;
    }
    for (int i=1;i<argc;i++)
    {
        if (std::string(argv[i]).compare("--dev")==0)
        {
            *dev = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--usb")==0)
        {
            *usb = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--timeout")==0)
        {
            *timeout = std::stod(std::string(argv[i+1]));
        }
        if (std::string(argv[i]).compare("--logfile")==0)
        {
            *logfile = std::string(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--help")==0)
        {
            return 1;
        }

    }
    return 0;
}
void logs(std::string txt,std::string logfile,double timestamp)
{
    std::ofstream ofs;
    ofs.open (logfile, std::ofstream::out | std::ofstream::app);

    ofs <<std::to_string(timestamp)+","+txt+"\n";
    ofs.flush();
    ofs.close();
}
void header(std::string logfile)
{
    std::ofstream ofs;
    ofs.open (logfile, std::ofstream::out | std::ofstream::app);
    ofs <<"timestamp,pressure,voltage\n";
    ofs.flush();
    ofs.close();
}
