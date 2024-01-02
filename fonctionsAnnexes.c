#define _POSIX_C_SOURCE 200809L

#include "fonctionsAnnexes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char errorMessage[30] = "An error has occurred\n";

/* Fonction pour concaténer les arguments entre guillemets tout en supprimant ces derniers */
char *concatenateArgument(char **argument, int firstQuoteIndex, int secondQuoteIndex)
{
    size_t totalLength = 0;
    for (int i = firstQuoteIndex; i <= secondQuoteIndex; i++)
    {
        totalLength += strlen(argument[i]);
    }
    totalLength += secondQuoteIndex - firstQuoteIndex;
    char *result = malloc(sizeof(char) * totalLength);
    strcpy(result, argument[firstQuoteIndex] + 1);
    for (int i = firstQuoteIndex + 1; i <= secondQuoteIndex; i++)
    {
        strcat(result, " ");
        strcat(result, argument[i]);
    }
    result[strlen(result) - 1] = '\0';
    return result;
}

/* Fonction qui retourne une erreur et exit */
void error()
{
    write(STDERR_FILENO, errorMessage, strlen(errorMessage));
    exit(EXIT_FAILURE);
}

/* Fonction pour exécuter la commande cd avec espace */
void executeChangePathWithSpace(char *argumentWithSpaces)
{
    if (chdir(argumentWithSpaces) == -1)
    {
        write(STDERR_FILENO, errorMessage, strlen(errorMessage));
    }
}

/* Fonction pour exécuter la commande cd sans espace */
void executeChangePathWithoutSpace(char **argument)
{
    /* Avec argument */
    if (argument[1])
    {
        if (argument[2])
        {
            printf("The cd command takes only one argument !\n");
        }
        else if (chdir(argument[1]) == -1)
        {
            write(STDERR_FILENO, errorMessage, strlen(errorMessage));
        }
    }
        /* Sans argument */
    else
    {
        char *homeDir = getenv("HOME");
        if (homeDir)
        {
            if (chdir(homeDir) == -1)
            {
                write(STDERR_FILENO, errorMessage, strlen(errorMessage));
            }
        }
        else
        {
            fprintf(stderr, "Error: HOME environment variable not set\n");
        }
    }
}

/* Fonction pour exécuter la commande path avec argument */
void executePathWithArguments(char **argument, int i)
{
    setenv("PATH", "", 1);
    for (int j = 1; j < i; j++)
    {
        if (access(argument[j], X_OK) == 0)
        {
            char *oldPath = getenv("PATH");
            char *newPath = malloc(sizeof(oldPath) + strlen(argument[j]) + 2);
            strcpy(newPath, oldPath);
            strcat(newPath, ":");
            strcat(newPath, argument[j]);
            int result = setenv("PATH", newPath, 1);
            free(newPath);
            if (result == 0)
            {
                printf("PATH environment variable successfully updated\n");
            }
            else
            {
                write(STDERR_FILENO, errorMessage, strlen(errorMessage));
                break;
            }
        }
        else
        {
            write(STDERR_FILENO, errorMessage, strlen(errorMessage));
            break;
        }
    }
}

/* Fonction pour exécuter la commande ls */
void executeListDirectoryContent(char **argument, int totalArgument)
{
    char **newArguments = malloc(sizeof(char *) * (totalArgument + 2));
    for (int j = 0; j < totalArgument; j++)
    {
        newArguments[j] = argument[j];
    }
    newArguments[totalArgument] = "--color=auto";
    newArguments[totalArgument + 1] = NULL;

    int __attribute__((unused)) val = execvp(newArguments[0], newArguments);
    free(newArguments);
}

/* Fonction pour exécuter le reste des commandes se situant dans le dossier /bin */
void executeBinCommands(char **argument)
{
    char *fullPath = searchExecutable(argument[0]);
    if (fullPath)
    {
        int __attribute__((unused)) val = execv(fullPath, argument);
        free(fullPath);
    }
    else
    {
        write(STDERR_FILENO, errorMessage, strlen(errorMessage));
        exit(EXIT_FAILURE);
    }
}

/* Fonction pour arrêter l'exécution du shell */
void exitCommand (char **argument, char *cmd, char *cmdBis,int secondQuoteIndex,char *argumentWithSpaces)
{
    if (argument[1])
    {
        printf("Try again with no argument for exit\n");
    }
    else
    {
        printf("Successful exit !\n");
        freeAllMemory(cmd, cmdBis, argument);
        if (secondQuoteIndex != 0)
            free(argumentWithSpaces);
        exit(0);
    }
}

/* Fonction pour trouver l'exécutable */
char *searchExecutable(const char *executable)
{
    char *pathEnv = getenv("PATH");

    /* Si la variable PATH n'est pas définie */
    if (!pathEnv)
        return NULL;

    char *pathEnvCopy = strdup(pathEnv);
    char *pathToken = strtok(pathEnvCopy, ":");
    char *fullPath = NULL;

    while (pathToken)
    {
        char *tempPath = malloc(strlen(pathToken) + strlen(executable) + 2);
        strcpy(tempPath, pathToken);
        strcat(tempPath, "/");
        strcat(tempPath, executable);

        /*Vérifier si l'exécutable est accessible */
        if (access(tempPath, X_OK) == 0)
        {
            fullPath = tempPath;
            break;
        }
        else
            free(tempPath);

        pathToken = strtok(NULL, ":");
    }
    free(pathEnvCopy);
    return fullPath;
}

/* Message de bienvenue */
void welcomeMessage()
{
    printf("\n");
    printf(" ____    _    ____  _   _\n");
    printf("|  _ \\  / \\  / ___|| | | |\n");
    printf("| | | |/ _ \\ \\___ \\| |_| |\n");
    printf("| |_| / ___ \\ ___) |  _  |\n");
    printf("|____/_/   \\_\\____/|_| |_|\n");
    printf("\n");
}

/* Fonction pour libérer la mémoire allouée */
void freeAllMemory(char *cmd, char *cmdBis, char **argument)
{
    free(cmd);
    free(cmdBis);
    free(argument);
}
