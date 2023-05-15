/**
 * File bmp_utils.c
 * @date May 9th, 2023
 * 
 * Implemented according to the following spec: 
 * http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
*/

#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

/**
 * Prints the content of a BMPFileHeader structure.
 *
 * @param file_header    Pointer to a BMPFileHeader structure to be printed.
 */
void bmp_print_file_header(BMPFileHeader *file_header)
{
  printf("--------------------------------------------\n");
  printf("BMP File Header\n");
  printf("--------------------------------------------\n");
  if (file_header == NULL)
  {
    printf("NULL Header\n");
    return;
  }

  printf("bfType: %c%c\n", file_header->bfType[0], file_header->bfType[1]);
  printf("bfSize: %u\n", file_header->bfSize);
  printf("bfReserved1: %u\n", file_header->bfReserved1);
  printf("bfReserved2: %u\n", file_header->bfReserved2);
  printf("brOffbits: %u\n", file_header->brOffbits);
  printf("--------------------------------------------\n");
}

/**
 * Prints the content of a BMPHeader structure.
 *
 * @param bmp_header    Pointer to a BMPHeader structure to be printed.
 */
void bmp_print_header(BMPHeader *bmp_header)
{
  printf("--------------------------------------------\n");
  printf("BMP Header\n");
  printf("--------------------------------------------\n");
  if (bmp_header == NULL)
  {
    printf("NULL Header\n");
    return;
  }

  printf("biSize: %u\n", bmp_header->biSize);
  printf("biWidth: %u\n", bmp_header->biWidth);
  printf("biHeight: %u\n", bmp_header->biHeight);
  printf("biPlanes: %u\n", bmp_header->biPlanes);
  printf("biBitCount: %u\n", bmp_header->biBitCount);
  printf("biCompression: %u\n", bmp_header->biCompression);
  printf("biSizeImage: %u\n", bmp_header->biSizeImage);
  printf("biXPelsPerMeter: %u\n", bmp_header->biXPelsPerMeter);
  printf("biYPelsPerMeter: %u\n", bmp_header->biYPelsPerMeter);
  printf("biClrUsed: %u\n", bmp_header->biClrUsed);
  printf("biClrImportant: %u\n", bmp_header->biClrImportant);
  printf("--------------------------------------------\n");
}

/**
 * Reads a BMP file and returns the data.
 *
 * @param filename      The name of the BMP file to read.
 * @param file_header   Pointer to a BMPFileHeader structure to store the file header.
 * @param bmp_header    Pointer to a BMPHeader structure to store the BMP header.
 *
 * @return              A pointer to a dynamically allocated BMPData structure containing the BMP file data.
 *                      Returns NULL if an error occurs during reading or memory allocation.
 */
BMPData * bmp_read(char * filename, BMPFileHeader * file_header, BMPHeader * bmp_header)
{
  FILE *fp;
  BMPData *bmp_data = NULL;
  unsigned int bmp_size = 0;

  // verify function partameters
  if(filename == NULL || file_header == NULL || bmp_header == NULL)
  {
    printf("ERROR: Invalid function parameters.\n");
    printf("Filename: %s\n", filename);
    printf("File header addr: %p\n", file_header);
    printf("BMP header addr: %p\n", bmp_header);
    return bmp_data;
  }
  
  fp = fopen(filename, "rb");

  if(fp == NULL)
  {
    printf("ERROR: Unable to open %s file for reading.\n", filename);
    return bmp_data;
  }

  fread(file_header, sizeof(BMPFileHeader), 1, fp);

  bmp_print_file_header(file_header);
  
  if(file_header->bfType[0] != 'B' && file_header->bfType[1] != 'M')
  {
    printf("ERROR: %s is not a BMP file\n", filename);
    fclose(fp);
    return bmp_data;
  }

  printf("Successfuly opened %s with %u bytes\n", filename, file_header->bfSize);

  fread(bmp_header, sizeof(BMPHeader), 1, fp);

  bmp_print_header(bmp_header);

  if (bmp_header->biSize < 40 || bmp_header->biPlanes != 1 || 
      bmp_header->biCompression != 0)
  {
    printf("ERROR: Invalid bitmap header.\n");
    fclose(fp);
    return bmp_data;
  }

  printf("Bitmap with %u x %u pixels\nand %u bits per pixel\n", 
      bmp_header->biWidth, bmp_header->biHeight, bmp_header->biBitCount);
  
  bmp_size = bmp_header->biWidth * bmp_header->biHeight;

  bmp_data = (BMPData *) malloc(bmp_size * sizeof(BMPData));

  if (bmp_data == NULL)
  {
    printf("ERROR: Unable to allocate memory for bitmap data\n");
    fclose(fp);
    return bmp_data;
  }

  unsigned int read_elements = fread(bmp_data, sizeof(BMPData), bmp_size, fp);
  if(read_elements != bmp_size) 
  {
    printf("ERROR reading BMP data(%u). Expected %u.\n", read_elements, bmp_size);
    free(bmp_data);
    bmp_data = NULL;
    fclose(fp);
    return bmp_data;
  }

  printf("Successfully loaded %d pixels from bitmap file.\n", read_elements);

  fclose(fp);
  return bmp_data;
}

//BMP_WRITE:
int bmp_write(char *filename, BMPFileHeader *file_header, BMPHeader *bmp_header, BMPData *bmp_data) {
  //abrindo o arquivo pra escrita de bits
  FILE *fp = fopen(filename, "wb");
  //caso nao leia o arquivo e fp aponte pra NULL
  if (fp == NULL) {
    printf("Erro ao abrir o arquivo para gravação.\n");
    exit(1);
  }
  // escreve o cabeçalho do arquivo BMP lido no arquivo de escrita
  fwrite(file_header, sizeof(BMPFileHeader), 1, fp);
  // escreve o cabeçalho do BMP lido no arquivo de escrita
  fwrite(bmp_header, sizeof(BMPHeader), 1, fp);

  //converter pixels para tons de cinza e escrever os dados do BMP

  //pega a area da imagem pra ler cada pixel, ja que é uma matriz
  unsigned int bmp_size = bmp_header->biWidth * bmp_header->biHeight;
  for (unsigned int i = 0; i < bmp_size; i++) {
    // aplicar a fórmula para de tons de cinza cada pixel
    unsigned int gray_value = ((bmp_data[i].r * 0.11) + (bmp_data[i].g * 0.59) + (bmp_data[i].b * 0.3));
  
    // atribuir o mesmo valor de cinza para os atriubtos rgb do bmp data de cada pixel da imagem
  
    bmp_data[i].r = gray_value;
    bmp_data[i].g = gray_value;
    bmp_data[i].b = gray_value;
  }

  // escreve os dados do BMP cinza no novo bmp
  fwrite(bmp_data, sizeof(BMPData), bmp_size, fp);

  size_t elements_written = fwrite(bmp_data, sizeof(BMPData), bmp_size, fp);

  //checa se foi escrito com sucesso os bytes no novo arquivo
  if (elements_written != bmp_size) {
    printf("Erro ao gravar os dados do BMP. O arquivo pode estar corrompido.\n");
    fclose(fp);
    exit(1);
  }
  fclose(fp);
  printf("Arquivo BMP gravado com sucesso: %s\n", filename);
  return 0;
}