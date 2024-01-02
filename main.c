#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fonctionsAnnexes.h"

int main()
{

    /* On clear la console à chaque lancement du main */
    system("clear");

    /* Déclaration des variables */
    char **argument = NULL;
    char *cmd = NULL, *cmdBis = NULL, *token = NULL, *argumentWithSpaces = NULL, *delim = " \n";
    int totalArgument, i, file, firstQuoteIndex, secondQuoteIndex;
    bool openedFile, outputFile, inQuotes;
    pid_t pid;
    size_t s = 0;

    /* Message de bienvenue */
    welcomeMessage();

    /* Entrée dans la boucle principale du programme */
    while (true)
    {

        /* Réinitialisation des variables tels que les booléens, les compteurs ... */
        cmd = NULL; // Je réinitialise le pointeur à NULL pour être sûr que la fonction getline puisse aboutir
        openedFile = outputFile = inQuotes = false;
        totalArgument = i = firstQuoteIndex = secondQuoteIndex = 0;

        /* Équivalent du $ d'un shell basique */
        printf("dash> ");

        /* Lecture de la commande et des arguments */
        if (getline(&cmd, &s, stdin) == -1)
        {
            error();
        }

        /* Énumération du nombre d'arguments pour allouer suffisamment de mémoire */
        cmdBis = strdup(cmd);
        if (cmdBis == NULL)
        {
            fprintf(stderr, "String duplication error.\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(cmd, delim);

        while (token)
        {
            /* Si un ">" est trouvé, on ne considère pas la suite comme des arguments de la commande */
            if (strcmp(token, ">") == 0)
            {
                outputFile = true;
                break;
            }
                /* Si un "<" est trouvé, on ne considère pas la suite comme des arguments de la commande */
            else if (strcmp(token, "<") == 0)
            {
                openedFile = true;
                break;
            }
                /* Vérification de la présence de guillemets */
            else if (token[0] == '"')
            {
                inQuotes = true;
                firstQuoteIndex = totalArgument;
                totalArgument += 1;
                token = strtok(NULL, delim);
            }
            else if (inQuotes)
            {
                if (strchr(token, '"'))
                {
                    inQuotes = false;
                    secondQuoteIndex = totalArgument;
                }
                totalArgument += 1;
                token = strtok(NULL, delim);
            }
                /* Si on est dans aucun cas précédent, on poursuit l'énumération des arguments normalement */
            else
            {
                totalArgument += 1;
                token = strtok(NULL, delim);
            }
        }
        /* Allocation de la mémoire pour le tableau de chaine de caractère qui va contenir les arguments */
        argument = malloc(sizeof(char *) * totalArgument + 1);
        if (argument == NULL)
        {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
        token = strtok(cmdBis, delim);

        /* Insérer les arguments dans le tableau */
        while (token)
        {
            if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0)
                break;
            else
            {
                argument[i] = token;
                i++;
                token = strtok(NULL, delim);
            }
        }
        argument[i] = NULL;

        /* Si des guillemets ont été utilisés, on concatène les arguments */
        if (secondQuoteIndex != 0)
        {
            argumentWithSpaces = concatenateArgument(argument, firstQuoteIndex, secondQuoteIndex);
        }

        /* Si l'on souhaite effectuer un 'change directory' */
        if (strcmp(argument[0], "cd") == 0)
        {
            /* Cas 1 : Dossier avec espace */
            if (secondQuoteIndex != 0)
            {
                executeChangePathWithSpace(argumentWithSpaces);
            }
                /* Cas 2 : Dossier sans espace */
            else
            {
                executeChangePathWithoutSpace(argument);
            }
        }

            /* Si l'on souhaite effectuer un 'path' */
        else if (strcmp(argument[0], "path") == 0)
        {
            if (argument[1])
            {
                /* Cas 1 : Avec argument */
                executePathWithArguments(argument, i);
            }
            else
            {
                /* Cas 2 : Sans argument : on print seulement la variable path */
                printf("PATH='%s'\n", getenv("PATH"));
            }
        }

            /* Le programme se termine si on tape exit */
        else if (strcmp(argument[0], "exit") == 0)
        {
            exitCommand(argument, cmd, cmdBis, secondQuoteIndex, argumentWithSpaces);
        }

            /* Si la commande se trouve dans le dossier /bin tel que : ls, wc, man, echo ... */
        else
        {
            pid = fork();
            /* Vérifier si le fork a abouti */
            if (pid == -1)
            {
                error();
            }
            /* Un proccessus fils effectue la commande car les commandes exec "écrasent" le reste du programme */
            if (pid == 0)
            {
                /* Cas 1 : Rediriger la sortie standard vers un fichier */
                if (outputFile)
                {
                    token = strtok(NULL, delim);
                    file = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    token = strtok(NULL, delim);
                    if ((token != NULL) || (file < 0) || (dup2(file, STDOUT_FILENO) < 0))
                    {
                        if (file >= 0)
                        {
                            close(file);
                        }
                        error();
                    }
                }
                    /* Cas 2 : Redirection standard */
                else if (openedFile)
                {
                    token = strtok(NULL, delim);
                    file = open(token, O_RDONLY, 0644);
                    token = strtok(NULL, delim);
                    if ((token != NULL) || (file < 0) || (dup2(file, STDIN_FILENO) < 0))
                    {
                        if (file >= 0)
                        {
                            close(file);
                        }
                        error();
                    }
                }

                /* Si l'on souhaite executer la commande ls, on utilise execvp pour la prise en charge des couleurs */
                if (strcmp(argument[0], "ls") == 0)
                {
                    executeListDirectoryContent(argument, totalArgument);
                }
                    /* Sinon, on utilise execv */
                else
                {
                    executeBinCommands(argument);
                }
                /* On ferme un fichier si un fichier a été ouvert */
                if (openedFile || outputFile)
                {
                    close(file);
                }
            }
            else
            {
                /* On fait appel à wait afin d'éviter la création d'un processus zombie */
                wait(NULL);
            }
        }
        /* On libère la mémoire allouée */
        freeAllMemory(cmd, cmdBis, argument);
        if (secondQuoteIndex != 0)
            free(argumentWithSpaces);
    }
    return 0;
}


