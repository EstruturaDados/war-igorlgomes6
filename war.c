#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

#define NUM_TERRITORIOS 5

int main() {
    Territorio lista_territorios[NUM_TERRITORIOS];
    int i;

    printf("--- Cadastro Inicial de %d Territorios ---\n", NUM_TERRITORIOS);
    printf("Por favor, insira os dados de cada territorio:\n\n");

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Territorio %d de %d:\n", i + 1, NUM_TERRITORIOS);
        
        printf("  Nome (max 29 caracteres): ");
        fgets(lista_territorios[i].nome, sizeof(lista_territorios[i].nome), stdin);
        lista_territorios[i].nome[strcspn(lista_territorios[i].nome, "\n")] = 0;

        printf("  Cor do exercito (Ex: Azul, Vermelho): ");
        if (scanf("%s", lista_territorios[i].cor) != 1) {
            return 1;
        }

        printf("  Numero de tropas: ");
        if (scanf("%d", &lista_territorios[i].tropas) != 1) {
            return 1;
        }

        printf("\n");
        while (getchar() != '\n'); 
    }

    printf("--------------------------------------------------\n");
    printf("        Relatorio de Territorios Cadastrados      \n");
    printf("--------------------------------------------------\n");
    
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  -> Nome: %s\n", lista_territorios[i].nome);
        printf("  -> Cor: %s\n", lista_territorios[i].cor);
        printf("  -> Tropas: %d\n", lista_territorios[i].tropas);
        printf("---\n");
    }
    
    printf("Fim do Cadastro e Exibicao.\n");

    return 0;
}
