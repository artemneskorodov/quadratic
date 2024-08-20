#include <stdio.h>

int main(void){
    int a = 0;
    scanf("%d", &a);
    switch(a){

        case 0: {
                int b = 4;
                printf("1");
                break;
        }
        case 1: printf("1");
                break;
        default: printf("chebureki");
                return 1;

    }
}
