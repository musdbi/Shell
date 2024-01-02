#ifndef PROJET_C_FONCTIONSANNEXES_H
#define PROJET_C_FONCTIONSANNEXES_H

char *concatenateArgument(char **argument, int firstQuoteIndex, int secondQuoteIndex);
void error();
void executeChangePathWithSpace(char *argumentWithSpaces);
void executeChangePathWithoutSpace(char **argument);
void executePathWithArguments(char **argument, int i);
void executeListDirectoryContent(char **argument, int totalArgument);
void executeBinCommands(char **argument);
void exitCommand(char **argument, char *cmd, char *cmdBis, int secondQuoteIndex, char *argumentWithSpaces);
char *searchExecutable(const char *executable);
void welcomeMessage();
void freeAllMemory(char *cmd, char *cmdBis, char **argument);

extern char errorMessage[30];

#endif // PROJET_C_FONCTIONSANNEXES_H


