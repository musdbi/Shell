#define _POSIX_C_SOURCE 200809L

#include "fonctionsAnnexes.h"
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void testExitCommand()
{
    printf("------------------ Exit command test ------------------\n");
    char **argument = malloc(2 * sizeof(char *));
    if (argument == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    argument[0] = "exit";
    argument[1] = NULL;
    char *cmd = NULL;
    char *cmdBis = NULL;
    /* On test la commande exit */
    exitCommand(argument, cmd, cmdBis, 0, "");
    /* Si le programme ne s'est pas arrêté, alors la commande exit ne fonctionne pas */
    printf("Still running, the exit command doesn't work !\n");
    printf("\n");
}

void testPathCommand()
{
    printf("------------------ Path command test ------------------\n");
    char **argument = malloc(3 * sizeof(char *));
    if (argument == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    argument[0] = "path";
    argument[1] = "/bin";
    argument[2] = NULL;
    /* On remplace la valeur de la variable path /bin */
    executePathWithArguments(argument, 2);
    char *path = getenv("PATH");
    printf("PATH variable should be /bin\n");
    printf("PATH = %s\n", path);
    if (strcmp(path, ":/bin") == 0)
    {
        /* Si Path = :/bin, alors la commande fonctionne correctement */
        printf("Path command worked successfully !\n");
    }
    else
    {
        /* Dans le cas d'une inégalité, il y a un souci avec la commande */
        printf("Path command didn't work \n");
    }
    printf("\n");
    free(argument);
}

void testConcatenateArgument()
{
    printf("------------------ ConcatenateArgument function test ------------------\n");
    char **argument = malloc(4 * sizeof(char *));
    if (argument == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    argument[0] = "echo"; // La commande n'a pas d'importance dans ce test, on veut juste vérifier que la concaténation d'arguments fonctionne
    argument[1] = "\"Hello";
    argument[2] = "World\"";
    argument[3] = NULL;
    char *expectedResult = "Hello World";
    char *result = concatenateArgument(argument, 1, 2);
    if (strcmp(expectedResult, result) == 0)
    {
        printf("The expected result was %s and the result is %s : it is equal !\n", expectedResult, result);
    }
    else
    {
        printf("The expected result was %s and the result is %s : it is not equal\n", expectedResult, result);
    }
    printf("\n");
    free(argument);
    free(result);
}

void testCdCommand()
{
    printf("------------------ CD command test ------------------\n");
    char **argument = malloc(3 * sizeof(char *));
    if (argument == NULL)
    {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    /* ATTENTION : BIEN VÉRIFIER QU'UN DOSSIER rmTest EXISTE DANS VOTRE RÉPERTOIRE */
    argument[0] = "cd";
    argument[1] = "rmTest";
    argument[2] = NULL;
    executeChangePathWithoutSpace(argument);
    char buff[PATH_MAX + 1];
    if (getcwd(buff, PATH_MAX + 1) != NULL)
    {
        /* On utilise strrchr (buff, '/') pour trouver la dernière occurence du caractère '/' et ajouter 1 pour
        avoir le nom du répertoire courant */
        char *lastSlash = strrchr(buff, '/');
        if (lastSlash != NULL && strcmp(lastSlash + 1, argument[1]) == 0)
        {
            printf("We are in the correct directory: %s\n", buff);
        }
        else
        {
            printf("We are not in the correct directory. Current directory is: %s\n", buff);
        }
    }
    else
    {
        printf("Can't get the current directory.");
    }
    printf("\n");
    free(argument);
}

int main()
{
    /* On applique chaque test */
    testPathCommand();
    testConcatenateArgument();
    testCdCommand();
    testExitCommand();
    return 0;
}


