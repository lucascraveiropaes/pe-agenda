#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <time.h>

#define MAX 200
#define NAME_MAX 150
#define PHONE_MAX 20
#define BIRTH_MAX 10

const char *FILENAME = "Agenda.txt";

// Fun��o que converte uma string inteira para lower case
char *toLower(char* source) {
    char* i = source;
	while(*i != 0) {
		*i = tolower(*i);
    	i++;
	}
	*i = 0;
}

// Fun��o que limpa a string removendo todos os espa�os
void trim(char* source) {
    char* i = source;
	char* j = source;
	while(*j != 0) {
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;
}

// Fun��o para exibir o menu
char menu() {
	char op = '0';
	
	printf("****** Agenda ******\n");
	printf("\nSelecione uma op��o: \n\n");
	printf("1 --------- Inserir um novo contato \n");
	printf("2 --------- Remover um contato pelo nome \n");
	printf("3 --------- Pesquisar um contato pelo nome \n");
	printf("4 --------- Listar todos os contatos \n");
	printf("5 --------- Listar aniversariantes do m�s \n");
	printf("6 --------- Sair da execu��o do algoritmo \n");
	
	printf("\nSua op��o: ");
	scanf("%c", &op);
	
	return op;	
}

// Fun��o para pesquisar pelo nome na agenda
int getContactByName() {
	char query[NAME_MAX], fName[NAME_MAX], fNameLowerCase[NAME_MAX], fPhone[PHONE_MAX], fBirthday[BIRTH_MAX], fBirthmonth[BIRTH_MAX], line[MAX];
	bool found = false; // Vari�vel de controle para saber se foi encontrado um resultado compat�vel no arquivo ou n�o
	FILE *fptr;
	
	// Input do nome para a pesquisa	
	printf("\nDigite o nome a ser pesquisado: ");
	gets(query);
	trim(query); // Remove os espa�os do nome inserido
	toLower(query); // Tranforma em lower case o nome para usar na compara��o
	
	fptr = fopen(FILENAME, "r"); // Abre arquivo para leitura
	
	if(fptr == NULL) { // Mensagem de erro caso n�o consiga abrir o arquivo
		printf("\nN�o foi poss�vel efetuar a pesquisa.");
		return 0;
	}
	
	while(fgets(line, MAX, fptr) != NULL) {
		sscanf(line, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth); // Pega os dados da linha do arquivo
		strcpy(fNameLowerCase, fName); // Faz uma c�pia do nome para ser tratado e usado na compara��o com a query
		toLower(fNameLowerCase); // Tranforma em lower case o nome do arquivo para usar na compara��o
				
		if(strcmp(fNameLowerCase, query) == 0) { // Compara o nome da linha atual do arquivo com o nome digitado pelo user
			printf("\nContato: %s\n", fName);
			printf("Telefone: %s\n", fPhone);
			printf("Anivers�rio: %s / %s", fBirthday, fBirthmonth);
			
			found = true;
			break;
		}	
	}
	
	if( !found ) //  Se n�o encontrar um resultado, exibe mensagem de erro
		printf("N�o existe um contato com esse nome.");	
	
	fclose(fptr);
}

// Fun��o para listar todos os nomes do arquivo
int showDataFromFile() {
	char name[NAME_MAX], phone[PHONE_MAX], birthday[BIRTH_MAX], birthmonth[BIRTH_MAX], line[MAX];
	int i=0;
	FILE *fptr = fopen(FILENAME, "rt");
	
	if(fptr == NULL) {
		printf("N�o foi poss�vel listar os contatos agenda.");
		return 0;
	}
	
	while(fgets(line, MAX, fptr) != NULL) {
		i++;
		sscanf(line, "%s %s %s %s\n", name, phone, birthday, birthmonth); // Pega os dados da linha do arquivo
		
		// Imprime os dados na tela
		printf("%d) Contato: %s\n", i, name);
		printf("   Telefone: %s\n", phone);
		printf("   Anivers�rio: %s / %s\n\n", birthday, birthmonth);
	}
	
	fclose(fptr);
}

// Fun��o para pegar os dados de um novo contato
// Return: retorna a string formatada para salvar no arquivo
char *getDataFromUser() {
	char name[NAME_MAX], phone[PHONE_MAX], birthday[BIRTH_MAX], birthmonth[BIRTH_MAX], resp[MAX];
	resp[0] = '\0';
	
	system("cls");
	
	printf("Nome: ");
	gets(name);
	trim(name);
	
	printf("\nTelefone: ");
	gets(phone);
	trim(phone);
	
	printf("\nDia do anivers�rio: ");
	gets(birthday);
	trim(birthday);
	
	printf("\nM�s do anivers�rio: ");
	gets(birthmonth);
	trim(birthmonth);
	
	
	FILE *fptr;
	fptr = fopen(FILENAME,"a+"); // Abre para escrita adicionando ao final do arquivo
	
	if(fptr == NULL) { // Mensagem de erro caso n�o consiga abrir o arquivo
		printf("N�o foi poss�vel salvar esse contato na agenda.");
		return 0;
	}	
	
	fprintf(fptr,"%s %s %s %s\n", name, phone, birthday, birthmonth); // Salva o conte�do enviado no par�metro no final do arquivo
	fclose(fptr);
	
	printf("Contato Salvo."); // Mensagem caso tudo d� certo
}

int deleteContact() {
	char query[NAME_MAX], fName[NAME_MAX], fNameLowerCase[NAME_MAX], fPhone[PHONE_MAX], fBirthday[BIRTH_MAX], fBirthmonth[BIRTH_MAX], line[MAX];	
	FILE *fptr = fopen(FILENAME, "a+");
	FILE *fptr2 = fopen("agenda2.txt","a+");
	
	// Input do nome para a pesquisa	
	printf("Digite o nome do contato a ser removido: ");
	gets(query);
	
	if(fptr == NULL || fptr2 == NULL) {
		printf("\nN�o foi poss�vel deletar o contato %s", query);
		return 0;
	}
	
	trim(query); // Remove os espa�os do nome inserido
	toLower(query); // Tranforma em lower case o nome para usar na compara��o
	
	// Todos os contatos que forem diferentes do contato inserido ser�o copiados para um arquivo auxiliar	
	while(fgets(line, MAX, fptr) != NULL) {	
		sscanf(line, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth);
		strcpy(fNameLowerCase, fName);
		toLower(fNameLowerCase);
		
		if(strcmp(fNameLowerCase, query) != 0) {
			fprintf(fptr2, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth);
		} else {
			printf("\nContato %s deletado com sucesso!", fName);
		}
	}
	
	fclose(fptr);
	fclose(fptr2);
	
	// Deleta arquivo principal, e renomeia arquivo auxiliar com o mesmo nome do principal
	remove(FILENAME);
	rename("agenda2.txt", FILENAME);
}

int getMonth() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	return tm.tm_mon+1;
}

int monthBirthDays() {
	char fName[NAME_MAX], fNameLowerCase[NAME_MAX], fPhone[PHONE_MAX], fBirthday[BIRTH_MAX], fBirthmonth[BIRTH_MAX], line[MAX];
	bool hasBirthmonth = false;  // Vari�vel para controlar se existe aniversariantes no m�s
	FILE *fptr  = fopen(FILENAME, "r"); // Abre arquivo para leitura
	
	if(fptr == NULL) { // Mensagem de erro caso n�o consiga abrir o arquivo
		printf("\nN�o foi poss�vel listar aniversariantes.");
		return 0;
	}
		
	while(fgets(line, MAX, fptr) != NULL) {
		sscanf(line, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth); // Pega os dados da linha do arquivo
				
		if(atoi(fBirthmonth) == getMonth()) { // Compara o nome da linha atual do arquivo com o nome digitado pelo user
			printf("\nContato: %s\n", fName);
			printf("Telefone: %s\n", fPhone);
			printf("Anivers�rio: %s / %s", fBirthday, fBirthmonth);
			
			hasBirthmonth = true; // Tranformando vari�vel pra true se existe aniversariantes nesse m�s
		}	
	}
	
	if( !hasBirthmonth ) { // Mensagem caso n�o exista aniversariantes
		printf("\nN�o existem aniversariantes esse m�s.");
	}
	
	fclose(fptr);
}


main() {
	setlocale(LC_ALL, "Portuguese");
	
	bool exit = false;
	
	while(!exit) {
		char option = menu();
		
		system("cls");
		fflush(stdin);
		
		switch(option) {
			case '1': {
				getDataFromUser();				
				break;
			}
			case '2': {
				deleteContact();
				break;
			}
			case '3': {
				getContactByName();
				break;
			}
			case '4': {
				showDataFromFile();
				break;
			}
			case '5': {
				monthBirthDays();
				break;
			}
			case '6': {
				exit = true;
				return 0;
				break;
			}
			default: {
				printf("Op��o Inv�lida.");
				break;
			}
		}
		
		printf("\n\nPressione qualquer tecla para voltar ao menu...");
	
		getch();
		system("cls");
	}
	
	getch();
}
