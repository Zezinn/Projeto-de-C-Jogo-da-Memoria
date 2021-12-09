/*
Projeto final P1 ADS Manh�. Di
Alunos: Gabriel Matias, Jos� Ricardo, Luiz Fernando, Daniel Luka e Sebasti�o Leal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <direct.h>

//Defini��o dos tipos de dados para do tabuleiro e do jogo.
typedef struct str_item
{
	char letra; //Letra que sera usada
	int vis;	//Se a letra esta oculta ou n�o
} item;

typedef struct str_jogo
{
	char nome[20];		   //Nome do player
	int jogadas;		   //Ranking do jogador (quantas jogadas foram realizadas)
	int modo;			   //Se optou por 4x4 ou 6x6
	struct str_jogo *prox; //Utiliza��o de ponteiro para inicializar a proxima partida
} partida;

//Declarando fun��es importantes
item **gera_tab(int x);

int menu(), escolha(int x, item ***tab);

partida *recebe_ranking();

void exibe_tab(int x, item **tab), exibe_ranking(partida *lista), libera_tab(int x, item **tab);

void edita_ranking(partida *lista, int x, int jogadas), libera_lista(partida *lista), creditos();

int main()
{
	setlocale(LC_ALL, "Portuguese");

	srand(time(NULL)); //Usar o tempo como uma indica��o de comando randomico para n�o repetir o mesmo game
	int n = 4, x;

	partida *lista; //Listagem do ranking
	printf("--- JOGO DA MEM�RIA ---\n\n");
	while (n)
	{
		n = menu(); // Exibir menu e retornar escolha do player

		switch (n)
		{
		case 1:
			printf("\nDeseja jogar num tabuleiro 4X4 OU 6X6? (DIGITE 4 OU 6):\n"
				   "Digite: ");
			scanf("%d", &x);
			//Se o player digitar errado
			while ((x != 4) && (x != 6))
			{
				printf("Informe o n�mero certo! (4 - 4X4, 6 - 6X6)\n"
					   "Digite: ");
				scanf("%d", &x);
			};

			item **tab = gera_tab(x); //Gerando tabuleiro do game por tamanho selecionado

			int res, pares = 0, jogadas = 0;

			exibe_tab(x, tab);
			//O programa recebe jogadas do player ate completar todo jogo
			while (pares < (x * x / 2))
			{
				//se receber valor 1 esta "correta", e 0 se for "errada"
				res = escolha(x, &tab);
				//O contador aumenta a cada jogada certa
				if (res)
				{
					pares++;
				};
				//As jogadas s�o contadas at� o final do jogo
				jogadas++;
			};

			exibe_tab(x, tab);

			printf("IHUUUUUUUU, voc� venceu no tabuleiro %dX%d! Total de tentativas: %d\n", x, x, jogadas);

			lista = recebe_ranking();

			edita_ranking(lista, x, jogadas);

			libera_tab(x, tab);
			break;

		case 2:
			lista = recebe_ranking();
			exibe_ranking(lista);
			break;

		case 3:
			creditos();
			break;

		case 0:
			printf("\nFim do programa.");
			return 0;
		}
	};
	return 0;
};

//Fun��o que recebe a escolha da jogada do player
int menu(){
	int op;
	printf("MENU GAME\n\n"
			"1 - NEW GAME\n"
			"2 - RANKING\n"
			"3 - CREDITS\n"
			"0 - EXIT GAME\n\n"
			"Digite: ");
	ERROOP:
	scanf("%d",&op);
	if((op==1)||(op==2)||(op==3)||(op==0)){
		return op;
	}
	else{
		printf("\nOpcao inv�lida!\n\n");
		sleep(1);
		printf("Nova op��o: ");
		goto ERROOP;
	};
};

//Fun��o que gera as letras no tabuleiro
item **gera_tab(int x)
{
	int i, j, k, tam_lista, r;
	//Gerando um vetor de letras
	tam_lista = (x * x);
	char *lista;
	lista = (char *)malloc(tam_lista * sizeof(char));
	if (!lista)
	{
		printf("ERRO DE LOC MEM�RIA");
		return 0;
	};
	switch (x)
	{
	case 4:
		strcpy(lista, "������@@<<==��$$");
		break;
	case 6:
		strcpy(lista, "������@@<<==��$$AABBCCDDEEFFGGHHIIJJ");
		break;
	default:
		printf("Erro, default problem, contate o DEV!\n");
		return;
	};
	//Gerando um novo tabuleiro
	item **tab;
	tab = (item **)malloc(x * sizeof(item *));
	if (!tab)
	{
		printf("ERRO DE LOC MEM�RIA\n");
		return 0;
	};
	//Gerando tudo que precisa do tabuleiro
	for (i = 0; i < x; i++)
	{
		tab[i] = (item *)malloc(x * sizeof(item));
		if (!(tab[i]))
		{
			printf("ERRO DE LOC MEM�RIA\n");
			return;
		};
		for (j = 0; j < x; j++)
		{
			r = rand() % tam_lista;		//Select em n�mero random, que corresponde a um espa�o do vetor
			tab[i][j].letra = lista[r]; //Transforma em item do tabuleiro
			tab[i][j].vis = 0;			//Deixa oculto de inicio do game
			for (k = r; k < tam_lista; k++)
			{
				lista[k] = lista[k + 1]; //Transforma cada item escolhido para uma uma posi��o e remove um item ja usado
			};
			tam_lista--; //Redu��o do tamanho de item
		};
	};
	return tab;
};

//Fun��o de exibi��o do tabuleiro
void exibe_tab(int x, item **tab)
{
	int i, j;
	system("cls");
	printf("*****************************\n\n-TABULEIRO-\n\n*****************************\n\n"
	"DE 1 A 4 LINHAS & DE 1 A 4 TRES COLUNAS || DE 1 A 6 LINHAS & DE 1 A 6 COLUNAS.\n\n");
	for (i = 0; i < x; i++)
	{
		for (j = 0; j < x; j++)
		{
			if (tab[i][j].vis)
			{ //Tratando erro se j� foi escolhido ou tratado
				printf("\t%c", tab[i][j].letra);
			}
			else
			{
				printf("\t-");
			};
		};
		printf("\n");
	};
	printf("\n");
};

//Fun��o que edita o tabuleiro
int escolha(int x, item ***tab)
{
	int x1, x2, y1, y2;
ERROXY1:
	printf("\nDigite a primeira coodernada: ");
	scanf("%d%d", &x1, &y1);
	//Caso o player digite coordenadas fora do tabuleiro
	if ((1 > x1) || (x1 > x) || (1 > y1) || (y1 > x))
	{
		printf("Coordenadas INEXISTENTES, tente novamente!\n");
		goto ERROXY1;
	};
	x1--;
	y1--;
	//Caso o player digite uma item j� pareado
	if ((*tab)[x1][y1].vis == 1)
	{
		printf("Esse item j� foi pareado, DIGITE OUTRA COORDENADA!\n");
		goto ERROXY1;
	};
	(*tab)[x1][y1].vis = 1;
	exibe_tab(x, *tab);
ERROXY2:
	printf("\nDigite a segunda coodernada: ");
	scanf("%d%d", &x2, &y2);
	//Caso o player digite coordenadas fora do tabuleiro
	if ((1 > x2) || (x2 > x) || (1 > y2) || (y2 > x))
	{
		printf("Coordenadas INEXISTENTES, tente novamente!\n");
		goto ERROXY2;
	};
	x2--;
	y2--;
	//Caso o digite o item anterior
	if ((x1 == x2) && (y1 == y2))
	{
		printf("Você precisa escolher dois itens diferentes!\n");
		goto ERROXY2;
	};
	//Caso o player escolha um item j� conclu�do
	if ((*tab)[x2][y2].vis == 1)
	{
		printf("Esse item j� foi pareado, DIGITE OUTRA COORDENADA!\n");
		goto ERROXY2;
	};
	(*tab)[x2][y2].vis = 1;
	exibe_tab(x, *tab);
	if ((*tab)[x1][y1].letra != (*tab)[x2][y2].letra)
	{
		printf("ITENS DIFERENTES, PAR n�o foi fechado!\n");
		//Retornando para ocultas
		(*tab)[x1][y1].vis = 0;
		(*tab)[x2][y2].vis = 0;
		return 0; //Errou o par
	}
	else
	{
		printf("IHUUUUUU. Par formado!\n");
		return 1; //Acertou
	};
};

//Fun��o para liberar a memoria usada
void libera_tab(int x, item **tab)
{
	int i;
	//Libera a memoria em cada col (arrays de inteiros)
	for (i = 0; i < x; i++)
	{
		free(tab[i]);
	};
	//Libera a memoria das rows (ponteiros)
	free(tab);
};

//Fun��o leitura de arquivo txt
partida *recebe_ranking()
{
	FILE *arquivo = fopen("RankingDoDota2.txt", "r");
	//Verifica abertura do arquivo:
	if (!arquivo)
	{
		printf("Erro!!!!!!!!! N�o conseguir ler o arquivo.\n");
		return 0;
	};
	//Start no arquivo
	partida *lista = (partida *)malloc(sizeof(partida));
	//Verifica aloc de memoria
	if (!lista)
	{
		printf("ERRO DE LOC MEM�RIA\n");
		return 0;
	};
	//Vari�vel de leitura de linha:
	char linha[40];
	//Primeiro item da linha
	fgets(linha, 40, arquivo);
	//Se n�o tiver nenhum registro no arquivo ele avisar.
	if (feof(arquivo))
	{
		printf("Ranking vazio igual minha conta banc�ria... Quer come�ar essa aventura?\n");
		return 0;
	};
	//Antes da virgula vem o nome do jogador
	char *nome;
	nome = strtok(linha, ",");
	//Salvando no primeiro element da lista
	strcpy(lista->nome, nome);

	//Depois da primeira virgula, vem o tipo do game, 4X4, 6X6
	char *modo_string = strtok(NULL, ",");
	//Transforma para INT e SAVE na lista txt:
	lista->modo = atoi(modo_string);

	//Depois da segunda virgula, vem a pontua��o do game
	char *jogadas_string = strtok(NULL, ",");
	//Transforma para INT e SAVE na lista txt:
	lista->jogadas = atoi(jogadas_string);

	lista->prox = NULL;

	//PROXIMOS PLAYERS
	fgets(linha, 60, arquivo);
	//Salvando o ponteiro anterior para iterar:
	partida *aux = lista;

	//Enquanto n�o for o final do txt
	while (!feof(arquivo))
	{
		//Aloc memoria para um nova jogada a ser lida:
		partida *novo = (partida *)malloc(sizeof(partida));
		//Verifica aloc
		if (!novo)
		{
			printf("ERRO DE LOC MEM�RIA\n");
			return 0;
		};

		//Copiando a o vetor de char lida antes da virgula:
		strcpy(novo->nome, strtok(linha, ","));
		//Capturando as strings depois das virgulas, transformando-as para inteiros e salvando no novo elemento da lista
		novo->modo = atoi(strtok(NULL, ","));
		novo->jogadas = atoi(strtok(NULL, ","));
		//Este � um novo elemento, depois dele n�o existe mais nenhum:
		novo->prox = 0;
		//O anterior que guardava o fim da fila, agora vai apontar para o novo elemento criado:
		aux->prox = novo;
		//Agora o novo elemento criado � o anterior, pois quando iterar sera gerado um novo elemento
		aux = novo;
		//Antes de iterar, deve-se ler a prox linha
		fgets(linha, 40, arquivo);
	}
	//CLOSE arquivo:
	fclose(arquivo);
	return lista;
};

//Fun��o para liberar a memoria
void libera_lista(partida *lista)
{
	//Uma var auxiliar � necessaria para liberar da original
	partida *aux;
	while (lista)
	{
		aux = lista;
		lista = lista->prox;
		free(aux);
	};
};

//fun��o para exibir a lista
void exibe_ranking(partida *lista)
{
	system("cls");
	int i, j, tam4, tam6;
	partida *aux = lista, *rank4, *rank6;
	//Iterar lista e contar quantas partida de cada tipo s�o existentes:
	for (tam4 = 0, tam6 = 0; aux; aux = aux->prox)
	{
		switch (aux->modo)
		{
		case 4:
			tam4++;
			break;
		case 6:
			tam6++;
			break;
		}
	};
	//Aloc a memoria para os dois tipos de ranking
	if (tam4)
	{
		rank4 = (partida *)malloc(tam4 * sizeof(partida));
		if (!rank4)
		{
			printf("ERRO DE LOC MEM�RIA\n");
			return;
		};
	};
	if (tam6)
	{
		rank6 = (partida *)malloc(tam6 * sizeof(partida));
		if (!rank6)
		{
			printf("ERRO DE LOC MEM�RIA\n");
			return;
		};
	};
	//� necessario separar as partidas em rela��o ao modo
	aux = lista; //� preciso voltar para o inicio da lista
	for (i = 0, j = 0; aux; aux = aux->prox)
	{
		switch (aux->modo)
		{
		case 4:
			strcpy(rank4[i].nome, aux->nome);
			rank4[i].jogadas = aux->jogadas;
			i++;
			break;
		case 6:
			strcpy(rank6[j].nome, aux->nome);
			rank6[j].jogadas = aux->jogadas;
			j++;
			break;
		default:
			return;
		}
	};
	//Reordenando todos elementos das duas listas
	/*Cada elemento � comparado com os anteriores.
	Caso encontre algum +, eles mudam de posi��o
	com a aux de um espace auxiliar*/

	partida aux_rank;
	for (i = 0; i < tam4; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (rank4[j].jogadas > rank4[i].jogadas)
			{
				aux_rank = rank4[j];
				rank4[j] = rank4[i];
				rank4[i] = aux_rank;
			};
		};
	};
	for (i = 0; i < tam6; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (rank6[j].jogadas > rank6[i].jogadas)
			{
				aux_rank = rank6[j];
				rank6[j] = rank6[i];
				rank6[i] = aux_rank;
			};
		};
	};
	//Para as partidas 4X4
	printf("RANKING ----------- MODO 4X4:\n\n");
	for (i = 0; i < tam4; i++)
	{
		printf("%d) %s: %d Tentativas\n", (i + 1), rank4[i].nome, rank4[i].jogadas);
	};
	//Para as partidas 6x6
	printf("\n\nRANKING ----------- MODO 6X6:\n\n");
	for (j = 0; j < tam6; j++)
	{
		printf("%d) %s: %d Tentativas\n", (j + 1), rank6[j].nome, rank6[j].jogadas);
	};
	free(rank4);
	free(rank6);
	libera_lista(lista);
	sleep(1);
	printf("\n(Pressione qualquer tecla para retornar ao MENU INICIAL!)\n");
	getch();
	system("cls");
	return;
};

//Fun��o que adiciona novos jogadores ao ranking, ou altera pontua��o de jogadores que ja s�o cadastrados
void edita_ranking(partida *lista, int x, int jogadas)
{
	char nome[20]; //Nome que vai ser recebido
	int i = 0;	   //Se uma partida desse modo j� foi ganha por esse player

	//Necess�rio saber se o nome do jogador j� ta no ranking
	printf("Informe seu nome (TAMANHO M�XIMO � 20 caracteres): ");
	scanf("%20s", &nome);
	//Para iterar na lista:
	partida *aux = lista;
	//OPEN arquivo
	FILE *arquivo = fopen("RankingDoDota2.txt", "w+");
	if (!arquivo)
	{
		printf("Erro ao abrir o arquivo.\n");
		return;
	};
	//Varre toda lista de partidas e reescreve o arquivo
	while (aux)
	{
		//Se encontrar nome e modo iguais com mais jogadas (pontua��o menor)
		if ((!strcmp(aux->nome, nome)) && (aux->modo == x) && (aux->jogadas >= jogadas))
		{
			//Reescreve no arquivo com o numero atualizado (TOP) de jogadas
			aux->jogadas = jogadas;
			fprintf(arquivo, "%s,%d,%d\n", aux->nome, aux->modo, aux->jogadas);
			i++;
		}
		//Caso n�o, reescreve a partida
		else
		{
			fprintf(arquivo, "%s,%d,%d\n", aux->nome, aux->modo, aux->jogadas);
		};
		//Vai para o prox partida da lista
		aux = aux->prox;
	};
	//Caso n�o tenha nenhum jogador no ranking com o mesmo nome e no mesmo modo de jogo, adicionar um novo arquivo
	if (!i)
	{
		fprintf(arquivo, "%s,%d,%d\n", nome, x, jogadas);
		fclose(arquivo);
		libera_lista(lista);
		printf("PONTUA��O gerada e adicionada com sucesso!\n\n");
		sleep(1);
		printf("(Pressione qualquer tecla para retornar ao MENU INICIAL!)\n");
		getch();
		system("cls");
		return;
	}
	else
	{
		fclose(arquivo);
		libera_lista(lista);
		printf("PONTUA��O gerada e adicionada com sucesso!\n\n");
		sleep(1);
		printf("(Pressione qualquer tecla para retornar ao MENU INICIAL!)");
		getch();
		system("cls");
		return;
	};
};

//FUN��O dos creditos
void creditos()
{
	system("cls");
	printf("*****************************\n-CREDITOS FINAIS-\n*****************************\n"
		   "\nMEMORY GAME(JOGO DA MEM�RIA) EM C.\n\n"
		   "Analise e Desenvolvimento de Sistemas | P1 Manh�\n"
		   "T�cnicas de Desenvolvimento de Algoritmos\n"
		   "Professor Leonardo DOTEIRO e NTFZEIRO\n"
		   "EQUIPE PRESS�O CODER\n\n");
	sleep(1);
	printf("(Pressione qualquer tecla para retornar ao MENU INICIAL!)\n");
	getch();
	system("cls");
	return;
}
