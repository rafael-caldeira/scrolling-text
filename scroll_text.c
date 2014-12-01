

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void simple_print(char *str[],int l)	{
	int i;
	for(i=0; i<l; i++)
		printf("%d:\n %s\n",i,str[i]);
}


/**
 * Recebe um array de strings com l linhas e cria um scrolling text com uma janela com largura 'janela'
 */
void scroll_text(char *_str[], const int l, int janela)	{
	int i,j;
	int head;
	int n = strlen(_str[0]);
	char *str[l];
	
	for(i=0; i<l; i++)
		str[i] = (char *) malloc((janela+n+3)*sizeof(char)); // +3 espaco inicial, final '\0'

	// Adicionar espacos nas strings
	for(i=0; i<l; i++)	{
		for(j=0; j<=janela; j++)
			str[i][j] = ' ';
		for(j=0; j<n; j++)
			str[i][janela+1+j] = _str[i][j];
		str[i][janela+1+n]=' ';
		str[i][janela+2+n]='\0';
	}

	// Imprimir fazendo scrolling
	n= strlen(str[0]);
	for(head=0; head<n; head++)	{ // nao imprimir os head primeiros caracteres
		printf("\x1b[0m\x1b[2J"); // limpa definicoes anteriores e reposiciona cursor
		for(i=0; i<l; i++)	{
			printf("\x1b[47m");		// muda cor do texto
			printf("\x1b[1;31m");	// muda cor do fundo
			printf("%.*s",janela,str[i]+head);
			printf("\x1b[0m\n");
		}
		usleep(40000);
	}

	for(i=0; i<l; i++)
		free(str[i]);
}

/**
 * Le de ficheiro com o caminho path uma sequencia de linhas com o mesmo numero de caracteres e
 * devolve o resultado num array de strings definindo o numero de linhas em n
 * 
 * Só serão lidas as primeiras 100 linhas do ficheiro, se existirem
 */
char ** ler_ascii(char *path, int *n)	{

	FILE *f = fopen(path,"r");
	char *line=NULL;
	int i;
	size_t len,plen=0;
	ssize_t read;
	char *linhas[100];

	if(!f)	{
		printf("ERRO: Impossivel abrir o ficheiro %s para leitura\n",path);
		return NULL; // Se nao consegue abrir o ficheiro
	}

	// le as linhas e copia para array linhas
	for(i=0; ((read = getline(&line, &len, f)) != -1 ) && i<100; i++)	{
		
		if(strlen(line)==1)	{
			free(line);
			break;
		} else if(plen!=0 && len!=plen)	{ // verifica se o conteudo tem o mesmo nr de caracteres
			printf("ERRO: A linhas %d tem cumprimento %zu e a linha %d tem cumprimento %zu\n.",i,len,i-1,plen);
			printf("%d(%.4zu):%s\n",i,strlen(line),line);
			return NULL;
		}

		// Apagar o \n do final da linha
		int j;
		for(j=0; j<(int)len; j++)
			if(line[j]=='\n') {
				line[j]='\0';
			}

		linhas[i] = line;	
		
		plen=len;
		line = NULL;
	}
	
	*n=i;
	char **dest = (char **) malloc(*n * sizeof(void *));
	for(i=0; i<*n; i++)
		dest[i] = linhas[i];

	return dest;
}

int main()	{

	int nawesome=0,nsite,nshare;
	char **awesome,**site,**share;
	
	awesome = ler_ascii("awesome.txt",&nawesome);
	site = ler_ascii("site.txt",&nsite);
	share  = ler_ascii("share.txt",&nshare);

	scroll_text(awesome,nawesome,110);
	scroll_text(site,nsite,110);
	while(1)
		scroll_text(share,nshare,110);

	return 0;
}                              
