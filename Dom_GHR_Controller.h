//Dom_GHR_Controller.h - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#ifndef DOM_GHR_CONTROLLER_H
#define DOM_GHR_CONTROLLER_H

#include "Dom_GHR_Model.h" 

void jogar();
void jogarPartida(int contraComputador, int novoJogo);
int turnoJogador(Peca pecas[], Mesa *mesa, char jogador);
int escolherPeca(Peca pecas[], char jogador);
int escolherExtremidade();

// Funcao de registro de acoes (Log)
void registarLog(const char* mensagem, int novoArquivo);

#endif
