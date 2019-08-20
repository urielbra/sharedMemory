/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int conta(char* substring){
	int index; 
	int contador = 0;
	char search = (char)(substring[0] + 32);
	printf("\n\n\n\nMy search Caracthers: %c\n", search);

	for (index = 1 ; index < 101 ; index++){
		printf("%d Analyzig Caracthers: %c\n",index, substring[index]);
		if (substring[index] == search) {
			printf("Found search %c on index: %d\n",search,index);
			contador++; 
		}
		// substring[index] = "*";
	}
	printf("I found %d occorrences of %c\n\n\n\n", contador, search);
	return contador;
}

int main()
{
	const char *name = "shared_memory";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;

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

	/* now read from the shared memory region */
	printf("%s\n",(char*)ptr);
	printf("Number of Caracthers: %d\n", strlen(ptr)  );
	printf("%d\n", *((int*)(ptr+strlen(ptr)+1)));
	
	ptr = ptr; // Read first 100 chars
	conta(ptr);
	// *ptr = '0';
	ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars
	// fork();
	conta(ptr);						// Read next 100 chars
	ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars
	// fork();
	conta(ptr);						// Read next 100 chars
	ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars
	conta(ptr);						// Read next 100 chars
	ptr = ptr + 101 * sizeof(char); // Move pointer to next 100 chars
	conta(ptr);						// Read next 100 chars
	
	printf("FULL STRING:\n%s\n",(char*)ptr - 4* 101 * sizeof(char));
	// sleep(60);
	/* remove the shared memory segment */
//	if (shm_unlink(name) == -1) {
//		printf("Error removing %s\n",name);
//		exit(-1);
//	}

	return 0;
}
