#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include <string.h>


void freeFile(File *fi)
{
	free(fi->name);
	free(fi->data);
	free(fi);
}

void createFs(Directory **root)
{
	(*root) = (Directory *) malloc(sizeof(Directory));
	(*root)->name = strdup("/");
	(*root)->parentDir = NULL;
	(*root)->headDirs = NULL;
	(*root)->headFiles = NULL;
}

void deleteFs(Directory *root)
{
	free(root->name);
	free(root);
}

void createNewFile(Directory **Director, char *str)
{
	int startpoz = 0;
	int endpoz = strlen(str) - 1;

	while (str[startpoz] != ' ' && startpoz < strlen(str))
		startpoz++;

	while (str[endpoz] != ' ' && endpoz >= 0)
		endpoz--;

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
			   strcmp(fisier->name, cap->fila->name) > 0)
			cap = cap->next;

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

void createNewDir(Directory **Director, char *str)
{
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str))
		startpoz++;

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
			   strcmp(newDirect->name, cap->Direct->name) > 0)
			cap = cap->next;

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

void LS(Directory *curent)
{
	Fila *capFile = curent->headFiles;
	Dir *capDirs =  curent->headDirs;
	int checker = 0;

	if (capFile != NULL)
		checker = 1;

	while (capFile != NULL) {
		printf("%s", capFile->fila->name);
		capFile = capFile->next;
		if (capFile != NULL)
			printf(" ");
	}

	if (capDirs != NULL && checker)
		printf(" ");

	while (capDirs != NULL) {
		printf("%s", capDirs->Direct->name);
		capDirs = capDirs->next;
		if (capDirs != NULL)
			printf(" ");
	}

	printf("\n");
}

void pwd(Directory *curent)
{
	if (curent->parentDir == NULL) {
		printf("/");
		return;
	}

	pwd(curent->parentDir);
	printf("%s/", curent->name);
}

void CD(Directory **curent, char *str)
{
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str))
		startpoz++;

	char *nume = strdup(str + startpoz + 1);

	if (strcmp(nume, "..") == 0) {
		if ((*curent)->parentDir != NULL)
			(*curent) = (*curent)->parentDir;

		free(nume);
		return;
	} else if (strcmp(nume, ".") == 0) {
		free(nume);
		return;
	}

	Dir *cap = (*curent)->headDirs;

	while (cap != NULL && strcmp(cap->Direct->name, nume) != 0)
		cap = cap->next;

	if (cap == NULL)
		printf("Cannot move to '%s': No such directory!\n", nume);
	else
		(*curent) = cap->Direct;

	free(nume);
}

void tree(Directory *root, int numberOfSpaces)
{
	Fila *capFile = root->headFiles;
	Dir *capDirs =  root->headDirs;

	while (capFile != NULL) {
		for (int i = 0; i < numberOfSpaces; ++i)
			printf(" ");

		printf("%s\n", capFile->fila->name);
		capFile = capFile->next;
	}

	while (capDirs != NULL) {
		for (int i = 0; i < numberOfSpaces; ++i)
			printf(" ");

		printf("%s\n", capDirs->Direct->name);
		tree(capDirs->Direct, numberOfSpaces + 4);
		capDirs = capDirs->next;
	}
}

void rmFile(Directory **curent, char *str)
{
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str))
		startpoz++;

	char *nume = strdup(str + startpoz + 1);
	Fila *capFile = (*curent)->headFiles;

	while (capFile != NULL && strcmp(capFile->fila->name, nume) != 0)
		capFile = capFile->next;

	if (capFile == NULL) {
		printf("Cannot remove '%s': No such file!\n", nume);
		free(nume);
		return;
	}

	if (capFile->next == NULL && capFile->prev == NULL) {
		(*curent)->headFiles = NULL;
		freeFile(capFile->fila);
		free(capFile);
	} else if (capFile->next == NULL) {
		capFile->prev->next = NULL;
		freeFile(capFile->fila);
		free(capFile);
	} else if (capFile->prev == NULL) {
		(*curent)->headFiles = capFile->next;
		(*curent)->headFiles->prev = NULL;
		freeFile(capFile->fila);
		free(capFile);
	} else {
		capFile->next->prev = capFile->prev;
		capFile->prev->next = capFile->next;
		capFile->next = NULL;
		capFile->prev = NULL;
		freeFile(capFile->fila);
		free(capFile);
	}

	free(nume);
}

void remover(Directory *curent)
{
	while (curent->headFiles != NULL) {
		Fila *nod = curent->headFiles;

		curent->headFiles = curent->headFiles->next;
		freeFile(nod->fila);
		free(nod);
	}

	while (curent->headDirs != NULL) {
		Dir *nod = curent->headDirs;

		remover(curent->headDirs->Direct);
		curent->headDirs = curent->headDirs->next;
		free(nod);
	}
	free(curent->name);
	free(curent);
}

void rmDir(Directory **curent, char *str)
{
	int startpoz = 0;

	while (str[startpoz] != ' ' && startpoz < strlen(str))
		startpoz++;

	char *nume = strdup(str + startpoz + 1);
	Dir *capDirs = (*curent)->headDirs;

	while (capDirs != NULL && strcmp(capDirs->Direct->name, nume) != 0)
		capDirs = capDirs->next;

	if (capDirs == NULL) {
		printf("Cannot remove '%s': No such directory!\n", nume);
		free(nume);
		return;
	}
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
	free(nume);
}

void findRec(Directory *curent, int currentPos,
			int depth, int min_size, int max_size, char *nume)
{
	if (currentPos > depth)
		return;

	Fila *cap = curent->headFiles;

	while (cap != NULL) {
		if (cap->fila->size >= min_size && cap->fila->size <= max_size
			&& strstr(cap->fila->data, nume)) {
			printf("%s ", cap->fila->name);
		}

		cap = cap->next;
	}

	Dir *capDirs = curent->headDirs;

	while (capDirs != NULL) {
		findRec(capDirs->Direct, currentPos + 1,
			depth, min_size, max_size, nume);
		capDirs = capDirs->next;
	}
}

void find(Directory *curent, char *str)
{
	char *p = strtok(str, " ");

	p = strtok(NULL, " ");
	int depth = atoi(p);

	p = strtok(NULL, " ");
	int min_size = atoi(p);

	p = strtok(NULL, " ");
	int max_size = atoi(p);
	p = strtok(NULL, " ");

	char *nume = strdup(p);

	p = strtok(NULL, " ");

	findRec(curent, 0, depth, min_size, max_size, nume);
}

int main(int argv, char **argc)
{
	char s[105];
	Directory *root = NULL;
	Directory *curent;

	while (fgets(s, 105, stdin)) {
		s[strlen(s) - 1] = '\0';
		if (strcmp(s, "create fs") == 0) {
			createFs(&root);
			curent = root;
		} else if (strcmp(s, "delete fs") == 0) {
			while (root->headDirs != NULL) {
				Dir *nod = root->headDirs;

				remover(root->headDirs->Direct);
				root->headDirs = root->headDirs->next;
				free(nod);
			}

			while (root->headFiles != NULL) {
				Fila *nod = root->headFiles;

				root->headFiles = root->headFiles->next;
				freeFile(nod->fila);
				free(nod);
			}
			deleteFs(root);
			break;
		} else if (s - strstr(s, "touch") == 0)
			createNewFile(&curent, s);
		else if (s - strstr(s, "mkdir") == 0)
			createNewDir(&curent, s);
		else if (strcmp(s, "ls") == 0)
			LS(curent);
		else if (strcmp(s, "pwd") == 0) {
			if (curent->parentDir == NULL) {
				printf("/\n");
			} else {
				pwd(curent->parentDir);
				printf("%s\n", curent->name);
			}
		} else if (s - strstr(s, "cd") == 0)
			CD(&curent, s);
		else if (strcmp(s, "tree") == 0) {
			printf("%s\n", curent->name);
			tree(curent, 4);
		} else if (s - strstr(s, "rmdir") == 0)
			rmDir(&curent, s);
		else if (s - strstr(s, "rm") == 0)
			rmFile(&curent, s);
		else if (s - strstr(s, "find") == 0) {
			find(curent, s);
			printf("\n");
		}
	}
	return 0;
}
