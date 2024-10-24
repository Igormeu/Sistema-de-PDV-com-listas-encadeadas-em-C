#include "Lista_encadeada.h"

int main(){
    No* produtos;
    int opcao = 0;
    char sair [2];
    produtos = inicializarLista();
    system("cls");
    exibirBemVindo();
    do{
        exibirMenu();
        char codigo[8];
        int quant;
        printf("\n");
        scanf("%d",&opcao);
        switch (opcao)
        {
        case 1:
            inserirElementos(&produtos);
            break;

        case 2:
            exibirElemento(produtos);
            break;

        case 3:
            printf("\nDigite o codigo do item a ser excluido: ");
            scanf("%s",codigo);
            excluirElemento(&produtos,codigo);
            break;
        
        case 4:
            printf("\nDigite o codigo do produto e a quantidade vendida: ");
            scanf("%s %d",codigo,&quant);
            saidaItens(produtos,codigo,quant);
            break;
        
        case 5:
            printf("\nDigite o codigo do item procurado: ");
            scanf("%s",codigo);
            buscarItens(produtos,codigo);
            break;

        case 6:
            printf("\nDigite o codigo do produto a ser atualizado: ");
            scanf("%s",codigo);
            atualizarItem(&produtos,codigo);
            break;

        case 7:
            printf("\nAo sair todos os dados serao apagados\nTem certeza que deseja sair ?(s/n)");
            scanf("%s",sair);
            if (strcmp(sair,"S") == 0  || strcmp(sair,"s") == 0){
                printf("Bye Bye");
                free(produtos);
            }
            else{
                opcao = 10;
            }
            break;

        default:
            printf("\nOpcao invalida");
            break;
        }
    }
    while (opcao != 7);
}