#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_SUPER_SECRET_KEY 3522

int getN(int numb)
{
    int n = 0;
    while (numb>0){
        numb /= 10;
        n++;
    }
    return n;
}

long ppow(int x, int y)
{
    int i;
    long res = 1; 
    for(i=0; i<y; i++) res*=x;
    return res;
}

int main(int argc, char* argv[])
{
    int key = DEFAULT_SUPER_SECRET_KEY;
    
    int opt;
    while ((opt = getopt(argc, argv, ":k:")) != -1) {
        switch(opt) {
            case 'k':
                sscanf(optarg,"%d",&key);
                break;
            default:
                break;
        }
    }

    int i = 0,right;
    long p;
    int N = getN(key);
    int new_N;
    int temp_key;
    long int cur_key = key;
    
    while (1){
        p = cur_key*cur_key;
        new_N = getN(p);
        right = new_N / 2;
        cur_key = p / ppow(10, right);
        cur_key = cur_key %  ppow(10, N);

        temp_key = (int) cur_key;
        do{
            printf("%c", (char)temp_key);
            temp_key>>=sizeof(char)*8;
            i++;
        }while(temp_key != 0);
        
        if (cur_key == 0) {cur_key = key;}
    }
    printf("\n");

    return 0;
}