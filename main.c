#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define TIME_CLIENTE 0
#define TIME_CAIXA 0
#define NUMERO_CLIENTE 1000000
#define NUMERO_CAIXA 4

typedef struct {
    int fila;
    pthread_t thread;
    sem_t empty;
} caixa;

caixa *caixas;
int numeroDeCaixas = NUMERO_CAIXA;
int numeroDeClientes = NUMERO_CLIENTE;
int contagemClientes = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void delay(int x) {
    int j;
    while (j < x) j++;
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

        pthread_mutex_lock(&mutex);
        
        int toAdd = menorFila();
        
        caixas[toAdd].fila++;
        
        pthread_mutex_unlock(&mutex);
        sem_post(&caixas[toAdd].empty);
        i++;
        delay(TIME_CLIENTE);
    }
    
    pthread_exit(NULL);
}

void atende(int id){
    sem_wait(&caixas[id].empty);
    caixas[id].fila--;
    contagemClientes--;
}

void* caixa_thread(void *arg) {
    int id = (int) arg;
    int cx;
    while (contagemClientes > 0) {
        pthread_mutex_lock(&mutex);
        if (caixas[id].fila > 0) {
            caixas[id].fila--;
            contagemClientes--;
            pthread_mutex_unlock(&mutex);
            sem_wait(&caixas[id].empty);
        } else {
           pthread_mutex_unlock(&mutex); 
        }
        
        pthread_mutex_lock(&mutex);
        if ((cx = verificaTamanhoFilas()) != -1) {
            caixas[cx].fila--;
            contagemClientes--;
            pthread_mutex_unlock(&mutex);
            sem_wait(&caixas[cx].empty);
        } else {
            pthread_mutex_unlock(&mutex);
        }
        
        delay(TIME_CAIXA);
    }
    pthread_exit(NULL);
}

void start() {
    contagemClientes = numeroDeClientes;
    caixas = malloc(sizeof (caixa) * numeroDeCaixas);

    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.fila = 0;
        sem_init(&x.empty, 0, 0);
        caixas[i] = x;
    }

    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, novoCliente_thread, NULL);

    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_create(&caixas[i].thread, NULL, caixa_thread, (void *) i);
    }
}

int main(int argc, char** argv) {
    start();

    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_join(caixas[i].thread, NULL);
    }
    
    return (EXIT_SUCCESS);
}