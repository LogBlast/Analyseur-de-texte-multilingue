// V1.03 : 18/12/24~12:00
#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <stdio.h>

#define MAX_MOTS 1000
#define MAX_LONGUEUR_MOT 50

struct Mot {
    char mot[MAX_LONGUEUR_MOT];
    int frequence;
};

// Plus de déclaration globale de fichierLangue ici.
// Elle est gérée dans langue.h/langue.c.

// Déclaration des fonctions de comptage
int compterMots(FILE *fichier);
int compterCaracteres(FILE *fichier);
int compterLignes(FILE* fichier);

// Déclaration de la fonction pour ouvrir un fichier
FILE* ouvrirFichierLecture(const char* chemin);

// Fonctions pour calculer la fréquence
void calculerFrequenceMots(FILE *fichier, struct Mot *tableauMots, int *nombreMots);
void ajouterMotOuIncrementer(char *mot, struct Mot *tableauMots, int *nombreMots);

// Fonctions pour trier et afficher
void trierMotsParFrequence(struct Mot* tableauMots, int nombreMots);
void afficherMotsFrequents(struct Mot* tableauMots, int nombreMots, int topN);

// Fonction pour sauvegarder les résultats
void sauvegarderResultats(const char* cheminSortie, int nombreLignes, int nombreMots, int nombreCaracteres,
                          struct Mot* tableauMots, int nombreMotsDistincts,
                          float longueurMoyenne, const char* phrasePlusLongue, const char* phrasePlusCourte, char **listePalindromes);

// Fonctions de comparaison
void comparaisonMots(struct Mot* tableauMots1, int nombreMots1, struct Mot* tableauMots2, int nombreMots2,
                     struct Mot* motsCommuns, int* nombreMotsCommuns,
                     struct Mot* motsUniques1, int* nombreMotsUniques1,
                     struct Mot* motsUniques2, int* nombreMotsUniques2);

void sauvegarderComparaison(const char* cheminSortie,
                            int nombreLignes1, int nombreMots1, int nombreCaracteres1, struct Mot* tableauMots1, int nombreMotsDistincts1,
                            int nombreLignes2, int nombreMots2, int nombreCaracteres2, struct Mot* tableauMots2, int nombreMotsDistincts2,
                            struct Mot* motsCommuns, int nombreMotsCommuns,
                            struct Mot* motsUniques1, int nombreMotsUniques1,
                            struct Mot* motsUniques2, int nombreMotsUniques2);

void menuComparaison();

// Palindromes
int estPalindrome(const char* mot);
void afficherPalindromes(FILE* fichier);
char** calculerPalindrome(FILE* fichier);

// Analyse de phrases
void analyserPhrases(FILE* fichier);
float calculerLongueurMoyennePhrases(FILE* fichier);
void chercherPhrasesExtremes(FILE* fichier, char* phrasePlusLongue, size_t tailleLongue, char* phrasePlusCourte, size_t tailleCourte);

void ouvrirFichierAvecEditeurParDefaut(const char* chemin);

#endif // ANALYSEUR_H
