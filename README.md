🎲 Projeto Dominó (C/C++)

Um jogo de Dominó completo desenvolvido para rodar no terminal, utilizando a linguagem C/C++. O projeto foi estruturado utilizando o padrão de arquitetura MVC (Model-View-Controller) e conta com interface rica em ASCII Art e cores, além de um sistema de Inteligência Artificial para partidas contra o computador.

Data: 12/08/2026

✨ Funcionalidades

Modo 2 Jogadores (Local): Jogue contra um amigo no mesmo terminal.

Modo 1 Jogador vs Computador (IA):

Dificuldade Normal: A IA prioriza jogar peças altas e dobradas.

Dificuldade Difícil (IA Dedutiva): A IA memoriza quando você não tem peças para jogar em uma determinada ponta e prioriza jogadas que te bloqueiem.

Sistema de Save/Load: Precisou parar? Interrompa o jogo (tecla S) e salve seu progresso. Você pode carregar e continuar de onde parou mais tarde.

Placar e Estatísticas: O jogo mantém o rastreamento de vitórias do Jogador 1, Jogador 2/CPU e empates durante a execução.

Histórico de Jogadas (Log): Todas as ações da partida são gravadas em um arquivo de texto (domino_log.txt), permitindo consultar o histórico posteriormente.

Interface Visual Aprimorada: O terminal exibe as peças da mesa e da mão do jogador utilizando ASCII Art e um sistema de cores (utilizando códigos ANSI) para facilitar a visualização das extremidades.

📂 Estrutura do Código (MVC)

O projeto foi dividido logicamente seguindo o padrão MVC para facilitar a manutenção e leitura do código:

Dom_GHR_PROJETO.cpp: Arquivo principal (main). Agrupa as chamadas e inicia o sistema.

Dom_GHR_Model (.h / .cpp): Contém toda a lógica de negócios, regras do jogo, IA, embaralhamento, contagem de pontos e o sistema de leitura/escrita de arquivos.

Dom_GHR_View (.h / .cpp): Responsável pela interface do usuário. Contém os menus, leitura de inputs, desenhos ASCII e sistema de cores no terminal.

Dom_GHR_Controller (.h / .cpp): Faz a ponte entre a View e o Model. Gerencia o fluxo de turnos, os laços de repetição do menu e as decisões da partida.

🚀 Como Compilar e Executar

Este projeto foi construído de forma que os arquivos .cpp do Model, View e Controller são incluídos diretamente no arquivo principal (Dom_GHR_PROJETO.cpp).

Opção 1: Via IDE (Dev-C++, Code::Blocks, etc)

Basta abrir o arquivo Dom_GHR_PROJETO.cpp na sua IDE e mandar compilar e rodar (F11 no Dev-C++). A ordem dos includes presentes nele já resolve as dependências do código.

Opção 2: Via Terminal / Git Bash / Linux

Se você estiver utilizando um terminal bash ou o prompt de comando com o compilador GCC instalado (g++), navegue até a pasta do projeto e execute:

# Compilar o projeto (compile apenas o arquivo principal)
g++ Dom_GHR_PROJETO.cpp -o domino.exe

# Executar o jogo (Windows)
./domino.exe

# Executar o jogo (Linux/Mac)
./domino


Aviso de Sistema Operacional: O comando de limpeza de tela (system("cls")) implementado na camada View é nativo de ambientes Windows. Caso rode no Linux/Mac, o terminal pode não limpar adequadamente a menos que seja adaptado para system("clear").

📜 Regras Básicas do Jogo implementadas

Cada jogador inicia com 7 peças.

Inicia o jogo quem possuir o duplo 6 (6-6), ou a maior bucha/carroça em mãos.

No seu turno, você deve conectar o número de uma de suas peças ao número disponível em uma das extremidades da mesa (Esquerda ou Direita).

Se não houver jogada possível, o jogador deve Comprar peças do estoque (dormida) até conseguir uma jogada viável ou até o estoque esvaziar.

Vence a rodada quem bater (ficar sem peças) primeiro.

Em caso de jogo "trancado" (ninguém joga e não há o que comprar), vence quem tiver a menor soma de pontos nas peças restantes na mão.
