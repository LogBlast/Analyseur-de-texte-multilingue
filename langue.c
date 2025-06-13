//
// V1.0 : 11/12-11.05
//

#include "langue.h"

#include <stdio.h>
#include <string.h>

FILE* fichierLangue;

// Fonction pour obtenir le texte associé à une clé dans le fichier de langue
char* getTextLangue(char* key) {
    static char value[256];
    char ligne[256]; // lire chaque ligne du fichier de langue

    rewind(fichierLangue); // Réinitialiser le pointeur de fichier au début du fichier

    // Lire chaque ligne du fichier de langue
    while (fgets(ligne, sizeof(ligne), fichierLangue)) {
        // Vérifier si la ligne commence par la clé
        if (strncmp(ligne, key, strlen(key)) == 0) {
            char* equalSign = strchr(ligne, '='); // Trouver le caractère '=' dans la ligne
            if (equalSign) {
                strcpy(value, equalSign + 1); // Copier la valeur après '='
                value[strcspn(value, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la valeur
                return value; // Retourner la valeur associée à la clé
            }
        }
    }
    return NULL; // Retourner NULL si la clé n'est pas trouvée
}