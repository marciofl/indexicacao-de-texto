/*
Este codigo em C le um arquivo de texto, cria indices usando listas encadeadas e 
arvores de busca binaria para armazenar palavras e linhas correspondentes. Apos a 
indexacao, o programa realiza buscas, permitindo ao usuario escolher entre uma lista 
ou uma arvore. Ele mede o tempo de busca e exibe resultados, incluindo ocorrencias e 
linhas associadas. A estrutura modular do codigo facilita a expansao para diferentes 
tipos de indices. O usuario interage atraves de comandos como "busca" e "fim", 
tornando a aplicacao interativa e util para analise de textos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct no_lista_palavras {
    char palavra[50];
    int linha;
    struct no_lista_palavras *proximo;
} no_lista_palavras;

typedef struct no_arvore_palavras {
    char palavra[50];
    int *linhas;
    int ocorrencias;
    struct no_arvore_palavras *esquerda;
    struct no_arvore_palavras *direita;
} no_arvore_palavras;

/*
---------------------------------------------------------------------------------
void normalizaPalavra() 
---------------------------------------------------------------------------------
Recebe uma string de entrada (entrada)
e cria uma versao normalizada dessa string,
armazenando-a em outra string (saida).
remove caracteres nao-alfabeticos
O resultado normalizado eh armazenado na string de saída,
que eh terminada com o caractere nulo '\0' para formar uma string de C valida.
*/
void normalizaPalavra(const char *entrada, char *saida) {
    int i, j = 0;
    for (i = 0; entrada[i] != '\0'; i++) {
        if (isalpha(entrada[i])) {
            saida[j++] = tolower(entrada[i]);
        }
    }
    saida[j] = '\0';
}


/* 
--------------------------------------------------------------------------------------
no_lista_palavras *criaNoListaPalavras()
--------------------------------------------------------------------------------------
Aloca dinamicamente um novo no para uma lista de palavras.
Ela copia a palavra e armazena o numero da linha no novo no,
configurando o proximo no como nulo. O no recem-criado eh entao retornado.
Essa funcao eh comumente usada para criar e adicionar
nos a uma lista encadeada de palavras.
*/
no_lista_palavras *criaNoListaPalavras(const char *palavra, int linha) {                 
    no_lista_palavras *novo_no = (no_lista_palavras *)malloc(sizeof(no_lista_palavras)); 
    strcpy(novo_no->palavra, palavra);                                                  
    novo_no->linha = linha;                                                              
    novo_no->proximo = NULL;                                                            
    return novo_no;
}

/*
---------------------------------------------------------------------------------------
no_lista_palavras *insereNoLista()
---------------------------------------------------------------------------------------
Adiciona um novo no a uma lista encadeada ordenada de palavras.
Ela utiliza a funcao criaNoListaPalavras para criar o novo no.
O no eh inserido de maneira ordenada com base no numero da linha.
Se a lista esta vazia ou se o novo no deve ser inserido no inicio,
ele se torna a nova cabeca da lista.
*/
no_lista_palavras *insereNoLista(no_lista_palavras *cabeca, const char *palavra, int linha) {
    no_lista_palavras *novo_no = criaNoListaPalavras(palavra, linha);  

    if (cabeca == NULL || linha < cabeca->linha) {                       
        novo_no->proximo = cabeca;
        return novo_no;
    }                                                                      

    no_lista_palavras *atual = cabeca;
    while (atual->proximo != NULL && linha > atual->proximo->linha) {
        atual = atual->proximo;
    }

    novo_no->proximo = atual->proximo;
    atual->proximo = novo_no;

    return cabeca; 
}

/*
---------------------------------------------------------------------------------------
void imprimeLista()
---------------------------------------------------------------------------------------
Imprime os elementos de uma lista encadeada de palavras.
A funcao percorre a lista a partir da cabeca, exibindo cada no.
Para cada no, imprime a palavra e o numero da linha formatados.
*/

void imprimeLista(no_lista_palavras *cabeca) {
    no_lista_palavras *atual = cabeca;
    while (atual != NULL) {
        printf("Palavra: %s - Linha: %05d\n", atual->palavra, atual->linha);
        atual = atual->proximo;
    }
}

/*
---------------------------------------------------------------------------------------
void liberaLista()
---------------------------------------------------------------------------------------
Libera a memoria associada a todos os nos de uma lista encadeada de palavras.
A funcao percorre a lista a partir da cabeca, liberando cada no.
Utiliza um ponteiro temporario para evitar perda de referencia e utiliza a
funcao free para desalocar a memoria de cada no.
*/

void liberaLista(no_lista_palavras *cabeca) {
    no_lista_palavras *atual = cabeca;
    while (atual != NULL) {
        no_lista_palavras *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

/*
---------------------------------------------------------------------------------------
no_lista_palavras *procuraNaLista()
---------------------------------------------------------------------------------------
Busca por ocorrencias de uma palavra em uma lista encadeada de palavras.
A funcao itera sobre a lista, normalizando cada palavra e comparando-a com
a palavra desejada. Se encontra ocorrencias, insere os resultados em uma
nova lista ordenada por linha. Imprime as ocorrencias e retorna a nova lista.
*/

no_lista_palavras *procuraNaLista(no_lista_palavras *cabeca, const char *palavra) {
    no_lista_palavras *respostaCabeca = NULL;
    int ocorrencias = 0;

    no_lista_palavras *atual = cabeca;
    while (atual != NULL) {
        char stringNormalizada[50];
        char palavraNormalizada[50];
        strcpy(palavraNormalizada, palavra);

        normalizaPalavra(atual->palavra, stringNormalizada);
        normalizaPalavra(palavraNormalizada, palavraNormalizada);

        if (strcmp(palavraNormalizada, stringNormalizada) == 0) {
            respostaCabeca = insereNoLista(respostaCabeca, stringNormalizada, atual->linha);
            ocorrencias++;
        }
        atual = atual->proximo;
    }

    if (ocorrencias > 0) {
        printf("Existem %d ocorrencias da palavra '%s' nas seguintes linhas:\n", ocorrencias, palavra);
    } else {
        printf("Palavra '%s' nao encontrada.\n", palavra);
    }

    return respostaCabeca;
}



/*
---------------------------------------------------------------------------------------
void imprimeResultadoLista()
---------------------------------------------------------------------------------------
Imprime os resultados da busca em uma lista encadeada de palavras.
A funcao percorre a lista de resultados, abrindo o arquivo especificado e
imprimindo as linhas correspondentes aos numeros armazenados nos nos da lista.
Imprime cada linha encontrada no formato "numero de linha: conteudo da linha".
*/
void imprimeResultadoLista(no_lista_palavras *respostaCabeca, const char *nomeDoArquivo) {
    no_lista_palavras *atual = respostaCabeca;

    while (atual != NULL) {
        printf("%05d: ", atual->linha);

        FILE *arquivo = fopen(nomeDoArquivo, "r");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nomeDoArquivo);
            return;
        }

        int linhaAtual = 1;
        char armazenamento[1024];

        while (fgets(armazenamento, sizeof(armazenamento), arquivo) != NULL) {
            if (linhaAtual == atual->linha) {
                armazenamento[strcspn(armazenamento, "\n")] = '\0';
                printf("%s", armazenamento);
                break;
            }
            linhaAtual++;
        }

        fclose(arquivo);

        printf("\n");

        atual = atual->proximo;
    }
}


/*
---------------------------------------------------------------------------------------
void imprimeResultadoArvore()
---------------------------------------------------------------------------------------
Imprime os resultados da busca em uma arvore de palavras.
A funcao verifica se ha resultados na arvore. Se houver, imprime o numero
de ocorrencias e as linhas correspondentes do arquivo, formatadas como
"numero de linha: conteudo da linha". Se nao houver resultados, imprime
uma mensagem indicando que a palavra nao foi encontrada.
*/
void imprimeResultadoArvore(no_arvore_palavras *respostaNo, const char *nomeDoArquivo) {
    if (respostaNo != NULL) {
        printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n", respostaNo->ocorrencias, respostaNo->palavra);
        for (int i = 0; i < respostaNo->ocorrencias; i++) {
            printf("%05d: ", respostaNo->linhas[i]);

            FILE *arquivo = fopen(nomeDoArquivo, "r");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo %s\n", nomeDoArquivo);
                return;
            }

            int linhaAtual = 1;
            char armazenamento[1024];

            while (fgets(armazenamento, sizeof(armazenamento), arquivo) != NULL) {
                if (linhaAtual == respostaNo->linhas[i]) {
                    armazenamento[strcspn(armazenamento, "\n")] = '\0';
                    printf("%s", armazenamento);
                    break;
                }
                linhaAtual++;
            }

            fclose(arquivo);

            printf("\n");
        }
    } else {
        printf("Palavra nao encontrada.\n");
    }
}


typedef struct no_arvore_palavras no_arvore_palavras;

/*
---------------------------------------------------------------------------------------
no_arvore_palavras *criaNoArvorePalavras()
---------------------------------------------------------------------------------------
Aloca dinamicamente e inicializa um novo no para uma arvore de palavras.
Copia a palavra para o novo no, configura o numero de ocorrencias como 1,
aloca espaco para armazenar a linha e a configura como a primeira linha do no.
Configura os ponteiros esquerdo e direito como nulos e retorna o novo no.
*/
no_arvore_palavras *criaNoArvorePalavras(const char *palavra, int linha) {
    no_arvore_palavras *novo_no = (no_arvore_palavras *)malloc(sizeof(no_arvore_palavras));
    strcpy(novo_no->palavra, palavra);
    novo_no->ocorrencias = 1;
    novo_no->linhas = (int *)malloc(sizeof(int));
    novo_no->linhas[0] = linha;
    novo_no->esquerda = novo_no->direita = NULL;
    return novo_no;
}

/*
---------------------------------------------------------------------------------------
no_arvore_palavras *criaNoArvore()
---------------------------------------------------------------------------------------
Adiciona um novo no a uma arvore de palavras.
Se a raiz eh nula, cria um novo no usando a funcao criaNoArvorePalavras.
Se a palavra ja existe na arvore, incrementa as ocorrencias e atualiza as linhas.
Se a palavra eh menor que a palavra na raiz, insere a esquerda; se maior, a direita.
Retorna a raiz atualizada da arvore.
*/
no_arvore_palavras *criaNoArvore(no_arvore_palavras *raiz, const char *palavra, int linha) {
    if (raiz == NULL) {
        return criaNoArvorePalavras(palavra, linha);
    }

    int resultadoComparacao = strcmp(palavra, raiz->palavra);

    if (resultadoComparacao == 0) {
        raiz->ocorrencias++;
        raiz->linhas = (int *)realloc(raiz->linhas, raiz->ocorrencias * sizeof(int));
        raiz->linhas[raiz->ocorrencias - 1] = linha;
    } else if (resultadoComparacao < 0) {
        raiz->esquerda = criaNoArvore(raiz->esquerda, palavra, linha);
    } else {
        raiz->direita = criaNoArvore(raiz->direita, palavra, linha);
    }

    return raiz;
}

/*
---------------------------------------------------------------------------------------
no_arvore_palavras *buscaArvore()
---------------------------------------------------------------------------------------
Realiza uma busca em uma arvore de palavras.
Se a raiz eh nula ou a palavra na raiz eh a palavra desejada, retorna a raiz.
Se a palavra desejada eh menor que a palavra na raiz, realiza a busca a esquerda.
Se maior, realiza a busca a direita. Retorna o no encontrado ou nulo se nao existe.
---------------------------------------------------------------------------------------
*/
no_arvore_palavras *buscaArvore(no_arvore_palavras *raiz, const char *palavra) {
    if (raiz == NULL || strcmp(palavra, raiz->palavra) == 0) {
        return raiz;
    }

    if (strcmp(palavra, raiz->palavra) < 0) {
        return buscaArvore(raiz->esquerda, palavra);
    } else {
        return buscaArvore(raiz->direita, palavra);
    }
}
/*
---------------------------------------------------------------------------------------
void imprimeArvore()
---------------------------------------------------------------------------------------
Imprime os elementos de uma arvore de palavras em ordem.
A funcao percorre a arvore de forma recursiva, imprimindo as palavras,
o numero de ocorrencias e as linhas correspondentes para cada no.
Os resultados sao apresentados em ordem crescente de palavras.
---------------------------------------------------------------------------------------
*/
void imprimeArvore(no_arvore_palavras *raiz) {
    if (raiz != NULL) {
        imprimeArvore(raiz->esquerda);
        printf("Palavra: %s - Ocorrencias: %d - Linhas:", raiz->palavra, raiz->ocorrencias);
        for (int i = 0; i < raiz->ocorrencias; i++) {
            printf(" %05d", raiz->linhas[i]);
        }
        imprimeArvore(raiz->direita);
    }
}

/*
---------------------------------------------------------------------------------------
void liberaArvore()
---------------------------------------------------------------------------------------
Libera a memoria associada a todos os nos de uma arvore de palavras.
A funcao percorre a arvore de forma recursiva, liberando a memoria
das linhas e dos nos. Utiliza a funcao free para desalocar a memoria.
---------------------------------------------------------------------------------------
*/
void liberaArvore(no_arvore_palavras *raiz) {
    if (raiz != NULL) {
        liberaArvore(raiz->esquerda);
        liberaArvore(raiz->direita);
        free(raiz->linhas);
        free(raiz);
    }
}

int main(int argc, char *argv[]) {
    /*
    Verifica se o numero de argumentos na linha de comando eh igual a 3. 
    Se nao for, imprime uma mensagem de uso e encerra o programa com codigo de erro 1.
    */
    if (argc != 3) {
        printf("Uso: %s <nome_arquivo> <tipo_indice>\n", argv[0]);
        return 1;
    }

    const char *nomeDoArquivo = argv[1];
    FILE *arquivo = fopen(nomeDoArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeDoArquivo);
        return 1;
    }

    printf("Tipo de indice: '%s'\n", argv[2]);

    char armazenamento[1024];
    int linha = 1;
    clock_t inicio, fim;
    double tempo_de_processamento;

    no_lista_palavras *cabeca_da_lista = NULL;
    no_arvore_palavras *raiz_da_arvore = NULL;

    inicio = clock();

    /*
    Faz a leitura do arquivo texto linha por linha usando a funcao fgets. 
    Para cada linha lida, a funcao strtok eh usada para dividir a linha em 
    tokens (palavras) com base nos delimitadores " \t\n". 
    Em seguida, cada token eh normalizado utilizando a funcao normalizaPalavra, 
    e as palavras normalizadas sao inseridas tanto em uma lista encadeada (cabeca_da_lista) 
    quanto em uma arvore binaria de busca (raiz_da_arvore).
    */

    while (fgets(armazenamento, sizeof(armazenamento), arquivo) != NULL) {
        char *token = strtok(armazenamento, " \t\n");

        while (token != NULL) {
            char tokenNormalizado[50];
            normalizaPalavra(token, tokenNormalizado);

            cabeca_da_lista = insereNoLista(cabeca_da_lista, tokenNormalizado, linha);
            raiz_da_arvore = criaNoArvore(raiz_da_arvore, tokenNormalizado, linha);

            token = strtok(NULL, " \t\n");
        }

        linha++;
    }

    fim = clock();

    printf("Arquivo texto: '%s'\n", nomeDoArquivo);
    printf("Numero de linhas no arquivo: %d\n", linha - 1);
    tempo_de_processamento = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
    printf("Tempo para carregar o arquivo e construir o indice: %.2f ms\n", tempo_de_processamento);

    char opcao[50];
    while (1) {
        printf("> ");
        scanf("%s", opcao);
        //comparacao se busca foi escrito corretamente
        if (strcmp(opcao, "busca") == 0) {
            char palavra_a_procurar[50];
            scanf(" %[^\n]", palavra_a_procurar);
            //Caso lista for escrita corretamente, procura na lista a palavra correspondente
            if (strcmp(argv[2], "lista") == 0) {
                char palavra_a_procurar_normalizada[50];
                normalizaPalavra(palavra_a_procurar, palavra_a_procurar_normalizada);
                inicio = clock();
                no_lista_palavras *respostaLista = procuraNaLista(cabeca_da_lista, palavra_a_procurar);
                imprimeResultadoLista(respostaLista, nomeDoArquivo);
                fim = clock();
                tempo_de_processamento = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
                printf("Tempo de busca na lista: %.2f ms\n", tempo_de_processamento);
                liberaLista(respostaLista);
            } else if (strcmp(argv[2], "arvore") == 0) {
                char palavra_a_procurar_normalizada[50];
                normalizaPalavra(palavra_a_procurar, palavra_a_procurar_normalizada);
                inicio = clock();
                no_arvore_palavras *respostaArvore = buscaArvore(raiz_da_arvore, palavra_a_procurar_normalizada);
                imprimeResultadoArvore(respostaArvore, nomeDoArquivo);
                fim = clock();
                tempo_de_processamento = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
                printf("Tempo de busca na arvore: %.2f ms\n", tempo_de_processamento);
            } else {
                printf("Tipo de indice invalido.\n");
            }
        } else if (strcmp(opcao, "fim") == 0) {
            break;
        } else {
            printf("Opcao invalida!\n");
        }
    }

    liberaLista(cabeca_da_lista);
    liberaArvore(raiz_da_arvore);

    fclose(arquivo);

    return 0;
}
