#include "gauge972.h"

gauge972::gauge972()
{
     idport=0;
     fd=0;
     status=0;
}
int gauge972::ouvrirport(int port,int vitesse) /* port = "0", "1" ou "2" , Vallee fev. 2012 */

{
  //char fichier[100];

    std::string fichier="/dev/ttyS"+std::to_string(port);
    //printf("On ouvre le port ttyS%d\n.",port);
  //strcpy(fichier.,"/dev/ttyS"); /* modif pour ordi mozart-fedora, ancien /dev/tty */
  //strcat(fichier,port);

  if ((fd = open(fichier.c_str(), O_RDWR)) == NOK)
  {
    fprintf(stderr,"Erreur lors de l'ouverture du port serie %s, fd =%d .\n",fichier.c_str(),fd);
    status = NOK;
    return NOK;
  }

  serie.c_iflag = IGNCR;     	/* ignore <CR> */
  serie.c_oflag = 0;
  serie.c_cflag = CS8 | CREAD | CLOCAL;
  serie.c_lflag = 0;
  serie.c_cc[VMIN] = 0;		/* nombre minimum de caracteres */
  serie.c_cc[VTIME] = 1;	/* nombre x 0.1 s d'attente max */
                /* lors d'un appel de read */
  if(vitesse==19200)
    serie.c_cflag = serie.c_cflag | B19200;
  else if(vitesse==115200)
    serie.c_cflag = serie.c_cflag | B115200;
  else if(vitesse==38400)
    serie.c_cflag = serie.c_cflag | B38400;
  else if(vitesse==9600)
    serie.c_cflag = serie.c_cflag | B9600;
  else if(vitesse==4800)
    serie.c_cflag = serie.c_cflag | B4800;
  else if(vitesse==2400)
    serie.c_cflag = serie.c_cflag | B2400;
  else
    serie.c_cflag = serie.c_cflag | B9600;

  if (ioctl(fd,TCSETA,&serie) == NOK)
  {
    printf("port = %d\n",port);
    printf("ioctl(fd,TCSETA,&serie) = %d\n",ioctl(fd,TCSETA,&serie));
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return NOK;
  }
  idport=fd;
  return 0;
}

int gauge972::configport()
{
  /* configuration du port */

  if (ioctl(fd,TCGETA,&serie) == NOK)
  {
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return NOK;
  }

  serie.c_iflag = 0;

  if (ioctl(fd,TCSETA,&serie) == NOK)
  {
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return (NOK);
  }
  idport=fd;
  return OK;
}

/*==========================================================================
    fonction: 	int fermerport(int fd)

    description:	Ferme le port serie.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int gauge972::fermerport()
{
  if (close(fd)== NOK)
  {
    fprintf(stderr,"Erreur lors de la fermeture du port serie.\n");
    return NOK;
  }
  return OK;
}

/*==========================================================================
    fonction: 	int ecrireport(int fd, char *str)

    description:	Envoie une chaine sur le port serie.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int gauge972::ecrireport(std::string str)
{
  int ncar;
  ncar=strlen(str.c_str());
  if(write(fd,str.c_str(),ncar)!=ncar)
  {
    fprintf(stderr,"Erreur lors de l'ecriture sur le port serie.\n");
    return NOK;
  }
  return OK;
}

/*==========================================================================
    fonction: 	int lireport(fd, char *str, int ncar)

    description:	lit au plus ncar sur le port serie, copie les
            caracteres lus dans la chaine str, et ajoute '\0'.

            Retourne le nombre de caracteres lus.
            Retourne 0 si aucun caractere lu.
            Retourne ERREUR si une erreur a eu lieu.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int gauge972::lireport (std::string *str,int ncar)

{
  int i,n;
  *str = "";
  char *tmpstr = new char[ncar];
  memset(tmpstr,0,ncar);

  for(i=0;i<ncar;i++)
  {
        n = read(fd,tmpstr+i,1);
        //printf("n: %d\n",n);
        if(n==0)
        {
          tmpstr[i]='\0';
          *str = conversion(tmpstr);
          delete [] tmpstr;
          return i;
        }
  }
  tmpstr[i]='\0';
  *str = conversion(tmpstr);
  delete [] tmpstr;
  return i;
}

/*==========================================================================
    fonction: 	int lirec(int fd, char c)

    description:	Lit sur le port serie jusqu'au caractere c.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
void gauge972::delay(int ms)
/*!
  *\brief Delay in ms
  * Create a delay. ms is delay time in millisecond
  */
{
    clock_t t0 = clock();
    while (clock()<t0+ms*1000)
        ;
}
int gauge972::lirec(char c)
{
  std::string str;
  int i=0;
  do
  {
      delay(500);
      i+=500;
    if(lireport(&str,1)==NOK) return NOK;
    if(i>=DELAIMAX) return NOK;
  } while(str[0]!=c);
  return OK;
}

std::string gauge972::conversion(std::string ss)
{
 std::string buff="";
  for (auto &c:ss)
  {
    buff+=c & 0x7F;

  }
  if (buff[buff.length()-1]=='\n')
  {
      buff[buff.length()-1] = '\0';
  }
  return buff;
}


