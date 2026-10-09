//Dom_GHR_Model - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Dom_GHR_Model.h"

Peca pecas[28];
Mesa mesa;

int modoContraComputador = 0;
char jogadorDaVez = JOGADOR1;
int jogoInterrompido = 0;

int dificuldadeIA = 0; 
int jogador1NaoTem[7] = {0};

int vitoriasJ1 = 0;
int vitoriasJ2 = 0;
int empatesPlacar = 0;

void criarPecas(Peca pecas[])
{
	int indice = 0;
	for(int i = 0; i <= 6; i++)
	{
		for(int j = i; j <= 6; j++ )
		{
			pecas[indice].lado1 = i;
			pecas[indice].lado2 = j;
			indice++;
		}
	}
}

void embaralharPecas(Peca pecas[])
{
	static int semeado = 0;
	if(!semeado)
	{
		srand(time(NULL));
		semeado = 1;
	}

	for(int i = 0; i < 28; i++)
	{
		int j = rand() % 28;
		Peca aux = pecas[i];
		pecas[i] = pecas[j];
		pecas[j] = aux;
	}
}

void distribuirPecas(Peca pecas[])
{
	for(int i = 0; i < 28; i++)
	{
		if(i < 7)
			pecas[i].status = JOGADOR1;
		else if(i < 14)
			pecas[i].status = JOGADOR2;
		else
			pecas[i].status = DISPONIVEL;
	}
}

char definirPrimeiroJogador(Peca pecas[], Mesa *mesa)
{
	int indice = -1;

	for(int valor = 6; valor >= 0 && indice == -1; valor--)
	{
		for(int i = 0; i < 28; i++)
		{
			if(pecas[i].lado1 == valor && pecas[i].lado2 == valor
				&& (pecas[i].status == JOGADOR1 || pecas[i].status == JOGADOR2))
			{
				indice = i;
				break;
			}
		}
	}

	if(indice == -1)
	{
		int melhorSoma = -1;
		for(int i = 0; i < 28; i++)
		{
			if(pecas[i].status == JOGADOR1 || pecas[i].status == JOGADOR2)
			{
				int soma = pecas[i].lado1 + pecas[i].lado2;
				if(soma > melhorSoma)
				{
					melhorSoma = soma;
					indice = i;
				}
			}
		}
	}

	char jogador = pecas[indice].status;
	jogarPeca(mesa, &pecas[indice], 1); 

	return jogador;
}

void criarMesa(Mesa *mesa)
{
	mesa->quantidade = 0;
}

int extremidadeEsquerda(Mesa mesa)
{
	return mesa.pecasJogadas[0].lado1;
}

int extremidadeDireita(Mesa mesa)
{
	return mesa.pecasJogadas[mesa.quantidade - 1].lado2;
}

int jogadaValida(Peca peca, int extremidade)
{
	return (peca.lado1 == extremidade || peca.lado2 == extremidade);
}

int jogarPeca(Mesa *mesa, Peca *peca, int ladoEscolhido)
{
	if(mesa->quantidade == 0)
	{
		mesa->pecasJogadas[0] = *peca;
		peca->status = MESA;
		mesa->quantidade++;
		return 1;
	}

	if(ladoEscolhido == 0) 
	{
		int extremidade = extremidadeEsquerda(*mesa);
		if(!jogadaValida(*peca, extremidade)) return 0;

		if(peca->lado2 != extremidade)
		{
			int aux = peca->lado1;
			peca->lado1 = peca->lado2;
			peca->lado2 = aux;
		}

		for(int i = mesa->quantidade; i > 0; i--)
			mesa->pecasJogadas[i] = mesa->pecasJogadas[i - 1];

		mesa->pecasJogadas[0] = *peca;
	}
	else 
	{
		int extremidade = extremidadeDireita(*mesa);
		if(!jogadaValida(*peca, extremidade)) return 0;

		if(peca->lado1 != extremidade)
		{
			int aux = peca->lado1;
			peca->lado1 = peca->lado2;
			peca->lado2 = aux;
		}

		mesa->pecasJogadas[mesa->quantidade] = *peca;
	}

	peca->status = MESA;
	mesa->quantidade++;
	return 1;
}

int comprarPeca(Peca pecas[], char jogador)
{
	for(int i = 0; i < 28; i++)
	{
		if(pecas[i].status == DISPONIVEL)
		{
			pecas[i].status = jogador;
			return i;
		}
	}
	return -1; 
}

int jogadorTemJogada(Peca pecas[], char jogador, Mesa mesa)
{
	if(mesa.quantidade == 0)
	{
		for(int i = 0; i < 28; i++)
			if(pecas[i].status == jogador) return 1;
		return 0;
	}

	int esq = extremidadeEsquerda(mesa);
	int dir = extremidadeDireita(mesa);

	for(int i = 0; i < 28; i++)
	{
		if(pecas[i].status == jogador)
		{
			if(jogadaValida(pecas[i], esq) || jogadaValida(pecas[i], dir)) return 1;
		}
	}
	return 0;
}

int contarPecas(Peca pecas[], char status)
{
	int total = 0;
	for(int i = 0; i < 28; i++)
		if(pecas[i].status == status) total++;
	return total;
}

int somaPontos(Peca pecas[], char jogador)
{
	int soma = 0;
	for(int i = 0; i < 28; i++)
		if(pecas[i].status == jogador)
			soma += pecas[i].lado1 + pecas[i].lado2;
	return soma;
}

int jogoTerminou(Peca pecas[], Mesa mesa)
{
	if(contarPecas(pecas, JOGADOR1) == 0 || contarPecas(pecas, JOGADOR2) == 0) return 1;

	if(contarPecas(pecas, DISPONIVEL) == 0 && !jogadorTemJogada(pecas, JOGADOR1, mesa) && !jogadorTemJogada(pecas, JOGADOR2, mesa)) return 1;

	return 0;
}

char definirVencedor(Peca pecas[])
{
	int qtdJ1 = contarPecas(pecas, JOGADOR1);
	int qtdJ2 = contarPecas(pecas, JOGADOR2);

	if(qtdJ1 < qtdJ2) return JOGADOR1;
	if(qtdJ2 < qtdJ1) return JOGADOR2;

	int pontosJ1 = somaPontos(pecas, JOGADOR1);
	int pontosJ2 = somaPontos(pecas, JOGADOR2);

	if(pontosJ1 < pontosJ2) return JOGADOR1;
	if(pontosJ2 < pontosJ1) return JOGADOR2;

	return EMPATE;
}

void iniciarMemoriaIA() {
	for(int i = 0; i < 7; i++) jogador1NaoTem[i] = 0;
}

void registrarFaltaPecaJogador(int esq, int dir) {
	jogador1NaoTem[esq] = 1;
	jogador1NaoTem[dir] = 1;
}

void jogadaComputador(Peca pecas[], Mesa *mesa)
{
	if(mesa->quantidade == 0)
	{
		int melhorIndice = -1;
		int maiorSoma = -1;
		
		for(int i = 0; i < 28; i++) {
			if(pecas[i].status == JOGADOR2) {
				if(pecas[i].lado1 == pecas[i].lado2 && (pecas[i].lado1 * 2 > maiorSoma)) {
					maiorSoma = pecas[i].lado1 * 2;
					melhorIndice = i;
				}
			}
		}
		
		if (melhorIndice == -1) {
			maiorSoma = -1;
			for(int i = 0; i < 28; i++) {
				if(pecas[i].status == JOGADOR2) {
					int soma = pecas[i].lado1 + pecas[i].lado2;
					if(soma > maiorSoma) {
						maiorSoma = soma;
						melhorIndice = i;
					}
				}
			}
		}
		jogarPeca(mesa, &pecas[melhorIndice], 1);
		return;
	}

	int esq = extremidadeEsquerda(*mesa);
	int dir = extremidadeDireita(*mesa);

	int melhorIndice = -1;
	int melhorLado = -1; 
	int maiorScore = -1;

	for(int i = 0; i < 28; i++)
	{
		if(pecas[i].status == JOGADOR2)
		{
			if(jogadaValida(pecas[i], esq))
			{
				int soma = pecas[i].lado1 + pecas[i].lado2;
				int ehDobrada = (pecas[i].lado1 == pecas[i].lado2) ? 1 : 0;
				int score = soma + (ehDobrada * 100); 

				if(dificuldadeIA == 1) {
					int novaEsq = (pecas[i].lado1 == esq) ? pecas[i].lado2 : pecas[i].lado1;
					if(jogador1NaoTem[novaEsq]) score += 500; 
				}

				if(score > maiorScore) {
					maiorScore = score;
					melhorIndice = i;
					melhorLado = 0; 
				}
			}
			
			if(jogadaValida(pecas[i], dir))
			{
				int soma = pecas[i].lado1 + pecas[i].lado2;
				int ehDobrada = (pecas[i].lado1 == pecas[i].lado2) ? 1 : 0;
				int score = soma + (ehDobrada * 100); 

				if(dificuldadeIA == 1) {
					int novaDir = (pecas[i].lado1 == dir) ? pecas[i].lado2 : pecas[i].lado1;
					if(jogador1NaoTem[novaDir]) score += 500; 
				}

				if(score > maiorScore) {
					maiorScore = score;
					melhorIndice = i;
					melhorLado = 1; 
				}
			}
		}
	}

	if(melhorIndice != -1) {
		jogarPeca(mesa, &pecas[melhorIndice], melhorLado);
	} else {
		comprarPeca(pecas, JOGADOR2);
	}
}

void apresentarEstatisticasFinais(Peca pecas[])
{
	int qtdJ1 = contarPecas(pecas, JOGADOR1);
	int qtdJ2 = contarPecas(pecas, JOGADOR2);
	int pontosJ1 = somaPontos(pecas, JOGADOR1);
	int pontosJ2 = somaPontos(pecas, JOGADOR2);

	printf("\n================ ESTATISTICAS FINAIS ================\n");
	printf(" Jogador 1 -> Pecas na mao: %d | Total de pontos: %d\n", qtdJ1, pontosJ1);
	printf(" Jogador 2 -> Pecas na mao: %d | Total de pontos: %d\n", qtdJ2, pontosJ2);
	printf("-----------------------------------------------------\n");
	printf(" PLACAR ACUMULADO:\n");
	printf(" Vitorias Jogador 1: %d\n", vitoriasJ1);
	printf(" Vitorias Jogador 2 / Computador: %d\n", vitoriasJ2);
	printf(" Empates: %d\n", empatesPlacar);
	printf("=====================================================\n");
}

int salvarJogoEmArquivo(const char* nomeArquivo) 
{
	FILE *f = fopen(nomeArquivo, "wb"); 
	if(f == NULL) return 0;

	fwrite(pecas, sizeof(Peca), 28, f);
	fwrite(&mesa, sizeof(Mesa), 1, f);
	fwrite(&jogadorDaVez, sizeof(char), 1, f);
	fwrite(&modoContraComputador, sizeof(int), 1, f);
	fwrite(&dificuldadeIA, sizeof(int), 1, f);
	fwrite(jogador1NaoTem, sizeof(int), 7, f);
	fwrite(&vitoriasJ1, sizeof(int), 1, f);
	fwrite(&vitoriasJ2, sizeof(int), 1, f);
	fwrite(&empatesPlacar, sizeof(int), 1, f);

	fclose(f);
	return 1;
}

int carregarJogoDeArquivo(const char* nomeArquivo) 
{
	FILE *f = fopen(nomeArquivo, "rb"); 
	if(f == NULL) return 0;

	fread(pecas, sizeof(Peca), 28, f);
	fread(&mesa, sizeof(Mesa), 1, f);
	fread(&jogadorDaVez, sizeof(char), 1, f);
	fread(&modoContraComputador, sizeof(int), 1, f);
	fread(&dificuldadeIA, sizeof(int), 1, f);
	fread(jogador1NaoTem, sizeof(int), 7, f);
	fread(&vitoriasJ1, sizeof(int), 1, f);
	fread(&vitoriasJ2, sizeof(int), 1, f);
	fread(&empatesPlacar, sizeof(int), 1, f);

	fclose(f);
	return 1;
}
