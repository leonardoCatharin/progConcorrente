#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

//#define SLEEP_CLIENTE 0
//#define SLEEP_CAIXA 1

enum boolean {
    true = 1, false = 0
};

typedef enum boolean bool;

typedef struct {
    bool bloqueado;
    int fila;
    pthread_t thread;
    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
} caixa;


caixa *caixas;
int numeroDeCaixas = 0;
int numeroDeClientes = 0;

pthread_mutex_t mutexContagemClientes = PTHREAD_MUTEX_INITIALIZER;
int contagemClientes = 0;

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

        sem_wait(&caixas[toAdd].empty); //decrementa o contador empty. Bloqueia se estiver cheio

        pthread_mutex_lock(&caixas[toAdd].mutex); //Entra na região crítica

        caixas[toAdd].fila++; //insere na fila indicada por toAdd
        printf("Cliente adicionado na fila %d\n", toAdd);

        pthread_mutex_unlock(&caixas[toAdd].mutex); //Sai da região crítica

        sem_post(&caixas[toAdd].full); //incrementa o contador full.

        i++;
    }
    pthread_exit(NULL);
}

void limpaTela() {
    printf("\e[1;1H\e[2J");
}

void finalizaThreads() {
    printf("$$$$$$$$$$$$$$$$$$$");
    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_exit(&caixas[i].thread);
    }
}

void* caixa_thread(void *arg) {
    int id = (int) arg;
    int cx;
    while (contagemClientes > 0) {
        if (caixas[id].fila > 0) {
            sem_wait(&caixas[id].full); // bloqueia se vazio, decrementa o contador full

            pthread_mutex_lock(&caixas[id].mutex); //entra na região crítica

            caixas[id].fila--;
            
            pthread_mutex_lock(&mutexContagemClientes); //entra na região crítica
            contagemClientes--;
            pthread_mutex_unlock(&mutexContagemClientes); //entra na região crítica
            
            printf("O caixa %d atendeu um cliente da sua própria fila!\n", id);

            pthread_mutex_unlock(&caixas[id].mutex);

            sem_post(&caixas[id].empty); //incrementa o contador empty
        } else if ((cx = verificaTamanhoFilas()) != -1) {

            sem_wait(&caixas[cx].full); // bloqueia se vazio, decrementa o contador full

            pthread_mutex_lock(&caixas[cx].mutex);

            caixas[cx].fila--;
            
            pthread_mutex_lock(&mutexContagemClientes); //entra na região crítica
            contagemClientes--;
            pthread_mutex_unlock(&mutexContagemClientes); //entra na região crítica
            
            printf("O caixa %d atendeu um cliente da fila do caixa %d!\n", id, cx);
            pthread_mutex_unlock(&caixas[cx].mutex);

            sem_post(&caixas[cx].empty); //incrementa o contador empty
        }
    }
    finalizaThreads();
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
        x.bloqueado = false;
        x.fila = 0;
        sem_init(&x.empty, 0, 10);
        sem_init(&x.full, 0, 0);
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