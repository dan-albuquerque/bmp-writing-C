#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

int main(int argc, char* argv[]) 
{
  BMPData *bmp_data = NULL;
  BMPFileHeader file_header;
  BMPHeader bmp_header;
  //LENDO OS ARQUIVOS POR LINHA DE COMANDO
  bmp_data = bmp_read(argv[1], &file_header, &bmp_header);

  if (bmp_data == NULL)
  {
    printf("Unable to process file\n");
    exit(1);
  }    
  int resposta = bmp_write(argv[2], &file_header, &bmp_header, bmp_data);
  if(resposta != 0){
    printf("falha ao escrever o arquivo BMP\n");
    exit(1);
  }else{
    printf("arquivo alterado com sucesso!\n");
  }
  //tom_de_cinza = (bmp_data->r*0.11) + (bmp_data->g*0.59) + (bmp_data->b*0.3);
  //LIBERANDO A MEMORIA:
  free(bmp_data);
  return 0;
}