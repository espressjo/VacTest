#include "gauge972.h"
#include <iostream>

int parse_arg(int argc, char *argv[], int *dev, std::string *cmd, int *baudrate, int *nbyte);
void help(void);
void check_cmd(std::string *cmd);
bool strip_answer(std::string *asw);
int main(int argc, char *argv[])
{
    std::string cmd="",reponse="";
    int baudrate=115200,dev=0;
    int nbyte=128;
    parse_arg(argc,argv,&dev,&cmd,&baudrate,&nbyte);
    check_cmd(&cmd);
    std::cout<<"Sending: "<<cmd.substr(0,cmd.length()-1)<<" to gauge 972 @ br: "<<baudrate<<" on port /dev/ttyS"<<dev<<std::endl;
    //check_cmd(&cmd);
    gauge972 gauge;
    gauge.ouvrirport(dev,baudrate);
    gauge.ecrireport(cmd);
    gauge.lireport(&reponse,nbyte);
    if (strip_answer(&reponse))
    {
        std::cout<<"Answer: "<<reponse<<std::endl;
    }
    else {
        std::cout<<"Did not receive anything from the 972 gauge"<<std::endl;
    }

    gauge.fermerport();


    return 0;
}

int parse_arg(int argc, char *argv[], int *dev, std::string *cmd, int *baudrate,int *nbyte)
{
    if (argc<2)
    {
        help();
        exit(0);
    }
    for (int i=1;i<argc;i++)
    {
        if (std::string(argv[i]).compare("--dev")==0)
        {
            *dev = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--nbyte")==0)
        {
            *nbyte = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--cmd")==0)
        {
            *cmd = std::string(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--baudrate")==0)
        {
            *baudrate = std::atoi(argv[i+1]);
        }
        if (std::string(argv[i]).compare("--help")==0)
        {
            help();
            exit(0);
        }

    }
    return 0;
}
void help(void)
{
    std::cout<<"Usage: Gauge972cmd --dev <device number> --cmd <MKS 972 command> --baudrate <baudrate> --nbyte <max byte to read, default 128>"<<std::endl;

}
bool strip_answer(std::string *asw)
{
    if (asw->substr(0,1).compare("@")!=0)
    {
        return false;
    }
    *asw = asw->substr(7,asw->length()-1);//strip the @253ack
    std::string tmp="";
    for (auto &c:*asw)
    {
        if (c==';')
        {
            break;
        }
        tmp+=c;
    }
    *asw = tmp;
    return true;
}
void check_cmd(std::string *cmd)
{
    char a[2];
    a[0]='\\';
    a[1]='f';

    std::string b=std::string(a);
    //strip if \\f is entered with the command
    if (cmd->substr(cmd->length()-2,2).compare(b)==0)
    {
        *cmd = cmd->substr(0,cmd->length()-2);
    }
    //strip if \f is entered with the command
    else if (cmd->substr(cmd->length()-1,1).compare("f")==0)
    {
        *cmd = cmd->substr(0,cmd->length()-1);
    }
    //strip the check if ;FF is at the end of the command
    if (cmd->substr(cmd->length()-3,3).compare(";FF")!=0)
    {
        *cmd+=";FF";
    }
    //make sure \f is the last character
    *cmd+="\f";
    return;
}

