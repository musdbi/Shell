# DASH Project

Étudiant : DOUBABI Mustapha

Ce projet est un shell simplifié, écrit en langage C. Il supporte les fonctions de base d'un shell standard telles que le changement de répertoire, la gestion des chemins, l'exécution de commandes basiques et plus encore.

## Compilation
Pour compiler le projet, vous aurez besoin d'un compilateur C comme GCC. Vous devez vous rendre dans le dossier du projet et taper la commande suivante :

```bash
gcc -o dashShell main.c fonctionsAnnexes.c
```
Cela générera un fichier exécutable appelé dashShell.

## Exécution
Après la compilation, vous pouvez exécuter le programme de la manière suivante :

```bash
./dashShell
```
Vous serez accueilli par un message de bienvenue et vous pourrez ensuite commencer à entrer des commandes.

## Tests unitaires
Nous avons également fourni des tests unitaires pour ce projet pour nous assurer qu'il fonctionne correctement.

Pour compiler et exécuter les tests, compilez le fichier de test avec la commande suivante :
```bash
gcc -o testUnitaires testsUnitaires.c fonctionsAnnexes.c
```
Cela générera un fichier exécutable appelé testUnitaires. À présent, exécutez les tests avec la commande suivante :
```bash
./testUnitaires
```
Cela lancera tous les tests unitaires. Vous verrez les résultats directement dans votre terminal.

## Notes

1) Il est important de noter que ce programme a été testé sous un environnement Unix/Linux. La compilation et l'exécution peuvent ne pas fonctionner comme prévu si vous êtes sous un autre système d'exploitation.


2) Le shell prend en compte les espaces pour les commandes "cd". Pour utiliser la commande cd vers un dossier contenant un espace, procédez comme suit :

```bash
dash> cd "Dossier Test"
```

3) Pour utiliser les tests unitaires, assurez-vous de disposer d'un dossier rmTest dans le répertoire où vous avez exécuté les tests pour vérifier que la commande cd fonctionne bien.
