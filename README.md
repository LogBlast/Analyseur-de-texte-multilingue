
# Projet : Analyseur de texte multilingue

## Description
Ce projet est un analyseur de texte en C permettant d'extraire et d'analyser diverses informations à partir d'un fichier texte. Il inclut des fonctionnalités comme le comptage des lignes, des mots, des caractères, la détection de mots palindromes, l'analyse de phrases, et bien plus encore. Il supporte également le multilinguisme (français et anglais).

### Fonctionnalités principales :
- Comptage des lignes, mots et caractères.
- Identification des mots les plus fréquents.
- Recherche de mots palindromes.
- Analyse des phrases pour obtenir leur longueur moyenne et leurs extrêmes (plus courte et plus longue).
- Sauvegarde des résultats d'analyse dans un fichier de sortie.
- Comparaison entre deux fichiers texte.

## Prérequis
- **Système d'exploitation** : Compatible avec tout système disposant d'un compilateur C (éprouvé sous Linux et Windows).
- **Compilateur** : GCC ou équivalent.
- **Langue** : Le projet utilise des fichiers de ressources linguistiques pour le français et l'anglais situés dans `../resource/`.

## Installation
1. Clonez le répertoire du projet :
   ```bash
   git clone <URL_du_dépôt>
   cd <Nom_du_dépôt>
   ```

2. Compilez le programme :
   ```bash
   gcc -o analyseur main.c analyseur.c langue.c -Wall -Wextra
   ```

3. Placez les fichiers de ressources linguistiques (à ajuster selon vos besoins) dans `../resource/` :
    - `langue_fr.txt`
    - `langue_en.txt`

## Utilisation
1. Exécutez le programme :
   ```bash
   ./analyseur
   ```

2. Choisissez la langue de l'interface :
    - `fr` pour le français.
    - `en` pour l'anglais.

3. Fournissez les chemins des fichiers d'entrée et de sortie lorsque demandé.

4. Naviguez dans le menu pour accéder aux différentes fonctionnalités :
    - **1** : Comptage des lignes.
    - **2** : Comptage des mots.
    - **3** : Comptage des caractères.
    - **4** : Affichage des mots les plus fréquents.
    - **5** : Recherche de mots palindromes.
    - **6** : Comparaison entre deux fichiers texte.
    - **7** : Analyse des phrases.
    - **8** : Sauvegarde des résultats d'analyse.
    - **9** : Changer le fichier d'entrée.
    - **10** : Ouvrir le fichier de sortie.
    - **11** : Quitter.

## Arborescence des fichiers
- `main.c` : Point d'entrée du programme.
- `analyseur.h` : Header définissant les structures et fonctions utilisées.
- `analyseur.c` : Implémentation des fonctions d'analyse et de gestion des fichiers.
- `langue.h` et `langue.c` : Gestion des ressources multilingues.
- `../resource/` : Contient les fichiers de langue (à placer à cet emplacement).

## Exemple de fichier de langue
Un fichier de langue suit le format clé=valeur. Exemple pour `langue_fr.txt` :
```
entrer_ChemEntree=Entrez le chemin du fichier d'entrée :
entrer_ChemSortie=Entrez le chemin du fichier de sortie :
menuAff_Ouverture=Menu principal :
menuAff_CompterLignes=Compter le nombre de lignes
...
```

## Auteur
Développé dans le cadre d'un projet étudiant en C.
Par DUBOIS Esteban, EL GARHI Myriam, BUCAK Samet
