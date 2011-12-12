#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <netinet/ether.h>
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 1000

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) {
        cerr << "Syntaxe:\n\t" << argv[0]
                << " " << "adresa port" << endl;
        return -1;
    }
    string servername = argv[1];
    int port = atoi(argv[2]);
    string filename = argv[3];
    
    string text("GET ");
    text += filename;
    text += " HTTP/1.0\nhost: ";
    text += servername;
    text += "\n\n";
//    string text("GET /art/cpp/tcp_klient_linux.html HTTP/1.0\nhost: www.builder.cz\n\n");
    hostent *host;                          // Vzdaleny pocitac
    sockaddr_in serverSock;                 // Vzdaleny "konec potrubi"
    int mySocket;                           // Soket    
    char buf[BUFSIZE];                      // Prijimaci buffer
    int size, totalSize = 0;                // Pocet prijatých a odeslanych bytù
    // Zjistime info o vzdalenem pocitaci
    if ((host = gethostbyname("www.builder.cz")) == NULL)
    {
        cerr << "Spatna adresa" << endl;
        return -1;
    }
    // Vytvorime soket
    if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        cerr << "Nelze vytvorit soket" << endl;
        return -1;
    }
    // Zaplníme strukturu sockaddr_in
    // 1) Rodina protokolu
    serverSock.sin_family = AF_INET;
    // 2) Cislo portu, ke kteremu se pripojime
    serverSock.sin_port = htons(80);
    // 3) Nastavení IP adresy, ke ktere se pripojime
    memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);
    // Pripojeni soketu
    if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
    {
        cerr << "Nelze navazat spojeni" << endl;
        return -1;
    }
    // Odeslání dat
//    cout << "text k odeslani: " << text << endl;
    if ((size = send(mySocket, text.c_str(), text.size() + 1, 0)) == -1)
    {
        cerr << "Problem s odeslanim dat" << endl;
        return -1;
    }
//    cout << "Odeslano " << size << endl;
    // Prijem dat
    text = "";
    while (((size = recv(mySocket, buf, BUFSIZE - 1, 0)) != -1) && (size != 0))
    {
        buf[size] = '\0';
        totalSize += size;
        text += buf;
    }      
    // Uzavru spojeni
    close(mySocket);
    cout << "Prijato " << totalSize << " bytu" << endl << "HTTP Hlavicka:" << endl;
    // Rozdelim HTTP odpoved
    int offset = text.find("\r\n\r\n");
    string hlavicka = text.substr(0,offset+2);
    string telo = text.substr(offset+4);
//    cout << "TEXT: " << endl << text << "ENDTEXT" << endl;
    cout << hlavicka << endl;
    cout << "HTTP Telo:\n" << telo << endl << endl;
    return 0;
}

/* Vice informaci k funkci copy je na drese 
                   http://www.builder.cz/art/cpp/copyalg.html
*/