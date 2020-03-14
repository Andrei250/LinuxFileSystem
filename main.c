#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include <string.h>

void createFs(Directory **root) {
	(*root) = (Directory *) malloc(sizeof(Directory));
	(*root)->name = strdup("/");
	(*root)->parentDir = NULL;
	(*root)->headDirs = NULL;
	(*root)->headFiles = NULL;
}

void deleteFs(Directory *root) {
	free(root->name);
	free(root);
}

void createNewFile(Directory **Director, char *str) {
	int startpoz = 0;
	int endpoz = strlen(str) - 1;

	while (str[startpoz] != ' ' && startpoz < strlen(str)) {
		startpoz ++;
	}

	while (str[endpoz] != ' ' && endpoz >= 0) {
		endpoz --;
	}
	 
	File *fisier = (File *)malloc(sizeof(File));
	fisier->name = strdup(str + startpoz + 1);
	fisier->name[endpoz - startpoz - 1] = '\0';
	fisier->data = strdup(str + endpoz + 1);
	fisier->size = strlen(fisier->data);
	fisier->dir = (*Director);

	if ((*Director)->headFiles == NULL) {
		(*Director)->headFiles = (Fila *)malloc(sizeof(Fila));
		(*Director)->headFiles->prev = NULL;
		(*Director)->headFiles->next = NULL;
		(*Director)->headFiles->fila = fisier;
	} else { 
		Fila *newFile = (Fila *)malloc(sizeof(Fila));
		newFile->prev = NULL;
		newFile->next = NULL;
		newFile->fila = fisier;

		Fila *cap = (*Director)->headFiles;

		while (cap->next != NULL && 
			   strcmp(fisier->name, cap->fila->name) > 0) {
			cap = cap->next;
		}

		if (strcmp(fisier->name, cap->fila->name) == 0) 
			return;

		if (strcmp(fisier->name, cap->fila->name) > 0) { // coada
			cap->next = newFile;
			newFile->prev = cap;
		} else {
			if (cap->prev == NULL) {
				cap->prev = newFile;
				newFile->next = cap;
				(*Director)->headFiles = newFile;
			} else {
				newFile->prev = cap->prev;
				newFile->next = cap;
				cap->prev->next = newFile;
				cap->prev = newFile;
			}
		}

	}

}

void createNewDir(Directory **Director, char *str) {
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str)) {
		startpoz ++;
	}
	//creez un nou director
	Directory *newDirect = (Directory *)malloc(sizeof(Directory));
	newDirect->name = strdup(str + startpoz + 1);
	newDirect->headFiles = NULL;
	newDirect->headDirs = NULL;
	newDirect->parentDir = (*Director);

	//adaug la lista
	if ((*Director)->headDirs == NULL) {
		(*Director)->headDirs = (Dir *)malloc(sizeof(Dir));
		(*Director)->headDirs->prev = NULL;
		(*Director)->headDirs->next = NULL;
		(*Director)->headDirs->Direct = newDirect;
	} else { 
		Dir *newDir = (Dir *)malloc(sizeof(Dir));
		newDir->prev = NULL;
		newDir->next = NULL;
		newDir->Direct = newDirect;

		Dir *cap = (*Director)->headDirs;



		while (cap->next != NULL && 
			   strcmp(newDirect->name, cap->Direct->name) > 0) {
			cap = cap->next;
		}

		if (strcmp(newDirect->name, cap->Direct->name) == 0) 
			return;

		if (strcmp(newDirect->name, cap->Direct->name) > 0) { // coada
			cap->next = newDir;
			newDir->prev = cap;
		} else {
			if (cap->prev == NULL) {
				cap->prev = newDir;
				newDir->next = cap;
				(*Director)->headDirs = newDir;
			} else {
				newDir->prev = cap->prev;
				newDir->next = cap;
				cap->prev->next = newDir;
				cap->prev = newDir;
			}
		}
	}

}

void LS(Directory *curent) {
	Fila *capFile = curent->headFiles;
	Dir *capDirs =  curent->headDirs;

	while (capFile != NULL) {
		printf("%s ", capFile->fila->name);
		capFile = capFile->next;
	}

	while (capDirs != NULL) {
		printf("%s ", capDirs->Direct->name);
		capDirs = capDirs->next;
	}

	printf("\n");
}

void pwd(Directory *curent) {
	if (curent->parentDir == NULL) {
		printf("/");
		return;
	}

	pwd(curent->parentDir);
	printf("%s/", curent->name);
}

void CD(Directory **curent, char *str) {
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str)) {
		startpoz ++;
	}

	char *nume = strdup(str + startpoz + 1);

	if (strcmp(nume, "..") == 0) {
		if ((*curent)->parentDir != NULL) {
			(*curent) = (*curent)->parentDir;
		}

		return;
	} else if (strcmp(nume, ".") == 0) {
		return;
	}

	Dir *cap = (*curent)->headDirs;
	
	while (cap != NULL && strcmp(cap->Direct->name, nume) != 0) {
		cap = cap->next;
	} 	

	if (cap == NULL) {
		printf("Cannot move to '%s': No such directory!\n", nume);
	} else {
		(*curent) = cap->Direct;
	}
}

void tree(Directory *root, int numberOfSpaces) {
	Fila *capFile = root->headFiles;
	Dir *capDirs =  root->headDirs;

	while (capFile != NULL) {
		for (int i = 0; i < numberOfSpaces; ++i) {
			printf(" ");
		}

		printf("%s\n", capFile->fila->name);
		capFile = capFile->next;
	}

	while (capDirs != NULL) {
		for (int i = 0; i < numberOfSpaces; ++i) {
			printf(" ");
		}

		printf("%s\n", capDirs->Direct->name);
		tree(capDirs->Direct, numberOfSpaces + 4);
		capDirs = capDirs->next;
	}
}

void rmFile(Directory **curent, char *str) {
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str)) {
		startpoz ++;
	}

	char *nume = strdup(str + startpoz + 1);
	Fila *capFile = (*curent)->headFiles;

	while (capFile != NULL && strcmp(capFile->fila->name, nume) != 0) {
		capFile = capFile->next;
	}

	if (capFile == NULL) {
		printf("Cannot remove ​'%s': No such file!\n", nume);
		return;
	} else {
		if (capFile->next == NULL && capFile->prev == NULL) {
			(*curent)->headFiles = NULL;
			free(capFile);
		} else if (capFile->next == NULL) {
			capFile->prev->next = NULL;
			free(capFile);
		} else if (capFile->prev == NULL) {
			(*curent)->headFiles = capFile->next;
			(*curent)->headFiles->prev = NULL;	
			free(capFile);
		} else {
			capFile->next->prev = capFile->prev;
			capFile->prev->next = capFile->next;
			free(capFile);
		}
	}
}

void remover(Directory *curent) {
	while (curent->headFiles != NULL) {
		Fila *nod = curent->headFiles;
		curent->headFiles = curent->headFiles->next;
		free(nod);
	}

	while (curent->headDirs != NULL) {
		Dir *nod = curent->headDirs;
		remover(curent->headDirs->Direct);
		curent->headDirs = curent->headDirs->next;
		free(nod);
	}

	free(curent);
}

void rmDir(Directory **curent, char *str) {
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str)) {
		startpoz ++;
	}

	char *nume = strdup(str + startpoz + 1);
	Dir *capDirs = (*curent)->headDirs;

	while (capDirs != NULL && strcmp(capDirs->Direct->name, nume) != 0) {
		capDirs = capDirs->next;
	}

	if (capDirs == NULL) {
		printf("Cannot remove ​'%s': No such directory!\n", nume);
		return;
	} else {
		remover(capDirs->Direct);
		if (capDirs->next == NULL && capDirs->prev == NULL) {
			(*curent)->headDirs = NULL;
			free(capDirs);
		} else if (capDirs->next == NULL) {
			capDirs->prev->next = NULL;
			free(capDirs);
		} else if (capDirs->prev == NULL) {
			(*curent)->headDirs = capDirs->next;
			(*curent)->headDirs->prev = NULL;	
			free(capDirs);
		} else {
			capDirs->next->prev = capDirs->prev;
			capDirs->prev->next = capDirs->next;
			free(capDirs);
		}
	}
}

int main(int argv, char **argc) {
	char s[105];
	Directory *root = NULL;
	Directory *curent;

	while (fgets(s, 105, stdin)) {
		s[strlen(s) - 1] = '\0';
		if (strcmp(s, "create fs") == 0) {
			createFs(&root);
			curent = root;
		} else if (strcmp(s, "delete fs") == 0){
			deleteFs(root);
			break;
		} else if (s - strstr(s, "touch") == 0) {
			createNewFile(&curent, s);
		} else if (s - strstr(s, "mkdir") == 0) {
			createNewDir(&curent, s);
		} else if (strcmp(s, "ls") == 0) {
			LS(curent);
		} else if (strcmp(s, "pwd") == 0) {
			if (curent->parentDir == NULL) {
				printf("/\n");
			} else {
				pwd(curent->parentDir);
				printf("%s\n", curent->name);
			}
		} else if (s - strstr(s, "cd") == 0) {
			CD(&curent, s);
		} else if (strcmp(s, "tree") == 0) {
			if (curent->parentDir != NULL) 
				printf("/%s\n", curent->name);
			else 
				printf("%s\n", curent->name);
			tree(curent, 4);
		} else if (s - strstr(s, "rmdir") == 0) {
			rmDir(&curent, s);
		} else if (s - strstr(s, "rm") == 0) {
			rmFile(&curent, s);
		} 
	}

	return 0;
}