//Dom_GHR_View.h - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#ifndef DOM_GHR_VIEW_H
#define DOM_GHR_VIEW_H

#include "Dom_GHR_Model.h" 

void limparTela();
void apresentarMensagem(char *mensagem);
int exibirMenu();
int exibirMenuDificuldade();

void apresentarMesa(Mesa mesa);
void apresentarPecasJogador(Peca pecas[], char jogador);
void apresentarResultado(Peca pecas[]);
char exibirMenuJogada();
void apresentarRegras();
void apresentarHistorico(); 
void limparHistorico(); // Adicionado e garantido aqui

#endif
