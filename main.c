/* 
 * File:   main.c
 * Author: igorsantana
 *
 * Created on October 20, 2016, 7:08 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum boolean { true = 1, false = 0 };

typedef enum boolean bool;
typedef struct { bool bloqueado; int fila; pthread_t thread; } caixa;

caixa *caixas;

int numeroDeCaixas = 0;
// Função responsável por encontrar o caixa com a menor fila
int menorFila(){
    int m = 0;
    for (int i = 1; i < numeroDeCaixas; i++) {
        if(caixas[i].fila < caixas[m].fila){
            m = i;
        }
    }
    return m;
}
// Função que adiciona clientes na menor fila.
void novoCliente(){
    int toAdd = menorFila();
    caixas[toAdd].fila++;
    
    for (int i = 0; i < numeroDeCaixas; i++) {
        if(caixas[i].fila > 0){
            printf("Caixa %d: [%d] pessoas na fila\n",i, caixas[i].fila);
        }
    }
}
//Função que NÃO FUNCIONA NO WINDOWS pra limpar a tela.
void limpaTela(){
    printf("\e[1;1H\e[2J");
}

void* caixa_thread(void *arg){
    int id = (int) arg;
    while(true){
        if(caixas[id].fila > 0){
            caixas[id].fila--;
            printf("Cliente do caixa %d atendido.\n", id);
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    int n;
    printf("Digite o numero de caixas:\n");
    scanf("%d", &n);
    numeroDeCaixas = n;
    caixas = malloc (sizeof (caixa) * numeroDeCaixas);
    
    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.bloqueado = false;
        x.fila = 0;
        caixas[i] = x;
        pthread_create(&x.thread, NULL, caixa_thread, (void *) i);
    }
    
    while(1){
        limpaTela();
        int x = 0;
        do {
            novoCliente();
            x++;
        } while(x < 3);
        
        sleep(1);
    }
    
    return (EXIT_SUCCESS);
}

