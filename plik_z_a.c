#include <stdio.h>
#include <stdlib.h>

int main(void){
    unsigned char bufor[4000000];
    File* plik;
    plik = fopen("/home/students/s473618/Desktop/s473618/3sem/sk/projekt", "w");
    memset(*plik, "a", sizeof(bufor));
    fclose(plik);
}
