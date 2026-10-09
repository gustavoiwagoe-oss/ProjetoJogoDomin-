//Dom_GHR_View - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#include <stdio.h>
#include "Dom_GHR_View.h"
#include <stdlib.h>

#define RESET_COR   "\x1b[0m"
#define VERMELHO    "\x1b[31m"
#define VERDE       "\x1b[32m"
#define AMARELO     "\x1b[33m"
#define CIANO       "\x1b[36m"
#define BRANCO      "\x1b[1;37m"

void limparTela()
{
	system("cls");
	system(" "); 
}

void apresentarMensagem(char *mensagem)
{
	printf("\n%s%s%s\n", AMARELO, mensagem, RESET_COR);
}

int exibirMenu()
{
	int opcao;

	printf("\n====== PROJETO DOMINO ======\n");
	printf("1 - Iniciar jogo (2 jogadores)\n");
	printf("2 - Iniciar jogo (contra o computador)\n");
	printf("3 - Retornar ao jogo interrompido\n");
	printf("4 - Regras gerais do Jogo\n");
	printf("5 - Salvar o jogo em arquivo\n");
	printf("6 - Recuperar o jogo salvo em arquivo\n");
	printf("7 - Ver Historico da Ultima Partida\n");
	printf("8 - Limpar Historico de Partidas\n");
	printf("0 - Sair do programa\n");
	printf("============================\n");
	printf("Escolha uma opcao: ");

	if(scanf("%d", &opcao) != 1)
	{
		// Entrada nao era um numero (ex: letra). Limpa o buffer
		// e forca um valor que cai no "default" do switch.
		while(getchar() != '\n');
		opcao = -1;
	}

	return opcao;
}

int exibirMenuDificuldade()
{
	int opcao;
	printf("\n%s--- DIFICULDADE DA IA ---%s\n", CIANO, RESET_COR);
	printf("1 - Normal (Prioriza pecas altas e dobradas)\n");
	printf("2 - Dificil (IA Dedutiva: Memoriza as suas fraquezas e bloqueia-o)\n");
	printf("Escolha uma opcao: ");

	if(scanf("%d", &opcao) != 1)
	{
		while(getchar() != '\n');
		opcao = 0; // entrada invalida cai no modo Normal por padrao
	}

	return (opcao == 2) ? 1 : 0; 
}

const char* getLinhaASCII(int valor, int linha) {
	switch(valor) {
		case 0:
			if(linha == 0) return "   ";
			if(linha == 1) return "   ";
			if(linha == 2) return "   ";
			break;
		case 1:
			if(linha == 0) return "   ";
			if(linha == 1) return " * ";
			if(linha == 2) return "   ";
			break;
		case 2:
			if(linha == 0) return "*  ";
			if(linha == 1) return "   ";
			if(linha == 2) return "  *";
			break;
		case 3:
			if(linha == 0) return "*  ";
			if(linha == 1) return " * ";
			if(linha == 2) return "  *";
			break;
		case 4:
			if(linha == 0) return "* *";
			if(linha == 1) return "   ";
			if(linha == 2) return "* *";
			break;
		case 5:
			if(linha == 0) return "* *";
			if(linha == 1) return " * ";
			if(linha == 2) return "* *";
			break;
		case 6:
			if(linha == 0) return "* *";
			if(linha == 1) return "* *";
			if(linha == 2) return "* *";
			break;
	}
	return "   ";
}

void apresentarMesa(Mesa mesa)
{
	printf("%s\n======================================= MESA =======================================\n\n", VERDE);

	if(mesa.quantidade == 0)
	{
		printf("                               (mesa vazia)\n\n");
	}
	else
	{
		int pecasPorLinha = 7; 
		for(int i = 0; i < mesa.quantidade; i += pecasPorLinha)
		{
			int fim = i + pecasPorLinha;
			if(fim > mesa.quantidade) fim = mesa.quantidade;

			for(int p = i; p < fim; p++) {
				if(p == 0) printf("%s", AMARELO);
				else if(p == mesa.quantidade - 1) printf("%s", CIANO);
				else printf("%s", VERDE);
				printf("+---+---+ ");
			}
			printf("\n");

			for(int p = i; p < fim; p++) {
				if(p == 0) printf("%s", AMARELO);
				else if(p == mesa.quantidade - 1) printf("%s", CIANO);
				else printf("%s", VERDE);
				printf("|%s|%s| ", getLinhaASCII(mesa.pecasJogadas[p].lado1, 0), getLinhaASCII(mesa.pecasJogadas[p].lado2, 0));
			}
			printf("\n");

			for(int p = i; p < fim; p++) {
				if(p == 0) printf("%s", AMARELO);
				else if(p == mesa.quantidade - 1) printf("%s", CIANO);
				else printf("%s", VERDE);
				printf("|%s|%s| ", getLinhaASCII(mesa.pecasJogadas[p].lado1, 1), getLinhaASCII(mesa.pecasJogadas[p].lado2, 1));
			}
			printf("\n");

			for(int p = i; p < fim; p++) {
				if(p == 0) printf("%s", AMARELO);
				else if(p == mesa.quantidade - 1) printf("%s", CIANO);
				else printf("%s", VERDE);
				printf("|%s|%s| ", getLinhaASCII(mesa.pecasJogadas[p].lado1, 2), getLinhaASCII(mesa.pecasJogadas[p].lado2, 2));
			}
			printf("\n");

			for(int p = i; p < fim; p++) {
				if(p == 0) printf("%s", AMARELO);
				else if(p == mesa.quantidade - 1) printf("%s", CIANO);
				else printf("%s", VERDE);
				printf("+---+---+ ");
			}
			printf("\n\n");
		}
	}

	printf("%s====================================================================================\n%s", VERDE, RESET_COR);

	if(mesa.quantidade > 0) {
		printf("Extremidades: %s[ESQUERDA: %d]%s e %s[DIREITA: %d]%s\n", 
			AMARELO, extremidadeEsquerda(mesa), RESET_COR, 
			CIANO, extremidadeDireita(mesa), RESET_COR);
	}
}

void apresentarPecasJogador(Peca pecas[], char jogador)
{
	printf("%s\nSuas pecas:\n", BRANCO);
	
	int indices[28];
	int qtd = 0;
	for(int i = 0; i < 28; i++)
	{
		if(pecas[i].status == jogador)
		{
			indices[qtd] = i;
			qtd++;
		}
	}

	int pecasPorLinha = 7;
	for(int i = 0; i < qtd; i += pecasPorLinha)
	{
		int fim = i + pecasPorLinha;
		if(fim > qtd) fim = qtd;

		for(int p = i; p < fim; p++) printf("   (%02d)   ", p + 1);
		printf("\n");

		for(int p = i; p < fim; p++) printf("+---+---+ ");
		printf("\n");

		for(int p = i; p < fim; p++) 
			printf("|%s|%s| ", getLinhaASCII(pecas[indices[p]].lado1, 0), getLinhaASCII(pecas[indices[p]].lado2, 0));
		printf("\n");

		for(int p = i; p < fim; p++) 
			printf("|%s|%s| ", getLinhaASCII(pecas[indices[p]].lado1, 1), getLinhaASCII(pecas[indices[p]].lado2, 1));
		printf("\n");

		for(int p = i; p < fim; p++) 
			printf("|%s|%s| ", getLinhaASCII(pecas[indices[p]].lado1, 2), getLinhaASCII(pecas[indices[p]].lado2, 2));
		printf("\n");

		for(int p = i; p < fim; p++) printf("+---+---+ ");
		printf("\n\n");
	}
	
	printf("%s", RESET_COR);
}

void apresentarResultado(Peca pecas[])
{
	char vencedor = definirVencedor(pecas);

	if(vencedor == EMPATE)
		apresentarMensagem("O jogo terminou empatado!");
	else if(vencedor == JOGADOR1)
		apresentarMensagem("Jogador 1 venceu a partida!");
	else
		apresentarMensagem("Jogador 2 venceu a partida!");
}

char exibirMenuJogada()
{
	char opcao;
	printf("\nJ - Jogar\nC - Comprar\nP - Passar\nS - Sair (interromper o jogo)\nOpcao: ");
	scanf(" %c", &opcao);
	return opcao;
}

void apresentarRegras()
{
	printf("\n--- Regras do jogo (versao 2 jogadores) ---\n");
	printf("Cada jogador comeca com 7 pecas.\n");
	printf("Quem tiver a peca seis-seis comeca jogando (ou a maior peca dobrada disponivel).\n");
	printf("Na sua vez, jogue uma peca que tenha o mesmo numero de uma das extremidades da mesa.\n");
	printf("Sem jogada possivel? Compre pecas do estoque ate conseguir jogar ou ele esvaziar.\n");
	printf("O jogo termina quando alguem fica sem pecas (bate) ou quando ninguem mais consegue jogar.\n");
	printf("Vence quem ficar com menos pecas; empate na quantidade e decidido pela soma dos pontos.\n\n");
}

void apresentarHistorico()
{
	FILE *f = fopen("domino_log.txt", "r");
	if (f == NULL)
	{
		apresentarMensagem("Ainda nao existe um historico. Jogue uma partida primeiro!");
		return;
	}

	printf("%s\n--- HISTORICO DE JOGADAS ---\n%s", CIANO, RESET_COR);
	char linha[150];
	while (fgets(linha, sizeof(linha), f) != NULL)
	{
		printf(" %s", linha);
	}
	printf("%s----------------------------\n%s", CIANO, RESET_COR);
	fclose(f);

	printf("\nPressione ENTER para voltar ao menu...");
	getchar(); getchar(); 
}

void limparHistorico()
{
	FILE *f = fopen("domino_log.txt", "w");
	if (f == NULL)
	{
		apresentarMensagem("Erro ao tentar limpar o historico.");
		return;
	}
	fclose(f);
	apresentarMensagem("Historico de partidas limpo com sucesso!");
}
