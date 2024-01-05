# DASH Project

Student : DOUBABI Mustapha

This project is a simplified shell written in the C programming language. It supports basic functions of a standard shell, including changing directories, path management, executing basic commands, and more.

## Compilation
To compile the project, you will need a C compiler such as GCC. You should navigate to the project directory and run the following command:

```bash
gcc -o dashShell main.c fonctionsAnnexes.c
```
This will generate an executable file called dashShell.

## Exécution
After compilation, you can run the program as follows:

```bash
./dashShell
```
You will be greeted with a welcome message, and you can then start entering commands.

## Tests unitaires
We have also provided unit tests for this project to ensure that it functions correctly.

To compile and run the tests, compile the test file with the following command:
```bash
gcc -o testUnitaires testsUnitaires.c fonctionsAnnexes.c
```
This will generate an executable file called testUnitaires. Now, execute the tests with the following command:
```bash
./testUnitaires
```
This will run all the unit tests, and you will see the results directly in your terminal.

## Notes

1) It's important to note that this program has been tested in a Unix/Linux environment. Compilation and execution may not work as expected if you are using a different operating system.


2) The shell considers spaces for "cd" commands. To use the "cd" command to navigate to a directory containing spaces, do as follows:

```bash
dash> cd "Test Folder"
```

3) To use the unit tests, make sure you have an rmTest folder in the directory where you executed the tests to ensure that the "cd" command works correctly.
