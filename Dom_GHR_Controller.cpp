//Dom_GHR_Controller - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#include <stdio.h>
#include "Dom_GHR_Controller.h"
#include "Dom_GHR_View.cpp"

void registarLog(const char* mensagem, int novoArquivo)
{
	FILE *f = fopen("domino_log.txt", novoArquivo ? "w" : "a");
	if(f != NULL)
	{
		fprintf(f, "%s\n", mensagem);
		fclose(f);
	}
}

void jogar()
{
	int opcao;
	int continuar = 1;

	while(continuar == 1)
	{
		opcao = exibirMenu();

		switch(opcao)
		{
			case 1:
				jogarPartida(0, 1); 
				break;

			case 2:
				dificuldadeIA = exibirMenuDificuldade(); 
				jogarPartida(1, 1); 
				break;

			case 3:
				if(jogoInterrompido) {
					jogarPartida(modoContraComputador, 0); 
				} else {
					apresentarMensagem("Nenhum jogo em andamento na memoria para retornar.");
				}
				break;

			case 4:
				limparTela();
				apresentarRegras();
				break;

			case 5:
				if(jogoInterrompido) {
					if(salvarJogoEmArquivo("domino_save.bin"))
						apresentarMensagem("Jogo salvo com sucesso!");
					else
						apresentarMensagem("Erro ao salvar o jogo.");
				} else {
					apresentarMensagem("Nenhum jogo interrompido para salvar. Inicie uma partida primeiro e aperte 'S' para interromper.");
				}
				break;

			case 6:
				if(carregarJogoDeArquivo("domino_save.bin")) {
					jogoInterrompido = 1; 
					apresentarMensagem("Jogo carregado! Escolha a opcao 3 no menu para continuar jogando.");
				} else {
					apresentarMensagem("Nenhum arquivo de jogo salvo foi encontrado.");
				}
				break;

			case 7:
				limparTela();
				apresentarHistorico();
				break;

			case 8:
				limparTela();
				limparHistorico();
				break;

			case 0:
				continuar = 0;
				apresentarMensagem("Finalizando o jogo...");
				break;

			default:
				limparTela();
				apresentarMensagem("Opcao invalida! Escolha uma opcao disponivel no menu.");
		}
	}
}

void jogarPartida(int contraComputador, int novoJogo)
{
	if(novoJogo)
	{
		criarPecas(pecas);
		embaralharPecas(pecas);
		distribuirPecas(pecas);
		criarMesa(&mesa);

		registarLog("--- NOVA PARTIDA INICIADA ---", 1);
		if(contraComputador) {
			if(dificuldadeIA == 1) registarLog("Modo: 1 Jogador vs Computador (IA DIFICIL)", 0);
			else registarLog("Modo: 1 Jogador vs Computador (IA NORMAL)", 0);
		} else {
			registarLog("Modo: 2 Jogadores", 0);
		}
		iniciarMemoriaIA();

		jogadorDaVez = definirPrimeiroJogador(pecas, &mesa);
		jogadorDaVez = (jogadorDaVez == JOGADOR1) ? JOGADOR2 : JOGADOR1;

		modoContraComputador = contraComputador;
	}

	int interrompido = 0;

	while(!interrompido && !jogoTerminou(pecas, mesa))
	{
		limparTela();
		apresentarMesa(mesa);

		if(jogadorDaVez == JOGADOR1)
		{
			apresentarPecasJogador(pecas, JOGADOR1);
			interrompido = !turnoJogador(pecas, &mesa, JOGADOR1);
		}
		else if(modoContraComputador)
		{
			apresentarMensagem("Vez do computador...");
			if(jogadorTemJogada(pecas, JOGADOR2, mesa)) {
				jogadaComputador(pecas, &mesa);
				registarLog("Computador jogou uma peca.", 0);
			} else {
				apresentarMensagem("Computador nao tem jogada. Comprando...");
				comprarPeca(pecas, JOGADOR2);
				registarLog("Computador precisou comprar uma peca.", 0);
			}
		}
		else
		{
			apresentarPecasJogador(pecas, JOGADOR2);
			interrompido = !turnoJogador(pecas, &mesa, JOGADOR2);
		}

		if(!interrompido)
			jogadorDaVez = (jogadorDaVez == JOGADOR1) ? JOGADOR2 : JOGADOR1;
	}

	if(!interrompido)
	{
		limparTela();
		apresentarMesa(mesa);
		apresentarResultado(pecas);
		
		char vencedor = definirVencedor(pecas);
		if(vencedor == JOGADOR1) vitoriasJ1++;
		else if(vencedor == JOGADOR2) vitoriasJ2++;
		else empatesPlacar++;

		apresentarEstatisticasFinais(pecas); 
		
		jogoInterrompido = 0; 
		registarLog("--- PARTIDA FINALIZADA COM ESTATISTICAS ---", 0);
		
		// Limpa o buffer do teclado antes de aguardar o ENTER
		int c;
		while ((c = getchar()) != '\n' && c != EOF);

		printf("\nPressione ENTER para voltar ao menu principal...");
		getchar();
	}
	else
	{
		jogoInterrompido = 1; 
	}
}

int turnoJogador(Peca pecas[], Mesa *mesa, char jogador)
{
	char msgLog[100]; 
	int comprouForcado = 0;
	
	if(jogador == JOGADOR1 && mesa->quantidade > 0 && !jogadorTemJogada(pecas, jogador, *mesa)) {
		registrarFaltaPecaJogador(extremidadeEsquerda(*mesa), extremidadeDireita(*mesa));
	}

	while(!jogadorTemJogada(pecas, jogador, *mesa) && contarPecas(pecas, DISPONIVEL) > 0)
	{
		apresentarMensagem("Voce nao tem jogada. Comprando peca...");
		comprarPeca(pecas, jogador);
		comprouForcado = 1;
	}
	
	if(comprouForcado) {
		apresentarPecasJogador(pecas, jogador);
		sprintf(msgLog, "Jogador %c comprou peca(s) por falta de jogada.", jogador);
		registarLog(msgLog, 0);
	}

	if(!jogadorTemJogada(pecas, jogador, *mesa) && contarPecas(pecas, DISPONIVEL) == 0)
	{
		apresentarMensagem("Sem jogadas e sem pecas para comprar. Passando a vez.");
		sprintf(msgLog, "Jogador %c passou a vez.", jogador);
		registarLog(msgLog, 0);
		return 1;
	}

	int jogou = 0;

	while(!jogou)
	{
		char opcao = exibirMenuJogada();

		if(opcao == 'J' || opcao == 'j')
		{
			int indice = escolherPeca(pecas, jogador);
			if(indice == -1)
			{
				apresentarMensagem("Peca invalida.");
				continue;
			}

			int lado = (mesa->quantidade == 0) ? 1 : escolherExtremidade();

			if(jogarPeca(mesa, &pecas[indice], lado)) {
				jogou = 1;
				sprintf(msgLog, "Jogador %c jogou a peca [%d|%d].", jogador, pecas[indice].lado1, pecas[indice].lado2);
				registarLog(msgLog, 0);
			}
			else
				apresentarMensagem("Jogada invalida, tente novamente.");
		}
		else if(opcao == 'C' || opcao == 'c')
		{
			if(jogador == JOGADOR1 && mesa->quantidade > 0) {
				registrarFaltaPecaJogador(extremidadeEsquerda(*mesa), extremidadeDireita(*mesa));
			}

			if(comprarPeca(pecas, jogador) == -1)
				apresentarMensagem("Nao ha mais pecas para comprar.");
			else {
				apresentarPecasJogador(pecas, jogador);
				sprintf(msgLog, "Jogador %c comprou uma peca (blefe).", jogador);
				registarLog(msgLog, 0);
			}
		}
		else if(opcao == 'P' || opcao == 'p')
		{
			if(!jogadorTemJogada(pecas, jogador, *mesa) && contarPecas(pecas, DISPONIVEL) == 0) {
				jogou = 1;
				sprintf(msgLog, "Jogador %c passou a vez.", jogador);
				registarLog(msgLog, 0);
			}
			else
				apresentarMensagem("Voce so pode passar se nao tiver jogada e nao houver pecas para comprar.");
		}
		else if(opcao == 'S' || opcao == 's')
		{
			apresentarMensagem("Interrompendo a partida...");
			registarLog("--- PARTIDA INTERROMPIDA ---", 0);
			return 0;
		}
		else
		{
			apresentarMensagem("Opcao invalida.");
		}
	}

	return 1;
}

int escolherPeca(Peca pecas[], char jogador)
{
	int numero;
	printf("Digite o numero da peca que deseja jogar: ");

	if(scanf("%d", &numero) != 1)
	{
		while(getchar() != '\n');
		return -1; // entrada invalida (nao numerica)
	}

	int contador = 0;
	for(int i = 0; i < 28; i++)
	{
		if(pecas[i].status == jogador)
		{
			contador++;
			if(contador == numero)
				return i;
		}
	}
	return -1;
}

int escolherExtremidade()
{
	char lado;
	printf("Jogar na esquerda (E) ou direita (D)? ");
	scanf(" %c", &lado);
	return (lado == 'E' || lado == 'e') ? 0 : 1;
}
