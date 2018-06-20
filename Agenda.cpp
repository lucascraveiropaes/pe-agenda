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

// Função que converte uma string inteira para lower case
char *toLower(char* source) {
    char* i = source;
	while(*i != 0) {
		*i = tolower(*i);
    	i++;
	}
	*i = 0;
}

// Função que limpa a string removendo todos os espaços
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

// Função para exibir o menu
char menu() {
	char op = '0';
	
	printf("****** Agenda ******\n");
	printf("\nSelecione uma opção: \n\n");
	printf("1 --------- Inserir um novo contato \n");
	printf("2 --------- Remover um contato pelo nome \n");
	printf("3 --------- Pesquisar um contato pelo nome \n");
	printf("4 --------- Listar todos os contatos \n");
	printf("5 --------- Listar aniversariantes do mês \n");
	printf("6 --------- Sair da execução do algoritmo \n");
	
	printf("\nSua opção: ");
	scanf("%c", &op);
	
	return op;	
}

// Função para pesquisar pelo nome na agenda
int getContactByName() {
	char query[NAME_MAX], fName[NAME_MAX], fNameLowerCase[NAME_MAX], fPhone[PHONE_MAX], fBirthday[BIRTH_MAX], fBirthmonth[BIRTH_MAX], line[MAX];
	bool found = false; // Variável de controle para saber se foi encontrado um resultado compatível no arquivo ou não
	FILE *fptr;
	
	// Input do nome para a pesquisa	
	printf("\nDigite o nome a ser pesquisado: ");
	gets(query);
	trim(query); // Remove os espaços do nome inserido
	toLower(query); // Tranforma em lower case o nome para usar na comparação
	
	fptr = fopen(FILENAME, "r"); // Abre arquivo para leitura
	
	if(fptr == NULL) { // Mensagem de erro caso não consiga abrir o arquivo
		printf("\nNão foi possível efetuar a pesquisa.");
		return 0;
	}
	
	while(fgets(line, MAX, fptr) != NULL) {
		sscanf(line, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth); // Pega os dados da linha do arquivo
		strcpy(fNameLowerCase, fName); // Faz uma cópia do nome para ser tratado e usado na comparação com a query
		toLower(fNameLowerCase); // Tranforma em lower case o nome do arquivo para usar na comparação
				
		if(strcmp(fNameLowerCase, query) == 0) { // Compara o nome da linha atual do arquivo com o nome digitado pelo user
			printf("\nContato: %s\n", fName);
			printf("Telefone: %s\n", fPhone);
			printf("Aniversário: %s / %s", fBirthday, fBirthmonth);
			
			found = true;
			break;
		}	
	}
	
	if( !found ) //  Se não encontrar um resultado, exibe mensagem de erro
		printf("Não existe um contato com esse nome.");	
	
	fclose(fptr);
}

// Função para listar todos os nomes do arquivo
int showDataFromFile() {
	char name[NAME_MAX], phone[PHONE_MAX], birthday[BIRTH_MAX], birthmonth[BIRTH_MAX], line[MAX];
	int i=0;
	FILE *fptr = fopen(FILENAME, "rt");
	
	if(fptr == NULL) {
		printf("Não foi possível listar os contatos agenda.");
		return 0;
	}
	
	while(fgets(line, MAX, fptr) != NULL) {
		i++;
		sscanf(line, "%s %s %s %s\n", name, phone, birthday, birthmonth); // Pega os dados da linha do arquivo
		
		// Imprime os dados na tela
		printf("%d) Contato: %s\n", i, name);
		printf("   Telefone: %s\n", phone);
		printf("   Aniversário: %s / %s\n\n", birthday, birthmonth);
	}
	
	fclose(fptr);
}

// Função para pegar os dados de um novo contato
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
	
	printf("\nDia do aniversário: ");
	gets(birthday);
	trim(birthday);
	
	printf("\nMês do aniversário: ");
	gets(birthmonth);
	trim(birthmonth);
	
	
	FILE *fptr;
	fptr = fopen(FILENAME,"a+"); // Abre para escrita adicionando ao final do arquivo
	
	if(fptr == NULL) { // Mensagem de erro caso não consiga abrir o arquivo
		printf("Não foi possível salvar esse contato na agenda.");
		return 0;
	}	
	
	fprintf(fptr,"%s %s %s %s\n", name, phone, birthday, birthmonth); // Salva o conteúdo enviado no parâmetro no final do arquivo
	fclose(fptr);
	
	printf("Contato Salvo."); // Mensagem caso tudo dê certo
}

int deleteContact() {
	char query[NAME_MAX], fName[NAME_MAX], fNameLowerCase[NAME_MAX], fPhone[PHONE_MAX], fBirthday[BIRTH_MAX], fBirthmonth[BIRTH_MAX], line[MAX];	
	FILE *fptr = fopen(FILENAME, "a+");
	FILE *fptr2 = fopen("agenda2.txt","a+");
	
	// Input do nome para a pesquisa	
	printf("Digite o nome do contato a ser removido: ");
	gets(query);
	
	if(fptr == NULL || fptr2 == NULL) {
		printf("\nNão foi possível deletar o contato %s", query);
		return 0;
	}
	
	trim(query); // Remove os espaços do nome inserido
	toLower(query); // Tranforma em lower case o nome para usar na comparação
	
	// Todos os contatos que forem diferentes do contato inserido serão copiados para um arquivo auxiliar	
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
	bool hasBirthmonth = false;  // Variável para controlar se existe aniversariantes no mês
	FILE *fptr  = fopen(FILENAME, "r"); // Abre arquivo para leitura
	
	if(fptr == NULL) { // Mensagem de erro caso não consiga abrir o arquivo
		printf("\nNão foi possível listar aniversariantes.");
		return 0;
	}
		
	while(fgets(line, MAX, fptr) != NULL) {
		sscanf(line, "%s %s %s %s\n", fName, fPhone, fBirthday, fBirthmonth); // Pega os dados da linha do arquivo
				
		if(atoi(fBirthmonth) == getMonth()) { // Compara o nome da linha atual do arquivo com o nome digitado pelo user
			printf("\nContato: %s\n", fName);
			printf("Telefone: %s\n", fPhone);
			printf("Aniversário: %s / %s", fBirthday, fBirthmonth);
			
			hasBirthmonth = true; // Tranformando variável pra true se existe aniversariantes nesse mês
		}	
	}
	
	if( !hasBirthmonth ) { // Mensagem caso não exista aniversariantes
		printf("\nNão existem aniversariantes esse mês.");
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
				printf("Opção Inválida.");
				break;
			}
		}
		
		printf("\n\nPressione qualquer tecla para voltar ao menu...");
	
		getch();
		system("cls");
	}
	
	getch();
}
