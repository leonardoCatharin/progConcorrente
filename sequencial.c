#include <stdio.h>
#include <stdlib.h>

#define NUMERO_CLIENTE 1000000
#define NUMERO_CAIXA 4

typedef struct {
    int fila;
} caixa;

caixa *caixas;
int numeroDeCaixas = NUMERO_CAIXA;
int numeroDeClientes = NUMERO_CLIENTE;

int menorFila() {
    int m = 0;
    for (int i = 1; i < numeroDeCaixas; i++) {
        if (caixas[i].fila < caixas[m].fila) {
            m = i;
        }
    }
    return m;
}

void atraso(int x) {
    int j;
    while (j < x) j++;
}

void adicionaCliente() {
    int i = 0;

    while (i < numeroDeClientes) {
        
        int toAdd;

        toAdd = menorFila();
        caixas[toAdd].fila++;
        i++;
        atraso(1000);
    }
}
int verificaTamanhoFilas() {
    for (int i = 0; i < numeroDeCaixas; i++) {
        if (caixas[i].fila > 0) {
            return i;
        }
    }

    return -1;
}
void atenderClientes() {
    while(verificaTamanhoFilas() != -1){
      for (int i = 0; i < numeroDeCaixas; i++) {
            caixas[i].fila--;
            atraso(10000);
        }  
    }
}

int main(int argc, char** argv) {

    caixas = malloc(sizeof (caixa) * numeroDeCaixas);

    for (int i = 0; i < numeroDeCaixas; i++) {
        caixa x;
        x.fila = 0;
        caixas[i] = x;
    }

    adicionaCliente();

    atenderClientes();

    return (EXIT_SUCCESS);
}