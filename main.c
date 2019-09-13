#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// **********************************************************************
// PUCRS/EP
// Leitura de arquivo PGM
//
// Marcelo Cohen
//
// marcelo.cohen@pucrs.br
// **********************************************************************

typedef struct {
    int largura, altura;
    unsigned char* imagemOrig;    // vetor com o conteúdo do arquivo
    int tamOrig;                  // quantidade de bytes lidos do arquivo
    unsigned char* imagemCompact; // vetor com a imagem compactada (a ser preenchido)
    int tamCompact;               // quantidade de bytes na imagem compactada (a ser preenchido)
} Imagem;

void carregaPGM(const char *nome, Imagem* img);
void dump(Imagem* img);

// **********************************************************************
void carregaPGM(const char *nome, Imagem* img)
{
    FILE *arq;
    arq = fopen (nome,"rt");
    if (arq == NULL)
    {
        printf("Problemas na abertura do arquivo %s...\n", nome);
        exit(1);
    }
    printf("Arquivo %s aberto!\n", nome);

    fseek(arq, 0L, SEEK_END);
    int sz = ftell(arq);
    rewind(arq);

    printf("Tamanho em bytes: %d\n", sz);

    unsigned char* ptr = malloc(sz);
    fread(ptr, sz, 1, arq);
    fclose(arq);

    // Reservando o dobro de espaco da imagem original
    // para a compactada (caso ela nao compacte bem)
    img->imagemCompact = malloc(sz*2);

    img->imagemOrig = ptr;
	img->tamOrig = sz;
}

// Teste: exibe primeiros 256 bytes do vetor original
// (primeiros bytes sao o header em formato texto)
void dump(Imagem* img)
{
	for(int i=0; i<256; i+=16)
	{
	    printf("%04d: ",i);
	    for(int j=0; j<16; j++)
            printf("%02X ", img->imagemOrig[i+j]);
        for(int j=0; j<16; j++)
            printf("%c ", isprint(img->imagemOrig[i+j]) ? img->imagemOrig[i+j] : '.');
        printf("\n");
	}
	printf("\n");
}

// **********************************************************************
int main()
{
    char nomeArquivo[] = "logofacin.pgm";
    int largura, altura;

	Imagem imagem;
    carregaPGM(nomeArquivo, &imagem);

	dump(&imagem);

	// Libera memoria de ambas as imagens
	free(imagem.imagemOrig);
    free(imagem.imagemCompact);
    return 0;
}
