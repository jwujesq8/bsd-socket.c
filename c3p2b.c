#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

#define IP(H) *((unsigned long*) (H)->h_addr_list[0])

int main(void)
{
    int gn;
    struct sockaddr_in adr;
    int port;
    struct hostent *h;
    char nazwa[512]; //name
    char bufor[1025]; //buffer
    char sciezka[512]; //path
    long dl_pliku, odebrano, odebrano_razem; //dl_of_the_file, received, received_in_total
    
    printf("Nazwa hosta / adres IP (hostname/IP address): ");
    scanf("%s", nazwa);
    h = gethostbyname(nazwa);
    if (h == NULL)
    {
        printf("Nieznany host (unknown host)\n");
        return 1;
    }
    printf("Numer portu (port number): ");
    scanf("%d", &port);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = IP(h);
    
    printf("Lacze sie z (connecting to) %s:%d\n",
        inet_ntoa(adr.sin_addr),
        port);
    
    gn = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(gn, (struct sockaddr*) &adr, sizeof(adr))<0)
    {
        printf("Nawiazanie polaczenia nie powiodlo sie (connection failed)\n");
        close(gn);
        return 1;
    }
    printf("Polaczenie nawiazane (connection accepted)\n");
    printf("Podaj sciezke do pliku (give the path to the file): \n");
    memset(sciezka, 0, 512);
    scanf("%s",sciezka);    
    printf("Wysylam sciezke (I'm sending filepath)\n");
    if (send(gn, sciezka, strlen(sciezka), 0) != strlen(sciezka))
    {
        printf("Blad przy wysylaniu sciezki (filepath send error)\n");
        close(gn);
        return 1;
    }
    printf("Sciezka wyslana (path sent). Odczytuje dlugosc pliku (reading filesize).\n");
    if (recv(gn, &dl_pliku, sizeof(long), 0) != sizeof(long))
    {
        printf("Blad przy odbieraniu dlugosci (recv error)\n");
        printf("Moze plik nie istnieje (perhaps the file don't exist)?\n");
        close(gn);
        return 1;
    }
    dl_pliku = ntohl(dl_pliku);
    printf("Plik ma dlugosc (the filesize is) %ld\n", dl_pliku);
    printf("----- ZAWARTOSC PLIKU ----- (file contents)\n");
    odebrano_razem = 0;
    while (odebrano_razem < dl_pliku) //total received
    {
        memset(bufor, 0, 1025);
        odebrano = recv(gn, bufor, 1024, 0);
        if (odebrano < 0)
            break;
        odebrano_razem += odebrano;
        fputs(bufor, stdout);
    }
    close(gn);
    if (odebrano_razem != dl_pliku)
        printf("*** BLAD W ODBIORZE PLIKU (error)***\n");
    else
        printf("*** PLIK ODEBRANY POPRAWNIE (ok) ***\n");
    return 0;
}




