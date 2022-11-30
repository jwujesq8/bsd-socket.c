#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define PORT htons(8080)

/*
  int main(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return(0);
}
*/

char showTheList(char* path){
    Dir* d;
    struct dirent* dir;
    if(chdir(path)<0){
        printf("Blędna ściezka");
        return;
    }
    else
    {
        chdir(path);
        d = opendir(path);
        while((dir = readdir(d)) != null) {
            printf("%s\n", dir->d_name);
            closedir(d);
    }
    }
}


void serviceConnection(int socket) //ServeConnection
{
    char path_to_file[512];
    unsigned long file_size, sended, all_sended, readen;
    struct stat fileinfo;
    FILE* file;
    unsigned char bufor[4000000];
    
    memset(path_to_file, 0, 512);
    if (recv(socket, path_to_file, 512, 0) <= 0)
    {
        printf("Potomny: blad przy odczycie sciezki\nChild (process): error while reading a path\n");
        return;
    }

    printf("Potomny: klient chce plik (Child: client wants a file) %s\n", path_to_file);

    //file information
        if (stat(path_to_file, &fileinfo) < 0)
        {
            printf("Potomny: nie moge pobrac informacji o pliku\n(Child: I can't obtain information about a file)\n");
            return;
        }
        if (fileinfo.st_size == 0)
        {
            printf("Potomny: rozmiar pliku (child: filesize) 0\n");
            return;
        }
    printf("Potomny: dlugosc pliku (child: filesize): %ld\n", fileinfo.st_size);
    file_size = htonl((long) fileinfo.st_size)

    if (send(socket, &file_size, sizeof(long), 0) != sizeof(long))
    {
	printf("Potomny: blad przy wysylaniu wielkosci pliku (error at sending a file)\n");
	return;
    }
    
    file_size = fileinfo.st_size;
    all_sended = 0;
    file = fopen(path_to_file, "rb");
    if (file == NULL)
    {
	printf("Potomny: blad przy otwarciu pliku (error at opening a file)\n");
	return;
    }
    
    while (all_sended < file_size)
    {
        readen = fread(bufor, 1, sizeof(file), file);
        sended = send(socket, bufor, readen, 0);
	    if (readen != sended) break;

        all_sended += sended;
	    printf("Potomny: wyslano %ld bajtow (total sent)\n", all_sended);
    }
    
    if (all_sended == file_size)
	    printf("Potomny: plik wyslany poprawnie (child: file sent ok)\n");
    else
	    printf("Potomny: blad przy wysylaniu pliku (child: file sending error)\n"); //error at se
    fclose(plik);
    return;    
}


int main(void)
{
    int s_listen, s_client;
    struct sockaddr_in adr;
    socklen_t address_size = sizeof(struct sockaddr_in);
    
    s_listen = socket(PF_INET, SOCK_STREAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = PORT;
    adr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(s_listen, (struct sockaddr*) &adr, address_size) < 0)
    {
        printf("Glowny: bind nie powiodl sie (Parent: bind failed)\n");
        return 1;
    }
    
    listen(s_listen, 10);
    
    while(1)
    {
	address_size = sizeof(struct sockaddr_in);
	s_client = accept(s_listen, (struct sockaddr*) &adr, &address_size);
	if (s_client < 0)
	{
	    printf("Glowny: accept zwrocil blad (Parent: accept error)\n");
	    continue;
	}
	printf("Glowny: polaczenie od (Parent: connection from) %s:%u\n", 
	    inet_ntoa(adr.sin_addr),
	    ntohs(adr.sin_port)
	    );
	printf("Glowny: tworze proces potomny (Parent: I'm creating child process)\n");
	if (fork() == 0)
	{
	    printf("Potomny: zaczynam obsluge (Child: start serving)\n");
        serviceConnection(s_client);
	    printf("Potomny: zamykam gniazdo (Child: closing socket)\n");
	    close(s_client);
	    printf("Potomny: koncze proces (Child: end of the process)\n");
	    exit(0);
	}
	else
	{
	    printf("Glowny: wracam do nasluchu (parent: return to listening)\n");
	    continue;
	}
    }
    return 0;
}




