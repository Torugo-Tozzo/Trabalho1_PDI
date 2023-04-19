/*=============================================================
 * UNIFAL = Universidade Federal de Alfenas .
 * BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho   : Imagem ASCII
 * Disciplina : Processamento de Imagens
 * Professor  : Luiz Eduardo da Silva
 * Aluno . . . . . : Victor Hugo Tozzo Filho
 * Data . . . . . . : 18/04/2023
 *=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagelib.h"

void msg(char *s)
{
    printf("\nNegative image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pgm] image-result.pgm n n '<caracteres>'\n\n", s);
    printf("    image-name[.pgm] is image file file \n");
    exit(1);
}

void resize_pgm(image In, image Out, int nl_in, int nc_in, int nl_out, int nc_out)
{
    double ratio_nl = (double)nl_in / nl_out;
    double ratio_nc = (double)nc_in / nc_out;

    for (int i = 0; i < nl_out; i++)
    {
        for (int j = 0; j < nc_out; j++)
        {
            int orig_i = (int)(i * ratio_nl);
            int orig_j = (int)(j * ratio_nc);

            Out[i * nc_out + j] = In[orig_i * nc_in + orig_j];
        }
    }
}

void convert_to_gray(image In, int nl, int nc, int num_tons_cinza)
{
    double step = 255.0 / (num_tons_cinza - 1);

    for (int i = 0; i < nl; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            int index = i * nc + j;
            int new_value = (int)((double)In[index] / step + 0.5) * step;
            In[index] = new_value;
        }
    }
}

void generate_ascii(image In, int nr, int nc, int ml, char *ascii_string, char *output_file)
{
    FILE *fp = fopen(output_file, "w");
    if (fp == NULL)
    {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
    }

    int len = strlen(ascii_string);
    int gray_range = 255 / len;

    for (int i = 0; i < nr; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            int pixel = In[i * nc + j];
            int ascii_index = pixel / gray_range;
            char ascii_char = ascii_string[ascii_index];
            fputc(ascii_char, fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);
}

int main(int argc, char *argv[])
{
    int nc, nr, ml;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 6)
        msg(argv[0]);
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, 2); // Assuming input image is PGM (grayscale)
    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, 2);
    //-- resize image
    int nl_out = atoi(argv[3]); // número de linhas desejado
    int nc_out = atoi(argv[4]); // número de colunas desejado
    Out = img_alloc(nl_out, nc_out);
    resize_pgm(In, Out, nr, nc, nl_out, nc_out);
    //-- convert to grayscale
    int num_tons_cinza = strlen(argv[5]); // número de tons de cinza desejado
    convert_to_gray(Out, nl_out, nc_out, num_tons_cinza);
    generate_ascii(Out, nl_out, nc_out, ml, argv[5], "output_ascii.txt");
    //-- save image
    img_put(Out, nameOut, nl_out, nc_out, ml, 2);
    //-- show image
    //sprintf(cmd, "%s %s &", VIEW, nameOut);
    sprintf(cmd, "cat output_ascii.txt");
    system(cmd);
    //free mem
    img_free(In);
    img_free(Out);
    return 0;
}
