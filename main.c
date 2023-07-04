#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Réalisé par Esteban H.
// Ce code est entièrement fonctionnel sur n'importe quel système d'exploitation

// Define des éléments servants à mettre un define au milieu d'une chaine de caractère
#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define STRINGIFY_HELPER(x) #x
// Define de la taille maximum d'une chaine de caractères
#define MAX_CHAR_SIZE 99

// Déclaration des structures (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
typedef struct member {
    int id;
    char firstName[MAX_CHAR_SIZE + 1];
    char lastName[MAX_CHAR_SIZE + 1];
    char email[MAX_CHAR_SIZE + 1];
} member_t;
typedef struct book {
    int id;
    char title[MAX_CHAR_SIZE + 1];
    char author[MAX_CHAR_SIZE + 1];
    char publishingHouse[MAX_CHAR_SIZE + 1];
    int quantity;
    int availableQuantity;
} book_t;
typedef struct loan {
    int id;
    int memberId;
    int bookId;
    long long startDate;
    long long endDate;
    bool wasReturned;
} loan_t;

// Prototypes
int spaceSelector(member_t**, int*, book_t**, int*, loan_t**, int*);
int membersOptionSelector(member_t**, int*, book_t**, int*, loan_t**, int*);
int booksOptionSelector(member_t**, int*, book_t**, int*, loan_t**, int*);
int loansOptionSelector(member_t**, int*, book_t**, int*, loan_t**, int*);
void addMember(member_t**, int*);
void removeMember(member_t**, int*);
void editMember(member_t**, int);
void searchMember(member_t*, int);
void showMembers(member_t*, int);
void showMember(member_t*, int);
int getIndexFromMemberId(member_t*, int, int);
bool readMembersFile(member_t**, int*);
bool writeMembersFile(member_t*, int);
void addBook(book_t**, int*);
void removeBook(book_t**, int*);
void editBook(book_t**, int);
void showBooks(book_t*, int);
void showBook(book_t*, int);
void searchBook(book_t*, int);
int getIndexFromBookId(book_t*, int, int);
bool readBooksFile(book_t**, int*);
bool writeBooksFile(book_t*, int);
void addLoan(loan_t**, int*, member_t*, int, book_t**, int);
void removeLoan(loan_t**, int*, book_t**, int);
void editLoan(loan_t**, int, member_t*, int, book_t*, int);
void searchLoan(loan_t*, int, member_t*, int, book_t*, int);
void makeReturned(loan_t**, int, book_t**, int);
void showLoans(loan_t*, int, member_t*, int, book_t*, int);
void showLoan(loan_t*, int, member_t*, int, book_t*, int);
void showBooksLoanedByAMember(loan_t*, int, book_t*, int);
void showMembersWhoHaveLoanedABook(loan_t*, int, member_t*, int);
int getIndexFromLoanId(loan_t*, int, int);
bool readLoansFile(loan_t**, int*);
bool writeLoansFile(loan_t*, int);
void flushStdin();
bool isMadeUpOfNumbers(char*);
char* toLowerCase(char*);

int main() {
    int membersSize = 0; // Taille initiale pour les membres
    member_t* members = NULL; // Tableau pour les membres
    int booksSize = 0; // Taille initiale pour les livres
    book_t* books = NULL; // Tableau pour livres
    int loansSize = 0; // Taille initiale pour les emprunts
    loan_t* loans = NULL; // Tableau pour les emprunts

    // Lecture des membres dans le fichier
    if (!readMembersFile(&members, &membersSize)) {
        return -1;
    }
    // Lecture des livres dans le fichier
    if (!readBooksFile(&books, &booksSize)) {
        return -1;
    }
    // Lecture des emprunts dans le fichier
    if (!readLoansFile(&loans, &loansSize)) {
        return -1;
    }

    // Sélection des catégories
    int code = spaceSelector(&members, &membersSize, &books, &booksSize, &loans, &loansSize);

    // Écriture des membres dans le fichier
    if (!writeMembersFile(members, membersSize)) {
        return -1;
    }
    // Écriture des livres dans le fichier
    if (!writeBooksFile(books, booksSize)) {
        return -1;
    }
    // Écriture des emprunts dans le fichier
    if (!writeLoansFile(loans, loansSize)) {
        return -1;
    }

    // Libérer la mémoire allouée pour les membres
    free(members);
    // Libérer la mémoire allouée pour les livres
    free(books);
    // Libérer la mémoire allouée pour les emprunts
    free(loans);

    return code;
}

int spaceSelector(member_t** members, int* membersSize, book_t** books, int* booksSize, loan_t** loans, int* loansSize) {
    // Choix de l'espace
    printf("Choisissez l'espace que vous souhaitez\n1) Membres\n2) Livres\n3) Emprunts\n0) Quitter\nVotre choix : ");
    int spaceChoice;
    do {
        scanf("%i", &spaceChoice);
        flushStdin();
        switch (spaceChoice) {
            case 0:
                break;
            case 1:
                return membersOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
            case 2:
                return booksOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
            case 3:
                return loansOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
            default:
                printf("Vous avez selectionne un nombre invalide.\n\n");
                return spaceSelector(members, membersSize, books, booksSize, loans, loansSize);
        }
    } while (spaceChoice != 0);
    return 0;
}

int membersOptionSelector(member_t** members, int* membersSize, book_t** books, int* booksSize, loan_t** loans, int* loansSize) {
    // Choix de l'option
    printf("Choisissez l'option que vous souhaitez\n1) Ajouter un membre\n2) Supprimer un membre\n3) Modifier un membre\n4) Consulter les membres\n5) Consulter un membre\n6) Rechercher un membre\n0) Retour\nVotre choix : ");
    int optionChoice;
    do {
        scanf("%i", &optionChoice);
        flushStdin();
        switch (optionChoice) {
            case 0:
                break;
            case 1:
                addMember(members, membersSize);
                break;
            case 2:
                removeMember(members, membersSize);
                break;
            case 3:
                editMember(members, *membersSize);
                break;
            case 4:
                showMembers(*members, *membersSize);
                break;
            case 5:
                showMember(*members, *membersSize);
                break;
            case 6:
                searchMember(*members, *membersSize);
                break;
            default:
                printf("Vous avez selectionne un nombre invalide.\n\n");
                return membersOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
        }
        break;
    } while (optionChoice != 0);
    return spaceSelector(members, membersSize, books, booksSize, loans, loansSize);;
}

int booksOptionSelector(member_t** members, int* membersSize, book_t** books, int* booksSize, loan_t** loans, int* loansSize) {
    // Choix de l'option
    printf("Choisissez l'option que vous souhaitez\n1) Ajouter un livre\n2) Supprimer un livre\n3) Modifier un livre\n4) Consulter les livres\n5) Consulter un livre\n6) Rechercher un livre\n0) Retour\nVotre choix : ");
    int optionChoice;
    do {
        scanf("%i", &optionChoice);
        flushStdin();
        switch (optionChoice) {
            case 0:
                break;
            case 1:
                addBook(books, booksSize);
                break;
            case 2:
                removeBook(books, booksSize);
                break;
            case 3:
                editBook(books, *booksSize);
                break;
            case 4:
                showBooks(*books, *booksSize);
                break;
            case 5:
                showBook(*books, *booksSize);
                break;
            case 6:
                searchBook(*books, *booksSize);
                break;
            default:
                printf("Vous avez selectionne un nombre invalide.\n\n");
                return booksOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
        }
        break;
    } while (optionChoice != 0);
    return spaceSelector(members, membersSize, books, booksSize, loans, loansSize);
}

int loansOptionSelector(member_t** members, int* membersSize, book_t** books, int* booksSize, loan_t** loans, int* loansSize) {
    // Choix de l'option
    printf("Choisissez l'option que vous souhaitez\n1) Ajouter un emprunt\n2) Supprimer un emprunt\n3) Modifier un emprunt\n4) Consulter les emprunts\n5) Consulter un emprunt\n6) Consulter les livres emprunte par un membre\n7) Consulter les membres qui ont emprunte un livre\n8) Rechercher un emprunt\n9) Confirmer le retour d'un emprunt\n0) Retour\nVotre choix : ");
    int optionChoice;
    do {
        scanf("%i", &optionChoice);
        flushStdin();
        switch (optionChoice) {
            case 0:
                break;
            case 1:
                addLoan(loans, loansSize, *members, *membersSize, books, *booksSize);
                break;
            case 2:
                removeLoan(loans, loansSize, books, *booksSize);
                break;
            case 3:
                editLoan(loans, *loansSize, *members, *membersSize, *books, *booksSize);
                break;
            case 4:
                showLoans(*loans, *loansSize, *members, *membersSize, *books, *booksSize);
                break;
            case 5:
                showLoan(*loans, *loansSize, *members, *membersSize, *books, *booksSize);
                break;
            case 6:
                showBooksLoanedByAMember(*loans, *loansSize, *books, *booksSize);
                break;
            case 7:
                showMembersWhoHaveLoanedABook(*loans, *loansSize, *members, *membersSize);
                break;
            case 8:
                searchLoan(*loans, *loansSize, *members, *membersSize, *books, *booksSize);
                break;
            case 9:
                makeReturned(loans, *loansSize, books, *booksSize);
                break;
            default:
                printf("Vous avez selectionne un nombre invalide.\n\n");
                return loansOptionSelector(members, membersSize, books, booksSize, loans, loansSize);
        }
        break;
    } while (optionChoice != 0);
    return spaceSelector(members, membersSize, books, booksSize, loans, loansSize);
}

void addMember(member_t** members, int* size) {
    // Mise à jour de la mémoire pour correspondre à la taille
    *members = realloc(*members, ((*size) + 1) * sizeof(member_t));
    if (*members != NULL) {
        // Définition de toutes les variables pour créer le membre
        (*members)[*size].id = (*size > 0) ? (*members)[*size - 1].id + 1 : 1;
        printf("Veuillez introduire le prenom : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[*size].firstName);
        flushStdin();
        printf("Veuillez introduire le nom : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[*size].lastName);
        flushStdin();
        printf("Veuillez introduire l'adresse email : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[*size].email);
        flushStdin();
        (*size)++;
        printf("Le membre a ete ajoute avec succes.\n\n");
    } else {
        printf("Erreur de memoire lors de l'ajout du membre.\n\n");
    }
}

void removeMember(member_t** members, int* size) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du membre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromMemberId(*members, *size, atoi(indexString));
    if (index != -1) {
        // Décalage d'une place en arrière de tous les éléments après l'ID
        for (int i = index; i < (*size) - 1; i++) {
            (*members)[i] = (*members)[i + 1];
        }
        (*size)--;
        // Mise à jour de la mémoire pour correspondre à la taille
        *members = realloc(*members, (*size) * sizeof(member_t));
        if (*members != NULL) {
            printf("Le membre a ete supprime avec succes.\n\n");
        } else {
            printf("Erreur de memoire lors de la suppression du membre.\n\n");
        }
    } else {
        printf("L'ID du membre est invalide.\n\n");
    }
}

void editMember(member_t** members, int size) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du membre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromMemberId(*members, size, atoi(indexString));
    if (index != -1) {
        // Définition de toutes les variables pour modifier le membre
        printf("Veuillez introduire le prenom [Valeur precedente : %s] : ", (*members)[index].firstName);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[index].firstName);
        flushStdin();
        printf("Veuillez introduire le nom [Valeur precedente : %s] : ", (*members)[index].lastName);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[index].lastName);
        flushStdin();
        printf("Veuillez introduire l'adresse email [Valeur precedente : %s] : ", (*members)[index].email);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*members)[index].email);
        flushStdin();
        printf("Le membre a ete modifie avec succes.\n\n");
    } else {
        printf("L'ID du membre est invalide.\n\n");
    }
}

void searchMember(member_t* members, int size) {
    // Déclaration du mot-clé (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char keyword[MAX_CHAR_SIZE + 1];
    printf("Veuillez indroduire votre recherche : ");
    scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", keyword);
    flushStdin();
    char* keywordLower = toLowerCase(keyword);
    printf("Resultats de la recherche pour le mot-cle '%s' :\n", keyword);
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("| ID   | Prenom               | Nom                  | Email                          |\n");
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    for (int i = 0; i < size; i++) {
        char* firstNameLower = toLowerCase(members[i].firstName);
        char* lastNameLower = toLowerCase(members[i].lastName);
        char* emailLower = toLowerCase(members[i].email);
        if (strstr(firstNameLower, keywordLower) != NULL || strstr(lastNameLower, keywordLower) != NULL || strstr(emailLower, keywordLower) != NULL) {
            printf("| %-4i | %-20s | %-20s | %-30s |\n", members[i].id, members[i].firstName, members[i].lastName, members[i].email);
        }
    }
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("Pour plus de details, consultez le membre individuellement.\n\n");
}

void showMembers(member_t* members, int size) {
    // Affichage des membres
    printf("Voici la liste des membres :\n");
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("| ID   | Prenom               | Nom                  | Email                          |\n");
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    for (int i = 0; i < size; i++) {
        printf("| %-4i | %-20s | %-20s | %-30s |\n", members[i].id, members[i].firstName, members[i].lastName, members[i].email);
    }
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("Pour plus de details, consultez le membre individuellement.\n\n");
}

void showMember(member_t* members, int size) {
    // Demande de l'ID (le MAX CHAR SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char memberIdString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du membre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", memberIdString);
        flushStdin();
    } while (!isMadeUpOfNumbers(memberIdString));
    int memberIndex = getIndexFromMemberId(members, size, atoi(memberIdString));
    // Affichage du livre
    if (memberIndex != -1) {
        printf("Voici les informations du membre :\n");
        printf("ID : %i\n", members[memberIndex].id);
        printf("Prenom : %s\n", members[memberIndex].firstName);
        printf("Nom : %s\n", members[memberIndex].lastName);
        printf("Adresse email : %s\n", members[memberIndex].email);
        printf("\n");
    } else {
        printf("L'ID du membre est invalide.\n\n");
    }
}

int getIndexFromMemberId(member_t* members, int size, int id) {
    // Regarde à quelle place se trouve l'ID
    for (int i = 0; i < size; i++) {
        if (members[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool readMembersFile(member_t** members, int* size) {
    // Ouverture du fichier
    FILE* file = fopen("members.bin", "rb");
    if (file != NULL) {
        // Définition de la taille
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        *size = fileSize / sizeof(member_t);
        if ((*size) > 0) {
            // Mise à jour de la mémoire pour correspondre à la taille
            *members = realloc(*members, (*size) * sizeof(member_t));
            if (*members != NULL) {
                // Lecture des données dans le fichier
                int elementsWritten = fread(*members, sizeof(member_t), *size, file);
                fclose(file);
                if (elementsWritten != (*size)) {
                    printf("Erreur lors de la lecture des donnees dans le fichier.\n\n");
                    return false;
                }
            } else {
                fclose(file);
                printf("Erreur de memoire lors de la lecture des donnees.\n\n");
                return false;
            }
        }
    }
    return true;
}

bool writeMembersFile(member_t* members, int size) {
    // Ouverture du fichier
    FILE* file = fopen("members.bin", "wb");
    if (file != NULL) {
        // Écriture des données dans le fichier
        int elementsWritten = fwrite(members, sizeof(member_t), size, file);
        fclose(file);
        if (elementsWritten != size) {
            printf("Erreur lors de l'ecriture des donnees dans le fichier.\n\n");
            return false;
        }
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n\n");
        return false;
    }
    return true;
}

void addBook(book_t** books, int* size) {
    // Mise à jour de la mémoire pour correspondre à la taille
    *books = realloc(*books, ((*size) + 1) * sizeof(book_t));
    if (*books != NULL) {
        // Définition de toutes les variables pour créer le livre
        (*books)[*size].id = (*size > 0) ? (*books)[*size - 1].id + 1 : 1;;
        printf("Veuillez introduire le titre : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[*size].title);
        flushStdin();
        printf("Veuillez introduire l'auteur : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[*size].author);
        flushStdin();
        printf("Veuillez introduire la maison d'edition : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[*size].publishingHouse);
        flushStdin();
        // Déclaration de la quantité (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char quantityString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire la quantite (en chiffres) : ");
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", quantityString);
            flushStdin();
        } while (!isMadeUpOfNumbers(quantityString));
        int quantity = atoi(quantityString);
        (*books)[*size].quantity = quantity;
        (*books)[*size].availableQuantity = quantity;
        (*size)++;
        printf("Le livre a ete ajoute avec succes.\n\n");
    } else {
        printf("Erreur de memoire lors de l'ajout du livre.\n\n");
    }
}

void removeBook(book_t** books, int* size) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du livre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromBookId(*books, *size, atoi(indexString));
    if (index != -1) {
        // Décalage d'une place en arrière de tous les éléments après l'ID
        for (int i = index; i < (*size) - 1; i++) {
            (*books)[i] = (*books)[i + 1];
        }
        (*size)--;
        // Mise à jour de la mémoire pour correspondre à la taille
        *books = realloc(*books, (*size) * sizeof(book_t));
        if (*books != NULL) {
            printf("Le livre a ete supprime avec succes.\n\n");
        } else {
            printf("Erreur de memoire lors de la suppression du livre.\n\n");
        }
    } else {
        printf("L'ID du livre est invalide.\n\n");
    }
}

void editBook(book_t** books, int size) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du livre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromBookId(*books, size, atoi(indexString));
    if (index != -1) {
        // Définition de toutes les variables pour modifier le livre
        printf("Veuillez introduire le titre [Valeur precedente : %s] : ", (*books)[index].title);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[index].title);
        flushStdin();
        printf("Veuillez introduire l'auteur [Valeur precedente : %s] : ", (*books)[index].author);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[index].author);
        flushStdin();
        printf("Veuillez introduire la maison d'edition [Valeur precedente : %s] : ", (*books)[index].publishingHouse);
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", (*books)[index].publishingHouse);
        flushStdin();
        // Déclaration de la quantité (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char quantityString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire la quantite (en chiffres) [Valeur precedente : %i] : ", (*books)[index].quantity);
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", quantityString);
            flushStdin();
        } while (!isMadeUpOfNumbers(quantityString));
        int quantity = atoi(quantityString);
        // Définition de la variable pour mettre à jour correctement les livres
        int usedBooks = (*books)[index].quantity - (*books)[index].availableQuantity;
        (*books)[index].quantity = quantity;
        (*books)[index].availableQuantity = quantity - usedBooks;
        printf("Le livre a ete modifie avec succes.\n\n");
    } else {
        printf("L'ID du livre est invalide.\n\n");
    }
}

void searchBook(book_t* books, int size) {
    // Déclaration du mot-clé (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char keyword[MAX_CHAR_SIZE + 1];
    printf("Veuillez indroduire votre recherche : ");
    scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", keyword);
    flushStdin();
    char* keywordLower = toLowerCase(keyword);
    printf("Resultats de la recherche pour le mot-cle '%s' :\n", keyword);
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    printf("| ID   | Titre                          | Auteur               | Maison d'edition     | Quantite | Q. disp. |\n");
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    for (int i = 0; i < size; i++) {
        char* titleLower = toLowerCase(books[i].title);
        char* authorLower = toLowerCase(books[i].author);
        char* publishingHouseLower = toLowerCase(books[i].publishingHouse);
        if (strstr(titleLower, keywordLower) != NULL || strstr(authorLower, keywordLower) != NULL || strstr(publishingHouseLower, keywordLower) != NULL) {
            printf("| %-4i | %-30s | %-20s | %-20s | %-8i | %-8i |\n", books[i].id, books[i].title, books[i].author, books[i].publishingHouse, books[i].quantity, books[i].availableQuantity);
        }
    }
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    printf("Pour plus de details, consultez le livre individuellement.\n\n");
}

void showBooks(book_t* books, int size) {
    // Affichage des livres
    printf("Voici la liste des livres :\n");
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    printf("| ID   | Titre                          | Auteur               | Maison d'edition     | Quantite | Q. disp. |\n");
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    for (int i = 0; i < size; i++) {
        printf("| %-4i | %-30s | %-20s | %-20s | %-8i | %-8i |\n", books[i].id, books[i].title, books[i].author, books[i].publishingHouse, books[i].quantity, books[i].availableQuantity);
    }
    printf("+------+--------------------------------+----------------------+----------------------+----------+----------+\n");
    printf("Pour plus de details, consultez le livre individuellement.\n\n");
}

void showBook(book_t* books, int size) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char bookIdString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du livre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", bookIdString);
        flushStdin();
    } while (!isMadeUpOfNumbers(bookIdString));
    int bookIndex = getIndexFromBookId(books, size, atoi(bookIdString));
    // Affichage du livre
    if (bookIndex != -1) {
        printf("Voici les informations du livre :\n");
        printf("ID : %i\n", books[bookIndex].id);
        printf("Titre : %s\n", books[bookIndex].title);
        printf("Auteur : %s\n", books[bookIndex].author);
        printf("Maison d'edition : %s\n", books[bookIndex].publishingHouse);
        printf("Quantite : %i\n", books[bookIndex].quantity);
        printf("Quantite disponible : %i\n", books[bookIndex].availableQuantity);
        printf("\n");
    } else {
        printf("L'ID du livre est invalide.\n\n");
    }
}

int getIndexFromBookId(book_t* books, int size, int id) {
    // Regarde à quelle place se trouve l'ID
    for(int i = 0; i < size; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool readBooksFile(book_t** books, int* size) {
    // Ouverture du fichier
    FILE* file = fopen("books.bin", "rb");
    if (file != NULL) {
        // Définition de la taille
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        *size = fileSize / sizeof(book_t);
        if ((*size) > 0) {
            // Mise à jour de la mémoire pour correspondre à la taille
            *books = realloc(*books, (*size) * sizeof(book_t));
            if (*books != NULL) {
                // Lecture des données dans le fichier
                int elementsWritten = fread(*books, sizeof(book_t), *size, file);
                fclose(file);
                if (elementsWritten != (*size)) {
                    printf("Erreur lors de la lecture des donnees dans le fichier.\n\n");
                    return false;
                }
            } else {
                fclose(file);
                printf("Erreur de memoire lors de la lecture des donnees.\n\n");
                return false;

            }
        }
    }
    return true;
}

bool writeBooksFile(book_t* books, int size) {
    // Ouverture du fichier
    FILE* file = fopen("books.bin", "wb");
    if (file != NULL) {
        // Écriture des données dans le fichier
        int elementsWritten = fwrite(books, sizeof(book_t), size, file);
        fclose(file);
        if (elementsWritten != size) {
            printf("Erreur lors de l'ecriture des donnees dans le fichier.\n\n");
            return false;
        }
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n\n");
        return false;
    }
    return true;
}

void addLoan(loan_t** loans, int* size, member_t* members, int membersSize, book_t** books, int booksSize) {
    // Mise à jour de la mémoire pour correspondre à la taille
    *loans = realloc(*loans, ((*size) + 1) * sizeof(loan_t));
    if (*loans != NULL) {
        // Définition de toutes les variables pour créer l'emprunt
        (*loans)[*size].id = (*size > 0) ? (*loans)[*size - 1].id + 1 : 1;
        // Déclaration de l'ID du membre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char memberIdString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire l'ID du membre (en chiffres) : ");
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", memberIdString);
            flushStdin();
        } while (!isMadeUpOfNumbers(memberIdString));
        int memberId = atoi(memberIdString);
        // Déclaration de l'ID du livre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char bookIdString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire l'ID du livre (en chiffres) : ");
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", bookIdString);
            flushStdin();
        } while (!isMadeUpOfNumbers(bookIdString));
        int bookId = atoi(bookIdString);
        int memberIndex = getIndexFromMemberId(members, membersSize, memberId);
        int bookIndex = getIndexFromBookId(*books, booksSize, bookId);
        if (memberIndex != -1 && bookIndex != -1) {
            if ((*books)[bookIndex].availableQuantity > 0) {
                (*loans)[*size].memberId = memberId;
                (*loans)[*size].bookId = bookId;
                // Définition de la date et de l'heure actuelles
                long long now = time(NULL);
                (*loans)[*size].startDate = now;
                char endTimeString[MAX_CHAR_SIZE + 1];
                do {
                    printf("Veuillez introduire la duree (en secondes) : ");
                    scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", endTimeString);
                    flushStdin();
                } while (!isMadeUpOfNumbers(endTimeString));
                (*loans)[*size].endDate = now + atoll(endTimeString);
                (*loans)[*size].wasReturned = false;
                (*books)[bookIndex].availableQuantity -= 1;
                (*size)++;
                printf("L'emprunt a ete ajoute avec succes.\n\n");
            } else {
                printf("Tous les livres ont ete empruntes.\n\n");
            }
        } else {
            printf("Le membre ou le livre n'existe pas.\n\n");
        }
    } else {
        printf("Erreur de memoire lors de l'ajout de l'emprunt.\n\n");
    }
}

void removeLoan(loan_t** loans, int* size, book_t** books, int booksSize) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID de l'emprunt (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromLoanId(*loans, *size, atoi(indexString));
    if (index != -1) {
        int bookIndex = getIndexFromBookId(*books, booksSize, (*loans)[index].bookId);
        // Décalage d'une place en arrière de tous les éléments après l'ID
        for (int i = index; i < (*size) - 1; i++) {
            (*loans)[i] = (*loans)[i + 1];
        }
        (*size)--;
        // Mise à jour de la mémoire pour correspondre à la taille
        *loans = realloc(*loans, (*size) * sizeof(loan_t));
        if (*loans != NULL) {
            if (bookIndex != -1) {
                (*books)[bookIndex].availableQuantity += 1;
                printf("L'emprunt a ete supprime avec succes.\n\n");
            } else {
                printf("L'ID du livre est invalide.\n\n");
            }
        } else {
            printf("Erreur de memoire lors de la suppression de l'emprunt.\n\n");
        }
    } else {
        printf("L'ID de l'emprunt est invalide.\n\n");
    }
}

void editLoan(loan_t** loans, int size, member_t* members, int membersSize, book_t* books, int booksSize) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID de l'emprunt (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromLoanId(*loans, size, atoi(indexString));
    if (index != -1) {
        // Définition de toutes les variables pour modifier l'emprunt
        // Déclaration de l'ID du membre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char memberIdString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire l'ID du membre (en chiffres) [Valeur precedente : %i] : ", (*loans)[index].memberId);
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", memberIdString);
            flushStdin();
        } while (!isMadeUpOfNumbers(memberIdString));
        int memberId = atoi(memberIdString);
        // Déclaration de l'ID du livre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
        char bookIdString[MAX_CHAR_SIZE + 1];
        do {
            printf("Veuillez introduire l'ID du livre (en chiffres) [Valeur precedente : %i] : ", (*loans)[index].bookId);
            scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", bookIdString);
            flushStdin();
        } while (!isMadeUpOfNumbers(bookIdString));
        int bookId = atoi(bookIdString);
        int memberIndex = getIndexFromMemberId(members, membersSize, memberId);
        int bookIndex = getIndexFromBookId(books, booksSize, bookId);
        if (memberIndex != -1 && bookIndex != -1) {
            (*loans)[index].memberId = memberId;
            (*loans)[index].bookId = bookId;
            char endTimeString[MAX_CHAR_SIZE + 1];
            do {
                printf("Veuillez introduire la duree (en secondes) [Valeur precedente : %lld] : ", ((*loans)[index].endDate - (*loans)[index].startDate));
                scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", endTimeString);
                flushStdin();
            } while (!isMadeUpOfNumbers(endTimeString));
            (*loans)[index].endDate = (*loans)[index].startDate + atoll(endTimeString);
            printf("L'emprunt a ete modifie avec succes.\n\n");
        } else {
            printf("Le membre ou le livre n'existe pas.\n\n");
        }
    } else {
        printf("L'ID de l'emprunt est invalide.\n\n");
    }
}

void searchLoan(loan_t* loans, int size, member_t* members, int membersSize, book_t* books, int booksSize) {
    // Déclaration du mot-clé (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char keyword[MAX_CHAR_SIZE + 1];
    printf("Veuillez indroduire votre recherche : ");
    scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", keyword);
    flushStdin();
    char* keywordLower = toLowerCase(keyword);
    printf("Resultats de la recherche pour le mot-cle '%s' :\n", keyword);
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    printf("| ID   | Prenom Nom                     | Titre Auteur                   | Date de debut    | Date de fin      | Retourne   |\n");
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    for (int i = 0; i < size; i++) {
        int memberIndex = getIndexFromMemberId(members, membersSize, loans[i].memberId);
        int bookIndex = getIndexFromBookId(books, booksSize, loans[i].bookId);
        if (memberIndex != -1 && bookIndex != -1) {
            char* firstNameLower = toLowerCase(members[memberIndex].firstName);
            char* lastNameLower = toLowerCase(members[memberIndex].lastName);
            char* emailLower = toLowerCase(members[memberIndex].email);
            char* titleLower = toLowerCase(books[bookIndex].title);
            char* authorLower = toLowerCase(books[bookIndex].author);
            char* publishingHouseLower = toLowerCase(books[bookIndex].publishingHouse);
            if (strstr(firstNameLower, keywordLower) != NULL || strstr(lastNameLower, keywordLower) != NULL || strstr(emailLower, keywordLower) != NULL ||  strstr(titleLower, keywordLower) != NULL || strstr(authorLower, keywordLower) != NULL || strstr(publishingHouseLower, keywordLower) != NULL) {
                char firstNameAndLastName[((MAX_CHAR_SIZE + 1) * 2) + 1];
                // Concaténation des éléments
                strcpy(firstNameAndLastName, members[memberIndex].firstName);
                strcat(firstNameAndLastName, " ");
                strcat(firstNameAndLastName, members[memberIndex].lastName);
                char titleAndAuthor[((MAX_CHAR_SIZE + 1) * 2) + 1];
                // Concaténation des éléments
                strcpy(titleAndAuthor, books[bookIndex].title);
                strcat(titleAndAuthor, " ");
                strcat(titleAndAuthor, books[bookIndex].author);
                char startDate[20];
                // Formatage de la date et de l'heure
                strftime(startDate, sizeof(startDate), "%d/%m/%Y %H:%M", localtime(&loans[i].startDate));
                char endDate[20];
                // Formatage de la date et de l'heure
                strftime(endDate, sizeof(endDate), "%d/%m/%Y %H:%M", localtime(&loans[i].endDate));
                printf("| %-4i | %-30s | %-30s | %-16s | %-16s | %-10s |\n", loans[i].id, firstNameAndLastName, titleAndAuthor, startDate, endDate, (loans[i].wasReturned ? "Oui" : "Non"));
            }
        }
    }
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    printf("Pour plus de details, consultez l'emprunt individuellement.\n\n");
}

void makeReturned(loan_t** loans, int size, book_t** books, int booksSize) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char indexString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID de l'emprunt (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", indexString);
        flushStdin();
    } while (!isMadeUpOfNumbers(indexString));
    int index = getIndexFromLoanId(*loans, size, atoi(indexString));
    if (index != -1) {
        int bookIndex = getIndexFromBookId(*books, booksSize, (*loans)[index].bookId);
        if (bookIndex != -1) {
            (*loans)[index].wasReturned = true;
            (*books)[bookIndex].availableQuantity += 1;
            printf("Le livre a ete retourne avec succes.\n\n");
        } else {
            printf("L'ID du livre est invalide.\n\n");
        }
    } else {
        printf("L'ID de l'emprunt est invalide.\n\n");
    }
}

void showLoans(loan_t* loans, int size, member_t* members, int membersSize, book_t* books, int booksSize) {
    // Affichage des emprunts
    printf("Voici la liste des emprunts :\n");
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    printf("| ID   | Prenom Nom                     | Titre Auteur                   | Date de debut    | Date de fin      | Retourne   |\n");
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    for (int i = 0; i < size; i++) {
        int memberIndex = getIndexFromMemberId(members, membersSize, loans[i].memberId);
        char firstNameAndLastName[((MAX_CHAR_SIZE + 1) * 2) + 1];
        // Concaténation des éléments
        if (memberIndex != -1) {
            strcpy(firstNameAndLastName, members[memberIndex].firstName);
            strcat(firstNameAndLastName, " ");
            strcat(firstNameAndLastName, members[memberIndex].lastName);
        } else {
            strcpy(firstNameAndLastName, "Erreur");
        }
        int bookIndex = getIndexFromBookId(books, booksSize, loans[i].bookId);
        char titleAndAuthor[((MAX_CHAR_SIZE + 1) * 2) + 1];
        // Concaténation des éléments
        if (bookIndex != -1) {
            strcpy(titleAndAuthor, books[bookIndex].title);
            strcat(titleAndAuthor, " ");
            strcat(titleAndAuthor, books[bookIndex].author);
        } else {
            strcpy(firstNameAndLastName, "Erreur");
        }
        char startDate[20];
        // Formatage de la date et de l'heure
        strftime(startDate, sizeof(startDate), "%d/%m/%Y %H:%M", localtime(&loans[i].startDate));
        char endDate[20];
        // Formatage de la date et de l'heure
        strftime(endDate, sizeof(endDate), "%d/%m/%Y %H:%M", localtime(&loans[i].endDate));
        printf("| %-4i | %-30s | %-30s | %-16s | %-16s | %-10s |\n", loans[i].id, firstNameAndLastName, titleAndAuthor, startDate, endDate, loans[i].wasReturned ? "Oui" : "Non");
    }
    printf("+------+--------------------------------+--------------------------------+------------------+------------------+------------+\n");
    printf("Pour plus de details, consultez l'emprunt individuellement.\n\n");
}

void showLoan(loan_t* loans, int size, member_t* members, int membersSize, book_t* books, int booksSize) {
    // Demande de l'ID (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char loanIdString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID de l'emprunt (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", loanIdString);
        flushStdin();
    } while (!isMadeUpOfNumbers(loanIdString));
    int loanIndex = getIndexFromLoanId(loans, size, atoi(loanIdString));
    // Affichage du livre
    if (loanIndex != -1) {
        int memberIndex = getIndexFromMemberId(members, membersSize, loans[loanIndex].memberId);
        int bookIndex = getIndexFromBookId(books, booksSize, loans[loanIndex].bookId);
        char availableQuantity[20];
        if (bookIndex != -1) {
            sprintf(availableQuantity, "%i", books[bookIndex].availableQuantity);
        } else {
            strcpy(availableQuantity, "Erreur");
        }
        char startDate[20];
        // Formatage de la date et de l'heure
        strftime(startDate, sizeof(startDate), "%d/%m/%Y %H:%M", localtime(&loans[bookIndex].startDate));
        char endDate[20];
        // Formatage de la date et de l'heure
        strftime(endDate, sizeof(endDate), "%d/%m/%Y %H:%M", localtime(&loans[bookIndex].endDate));
        printf("Voici les informations de l'emprunt :\n");
        printf("ID : %i\n", loans[loanIndex].id);
        printf("Prenom de l'emprunteur : %s\n", (memberIndex != -1) ? members[memberIndex].firstName : "Erreur");
        printf("Nom de l'emprunteur : %s\n", (memberIndex != -1) ? members[memberIndex].lastName : "Erreur");
        printf("Adresse email de l'emprunteur : %s\n", (memberIndex != -1) ? members[memberIndex].email : "Erreur");
        printf("Titre du livre : %s\n", (bookIndex != -1) ? books[bookIndex].title : "Erreur");
        printf("Auteur du livre : %s\n", (bookIndex != -1) ? books[bookIndex].author : "Erreur");
        printf("Maison d'edition du livre : %s\n", (bookIndex != -1) ? books[bookIndex].publishingHouse : "Erreur");
        printf("Quantite disponible de livre(s) : %s\n", availableQuantity);
        printf("Date de début : %s\n", startDate);
        printf("Date de fin : %s\n", endDate);
        printf("Retourne : %s\n", loans[loanIndex].wasReturned ? "Oui" : "Non");
        printf("\n");
    } else {
        printf("L'ID de l'emprunt est invalide.\n\n");
    }
}

void showBooksLoanedByAMember(loan_t* loans, int loansSize, book_t* books, int booksSize) {
    // Déclaration de l'ID du membre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char memberIdString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du membre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", memberIdString);
        flushStdin();
    } while (!isMadeUpOfNumbers(memberIdString));
    int memberId = atoi(memberIdString);
    // Affichage des livres
    printf("Voici la liste des livres :\n");
    printf("+------+--------------------------------+----------------------+----------------------+\n");
    printf("| ID   | Titre                          | Auteur               | Maison d'edition     |\n");
    printf("+------+--------------------------------+----------------------+----------------------+\n");
    for (int i = 0; i < loansSize; i++) {
        if (loans[i].memberId == memberId) {
            int index = getIndexFromBookId(books, booksSize, loans[i].bookId);
            if (index != -1) {
                printf("| %-4i | %-30s | %-20s | %-20s |\n", books[index].id, books[index].title, books[index].author, books[index].publishingHouse);
            }
        }
    }
    printf("+------+--------------------------------+----------------------+----------------------+\n");
    printf("Pour plus de details, consultez le livre individuellement.\n\n");
}

void showMembersWhoHaveLoanedABook(loan_t* loans, int loansSize, member_t* members, int membersSize) {
    // Déclaration de l'ID du livre (le MAX_CHAR_SIZE a un + 1 car je réserve une place en plus pour le \0 de fin)
    char bookIdString[MAX_CHAR_SIZE + 1];
    do {
        printf("Veuillez introduire l'ID du livre (en chiffres) : ");
        scanf("%" STRINGIFY(MAX_CHAR_SIZE) "[^\n]", bookIdString);
        flushStdin();
    } while (!isMadeUpOfNumbers(bookIdString));
    int bookId = atoi(bookIdString);
    // Affichage des membres
    printf("Voici la liste des membres :\n");
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("| ID   | Prenom               | Nom                  | Email                          |\n");
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    for (int i = 0; i < loansSize; i++) {
        if (loans[i].bookId == bookId) {
            int index = getIndexFromMemberId(members, membersSize, loans[i].memberId);
            if (index != -1) {
                printf("| %-4i | %-20s | %-20s | %-30s |\n", members[index].id, members[index].firstName, members[index].lastName, members[index].email);
            }
        }
    }
    printf("+------+----------------------+----------------------+--------------------------------+\n");
    printf("Pour plus de details, consultez le membre individuellement.\n\n");
}

int getIndexFromLoanId(loan_t* loans, int size, int id) {
    // Regarde à quelle place se trouve l'ID
    for (int i = 0; i < size; i++) {
        if (loans[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool readLoansFile(loan_t** loans, int* size) {
    // Ouverture du fichier
    FILE* file = fopen("loans.bin", "rb");
    if (file != NULL) {
        // Définition de la taille
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);
        *size = fileSize / sizeof(loan_t);
        if ((*size) > 0) {
            // Mise à jour de la mémoire pour correspondre à la taille
            *loans = realloc(*loans, (*size) * sizeof(loan_t));
            if (*loans != NULL) {
                // Lecture des données dans le fichier
                int elementsWritten = fread(*loans, sizeof(loan_t), *size, file);
                fclose(file);
                if (elementsWritten != (*size)) {
                    printf("Erreur lors de la lecture des donnees dans le fichier.\n\n");
                    return false;
                }
            } else {
                fclose(file);
                printf("Erreur de memoire lors de la lecture des donnees.\n\n");
                return false;
            }
        }
    }
    return true;
}

bool writeLoansFile(loan_t* loans, int size) {
    // Ouverture du fichier
    FILE* file = fopen("loans.bin", "wb");
    if (file != NULL) {
        // Écriture des données dans le fichier
        int elementsWritten = fwrite(loans, sizeof(loan_t), size, file);
        fclose(file);
        if (elementsWritten != size) {
            printf("Erreur lors de l'ecriture des donnees dans le fichier.\n\n");
            return false;
        }
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n\n");
        return false;
    }
    return true;
}

// Fonction pour vider le tampon d'entrée
void flushStdin() {
    while (1) {
        int c = getc(stdin);
        if (c == EOF || c == '\n') {
            break;
        }
    }
}

// Fonction pour vérifier si une chaine de caractères est un entier
bool isMadeUpOfNumbers(char* string) {
    int length = strlen(string);
    if (length == 0) {
        return false;
    }
    for (int i = 0; i < length; i++) {
        if (!isdigit(string[i])) {
            return false;
        }
    }
    return true;
}

// Fonction pour mettre une chaine de caractère en minuscule
char* toLowerCase(char* string) {
    char* lowerCaseString = strdup(string);
    for (int i = 0; i < strlen(lowerCaseString); i++) {
        lowerCaseString[i] = tolower(lowerCaseString[i]);
    }
    return lowerCaseString;
}
