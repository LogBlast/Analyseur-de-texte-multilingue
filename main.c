// V1.03 : 19/12/24~16:00

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur.h"
#include "langue.h"

#define TAILLE_MAX_MOTS 1000

char cheminFichier[256];
char cheminSortie[256];

int nombreLignes;
int nombreMots;
int nombreCaracteres;
int nombreMotsDistincts = 0;
struct Mot tableauMots[TAILLE_MAX_MOTS]; // Tableau de mots

int main() {
    setlocale(LC_ALL, "fr_FR.UTF-8");
    system("chcp 65001");

    // Choix de la langue
    char langue[5];

    // Choisir la langue (fr pour français, en pour anglais)
    printf("Choisissez la langue (fr pour français, en pour anglais) : ");
    scanf("%s", langue);

    if (strcmp(langue, "fr") == 0) {
        fichierLangue = ouvrirFichierLecture("../resource/langue_fr.txt");
    } else {
        fichierLangue = ouvrirFichierLecture("../resource/langue_en.txt");
    }

    // Demande à l'utilisateur de saisir le chemin du fichier à analyser
    printf("%s ", getTextLangue("entrer_ChemEntree"));
    scanf("%255s", cheminFichier);

    // Demander à l'utilisateur le chemin du fichier de sortie
    printf("%s ", getTextLangue("entrer_ChemSortie"));
    scanf("%255s", cheminSortie);

    // Ouvre le fichier en mode lecture
    FILE* fichier = ouvrirFichierLecture(cheminFichier);
    if (fichier == NULL) {
        // La fonction ouvrirFichierEnLecture gère déjà l'affichage de l'erreur
        return EXIT_FAILURE;
    }
    printf("%s \n",getTextLangue("succes_Ouverture"));

    // Menu
    int choix;
    do {
        printf("\n");
        printf("%s \n", getTextLangue("menuAff_Ouverture"));
        printf("1. %s \n", getTextLangue("menuAff_CompterLignes"));
        printf("2. %s \n", getTextLangue("menuAff_CompterMots"));
        printf("3. %s \n", getTextLangue("menuAff_CompterCaracteres"));
        printf("4. %s \n", getTextLangue("menuAff_MotsFrequents"));
        printf("5. %s \n", getTextLangue("menuAff_Palindromes"));
        printf("6. %s \n", getTextLangue("menuAff_Comparaison"));
        printf("7. %s \n", getTextLangue("menuAff_AnalysePhrases"));
        printf("8. %s \n", getTextLangue("menuAff_SauvResultats"));
        printf("9. %s \n", getTextLangue("menuAff_ChangerFichierEntree"));
        printf("10. %s \n", getTextLangue("menuAff_ouvrirFichierSortie"));
        printf("11. %s \n", getTextLangue("menuAff_Quitter"));

        printf("%s ", getTextLangue("menuAff_Choix"));
        scanf("%d", &choix);
        printf("\n");

        switch (choix) {
            case 1:
                // Compter les lignes dans le fichier
                nombreLignes = compterLignes(fichier);
                printf("%s %d\n", getTextLangue("switchAff_NbLignes") , nombreLignes);
                break;
            case 2:
                // Compter les mots dans le fichier
                nombreMots = compterMots(fichier);
                printf("%s %d\n", getTextLangue("switchAff_NbMots") , nombreMots);
                break;
            case 3:
                // Compter les caractères dans le fichier
                nombreCaracteres = compterCaracteres(fichier);
                printf("%s %d\n", getTextLangue("switchAff_NbCaracteres") , nombreCaracteres);
                break;
            case 4:
                // Calculer la fréquence des mots
                calculerFrequenceMots(fichier, tableauMots, &nombreMotsDistincts);
                // Trier les mots par fréquence décroissante
                trierMotsParFrequence(tableauMots, nombreMotsDistincts);
                // Afficher les mots les plus fréquents
                afficherMotsFrequents(tableauMots, nombreMotsDistincts, 10);
                break;
            case 5:
                // Afficher les mots palindromes
                afficherPalindromes(fichier);
                break;
            case 6:
                menuComparaison();
                break;
            case 7:
                printf("%s \n",getTextLangue("switchAff_Analyser"));
                analyserPhrases(fichier);
                break;
            case 8:
                nombreLignes = compterLignes(fichier);
                nombreMots = compterMots(fichier);
                nombreCaracteres = compterCaracteres(fichier);
                calculerFrequenceMots(fichier, tableauMots, &nombreMotsDistincts);
                trierMotsParFrequence(tableauMots, nombreMotsDistincts);

                char **listePalindromes = calculerPalindrome(fichier);

                // Analyse des phrases pour la sauvegarde
                {
                    float longueurMoyenne = calculerLongueurMoyennePhrases(fichier);
                    char phrasePlusLongue[1024];
                    char phrasePlusCourte[1024];
                    chercherPhrasesExtremes(fichier, phrasePlusLongue, sizeof(phrasePlusLongue), phrasePlusCourte,
                                            sizeof(phrasePlusCourte));

                    sauvegarderResultats(cheminSortie, nombreLignes, nombreMots, nombreCaracteres,
                                         tableauMots, nombreMotsDistincts,
                                         longueurMoyenne, phrasePlusLongue, phrasePlusCourte, listePalindromes);
                }
                break;
            case 9:
                fclose(fichier);
                printf("%s ",getTextLangue("switchAff_Chemin"));
                scanf("%255s", cheminFichier);
                fichier = ouvrirFichierLecture(cheminFichier);
                nombreLignes = 0;
                nombreMots = 0;
                nombreCaracteres = 0;
                nombreMotsDistincts = 0;
                break;
            case 10:
                printf("%s \n", getTextLangue("switchAff_ouvertureFichierSortie"));
                ouvrirFichierAvecEditeurParDefaut(cheminSortie);
                break;
            case 11:
                printf("%s \n", getTextLangue("switchAff_Quitter"));
                fclose(fichier);
                break;
            default:
                printf("%s \n", getTextLangue("switchAff_Erreur"));
                while (getchar() != '\n'); // Vider le tampon
                break;
        }
    } while (choix != 11);

    return EXIT_SUCCESS;
}
