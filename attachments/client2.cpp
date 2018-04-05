 /* Client code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <iostream>
  #include <string> 
  #include <thread>  
  #include <sstream> 

  using namespace std;

  void readC(int SocketP,char buffer[]) //leer 
  {
    int n=0;

   bzero(buffer,256);
    n = read(SocketP, buffer, 256);
    if(n < 0)
    perror("Error de lectura del socket");
    
  }



  string convertirHeader(string mensajeIngresado)
  { 
    string tmp;
    int tama;
    tama=mensajeIngresado.size();
    stringstream ss;
    ss<<tama;
    tmp=ss.str();
    while(tmp.size()<4)
    {
      tmp="0"+ tmp;
    } 
    return tmp+mensajeIngresado;

  }

  void writeC(int SocketP)
  {
    string mensajeIngresado, header;
    string nickname;
    int tam;
    //char msg[256];
    int n=0;
    cout<<"Login: ";
    getline(cin,mensajeIngresado);
    mensajeIngresado=convertirHeader(mensajeIngresado);
    cout<<"mensaje en write Cliente  "<<mensajeIngresado<<endl;
    //aqui le agrego header jee

   
    tam=mensajeIngresado.size();
    char msg[tam];
    strcpy(msg,mensajeIngresado.c_str());
   

    n = write(SocketP,msg,tam);

    if(n < 0)
      perror("Error escribiendo a socket");

  }

  int chat(int SocketFD)
{
    char buffer[256];
    char buffer2[256];
    int n;
    do
    { 
      
       thread writee(writeC,SocketFD);
       writee.join();

       thread rd(readC,SocketFD,buffer2);
       rd.join();
        printf("El: [%s]\n",buffer2);
    }while (strncmp("END",buffer,3)!=0);

    shutdown(SocketFD, SHUT_RDWR);
 
    close(SocketFD);
}
 
  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
      return 0;
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(2100);
    //Res = inet_pton(AF_INET, "192.168.8.101", &stSockAddr.sin_addr);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
 
    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
      return 0;
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
      return 0;
    }
 
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))  //conectarme con el servidor
    {
      perror("connecte failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
      return 0;
    }

    chat(SocketFD); //cierra el recurso 

    return 0;
  }
