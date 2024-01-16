# README

## EP de Introdução a Análise de Algoritmos I

Este projeto consiste em um sistema de busca e indexicação de palavras em um arquivo de texto usando estruturas de dados como lista encadeada e árvore binária de busca.

### Compilação

Certifique-se de ter um compilador C instalado no seu sistema, como GCC.

1. Abra o terminal.

2. Navegue até o diretório onde os arquivos fonte estão localizados.

3. Compile o programa usando o seguinte comando:
   gcc ep.c -o ep
		 
4. Ainda no terminal, execute o arquivo compilado com o seguinte comando:
   ./ep <nome_do_arquivo> <indice_escolhido>
	exemplo:
	 ./ep the-little-prince.txt lista
	 ou 
	 ./ep the-hunger-games-short.txt arvore


5. Executados os comandos corretamente, o programa irá rodar permitindo com que o usuário forneça palavras que deseja buscar no arquivo, comando que deve ser executado da seguinte forma:
	busca <palavra_a_ser_buscada>
	onde:
	 <palavra_a_ser_buscada> deve ser substituido pela palavra que o usuário deseja encontrar no arquivo.

6. A cada execução, o programa irá fornecer o usuário a continuidade de busca por novas palavras. Caso o usuário digite qualquer comando diferente de "busca", o programa informará que está tentando fazer uma interação que é inválida.

7. Caso o usuário tenha o desejo de encerrar as buscas, deve digitar o comando "fim" e o programa encerrará.