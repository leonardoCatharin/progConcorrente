/* 
 * File:   main.c
 * Author: igorsantana and leonardocatharin
 *
 * Created on October 20, 2016, 7:08 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

enum boolean {
    true = 1, false = 0
};

typedef enum boolean bool;

typedef struct {
    bool bloqueado;
    int fila;
    pthread_t thread;
    sem_t semaforo;
} caixa;


caixa *caixas;
int numeroDeCaixas = 0;
int numeroDeClientes = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int menorFila() {
    int m = 0;
    for (int i = 1; i < numeroDeCaixas; i++) {
        if (caixas[i].fila < caixas[m].fila) {
            m = i;
        }
    }
    return m;
}

void* novoCliente_thread(void *arg) {
    int added = 0;
    while (true) {
        pthread_mutex_lock(&mutex);

        int toAdd = menorFila();
        //sem_wait(&caixas[toAdd].semaforo);
        caixas[toAdd].fila++;

        pthread_mutex_unlock(&mutex);
        sem_post(&caixas[toAdd].semaforo);

        printf("Cliente adicionado na fila %d\n", toAdd);
        added++;
        if (added == numeroDeClientes) break;
    }

    pthread_exit(NULL);

}

void limpaTela() {
    printf("\e[1;1H\e[2J");
}

// Essa função vai ser responsável por conter os códigos das threads. Ela não pode ter nenhum parametro
// além de void *arg.

void* caixa_thread(void *arg) {
    int id = (int) arg;

    while (true) {

        sem_wait(&caixas[id].semaforo);
        pthread_mutex_lock(&mutex);
        if (caixas[id].fila > 0) {
            caixas[id].fila--;
            pthread_mutex_unlock(&mutex);
            printf("O caixa %d atendeu um cliente da sua própria fila!\n", id);
        } else {
            int cx = -1;
            for (int i = 0; i < numeroDeCaixas; i++) {
                if (caixas[i].fila > 0) {
                    cx = i;
                    break;
                }
            }
            if (cx == -1) {
                pthread_mutex_unlock(&mutex);
                break;

            } else {
                caixas[cx].fila--;
                pthread_mutex_unlock(&mutex);
                printf("O caixa %d atendeu um cliente da fila do caixa %d!\n", id, cx);
            }
        }

        //sem_wait(&caixas[id].semaforo);
    }
    pthread_exit(NULL);
}

void start() {

    printf("Digite a quantidade de caixas: ");
    scanf("%i", &numeroDeCaixas);

    caixas = malloc(sizeof (caixa) * numeroDeCaixas);

    printf("Digite a quantidade de clientes: ");
    scanf("%i", &numeroDeClientes);

    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.bloqueado = false;
        x.fila = 0;
        caixas[i] = x;
        sem_init(&x.semaforo, 0, x.fila);
    }

    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, novoCliente_thread, NULL);

    //    for (int i = 0; i < numeroDeClientes; i++) {
    //        int toAdd = menorFila();
    //        caixas[toAdd].fila++;
    //        printf("Cliente adicionado na fila %d\n", toAdd);
    //    }

    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_create(&caixas[i].thread, NULL, caixa_thread, (void *) i);
    }

}

int main(int argc, char** argv) {

    printf("*** Welcome to SuperPthread ***\n");

    start();
    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_join(caixas[i].thread, NULL);
    }
    printf("\nTodos os clientes foram atendidos com sucesso!\n");

    return (EXIT_SUCCESS);
}