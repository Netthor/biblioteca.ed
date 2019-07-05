#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    long long cdg;
    char autor[51];
    char titulo[101];
} Livro;

int32_t binaria(Livro livros[], uint32_t qntdLivros, long long cdgBusca, FILE **output)
{
    int32_t i = 0, j = qntdLivros - 1, p = (i + j) / 2, qntdpassos = 0;
    qntdpassos++;
    while (j >= i && livros[p].cdg != cdgBusca)
    {
        if (livros[p].cdg > cdgBusca)
            j = p - 1;
        else
            i = p + 1;
        p = (i + j) / 2;
        qntdpassos++;
    }
    fprintf(*output, "B=%i", qntdpassos);
    return qntdpassos;
}

int32_t interpolada(Livro livros[], int32_t qntdLivros, long long cdgBusca, FILE **output)
{
    int32_t qntdpassos = 0, inicio, meio, final;
    inicio = 0;
    final = qntdLivros - 1;
    while (inicio <= final)
    {
        meio = (inicio + (livros[final].cdg - livros[inicio].cdg) % (final - inicio + 1));
        qntdpassos++;
        if (cdgBusca < livros[meio].cdg)
            final = meio - 1;
        else if (cdgBusca > livros[meio].cdg)
            inicio = meio + 1;
        else
        {
            fprintf(*output, "I=%i", qntdpassos);
            fprintf(*output, ":Author:%s,Title:%s\n", livros[meio].autor, livros[meio].titulo);
            return qntdpassos;
        }
    }
    fprintf(*output, "I=%i", qntdpassos);
    fprintf(*output, ":ISBN NOT FOUND\n");
    return qntdpassos;
}

int main(int argc, char *argv[])
{
    int32_t qntdLivros, qntdBuscas, passosB, passosI, spassosB = 0, spassosI = 0, B = 0, I = 0;
    long long cdg, cdgBusca;
    char autor[50], titulo[100];

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    fscanf(input, "%i", &qntdLivros);
    Livro *livros;
    livros = (Livro *) malloc(qntdLivros * sizeof(Livro));
    for (int32_t i = 0; i < qntdLivros; i++)
    {
        fscanf(input, "%lld %[^&]%*c %[^\n]s", &cdg, autor, titulo);
        livros[i].cdg = cdg;
        strcpy(livros[i].autor, autor);
        strcpy(livros[i].titulo, titulo);
    }
    fscanf(input, "%i", &qntdBuscas);
    while (fscanf(input, "%lld", &cdgBusca) != EOF)
    {
        fprintf(output, "[%lld]", cdgBusca);
        passosB = binaria(livros, qntdLivros, cdgBusca, &output);
        fprintf(output, ",");
        passosI = interpolada(livros, qntdLivros, cdgBusca, &output);
        spassosB += passosB;
        spassosI += passosI;
        if (passosI <= passosB)
            I++;
        else
            B++;
    }

    fprintf(output, "BINARY=%i:%i\n", B, spassosB/qntdBuscas);
    fprintf(output, "INTERPOLATION=%i:%i\n", I, spassosI/qntdBuscas);

    fclose(input);
    fclose(output);

    return 0;
}