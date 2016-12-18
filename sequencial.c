/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sequencial.c
 * Author: igorsantana
 *
 * Created on December 18, 2016, 6:33 PM
 */

#include <stdio.h>
#include <stdlib.h>

int* caixas;
int numero_caixas, numero_clientes, clientes_adicionados = 0;

void inicializa_caixas(){
    int i;
    for(i = 0; i < numero_caixas; i++) caixas[i] = 0;
}

void adiciona_clientes(){
    int i = 0;
    while(clientes_adicionados < numero_clientes){
        for(i = numero_caixas; i >= 0; i--){
            if(clientes_adicionados == numero_clientes) break;
            printf("Cliente adicionado no caixa %d\n", i);
            caixas[i]++;
            clientes_adicionados++;
        }
    }
}

void printa_caixas(){
    int i;
    for(i = 0; i < numero_caixas; i++){
        printf("Caixa %d tem %d clientes\n", i, caixas[i]);
    }
}

int caixa_com_fila(){
    int ret = -1, i;
    for(i = 0; i < numero_caixas; i++){
        if(caixas[i] > 0) return i;
    }
    return -1;
}

void atendimento_caixas(){
    int i = 0, x = 0;
    while(1){
        x = 0;
        for(i = 0; i < numero_caixas; i++){
            if(caixas[i] > 0){
                printf("O caixa %d atendeu um cliente da sua própria fila\n", i);
                caixas[i]--;
            } else {
                x++;
                int cx = caixa_com_fila();
                if(cx != -1){
                    caixas[cx]--;     
                    printf("O caixa %d atendeu um cliente da fila do caixa %d\n", i, cx);
                }
            }
        }
        if(x == numero_caixas) break;
    }
}

int main(int argc, char** argv) {
    
    printf("Digite a quantidade de caixas: ");
    scanf("%i", &numero_caixas);

    printf("\nDigite a quantidade de clientes: ");
    scanf("%i", &numero_clientes);
    
    caixas = malloc(sizeof(int) * numero_caixas);
    inicializa_caixas();
    
    adiciona_clientes();
        printa_caixas();
    
    atendimento_caixas();
        printa_caixas();
    
    return (EXIT_SUCCESS);
}

