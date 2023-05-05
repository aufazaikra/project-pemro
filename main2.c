#include<stdio.h>

int main(){
    FILE *fpw = fopen("database/login.bin", "wb");
    char login[20] = "aufazaikra@aufa";

    fwrite(login, sizeof(char), sizeof(login)/sizeof(char), fpw);
    
    fclose(fpw);
    return 0;
}