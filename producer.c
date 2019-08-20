//gcc -o producer shm-posix-producer.c -lrt

/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define PASSO 210
#define NUM_PROCESSOS 5

int main()
{
	const int SIZE = 4096;
	const char *name = "shared_memory";

	int shm_fd;
	char *ptr, *start;
	srand(time(0));

	/* Cria um segmento de memória compartilhado 
	e retorna um descritor de arquivo. Este
	arquivo não existe fisicamente em disco,
	ele é apenas um arquivo virtual criado em
	em um sistema de arquivos temporário (tmpfs)
	dentro de /dev/shm/xxx. */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/*shm_open: cria ou abre um objeto de
	memória compartilhado.

	/* O tamanho inicial de um segmento é de 0 bytes.
	A função "ftruncate" define o tamanho do
	segmento.*/
	ftruncate(shm_fd,SIZE);

	/* Mapeia o segmento de memória para o espaço
	 de endereçamento do processo. Dessa forma, o
	segmento poderá ser acessado por meio de um
	ponteiro. 

	/* O primeiro parâmetro "0" indica que o kernel
	escolhe o endereço no qual o mapeamento será 
	criado. Valores diferentes de "0" são utilizados
	como sugestões para o Kernel.
	O último parâmetro "0" indica o ponto de início
	do mapeamento no arquivo.
	No caso abaixo, o mapeamento contém SIZE bytes
	iniciando em 0.*/
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	/*Depois que o segmento de memória foi
	mapeado para o espaço de endereçamento
	do processo, o arquivo ("shm_fd") pode
	ser fechado.*/

	/*Escreve na memória compartilhada.*/
	// start = ptr;
	// sprintf(ptr,"Hello: ");
	// ptr += 7; //Move o ponteiro
	char c;
	int contador = 0;
	int choice;
	printf("O que deseja fazer? \n");
	printf("1.Preencher a Memória? \n");
	printf("2.Ler na memória \n");
	scanf("%d",&choice);
	if(choice==1){
		for (contador = 0; contador < NUM_PROCESSOS*PASSO; contador++,ptr++) {
		if(contador%PASSO==0){
			*ptr = 'A' + rand()%26;
        	printf("%d|Char to read: %c\n" ,contador, *ptr);
		} else {
        	*ptr = 'a' + rand()%26;
        	printf("%d|Saving on memory: %c\n" ,contador, *ptr);			
		}
    }   	
	*ptr = '\0';
	ptr++;
	} else {
		 printf("%s\n",(char*)ptr);
	}

	//*ptr=10;
	
	//memcpy(ptr,"conteudo",tamanho);
	return 0;
}
