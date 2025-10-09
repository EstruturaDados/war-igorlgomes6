#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   
#include <stdbool.h> 

#define NUM_TERRITORIOS 5
#define TAM_NOME_TERRITORIO 30
#define TAM_NOME_COR 10
#define TAM_MISSAO 100
#define NUM_MISSOES 5 

typedef struct {
    char nome[TAM_NOME_TERRITORIO];
    char cor[TAM_NOME_COR];
    int tropas;
} Territorio;

void limpar_buffer();
Territorio* inicializar_mapa();
void exibir_territorios(const Territorio *mapa, int num);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int num); 
void simular_ataque(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa, char *missao);

char *LISTA_MISSOES[NUM_MISSOES] = {
    "Destruir o exercito VERDE completamente.",
    "Conquistar 3 novos territorios de cores diferentes.",
    "Eliminar todas as tropas da cor AZUL.",
    "Conquistar 5 ou mais territorios no total.",
    "Ter pelo menos 8 tropas no territorio Alasca.",
};

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Territorio* inicializar_mapa() {
    Territorio *mapa = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return NULL;
    }

    strcpy(mapa[0].nome, "Asia Central"); strcpy(mapa[0].cor, "Vermelho"); mapa[0].tropas = 3;
    strcpy(mapa[1].nome, "Moscou");       strcpy(mapa[1].cor, "Verde");    mapa[1].tropas = 4;
    strcpy(mapa[2].nome, "Egito");        strcpy(mapa[2].cor, "Azul");     mapa[2].tropas = 2;
    strcpy(mapa[3].nome, "Brasil");       strcpy(mapa[3].cor, "Vermelho"); mapa[3].tropas = 2;
    strcpy(mapa[4].nome, "Alasca");       strcpy(mapa[4].cor, "Verde");    mapa[4].tropas = 3;

    return mapa;
}

void exibir_territorios(const Territorio *mapa, int num) {
    printf("\n--- Estado Atual do Mapa ---\n");
    for (int i = 0; i < num; i++) {
        printf("%d. %s [%s] -> Tropas: %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("---------------------------\n");
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice_sorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indice_sorteado]);
}

void simular_ataque(Territorio *atacante, Territorio *defensor) {
    if (atacante->tropas <= 1 || defensor->tropas <= 0 || strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nCondicoes invalidas para ataque.\n");
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
            
            if (atacante->tropas > 1 && tropas_a_transferir == 0) {
                tropas_a_transferir = 1;
            } else if (atacante->tropas == 1) {
                tropas_a_transferir = 0;
            }

            if (tropas_a_transferir > 0) {
                strcpy(defensor->cor, atacante->cor);
                defensor->tropas = tropas_a_transferir;
                atacante->tropas -= tropas_a_transferir;
                
                printf("  %s agora pertence ao exercito %s e recebeu %d tropas de ocupacao.\n", 
                       defensor->nome, atacante->cor, tropas_a_transferir);
            } else {
                 printf("  Conquista falhou: Atacante nao tem tropas suficientes para ocupar.\n");
            }
        }
    } else {
        printf("  Resultado: DERROTA do atacante! %s perde 1 tropa.\n", atacante->nome);
        atacante->tropas--;
    }
}

int verificarMissao(const char* missao, const Territorio* mapa, int num) {
    printf("\n--- Verificando Missao: %s ---\n", missao);

    if (strstr(missao, "VERDE") != NULL) {
        int tropas_alvo = 0;
        for (int i = 0; i < num; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                tropas_alvo += mapa[i].tropas;
            }
        }
        if (tropas_alvo == 0) return 1;
        printf("Fracasso. O exercito VERDE ainda tem %d tropas.\n", tropas_alvo);
        return 0;
    }
    
    if (strstr(missao, "AZUL") != NULL) {
        int tropas_alvo = 0;
        for (int i = 0; i < num; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                tropas_alvo += mapa[i].tropas;
            }
        }
        if (tropas_alvo == 0) return 1;
        printf("Fracasso. O exercito AZUL ainda tem %d tropas.\n", tropas_alvo);
        return 0;
    }

    if (strstr(missao, "5 ou mais territorios") != NULL) {
        int territorios_vermelhos = 0;
        for (int i = 0; i < num; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                territorios_vermelhos++;
            }
        }
        if (territorios_vermelhos >= 5) return 1;
        printf("Fracasso. Possui apenas %d/5 territorios.\n", territorios_vermelhos);
        return 0;
    }

    if (strstr(missao, "Alasca") != NULL) {
        for (int i = 0; i < num; i++) {
            if (strcmp(mapa[i].nome, "Alasca") == 0 && strcmp(mapa[i].cor, "Vermelho") == 0) {
                if (mapa[i].tropas >= 8) return 1;
                printf("Fracasso. O Alasca tem apenas %d/8 tropas.\n", mapa[i].tropas);
                return 0;
            }
        }
        printf("Fracasso. Alasca nao e seu territorio.\n");
        return 0;
    }

    printf("Fracasso. (Missao nao implementada para verificacao automatica: %s)\n", missao);
    return 0;
}

void liberarMemoria(Territorio *mapa, char *missao) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (missao != NULL) {
        free(missao);
    }
}

int main() {
    srand(time(NULL));

    Territorio *mapa = inicializar_mapa();
    if (mapa == NULL) return 1;
    
    char *missao_do_jogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missao_do_jogador == NULL) {
        liberarMemoria(mapa, NULL);
        return 1;
    }

    atribuirMissao(missao_do_jogador, LISTA_MISSOES, NUM_MISSOES);

    printf("**************************************************\n");
    printf("    WAR - Nivel Mestre: Missões e Modularização   \n");
    printf("**************************************************\n");
    printf("Seu exercito e o Vermelho. Sua Missao e:\n-> %s\n", missao_do_jogador);

    int opcao;
    int atacante_idx, defensor_idx;
    bool jogo_vencido = false;

    do {
        exibir_territorios(mapa, NUM_TERRITORIOS);
        
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha sua acao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = -1; 
        }
        limpar_buffer();

        switch (opcao) {
            case 1: 
                printf("\n--- FASE DE ATAQUE ---\n");
                printf("Escolha o TERRITORIO ATACANTE (1 a %d): ", NUM_TERRITORIOS);
                if (scanf("%d", &atacante_idx) != 1) { limpar_buffer(); break; }

                printf("Escolha o TERRITORIO DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
                if (scanf("%d", &defensor_idx) != 1) { limpar_buffer(); break; }
                
                limpar_buffer();
                
                int i_at = atacante_idx - 1;
                int i_def = defensor_idx - 1;

                if (i_at < 0 || i_at >= NUM_TERRITORIOS || i_def < 0 || i_def >= NUM_TERRITORIOS) {
                    printf("Selecao de territorio invalida.\n");
                    break;
                }
                
                if (strcmp(mapa[i_at].cor, "Vermelho") != 0) {
                    printf("Voce deve atacar de um territorio Vermelho.\n");
                    break;
                }

                simular_ataque(&mapa[i_at], &mapa[i_def]);
                break;

            case 2: 
                jogo_vencido = verificarMissao(missao_do_jogador, mapa, NUM_TERRITORIOS);
                if (jogo_vencido) {
                    printf("\n*** PARABENS! VOCE VENCEU O JOGO CUMPRINDO SUA MISSAO! ***\n");
                    opcao = 0; 
                }
                break;

            case 0: 
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida. Digite 0, 1 ou 2.\n");
                break;
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missao_do_jogador);
    printf("Memoria liberada. Fim do Jogo War.\n");

    return 0;
}
