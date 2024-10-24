#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dados{
    char cod[8];
    char nome[50];
    int quant;
    float preco;
} Dados;

typedef struct no{
    Dados info;
    struct no *proximo;
} No;


No* inicializarLista(){
    return NULL;
}

void registrarHistorico(No* value,No* lista) {
    FILE *file = fopen("registroSaida.txt", "a+");

    if (file == NULL) {
        printf("Não foi possível abrir o arquivo\n");
        return;
    }

    fprintf(file,"Foram dada a saída de %d unidade(s) de %s. (Codigo: %s | Quant. em estoque: %d)\n",value->info.quant,value->info.nome,value->info.cod,lista->info.quant);
    
    fclose(file); 
}

void registrarAlteracao(No* anterior,No* lista,int opcao) {
    FILE *file = fopen("logModificacoes.txt", "a+");

    if (file == NULL) {
        printf("Não foi possível abrir o arquivo\n");
        return;
    }
        switch (opcao)
        {
        case 1:
            fprintf(file,"O nome de exibicao do código %s foi alterado | %s -> %s",lista->info.cod,anterior->info.nome,lista->info.nome);
            break;
        case 2:
            fprintf(file,"A quantidade do código %s foi alterado | %d -> %d",lista->info.cod,anterior->info.quant,lista->info.quant);
            break;
        case 3:
            fprintf(file,"O preco do código %s foi alterado | %.2f -> %.2f",lista->info.cod,anterior->info.preco,lista->info.preco);
            break;
        default:
            break;
        }
    
    fclose(file); 
}

int inserirElementos (No **lista){
    No* newno = malloc(sizeof(No));
    No* aux;
    char codigo[8];
    char nome[50];

    if(newno == NULL){
        printf("Nao foi possível alocar memória");
        return 0;
    }
    else{
        printf("\nDigite o codigo do produto: ");
        scanf("%7s",codigo);

        //evitar que tenha dois produtos com o mesmo codigo
        for (aux = *lista;aux != NULL;aux = aux->proximo){
            if(strcmp(codigo,aux->info.cod) == 0){
                printf("\nO codigo ja foi cadastrado (%s - %s)\n",aux->info.cod,aux->info.nome);
                free(newno);
                return 0;
            }
        }

        strcpy(newno->info.cod,codigo);

        printf("Digite o nome do produto: ");
        scanf("%49s",nome);
        strcpy(newno->info.nome,nome);

        printf("Digite a quantidade do produto em estoque: ");
        scanf("%d",&newno->info.quant);

        printf("Digite o preco do produto: ");
        scanf("%f",&newno->info.preco);

        //Inserir no inicio
        newno->proximo = *lista;
        *lista = newno;
        return 1;
    }
}

int exibirElemento (No *lista) {
    printf("\n\n=====Lista de produtos armazenados=====\n\n");
    No* p = lista;
    while (p != NULL) {
        printf("Codigo: %s\n", p->info.cod);
        printf("Nome: %s\n", p->info.nome);
        printf("Quantidade: %d\n", p->info.quant);
        printf("Preco: R$ %.2f\n\n", p->info.preco);
        p = p->proximo;
    }
    return 0;
}

int excluirElemento (No **lista, char codigo[8]){
    No *aux = *lista;
    No *anterior = NULL;

    while (aux != NULL && strcmp(aux->info.cod,codigo) != 0) {
        anterior = aux;
        aux = aux->proximo;
        // printf("%s | %s | %d | %f",aux->info.cod,aux->info.nome,aux->info.quant,aux->info.preco);
    }

    if (aux == NULL){
        printf("\nO item nao foi encontrado\n");
        return -1;
    }
    else{
        if(aux->info.quant > 0){
        printf("\nNao sera possivel excluir o item (%s) pois ha itens em estoque (%d)",aux->info.cod,aux->info.quant);
        return -1;
        }
        else{
            if (anterior == NULL) {
                *lista = aux->proximo;
            } 
            else {
                anterior->proximo = aux->proximo;
            }

            free(aux);
            printf("\nO item foi removido\n");
            return 0;
        }
    } 
}

No* saidaItens (No* lista,char codigo[8],int quantidade){
    No *regSaida = malloc(sizeof(No));
    No *aux = lista;

    while (aux != NULL && strcmp(aux->info.cod,codigo) != 0) {
        aux = aux->proximo;
        // printf("%s | %s | %d | %f",aux->info.cod,aux->info.nome,aux->info.quant,aux->info.preco);
    }

    if (aux == NULL){
        printf("\nO item nao foi encontrado, revise o codigo e tente novamente\n");
        return NULL;
    }
    else{
        int quantEstoque = aux->info.quant;
        quantEstoque -= quantidade; // nova quantidade

        if(quantEstoque < 0){
            char resp [2];
            
            printf("\nA quantidade que deseja retirar e maior que a presente em estoque");
            printf("\nDeseja modificar a quantidade (s/n)? ");
            scanf("%s",resp);

            if (strcmp(resp,"s") == 0 || strcmp(resp,"S") == 0){
                while (quantEstoque < 0){
                    printf("\nImpossivel realizar a operacao anterior, digite uma nova quantidade: ");
                    scanf("%d",&quantidade);
                    quantEstoque = aux->info.quant - quantidade;
                }
            }

            else{
                printf("\nA saida nao foi registrada");
                return NULL;
            }
        }
        
        strcpy(regSaida->info.cod,aux->info.cod);
        strcpy(regSaida->info.nome,aux->info.nome);
        regSaida->info.quant = quantidade;
        regSaida->info.preco = quantidade * aux->info.preco;

        aux->info.quant = quantEstoque;

        registrarHistorico(regSaida,lista);

        printf("Processamento da venda realizado com sucesso !");

        return regSaida;
    }
}

No* buscarItens (No* lista, char codigo[8]){
    No *aux = lista;

    while (aux != NULL && strcmp(aux->info.cod,codigo) != 0) {
        aux = aux->proximo;
    }

    if (aux == NULL){
        printf("\nO item nao foi encontrado, revise o codigo e tente novamente\n");
        return NULL;
    }

    else{
        printf("\n=============Resultado da busca=============\n");
        printf ("\nCodigo: %s\nNome: %s\nQuantidade: %d\nPreco: R$ %.2f",aux->info.cod,aux->info.nome,aux->info.quant,aux->info.preco);
        return aux;
    }  
}

int atualizarItem (No **lista,char codigo[8]){
    No *aux = *lista;

    while (aux != NULL && strcmp(aux->info.cod,codigo) != 0) {
        aux = aux->proximo;
    }

    if (aux == NULL){
        printf("\nO item nao foi encontrado\n");
        return -1;
    }
    else{
        No* regAlter = malloc(sizeof(No));
        int opcao;
        char nome[50];
        int quant;
        float preco;

        printf("\n=========Layout de alteracao=========\n");
        printf("\n1 - Nome");
        printf("\n2 - Quantidade");
        printf("\n3 - Preco");

        printf("\n\nDigite a opcao que deseja alterar: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Digite o novo nome do item: ");
            scanf("%s", nome);
            strcpy (regAlter->info.nome,aux->info.nome);
            strcpy (aux->info.nome,nome);
            
            registrarAlteracao(regAlter,*lista,opcao);
            break;
        
        case 2:
            printf("Digite a nova quantidade do item: ");
            scanf("%d", &quant);
            regAlter->info.quant = aux->info.quant;
            aux->info.quant = quant;
            registrarAlteracao(regAlter,*lista,opcao);
            break;

        case 3:
            printf("Digite o novo preco do item: ");
            scanf("%f", &preco);
            regAlter->info.preco = aux->info.preco;
            aux->info.preco = preco;
            registrarAlteracao(regAlter,*lista,opcao);
            break;

        default:
            return -1;
            break;
        }

        free(regAlter);
        printf("Alteracao realizada com sucesso");
        return 1;
    }
}

void exibirBemVindo() {
    printf("******************************************************************************\n");
    printf("*                                                                            *\n");
    printf("*  BBBBBB   EEEEEEEE  M     M    V     V  IIIIIII  N     N  DDDDDD   OOOOOO  *\n");
    printf("*  B     B  E         MM   MM    V     V     I     NN    N  D     D  O    O  *\n");
    printf("*  BBBBBB   EEEEE     M M M M    V     V     I     N N   N  D     D  O    O  *\n");
    printf("*  B     B  E         M  M  M     V   V      I     N  N  N  D     D  O    O  *\n");
    printf("*  BBBBBB   EEEEEEEE  M     M      VVV    IIIIIII  N   N N  DDDDDD   OOOOOO  *\n");
    printf("*                                                                            *\n");
    printf("******************************************************************************\n");
    printf("\n\nFeito por: Igor Stenio Pereira Santos\nMatricula: 202403930111\n\n");
}

void exibirMenu(){
    printf("\n============MENU============\n");
    printf("1 - Inserir Produtos\n");
    printf("2 - Exibir lista de produtos cadastrados\n");
    printf("3 - Excluir produto\n");
    printf("4 - Resgistrar venda\n");
    printf("5 - Buscar Item\n");
    printf("6 - Atualizar item\n");

    printf("7 - SAIR");
}


