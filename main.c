#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"

struct prefixo start_prefixo = {.mpg1 = 0x00,.mpg2 = 0x00 , .mpg3 = 0x01};

int main(int argc, char *argv[]){
    FILE * arq_in = fopen(argv[1] , "rb");
    int cont = 0;
    char flag,tipo;
    unsigned char a;
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
    unsigned int largura;
    unsigned int altura;
    unsigned int frame_rate;
    float fps;
    if(arq_in == NULL){
        printf("FODEUUU\n");
    }
    struct prefixo tmp;
    while(!feof(arq_in)){
        flag = 'f';
    fread(&tmp,sizeof(struct prefixo),1,arq_in);
    if(tmp.mpg1 == start_prefixo.mpg1 && tmp.mpg2 == start_prefixo.mpg2 && tmp.mpg3== start_prefixo.mpg3){
        fread(&a,1,1,arq_in);
        if(a == 0xBA){
            printf("--> Codigo: %.2X -- Pack\n",a);
        }
        else if(a == 0xBA){
            printf("--> Codigo: %.2X -- System\n",a);
        }
        else if(a == 0xB3){
            byte1 = fgetc(arq_in);
            byte2 = fgetc(arq_in);
            byte3 = fgetc(arq_in);
            byte4 = fgetc(arq_in);
            largura = byte1 * 16 + (byte2 >> 4);
            altura = ( byte2 & 0x0F) * 256 + byte3;
            frame_rate = byte4 & 0x0f;
            if(frame_rate == 1){
                fps = 23.976;
            }
            else if(frame_rate == 2){
                fps = 24;
            }
            else if(frame_rate == 3){
                fps = 25;
            }
            else if(frame_rate == 4){
                fps = 29.970;
            }
            else if(frame_rate == 5){
                fps = 30;
            }
            else if(frame_rate == 6){
                fps = 50;
            }
            else if(frame_rate == 7){
                fps = 59.940;
            }
            else if(frame_rate == 8){
                fps = 60;
            }

            printf("--> Codigo: %.2X -- Sequence HEader -- Widith = %d -- Height = %d -- Frame Rate = %.3f\n",a,largura,altura,fps);
        }
        else if(a == 0xB8){
            printf("--> Codigo: %.2X -- Group of Pictures\n",a);
        }
        else if(a == 0x00){
            byte1 = fgetc(arq_in);
            byte2 = fgetc(arq_in);
            byte2 = (byte2 >> 3) & 0x07;
            if(byte2 == 0x01){
                tipo = 'I';
            }
            else if(byte2 == 0x02){
                tipo = 'P';
            }
            else if(byte2 == 0x03){
                tipo = 'B';
            }
            printf("--> Codigo: %.2X -- Picture -- Tipo = %c\n",a,tipo);
        }
        else if(a >= 0x01 && a <= 0xAF){
            printf("--> Codigo: %.2X -- Slice\n",a);
        }
        else if(a >= 0xC0 && a <= 0xDF){
            printf("--> Codigo: %.2X -- Packet Video\n",a);
        }
        else if(a >= 0xE0 && a <= 0xEF){
            printf("--> Codigo: %.2X -- Packet Audio\n",a);
        }
        else{
            printf("--> Codigo: %.2X -- Nao implementado.\n",a);
        }
        flag = 'v';
    }

    if(flag == 'f'){
        fseek(arq_in,-2,SEEK_CUR);
    }
    cont ++;
    }

}