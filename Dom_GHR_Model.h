//Dom_GHR_Model.h - Projeto Domino
//12/08/2026
//Gustavo Iwagoe, Henrique Oliveira, Rafael Taffo

#ifndef DOM_GHR_MODEL_H
#define DOM_GHR_MODEL_H

#define DISPONIVEL 'D'
#define JOGADOR1   '1'
#define JOGADOR2   '2'
#define MESA       'M'
#define EMPATE     'E'

typedef struct
{
	int lado1;
	int lado2;
	char status; 
} Peca;

typedef struct
{
	Peca pecasJogadas[28];
	int quantidade; 
} Mesa;

extern Peca pecas[28];
extern Mesa mesa;

extern int modoContraComputador;
extern char jogadorDaVez;
extern int jogoInterrompido;

extern int dificuldadeIA;
extern int jogador1NaoTem[7];

extern int vitoriasJ1;
extern int vitoriasJ2;
extern int empatesPlacar;

void criarPecas(Peca pecas[]);
void embaralharPecas(Peca pecas[]);

void distribuirPecas(Peca pecas[]);
char definirPrimeiroJogador(Peca pecas[], Mesa *mesa);

void criarMesa(Mesa *mesa);
int extremidadeEsquerda(Mesa mesa);
int extremidadeDireita(Mesa mesa);

int jogadaValida(Peca peca, int extremidade);
int jogarPeca(Mesa *mesa, Peca *peca, int ladoEscolhido); 

int comprarPeca(Peca pecas[], char jogador);
int jogadorTemJogada(Peca pecas[], char jogador, Mesa mesa);

int contarPecas(Peca pecas[], char status);
int somaPontos(Peca pecas[], char jogador);

int jogoTerminou(Peca pecas[], Mesa mesa);
char definirVencedor(Peca pecas[]);

void iniciarMemoriaIA();
void registrarFaltaPecaJogador(int esq, int dir);
void jogadaComputador(Peca pecas[], Mesa *mesa);
void apresentarEstatisticasFinais(Peca pecas[]);
int salvarJogoEmArquivo(const char* nomeArquivo);
int carregarJogoDeArquivo(const char* nomeArquivo);

#endif
