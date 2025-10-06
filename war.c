#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void exibir_territorios(const Territorio *mapa, int num);
void simular_ataque(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa);

void exibir_territorios(const Territorio *mapa, int num) {
    printf("\n--- Estado Atual do Mapa ---\n");
    for (int i = 0; i < num; i++) {
        printf("%d. %s [%s] -> Tropas: %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("---------------------------\n");
}

void simular_ataque(Territorio *atacante, Territorio *defensor) {
    if (atacante->tropas <= 1) {
        printf("Ataque cancelado: %s precisa de mais de 1 tropa para atacar!\n", atacante->nome);
        return;
    }
    if (defensor->tropas <= 0) {
        printf("Ataque cancelado: %s ja foi conquistado ou nao tem tropas!\n", defensor->nome);
        return;
    }
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque cancelado: Nao e permitido atacar o proprio territorio.\n");
        return;
    }

    int dado_ataque = rand() % 6 + 1;
    int dado_defesa = rand() % 6 + 1;

    printf("\n>>> BATALHA: %s ataca %s\n", atacante->nome, defensor->nome);
    printf("  [DADOS] Atacante (%s): %d | Defensor (%s): %d\n", 
           atacante->cor, dado_ataque, defensor->cor, dado_defesa);

    if (dado_ataque >= dado_defesa) {
        printf("  Resultado: VITORIA do atacante! %s perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;
        
        if (defensor->tropas <= 0) {
            printf("  *** %s CONQUISTADO! ***\n", defensor->nome);
            
            int tropas_a_transferir = atacante->tropas / 2;
            
            if (tropas_a_transferir >= atacante->tropas) {
                tropas_a_transferir = atacante->tropas - 1; 
            }
            if (tropas_a_transferir == 0) {
                 tropas_a_transferir = 1; 
            }

            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = tropas_a_transferir;
            atacante->tropas -= tropas_a_transferir;
            
            printf("  %s agora pertence ao exercito %s e recebeu %d tropas de reforco.\n", 
                   defensor->nome, atacante->cor, tropas_a_transferir);
        }
    } else {
        printf("  Resultado: DERROTA do atacante! %s perde 1 tropa.\n", atacante->nome);
        atacante->tropas--;
    }
}

void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("Memoria liberada com sucesso.\n");
    }
}

int main() {
    srand(time(NULL));

    int num_territorios; 
    
    printf("--- WAR Estruturado ---\n");
    printf("Digite o numero total de territorios para o mapa: ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios <= 1) {
        printf("Numero de territorios invalido. Encerrando.\n");
        return 1;
    }
    
    Territorio *mapa = (Territorio *)calloc(num_territorios, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF); 

    printf("\n--- Cadastro Inicial de %d Territorios ---\n", num_territorios);
    for (int i = 0; i < num_territorios; i++) {
        printf("Territorio %d de %d:\n", i + 1, num_territorios);
        
        printf("  Nome (max 29 caracteres): ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("  Cor do exercito (Ex: Azul, Vermelho): ");
        if (scanf("%s", mapa[i].cor) != 1) {
            liberarMemoria(mapa);
            return 1;
        }

        printf("  Numero de tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
            liberarMemoria(mapa);
            return 1;
        }

        while (getchar() != '\n'); 
        printf("\n");
    }

    int escolha_atacante, escolha_defensor;
    
    while (1) {
        exibir_territorios(mapa, num_territorios);

        printf("\n*** FASE DE ATAQUE ***\n");
        printf("Digite 0 para SAIR da fase de ataque.\n");
        
        printf("Escolha o TERRITORIO ATACANTE (1 a %d): ", num_territorios);
        if (scanf("%d", &escolha_atacante) != 1) {
            while (getchar() != '\n');
            continue; 
        }
        
        if (escolha_atacante == 0) break; 

        printf("Escolha o TERRITORIO DEFENSOR (1 a %d): ", num_territorios);
        if (scanf("%d", &escolha_defensor) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');

        if (escolha_atacante < 1 || escolha_atacante > num_territorios ||
            escolha_defensor < 1 || escolha_defensor > num_territorios) {
            printf("Selecao invalida. Tente novamente.\n");
            continue;
        }

        simular_ataque(&mapa[escolha_atacante - 1], &mapa[escolha_defensor - 1]);
    }

    printf("\nFase de ataque encerrada. Estado final do mapa:\n");
    exibir_territorios(mapa, num_territorios);
    
    liberarMemoria(mapa);
    printf("Fim do programa.\n");

    return 0;
}
