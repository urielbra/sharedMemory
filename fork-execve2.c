#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define NUM_PROCESSOS 5


int conta(char* substring){
	int index; 
	int contador = 0;
	char search = (char)(substring[0] + 32);
	printf("Process: %d, My search Caracthers: %c\n",getpid(), search);

	for (index = 1 ; index < 101 ; index++){
		// printf("%d Analyzig Caracthers: %c\n",index, substring[index]);
		if (substring[index] == search) {
			printf("Found search %c on index: %d\n",search,index);
			contador++; 
		}
		// substring[index] = "*";
	}
	printf("I found %d occorrences of %c\n\n\n\n", contador, search);
	return contador;
}


int main (int argc, char *argv[], char *envp[])
{
	int retval,i;
	int shm_fd;
	void *ptr;
	const char *name = "shared_memory";
	const int SIZE = 4096;
	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the 
	address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}
	printf("%s\n",(char*)ptr);
	printf("Number of Caracthers: %d\n", strlen(ptr)  );
	printf("%d\n", *((int*)(ptr+strlen(ptr)+1)));

	printf ("Ola, sou o processo %5d\n", getpid()) ;
	//Cria NUM_PROCESSOS	
	ptr = ptr - 101 * sizeof(char); // Move pointer to next 100 chars
	for (i=0;i<NUM_PROCESSOS;i++){
		ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars 	
		retval = fork () ;
		if (retval == 0){ //se processo filho			
			// printf ("[retval: %5d] sou %5d, filho de %5d\n",
										// retval, getpid(), getppid()) ;
			break;
		}
	}
	if ( retval == 0 ) {
	printf("\n\nProcess executing: %d \n", getpid());

	/* now read from the shared memory region */
	// printf("%s\n",(char*)ptr);
	// printf("Number of Caracthers: %d\n", strlen(ptr)  );
	// printf("%d\n", *((int*)(ptr+strlen(ptr)+1)));
	
	ptr = ptr; // Read first 100 chars
	conta(ptr);
	}
	else {
		for (i=0;i<NUM_PROCESSOS;i++)
			printf("%d terminou\n",wait(0));
	}
	// printf ("Tchau de %5d!\n",getpid()) ;
	ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars 	
	exit (0) ;
}
