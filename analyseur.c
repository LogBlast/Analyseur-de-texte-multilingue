// V1.03 : 18/12/24~12:00
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "analyseur.h"
#include "langue.h"
#include <limits.h>
#include <ctype.h>

#define TAILLE_MAX_MOTS 1000

// NE PAS redéfinir fichierLangue ici !
// Cette variable est définie une seule fois dans langue.c

// Cette fonction ouvre un fichier en lecture de manière sécurisée.
// Elle demande un chemin valide tant que l'ouverture échoue.
FILE* ouvrirFichierLecture(const char* chemin) {
    FILE* fichier = NULL;
    char cheminBuffer[256];
    strcpy(cheminBuffer, chemin);
    while ((fichier = fopen(cheminBuffer, "r")) == NULL) {
        printf("%s \n",getTextLangue("fonct_OuvrirFichier_Erreur"));
        scanf("%255s", cheminBuffer);
    }
    return fichier;
}


// Cette fonction compte le nombre de lignes dans un fichier donné.
// Elle utilise `fgets` pour parcourir chaque ligne.
int compterLignes(FILE *fichier) {
    int lignes = 0;
    char ligne[1024];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        lignes++;
    }
    rewind(fichier);
    return lignes;
}


// Ajoute un mot au tableau des mots ou incrémente sa fréquence s'il existe déjà.
// Utilise une recherche linéaire pour trouver le mot dans le tableau.
void ajouterMotOuIncrementer(char *mot, struct Mot *tableauMots, int *nombreMots) {
    for (int i = 0; i < *nombreMots; i++) {
        if (strcmp(tableauMots[i].mot, mot) == 0) {
            tableauMots[i].frequence++;
            return;
        }
    }
    strcpy(tableauMots[*nombreMots].mot, mot);
    tableauMots[*nombreMots].frequence = 1;
    (*nombreMots)++;
}


// Calcule la fréquence des mots dans un fichier.
// Elle considère les caractères alphanumériques pour construire les mots.
void calculerFrequenceMots(FILE *fichier, struct Mot *tableauMots, int *nombreMots) {
    char mot[MAX_LONGUEUR_MOT];
    int index = 0;
    char c;

    while ((c = fgetc(fichier)) != EOF) {
        if (isalnum((unsigned char)c)) {
            mot[index++] = tolower((unsigned char)c);
            if (index >= MAX_LONGUEUR_MOT - 1) {
                mot[index] = '\0';
                ajouterMotOuIncrementer(mot, tableauMots, nombreMots);
                index = 0;
            }
        } else if (index > 0) {
            mot[index] = '\0';
            ajouterMotOuIncrementer(mot, tableauMots, nombreMots);
            index = 0;
        }
    }

    if (index > 0) {
        mot[index] = '\0';
        ajouterMotOuIncrementer(mot, tableauMots, nombreMots);
    }

    rewind(fichier);
}


// Cette fonction calcule le nombre de mots dans un fichier.
// Elle utilise une logique pour détecter les transitions entre mots.
int compterMots(FILE *fichier) {
    int mots = 0;
    char c;
    int estDansMot = 0;

    while ((c = fgetc(fichier)) != EOF) {
        if (isalnum((unsigned char)c)) {
            if (!estDansMot) {
                mots++;
                estDansMot = 1;
            }
        } else {
            estDansMot = 0;
        }
    }
    rewind(fichier);
    return mots;
}


// Compte les caractères non-espaces dans un fichier.
int compterCaracteres(FILE *fichier) {

    int compteur = 0;
    int car;

    // Lecture du fichier
    while ((car = fgetc(fichier)) != EOF) {
        if (!isspace(car)) { // Vérifie si ce n'est pas un espace, tabulation ou saut de ligne
            compteur++;
        }
    }

    rewind(fichier); // Remettre le fichier au début
    return compteur;

}

void trierMotsParFrequence(struct Mot* tableauMots, int nombreMots) {
    for (int i = 0; i < nombreMots - 1; i++) {
        for (int j = 0; j < nombreMots - i - 1; j++) {
            if (tableauMots[j].frequence < tableauMots[j + 1].frequence) {
                struct Mot temp = tableauMots[j];
                tableauMots[j] = tableauMots[j + 1];
                tableauMots[j + 1] = temp;
            }
        }
    }
}

void afficherMotsFrequents(struct Mot* tableauMots, int nombreMots, int topN) {
    if (topN > nombreMots) {
        topN = nombreMots;
    }

    printf("%d %s \n",topN,getTextLangue("fonct_afficherMotsFrequents_aff"));

    for (int i = 0; i < topN; i++) {
        printf("%s : %d %s\n", tableauMots[i].mot, tableauMots[i].frequence,getTextLangue("fonct_afficherMotsFrequents_fois"));
    }
}


// Enregistre les résultats d'une analyse textuelle dans un fichier.
// Les statistiques incluent les lignes, mots, caractères, et les mots fréquents.
void sauvegarderResultats(const char* cheminSortie, int nombreLignes, int nombreMots, int nombreCaracteres,
                          struct Mot* tableauMots, int nombreMotsDistincts,
                          float longueurMoyenne, const char* phrasePlusLongue, const char* phrasePlusCourte , char **listePalindromes) {
    FILE *fichierSortie = fopen(cheminSortie, "w");
    if (fichierSortie == NULL) {
        printf("%s %s\n", getTextLangue("fonct_sauvegarderResultats_erreurOuverture"), cheminSortie);
        return;
    }

    fprintf(fichierSortie, "%s \n", getTextLangue("fonct_sauvegarderResultats_sauv_Aff"));
    fprintf(fichierSortie, "\n");
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_sauvegarderResultats_sauv_Lignes"), nombreLignes);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_sauvegarderResultats_sauv_Mots"), nombreMots);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_sauvegarderResultats_sauv_Caracteres"), nombreCaracteres);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_sauvegarderResultats_sauv_MotsDistincts"),
            nombreMotsDistincts);

    fprintf(fichierSortie, "%s : %.2f\n", getTextLangue("analysePhrases_longueurMoyenne"), longueurMoyenne);
    fprintf(fichierSortie, "%s : %s\n", getTextLangue("analysePhrases_phrasePlusLongue"), phrasePlusLongue);
    fprintf(fichierSortie, "%s : %s\n", getTextLangue("analysePhrases_phrasePlusCourte"), phrasePlusCourte);
    fprintf(fichierSortie, "\n");

    fprintf(fichierSortie, "%s \n", getTextLangue("fonct_sauvegarderResultats_sauv_MotsFrequents"));
    for (int i = 0; i < nombreMotsDistincts; i++) {
        fprintf(fichierSortie, "%s : %d\n", tableauMots[i].mot, tableauMots[i].frequence);
    }

    fprintf(fichierSortie, "\n");
    fprintf(fichierSortie, "%s \n", getTextLangue("fonct_sauvegarderResultats_sauv_Palindromes"));

    if (listePalindromes != NULL) {
        for (int i = 0; listePalindromes[i] != NULL; i++) {
            fprintf(fichierSortie, "%s\n", listePalindromes[i]);
        }
    }else {
        fprintf(fichierSortie, "%s \n", getTextLangue("result_palindrome_no"));
    }

    fprintf(fichierSortie, "\n");

    fprintf(fichierSortie, "%s \n", getTextLangue("fonct_sauvegarderResultats_sauv_credits"));

    fclose(fichierSortie);

    printf("%s %s\n", getTextLangue("fonct_sauvegarderResultats_affSauv"), cheminSortie);
}


// Compare les mots de deux fichiers texte.
// Identifie les mots communs et les mots uniques à chaque fichier.
// Remplit trois tableaux : mots communs, mots uniques au fichier 1, mots uniques au fichier 2.
void comparaisonMots(struct Mot* tableauMots1, int nombreMots1, struct Mot* tableauMots2, int nombreMots2,
                     struct Mot* motsCommuns, int* nombreMotsCommuns,
                     struct Mot* motsUniques1, int* nombreMotsUniques1,
                     struct Mot* motsUniques2, int* nombreMotsUniques2) {
    *nombreMotsCommuns = 0;
    *nombreMotsUniques1 = 0;
    *nombreMotsUniques2 = 0;

    for (int i = 0; i < nombreMots1; i++) {
        int found = 0;
        for (int j = 0; j < nombreMots2; j++) {
            if (strcmp(tableauMots1[i].mot, tableauMots2[j].mot) == 0) {
                motsCommuns[*nombreMotsCommuns] = tableauMots1[i];
                (*nombreMotsCommuns)++;
                found = 1;
                break;
            }
        }
        if (!found) {
            motsUniques1[*nombreMotsUniques1] = tableauMots1[i];
            (*nombreMotsUniques1)++;
        }
    }

    for (int j = 0; j < nombreMots2; j++) {
        int found = 0;
        for (int i = 0; i < nombreMots1; i++) {
            if (strcmp(tableauMots2[j].mot, tableauMots1[i].mot) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            motsUniques2[*nombreMotsUniques2] = tableauMots2[j];
            (*nombreMotsUniques2)++;
        }
    }
}

void sauvegarderComparaison(const char* cheminSortie,
                            int nombreLignes1, int nombreMots1, int nombreCaracteres1, struct Mot* tableauMots1, int nombreMotsDistincts1,
                            int nombreLignes2, int nombreMots2, int nombreCaracteres2, struct Mot* tableauMots2, int nombreMotsDistincts2,
                            struct Mot* motsCommuns, int nombreMotsCommuns,
                            struct Mot* motsUniques1, int nombreMotsUniques1,
                            struct Mot* motsUniques2, int nombreMotsUniques2) {
    FILE* fichierSortie = fopen(cheminSortie, "w");
    if (fichierSortie == NULL) {
        printf("%s %s\n", getTextLangue("fonct_sauvegarderResultats_erreurOuverture"), cheminSortie);
        return;
    }

    fprintf(fichierSortie, "%s\n", getTextLangue("fonct_MenuComp_StatsFichier1"));
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbLignes"), nombreLignes1);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbMots"), nombreMots1);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbCaracteres"), nombreCaracteres1);
    fprintf(fichierSortie, "%s %d\n\n", getTextLangue("fonct_MenuComp_nbMotsDistincts"), nombreMotsDistincts1);

    fprintf(fichierSortie, "%s\n", getTextLangue("fonct_MenuComp_StatsFichier2"));
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbLignes"), nombreLignes2);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbMots"), nombreMots2);
    fprintf(fichierSortie, "%s %d\n", getTextLangue("fonct_MenuComp_nbCaracteres"), nombreCaracteres2);
    fprintf(fichierSortie, "%s %d\n\n", getTextLangue("fonct_MenuComp_nbMotsDistincts"), nombreMotsDistincts2);

    fprintf(fichierSortie, "%s (%d %s) :\n", getTextLangue("fonct_MenuComp_nbMotsCommuns"), nombreMotsCommuns, getTextLangue("fonct_MenuComp_nbMots"));
    for (int i = 0; i < nombreMotsCommuns; i++) {
        int frequence2 = 0;
        for (int j = 0; j < nombreMotsDistincts2; j++) {
            if (strcmp(motsCommuns[i].mot, tableauMots2[j].mot) == 0) {
                frequence2 = tableauMots2[j].frequence;
                break;
            }
        }
        fprintf(fichierSortie, "%s : %s - %d %s, %s - %d %s\n",
                motsCommuns[i].mot, getTextLangue("fonct_MenuComp_StatsFichier1"), motsCommuns[i].frequence, getTextLangue("fonct_afficherMotsFrequents_fois"),
                getTextLangue("fonct_MenuComp_StatsFichier2"), frequence2, getTextLangue("fonct_afficherMotsFrequents_fois"));
    }
    fprintf(fichierSortie, "\n");

    fprintf(fichierSortie, "%s (%d %s) \n", getTextLangue("fonct_MenuComp_nbMotsUniqueFichier1"), nombreMotsUniques1, getTextLangue("fonct_MenuComp_nbMots"));
    for (int i = 0; i < nombreMotsUniques1; i++) {
        fprintf(fichierSortie, "%s : %d %s\n", motsUniques1[i].mot, motsUniques1[i].frequence, getTextLangue("fonct_afficherMotsFrequents_fois"));
    }
    fprintf(fichierSortie, "\n");

    fprintf(fichierSortie, "%s (%d %s) :\n", getTextLangue("fonct_MenuComp_nbMotsUniqueFichier2"), nombreMotsUniques2, getTextLangue("fonct_MenuComp_nbMots"));
    for (int i = 0; i < nombreMotsUniques2; i++) {
        fprintf(fichierSortie, "%s %d %s\n", motsUniques2[i].mot, motsUniques2[i].frequence, getTextLangue("fonct_afficherMotsFrequents_fois"));
    }
    fprintf(fichierSortie, "\n");

    fclose(fichierSortie);

    printf("%s %s\n", getTextLangue("fonct_sauvegarderResultats_affSauv"), cheminSortie);
}

void menuComparaison() {
    char cheminFichier1[256];
    char cheminFichier2[256];
    char cheminSortie[256];

    int nombreLignes1, nombreMots1, nombreCaracteres1, nombreMotsDistincts1 = 0;
    struct Mot tableauMots1[TAILLE_MAX_MOTS];

    int nombreLignes2, nombreMots2, nombreCaracteres2, nombreMotsDistincts2 = 0;
    struct Mot tableauMots2[TAILLE_MAX_MOTS];

    struct Mot motsCommuns[TAILLE_MAX_MOTS];
    int nombreMotsCommuns = 0;
    struct Mot motsUniques1[TAILLE_MAX_MOTS];
    int nombreMotsUniques1 = 0;
    struct Mot motsUniques2[TAILLE_MAX_MOTS];
    int nombreMotsUniques2 = 0;

    printf("%s ", getTextLangue("menuAff_Comp_Fichier1"));
    scanf("%s", cheminFichier1);
    printf("%s ", getTextLangue("menuAff_Comp_Fichier2"));
    scanf("%s", cheminFichier2);
    printf("%s ", getTextLangue("menuAff_Comp_FichierSortie"));
    scanf("%s", cheminSortie);

    // Analyse du premier fichier.
    FILE* fichier1 = ouvrirFichierLecture(cheminFichier1);
    nombreLignes1 = compterLignes(fichier1);
    nombreMots1 = compterMots(fichier1);
    nombreCaracteres1 = compterCaracteres(fichier1);
    calculerFrequenceMots(fichier1, tableauMots1, &nombreMotsDistincts1);
    trierMotsParFrequence(tableauMots1, nombreMotsDistincts1);
    fclose(fichier1);

    // Analyse du second fichier.
    FILE* fichier2 = ouvrirFichierLecture(cheminFichier2);
    nombreLignes2 = compterLignes(fichier2);
    nombreMots2 = compterMots(fichier2);
    nombreCaracteres2 = compterCaracteres(fichier2);
    calculerFrequenceMots(fichier2, tableauMots2, &nombreMotsDistincts2);
    trierMotsParFrequence(tableauMots2, nombreMotsDistincts2);
    fclose(fichier2);

    // Comparaison des mots entre les deux fichiers.
    comparaisonMots(tableauMots1, nombreMotsDistincts1, tableauMots2, nombreMotsDistincts2,
                    motsCommuns, &nombreMotsCommuns, motsUniques1, &nombreMotsUniques1, motsUniques2,
                    &nombreMotsUniques2);

    printf("\n%s\n", getTextLangue("fonct_MenuComp_StatsFichier1"));
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbLignes"), nombreLignes1);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMots"), nombreMots1);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbCaracteres"), nombreCaracteres1);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMotsDistincts"), nombreMotsDistincts1);

    printf("\n%s\n", getTextLangue("fonct_MenuComp_StatsFichier2"));
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbLignes"), nombreLignes2);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMots"), nombreMots2);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbCaracteres"), nombreCaracteres2);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMotsDistincts"), nombreMotsDistincts2);

    printf("\n%s %d\n", getTextLangue("fonct_MenuComp_nbMotsCommuns"), nombreMotsCommuns);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMotsUniqueFichier1"), nombreMotsUniques1);
    printf("%s %d\n", getTextLangue("fonct_MenuComp_nbMotsUniqueFichier2"), nombreMotsUniques2);

    sauvegarderComparaison(cheminSortie,
                           nombreLignes1, nombreMots1, nombreCaracteres1, tableauMots1, nombreMotsDistincts1,
                           nombreLignes2, nombreMots2, nombreCaracteres2, tableauMots2, nombreMotsDistincts2,
                           motsCommuns, nombreMotsCommuns,
                           motsUniques1, nombreMotsUniques1,
                           motsUniques2, nombreMotsUniques2);
}

// Vérifie si un mot donné est un palindrome (se lit de la même manière dans les deux sens).
// Renvoie 1 si c'est un palindrome, sinon 0.
int estPalindrome(const char* mot) {
    int taille = (int)strlen(mot);
    if (taille < 3) {
        return 0;
    }
    for (int i = 0, j = taille - 1; i < j; i++, j--) {
        if (mot[i] != mot[j]) {
            return 0;
        }
    }
    return 1;
}


// Parcourt un fichier pour trouver et afficher tous les mots qui sont des palindromes.
void afficherPalindromes(FILE* fichier) {
    char mot[256];
    int totalPalindromes = 0;

    while (fscanf(fichier, "%s", mot) == 1) {
        if (estPalindrome(mot)) {
            //printf("Palindrome trouvé : %s\n", mot);
            printf("%s %s \n",getTextLangue("result_palindrome_yes"), mot);
            totalPalindromes++;
        }
    }

    if (totalPalindromes == 0) {
        printf("%s \n",getTextLangue("result_palindrome_no"));
    }

    rewind(fichier);
}

// Identifie les palindromes dans un fichier sans les afficher immédiatement.
// Retourne un tableau dynamique contenant les palindromes trouvés.
char** calculerPalindrome(FILE* fichier) {
    char mot[256];
    int totalPalindromes = 0;
    char** palindromes = malloc(1000 * sizeof(char*));;

    while (fscanf(fichier, "%s", mot) == 1) {
        if (estPalindrome(mot)) {
            palindromes[totalPalindromes] = strdup(mot);
            totalPalindromes++;
        }
    }

    if (totalPalindromes == 0) {
        return NULL;
    }

    rewind(fichier);
    palindromes[totalPalindromes] = NULL;
    return palindromes;
}

float calculerLongueurMoyennePhrases(FILE* fichier) {
    char ligne[1024];
    int totalCaracteres = 0;
    int totalPhrases = 0;
    const char* delimiters = ".!?";

    rewind(fichier);

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char* token = strtok(ligne, delimiters);
        while (token != NULL) {
            while (isspace((unsigned char)*token)) token++;
            char* end = token + strlen(token) - 1;
            while (end > token && isspace((unsigned char)*end)) {
                *end = '\0';
                end--;
            }

            if (strlen(token) > 0) {
                totalCaracteres += (int)strlen(token);
                totalPhrases++;
            }

            token = strtok(NULL, delimiters);
        }
    }

    rewind(fichier);

    return (totalPhrases > 0) ? (float)totalCaracteres / totalPhrases : 0.0;
}


// Trouve la phrase la plus longue et la plus courte dans un fichier.
// Les phrases sont identifiées par les délimiteurs '.', '!', et '?'.
void chercherPhrasesExtremes(FILE* fichier, char* phrasePlusLongue, size_t tailleLongue,
                             char* phrasePlusCourte, size_t tailleCourte) {
    char ligne[1024];
    const char* delimiters = ".!?";
    int longueurMax = 0;
    int longueurMin = INT_MAX;

    phrasePlusLongue[0] = '\0';
    phrasePlusCourte[0] = '\0';

    rewind(fichier);

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char* token = strtok(ligne, delimiters);
        while (token != NULL) {
            while (isspace((unsigned char)*token)) token++;
            char* end = token + strlen(token) - 1;
            while (end > token && isspace((unsigned char)*end)) {
                *end = '\0';
                end--;
            }

            int longueur = (int)strlen(token);
            if (longueur > 0) {
                if (longueur > longueurMax) {
                    longueurMax = longueur;
                    strncpy(phrasePlusLongue, token, tailleLongue - 1);
                    phrasePlusLongue[tailleLongue - 1] = '\0';
                }

                if (longueur < longueurMin) {
                    longueurMin = longueur;
                    strncpy(phrasePlusCourte, token, tailleCourte - 1);
                    phrasePlusCourte[tailleCourte - 1] = '\0';
                }
            }

            token = strtok(NULL, delimiters);
        }
    }

    rewind(fichier);
}

void analyserPhrases(FILE* fichier) {
    char phrasePlusLongue[1024] = "";
    char phrasePlusCourte[1024] = "";

    float longueurMoyenne = calculerLongueurMoyennePhrases(fichier);
    chercherPhrasesExtremes(fichier, phrasePlusLongue, sizeof(phrasePlusLongue), phrasePlusCourte,
                            sizeof(phrasePlusCourte));

    printf("%s : %.2f\n", getTextLangue("analysePhrases_longueurMoyenne"), longueurMoyenne);
    printf("%s : %s\n", getTextLangue("analysePhrases_phrasePlusLongue"), phrasePlusLongue);
    printf("%s : %s\n", getTextLangue("analysePhrases_phrasePlusCourte"), phrasePlusCourte);
}


void ouvrirFichierAvecEditeurParDefaut(const char* chemin) {
    char commande[512];
    snprintf(commande, sizeof(commande), "notepad %s", chemin);
    system(commande);
}
