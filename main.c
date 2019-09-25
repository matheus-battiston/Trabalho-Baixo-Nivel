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

void valida(Imagem* img)
{
    int tamOrig  = img->largura * img->altura;
    int tamTotal = 0;
    int pos = 0;
    unsigned char* ptr = img->imagemCompact;
    while(pos < img->tamCompact) {
        int qtd = *ptr;
        unsigned char val = *(ptr+1);
        printf("Out: %d x %02X\n", qtd, val);
        tamTotal += qtd;
        ptr += 2;
        pos += 2;
    }
    printf("Tamanho total/original: %d / %d\n", tamTotal, tamOrig);
}
void compactar(Imagem* img){
    
    unsigned char* comparar;
    int i, cont;
    cont = 0;
    for (i=0; i < img -> tamOrig; i++){
        if (img -> imagemOrig[i] == 0x0A){
            cont++;
            if (cont == 3){
                i++;
                break;
            }
        } 
    }

    comparar = img -> imagemOrig[i];
    int cont2 = 0;
    cont = 0;
    for (i;i < img -> tamOrig; i++){
        if (img -> imagemOrig[i] == comparar){
            if(cont<=254){
            cont++;
            }
            else{
                img -> imagemCompact[cont2] = cont;
                cont2++;
                cont=0;
            }
            }

            
        else {
            comparar=img->imagemOrig[i];
            img -> imagemCompact[cont2] = cont;
            cont2++;
            cont = 0;
        }
        
    }
    
    for (i = 0; i < cont2+1; i++){
        printf("%d\n", img -> imagemCompact[i]);
    }


    

}


// **********************************************************************
int main()
{
    char nomeArquivo[] = "x.pgm";
    int largura, altura;

	Imagem imagem;
    carregaPGM(nomeArquivo, &imagem);

	dump(&imagem);
    compactar(&imagem);

	// Libera memoria de ambas as imagens
	free(imagem.imagemOrig);
    free(imagem.imagemCompact);
    return 0;
}
