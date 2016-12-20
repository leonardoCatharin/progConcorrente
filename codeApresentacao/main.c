/*Código paralelo*/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

//Constantes
#define TIME_CLIENTE 100
#define TIME_CAIXA 1000

//Estruturas
typedef struct {
    int fila;
    pthread_t thread;
    sem_t empty;
} caixa;

//Variáveis Globais
caixa *caixas;
int numeroDeCaixas;
int numeroDeClientes;
int contagemClientes = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
 * Verifica se há alguma fila com clientes para serem atendidos
 * Retorna o índice no array de caixas referente a fila que ainda há clientes
 * A prioridade é a primeira que for encontrada na sequência de testes dentro do for
 */
int verificaTamanhoFilas() {
    for (int i = 0; i < numeroDeCaixas; i++) {
        if (caixas[i].fila > 0) {
            return i;
        }
    }
    return -1;
}

/*
 * Thread responsável pela inserção de clientes na menor fila
 * Só é finalizada quando todos os clientes são inseridos
 */
void* novoCliente_thread(void *arg) {
    int i = 0;

    while (i < numeroDeClientes) {

        pthread_mutex_lock(&mutex); //Entra na área crítica

        int toAdd = menorFila(); //Encontra menor fila

        caixas[toAdd].fila++;
        printf("\nO cliente foi adicionado na fila %i", toAdd);
        pthread_mutex_unlock(&mutex);
        sem_post(&caixas[toAdd].empty); //incrementa empty
        i++;
        delay(TIME_CLIENTE);
        tempoEspera(1);
    }

    pthread_exit(NULL);
}

/*
 * Thread responsável pelo atendimento de clientes em suas respecitivas filas ou
 * ajudar outro caixa quando estiver com fila = 0
 * Só é finalizada quando todos os clientes são atendidos
 */
void* caixa_thread(void *arg) {
    int id = (int) arg;
    int cx;
    //Quando contagemDeClientes zerar, todos os clientes foram atendidos
    while (contagemClientes > 0) {
        pthread_mutex_lock(&mutex); //Entra na área crítica

        //Se caixa contém cliente na fila atende
        if (caixas[id].fila > 0) {
            caixas[id].fila--;
            contagemClientes--;
            printf("\nO caixa %i atendeu um cliente da sua própria fila!", id);
            pthread_mutex_unlock(&mutex); //Sai da área crítica
            sem_wait(&caixas[id].empty); //bloqueia se vazio, decrementa empty
        } else {
            pthread_mutex_unlock(&mutex); //Sai da área crítica

            pthread_mutex_lock(&mutex); //Entra na área crítica
            if ((cx = verificaTamanhoFilas()) != -1) {
                caixas[cx].fila--;
                contagemClientes--;
                printf("\nO caixa %i atendeu um cliente do caixa %i!", id, cx);
                pthread_mutex_unlock(&mutex); //Sai da área crítica
                sem_wait(&caixas[cx].empty); //bloqueia se vazio, decrementa empty
            } else {
                pthread_mutex_unlock(&mutex); //Sai da área crítica
            }
        }
        delay(TIME_CAIXA);
        tempoEspera(2);
    }
    pthread_exit(NULL);
}

/*
 * Função utilizada para a inicialização dos caixas e das threads
 */
void start() {

    //variável usada para decremento no atendimento dos clientes
    contagemClientes = numeroDeClientes;

    caixas = malloc(sizeof (caixa) * numeroDeCaixas); //alocação de memória dinâmica para os caixas

    //Inicialização dos caixas
    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.fila = 0;
        sem_init(&x.empty, 0, 0);
        caixas[i] = x;
    }

    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, novoCliente_thread, NULL);

    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_create(&caixas[i].thread, NULL, caixa_thread, (void *) i); //Criação de threads
    }
}

/*
 * MAIN
 * Finaliza quando todas as threads são finalizadas
 */

int main(int argc, char** argv) {
    printf("\nQuantidade de caixas: ");
    scanf("%i", &numeroDeCaixas);
    
    printf("\nQuantidade de clientes: ");
    scanf("%i", &numeroDeClientes);
    
    start();

    for (int i = 0; i < numeroDeCaixas; i++) {
        pthread_join(caixas[i].thread, NULL); //faz esperar todas as threads serem finalizadas para finalizar
    }

    return (EXIT_SUCCESS);
}