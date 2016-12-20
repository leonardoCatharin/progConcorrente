/*Código sequencial*/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>

//Constantes
#define TIME_CLIENTE 100
#define TIME_CAIXA 1000

//Estruturas

typedef struct {
    int fila;
} caixa;

//Variáveis Globais
caixa *caixas;
int numeroDeCaixas;
int numeroDeClientes;

/*
 * Função utilizada para simular o atraso de inserção e remoção dos clientes na fila
 * Parametro x: Tempo em ciclos para o delay
 */
void delay(int x) {
    int j;
    while (j < x) j++;
}

/*
 * Função que substitui o Sleep
 * Tempo dado em segundos
 */
void tempoEspera(int tempo) {
    int retTempo = time(0) + tempo;
    while (time(0) < retTempo);
}

/*
 * Encontra a menor fila entre todos os caixas criados
 * Retorna o índice no array de caixas referente a menor fila
 */
int menorFila() {
    int m = 0;
    for (int i = 1; i < numeroDeCaixas; i++) {
        if (caixas[i].fila < caixas[m].fila) {
            m = i;
        }
    }
    return m;
}

/*
 * Adiciona todos os clientes, sempre na menor fila
 * Finaliza depois que todos os clientes foram adicionados
 */
void adicionaCliente() {
    int i = 0;

    while (i < numeroDeClientes) {
        int toAdd;
        toAdd = menorFila();
        caixas[toAdd].fila++;
        printf("\nO cliente foi adicionado na fila %i", toAdd);
        i++;
        delay(TIME_CLIENTE);
        tempoEspera(1);
    }
}

/*
 * Atende todos os clientes em ordem sequencial: caixa c, caixa c + 1,..., caixa n
 * Finaliza depois que todos os clientes foram adicionados
 */
void atendeClientes() {
    for (int i = 0; i < numeroDeCaixas; i++) {
        while (caixas[i].fila > 0) {
            caixas[i].fila--;
            printf("\nO caixa %i atendeu um cliente da sua própria fila!", i);
            delay(TIME_CAIXA);
            tempoEspera(2);
        }
    }
}

/*
 * MAIN
 * Finaliza depois que todos os clientes foram adicionados e atendidos
 */
int main(int argc, char** argv) {

    printf("\nQuantidade de caixas: ");
    scanf("%i", &numeroDeCaixas);

    printf("\nQuantidade de clientes: ");
    scanf("%i", &numeroDeClientes);

    caixas = malloc(sizeof (caixa) * numeroDeCaixas);

    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.fila = 0;
        caixas[i] = x;
    }

    adicionaCliente();

    atendeClientes();

    return (EXIT_SUCCESS);
}
