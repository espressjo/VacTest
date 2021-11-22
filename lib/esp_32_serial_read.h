#ifndef ESP_32_SERIAL_READ_H
#define ESP_32_SERIAL_READ_H
#include <stdio.h>		/* fprintf */
#include <stdlib.h>
#include <string.h>		/* strcpy, strlen */
#include <ctype.h>		/* tolower */
#include <fcntl.h>      	/* open */
#include <unistd.h>		/* read, write, close */
#include <termio.h>		/* ioctl */
#include <string>
#define DELAIMAX 5000	/* delai maximum pour l'attente en ms*/


#define NOK -1            	/* valeur de retour avec erreur */
#define OK 0

class esp_32_serial_read
{
    public:
        esp_32_serial_read();
        int ouvrirport(int port,int vitesse) ;
        int configport();
        int fermerport();
        int ecrireport(std::string str);
        int lireport (std::string *str, int ncar);
        int lirec(char c);
        int verification(std::string str);
            int status;
    private:
        struct termio serie;		/* pour la configuration du port serie */
        int idport;
        int fd;
        std::string conversion(std::string ss);
        void delay(int ms);

};
#endif // ESP_32_SERIAL_READ_H
