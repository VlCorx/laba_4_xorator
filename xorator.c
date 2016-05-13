#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 30000

int main(int argc, char* argv[])
{
    int pipe_cat[2];
    int pipe_gen[2];

    char buf[MAX_LENGTH];
    
	int key = 0;
    char mas_gen[1000];
    char mas_cat[1000];
    char file[1000];

    int opt;
    while ((opt = getopt(argc, argv, ":g:c:k:f:")) != -1) {
        switch(opt) {
            case 'g':
                strcpy(mas_gen, optarg);
                break;
            case 'c':
                strcpy(mas_cat, optarg);
                break;
            case 'f':
                strcpy(file, optarg);
                break;
            case 'k':
                sscanf(optarg,"%d",&key);
                break;
            default:
                break;
        }
    }
    if ((strlen(file) < 1) || (strlen(mas_cat) < 1) || (strlen(mas_gen) < 1)) {
        printf("nope\n"); 
        exit(1);
    }
    
    if (pipe(pipe_gen) != 0) {
        printf("Ошибка создания pipe");        
        exit(1);
    }
    if (pipe(pipe_cat) != 0) {
        printf("Ошибка создания pipe");        
        exit(1);
    }

    if (fork() == 0){
        close(1);
        dup(pipe_cat[1]);
        close(pipe_cat[1]);
        execlp(mas_cat, mas_cat, file, (char*)0);
        _exit(0);
    }

    int buf_length = read(pipe_cat[0], buf, MAX_LENGTH);
    
    char args[2][1000];
    sprintf(args[0], "%d", buf_length);
    sprintf(args[1], "%d", key);


    if (fork() == 0){
        close(1);
        dup(pipe_gen[1]);
        close(pipe_gen[1]);

        if (key > 0) 
            execlp(mas_gen, mas_gen, "-l", args[0],"-k", args[1], (char*)0);
        else
            execlp(mas_gen, mas_gen, "-l", args[0], (char*)0);
        
        _exit(0);
    }

    char b[1];
    char res;
    int i;
    for(i = 0; i < buf_length; i++){
        read(pipe_gen[0], b, 1);
        res = b[0] ^ buf[i];
        printf("%c",res);
    }

    return 1;
}