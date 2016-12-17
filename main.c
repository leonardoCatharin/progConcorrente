#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define TIME_CLIENTE 0
#define TIME_CAIXA 0

typedef struct {
    int fila;
    pthread_t thread;
    sem_t empty;
    pthread_mutex_t mutex;
} caixa;


caixa *caixas;
int numeroDeCaixas = 0;
int numeroDeClientes = 0;

pthread_mutex_t mutexContagemClientes = PTHREAD_MUTEX_INITIALIZER;
int contagemClientes = 0;

void delay(int tempo) {
    int retTempo = time(0) + tempo;
    while (time(0) < retTempo);
}

void destroiThreads() {
    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_cancel(caixas[i].thread);
    }
}

int menorFila() {
    int m = 0;
    for (int i = 1; i < numeroDeCaixas; i++) {
        if (caixas[i].fila < caixas[m].fila) {
            m = i;
        }
    }
    return m;
}

int verificaTamanhoFilas() {
    for (int i = 0; i < numeroDeCaixas; i++) {
        if (caixas[i].fila > 0) {
            return i;
        }
    }

    return -1;
}

void* novoCliente_thread(void *arg) {
    int i = 0;

    while (i < numeroDeClientes) {
        int toAdd = menorFila(); // encontra a menor fila pra adicionar o cliente

        pthread_mutex_lock(&caixas[toAdd].mutex); //Entra na região crítica

        caixas[toAdd].fila++; //insere na fila indicada por toAdd
        printf("Cliente adicionado na fila %d\n", toAdd);

        pthread_mutex_unlock(&caixas[toAdd].mutex); //Sai da região crítica

        sem_post(&caixas[toAdd].empty); //incrementa o contador empty.

        i++;

        delay(TIME_CLIENTE);
    }
    pthread_exit(NULL);
}

void limpaTela() {
    printf("\e[1;1H\e[2J");
}

void* caixa_thread(void *arg) {
    int id = (int) arg;
    int cx;
    while (contagemClientes > 0) {
        
        if (caixas[id].fila > 0) {
            sem_wait(&caixas[id].empty); // bloqueia se vazio, decrementa o contador empty

            pthread_mutex_lock(&caixas[id].mutex); //entra na região crítica
            caixas[id].fila--;
            printf("O caixa %d atendeu um cliente da sua própria fila!\n", id);
            pthread_mutex_unlock(&caixas[id].mutex);

            pthread_mutex_lock(&mutexContagemClientes); //entra na região crítica
            contagemClientes--;
            pthread_mutex_unlock(&mutexContagemClientes); //entra na região crítica

        } else if ((cx = verificaTamanhoFilas()) != -1) {
            sem_wait(&caixas[cx].empty); // bloqueia se vazio, decrementa o contador empty

            pthread_mutex_lock(&caixas[cx].mutex);
            caixas[cx].fila--;
            printf("O caixa %d atendeu um cliente da fila do caixa %d!\n", id, cx);
            pthread_mutex_unlock(&caixas[cx].mutex);

            pthread_mutex_lock(&mutexContagemClientes); //entra na região crítica
            contagemClientes--;
            pthread_mutex_unlock(&mutexContagemClientes); //entra na região crítica
        }
        delay(TIME_CAIXA);
    }
    destroiThreads();
    pthread_exit(NULL);
}

void start() {
    printf("Digite a quantidade de caixas: ");
    scanf("%i", &numeroDeCaixas);

    printf("\nDigite a quantidade de clientes: ");
    scanf("%i", &numeroDeClientes);

    contagemClientes = numeroDeClientes;

    caixas = malloc(sizeof (caixa) * numeroDeCaixas); //alocação dinâmica dos caixas

    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.fila = 0;
        sem_init(&x.empty, 0, 0);
        pthread_mutex_init(&x.mutex, NULL);
        caixas[i] = x;
    }

    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, novoCliente_thread, NULL);

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