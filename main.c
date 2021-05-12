#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LISTE_OUVRIER "ouvrier.dat"
#define LISTE_CHANTIER "chantier.dat"
#define LISTE_HORAIRE "horaire.dat"

#define MAX_SIZE 255
#define MAX_DATE 11

#define MAX_OUVRIER 100
#define MAX_CHANTIER 20
#define MAX_OCCUPE 2000
#define MAX_HEURES 6

#define MAX_NOM 25
#define MAX_PRENOM 30
#define MAX_RUE 50
#define MAX_VILLE 20
#define MAX_SPE 20

#define MAX_NOMCHAN 25
#define MAX_MAITRE 30
#define MAX_GERANT 5

/* Les structures */
struct _date {
    short jour;
    short mois;
    short annee;
};
typedef struct _date Date;

struct _ouvriers {
    long NumRegistre ;
    char nom[MAX_NOM] ;
    char prenom[MAX_PRENOM] ;
    char rue[MAX_RUE] ;
    int CodePostal ;
    char ville[MAX_VILLE] ;
    char specialite[MAX_SPE] ;
    Date d_engag ;
    Date d_naiss ;
};
typedef struct _ouvriers Ouvrier;

struct _EstOccupe {
    long NumRegistre;
    long NumChantier;
    Date date;
    char heures [MAX_HEURES];
};
typedef struct _EstOccupe EstOccupe;

struct _chantier {
    long Numchantier;
    char NomChantier[MAX_NOMCHAN] ;
    char rue[MAX_RUE] ;
    int CodePostal ;
    char ville[MAX_VILLE];
    char MaitreOuvrage[MAX_MAITRE] ;
    float Montant;
    int Duree;
    char CodeGerant[MAX_GERANT] ;
};
typedef struct _chantier Chantier;

struct _indexOuvrier {
    int adresse;
    long NumRegistre ;
    char nom[MAX_NOM] ;
    char prenom[MAX_PRENOM];
};
typedef struct _indexOuvrier IndexOuvrier;

struct _indexChantier {
    int adresse;
    long Numchantier;
    char NomChantier[MAX_NOMCHAN];
};
typedef struct _indexChantier IndexChantier;

/* Les fonctions */
/* Saisie */
int LireEntier (int, int);

/* Ouvrier */
void initialisationOuvrier(IndexOuvrier *);
void triIndexOuvrier(IndexOuvrier *);
void EchangeOuvrier(IndexOuvrier *, IndexOuvrier *);

void AjoutOuvrier(IndexOuvrier *);
void ModifierOuvrier(IndexOuvrier *);
void SuppressionOuvrier(IndexOuvrier *);

void AffichageOuvrierList(IndexOuvrier *);
void AffichageOuvrierDetails(IndexOuvrier *, IndexChantier *);

/* Chantier */
void initialisationChantier(IndexChantier *);
void triIndexChantier(IndexChantier *);
void EchangeChantier(IndexChantier *, IndexChantier *);
short RechercheIndexChantier(IndexChantier *, long);

void AjoutChantier(IndexChantier *);

void AffichageChantierList(IndexChantier *);
void AffichageOuvrierPourChantier(IndexChantier *, IndexOuvrier *);

/* Horaire */
void AjoutHoraire(IndexChantier *,IndexOuvrier *);
void AffichageHoraires();

int main () {
    IndexChantier listChantier[MAX_CHANTIER];
    IndexOuvrier listOuvrier[MAX_OUVRIER];
    int choix, restart = 1;

    initialisationOuvrier(listOuvrier);
    initialisationChantier(listChantier);
    triIndexOuvrier(listOuvrier);
    triIndexChantier(listChantier);

    while (restart == 1) {
        printf("====================== GESTION DES OUVRIERS ====================== \n");
        printf(" 1. Ajout d'un ouvrier \n");
        printf(" 2. Modifier un ouvrier \n");
        printf(" 3. Supprimer un ouvrier \n");
        printf(" 4. Lister les ouvriers \n");
        printf(" 5. Affichage horaire d'un ouvrier \n");
        printf("====================== GESTION DES CHANTIERS ===================== \n");
        printf(" 6. Ajouter un chantier \n");
        printf(" 7. Afficher le(s) chantier(s) \n");
        printf(" 8. Afficher le(s) ouvriers(s) ayant travailler sur un chantier specifique \n");
        printf("=============== GESTION DES HORAIRES DES OUVRIERS ================ \n");
        printf(" 9. Ajouter une prestation \n");
        printf(" 10. Affichage des prestations \n");
        printf(" 11. Arret du programme \n");

        printf("\n Choix: ");
        choix = LireEntier(1,11);

        switch (choix) {
            case 1:
                AjoutOuvrier(listOuvrier);
                triIndexOuvrier(listOuvrier);
                break;
            case 2:
                ModifierOuvrier(listOuvrier);
                triIndexOuvrier(listOuvrier);
                break;
            case 3:
                SuppressionOuvrier(listOuvrier);
                triIndexOuvrier(listOuvrier);
                break;
            case 4:
                AffichageOuvrierList(listOuvrier);
                break;
            case 5:
                AffichageOuvrierDetails(listOuvrier, listChantier);
                break;
            case 6:
                AjoutChantier(listChantier);
                triIndexChantier(listChantier);
                break;
            case 7:
                AffichageChantierList(listChantier);
                break;
            case 8:
                AffichageOuvrierPourChantier(listChantier, listOuvrier);
                break;
            case 9:
                AjoutHoraire(listChantier, listOuvrier);
                break;
            case 10:
                AffichageHoraires();
                break;
            case 11:
                break;
            default:
                printf("Valeur inconnue \n");
        }
        printf("Voulez-vous relancer le programme ? (Oui = 1, Non = 2): ");
        restart = LireEntier(1,2);
        printf("\n\n\n");
    }
    printf("Fin du programme ! \n");

    return 0;
}

/* Saisie */
void LireChaine (char *pc, int max) {
    /* Lis une chaine de caracteres */
    char c;
    int i=0;

    fflush(stdin);
    c = getchar();
    while ((i < max-1) && (c != '\n')) {
        *pc = c;
        pc++;
        i++;
        c = getchar();
    }
    *pc = '\0';
}
void LireNomPropre (char *pc, int max) {
    char c;
    int i, erreur = 0;

    do {
        if (erreur == 1) printf("Erreur : Introduisez un nom propre valide ! \n");
        erreur = 0;

        i = 0;
        strcpy(pc, " ");

        fflush(stdin);
        c = getchar();

        while ((i<max-1) && (c!='\n') && erreur == 0) {
            if (i == 0 && c == ' ') {
                erreur = 1;
            } else {
                if (isalpha(c) || c == ' ' || c == '-') {
                    if (i != 0 && ((*(pc-1) == ' ' || *(pc-1) == '-') && ((*pc == ' ') || (*pc == '-')))) {
                        erreur = 1;
                    } else {
                        if (i==0 || *(pc-1) == ' ' || *(pc-1) == '-')
                            *pc = toupper(c);
                        else
                            *pc = tolower(c);

                        pc++;
                        i++;
                        c = getchar();
                    }
                } else {
                    erreur = 1;
                }
            }
        }
        *pc = '\0';

    } while (erreur != 0);
}
int LireEntier (int min, int max) {
    /* Lis une chaine de caracteres, la converti en int et vérifie que le nombre se situe entre le min et le max */
    char str[MAX_SIZE] = {};
    int i, nombre, erreur = 0;

    do {
        if (erreur == 1) printf("Erreur : La chaine de caracteres n'est pas un nombre entier ! \n");
        if (erreur == 2) printf("Erreur : Le nombre doit etre compris entre %d et %d ! \n", min, max);
        erreur = 0;

        LireChaine(str, MAX_SIZE);

        for (i = 0; i < MAX_SIZE && *(str + i) != '\0'; i++) {
            if (!isdigit(*(str + i))) {
                erreur = 1;
            }
        }

        nombre = atoi(str);
        if (nombre < min || nombre > max) {
            erreur = 2;
        }
    } while (erreur != 0);

    return nombre;
}
long LireLong (long min, long max) {
    /* Lis une chaine de caracteres, la converti en long et vérifie que le nombre se situe entre le min et le max */
    char str[MAX_SIZE] = {};
    long nombre;
    int i, erreur = 0;

    do {
        if (erreur == 1) printf("Erreur : La chaine de caracteres n'est pas un nombre ! \n");
        if (erreur == 2) printf("Erreur : Le nombre doit etre compris entre %ld et %ld ! \n", min, max);
        erreur = 0;

        LireChaine(str, MAX_SIZE);

        for (i = 0; i < MAX_SIZE && *(str + i) != '\0'; i++) {
            if (!isdigit(*(str + i))) {
                erreur = 1;
            }
        }

        nombre = atol(str);
        if (nombre < min || nombre > max) {
            erreur = 2;
        }
    } while (erreur != 0);

    return nombre;
}
float LireFloat (long min, long max) {
    /* Lis une chaine de caracteres, la converti en long et vérifie que le nombre se situe entre le min et le max */
    char str[MAX_SIZE] = {};
    float nombre;
    int i, erreur = 0;

    do {
        if (erreur == 1) printf("Erreur : La chaine de caracteres n'est pas un nombre ! \n");
        if (erreur == 2) printf("Erreur : Le nombre doit etre compris entre %ld et %ld ! \n", min, max);
        erreur = 0;

        LireChaine(str, MAX_SIZE);

        for (i = 0; i < MAX_SIZE && *(str + i) != '\0'; i++) {
            if (!isdigit(*(str + i)) && *(str + i) != '.') {
                erreur = 1;
            }
        }

        nombre = atof(str);
        if (nombre < min || nombre > max) {
            erreur = 2;
        }
    } while (erreur != 0);

    return nombre;
}
short LireDate(struct _date *pdate, char *ch) {
    short i = 0, valide = 0;
    char *ch_tmp = ch;

    do {
        if (isdigit(*ch) && (i < 2 || (i > 2 && i < 5) || i > 5)) valide = 1;
        else if ((i == 2 || i == 5) && *ch == '/')
            valide = 1;

        ch++;
        i++;
    } while (*ch != '\0' && valide);

    if (valide) {
        valide = 0;
        ch = ch_tmp;
        pdate->jour = (short)(*ch-'0')*10 +(short)(*(ch+1)-'0');

        ch += 3;
        pdate->mois = (short)(*ch-'0')*10
                      +(short)(*(ch+1)-'0');

        ch += 3;
        if (i == 8) {
            valide = 1;
            pdate->annee = (short)(*ch-'0')*10
                           +(short)(*(ch+1)-'0');
        } else if (i == 10) {
            valide = 1;
            pdate->annee = (short)(*ch-'0')*1000
                           +(short)(*(ch+1)-'0')*100
                           +(short)(*(ch+2)-'0')*10
                           +(short)(*(ch+3)-'0');
        }
    }

    return valide;
}
short ValideDate(Date *date) {
    short valide = 0;
    short annee = date->annee;
    short mois  = date->mois;
    short jour  = date->jour;

    if (annee >= 1900 && annee <= 2021 && mois <= 12 && mois >= 1 && jour > 0) {
        switch (mois) {
            case 2:
                if ((((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0) && jour <= 29) || jour <= 28)
                    valide = 1;
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if (jour <= 30)
                    valide = 1;
                break;
            default:
                if (jour <= 31)
                    valide = 1;
        }
    }
    return valide;
}
void LireHeure(char *pc, int min, int max) {
    char strHeures[MAX_HEURES] = {}, strMinutes[MAX_HEURES] = {}, strTmp[MAX_HEURES] = {};
    int heures, minutes;

    do {
        printf("Heures sans minutes: ");
        heures = LireEntier(min, max);
        printf("Minutes: ");
        minutes = LireEntier(0, 60);
    } while (heures == 10 && minutes > 0);

    itoa(heures, strHeures, 10);
    itoa(minutes, strMinutes, 10);

    if (minutes == 0) strcpy(strMinutes, "00");
    if (minutes < 10) {
        strcpy(strTmp, strMinutes);
        strMinutes[0] = '0';
        strMinutes[1] = strTmp[0];
        strMinutes[2] = '\0';
    }

    strcpy(pc, strHeures);
    strcat(pc, ":");
    strcat(pc, strMinutes);
}

/* Ouvrier */
short verificationUtilisation(long val) {
    short res = 0;
    FILE *fp = fopen(LISTE_HORAIRE, "r+b");
    EstOccupe tmp;

    if (fp == NULL) {
        res = 2;
    } else {
        fread(&tmp,sizeof(EstOccupe), 1, fp);
        while (!feof(fp) && res != 1) {
            if (tmp.NumRegistre == val) {
                printf("Erreur : Des horaires sont associes a cet ouvriers: => suppression impossible ! \n");
                res = 1;
            }

            fread(&tmp,sizeof(EstOccupe), 1, fp);
        }
        fclose(fp);
    }

    return res;
}
int verificationPlaceIndexOuvrier(IndexOuvrier list[MAX_OUVRIER]) {
    short i, trouve = 0;
    int val = 0;

    for(i = 0;i<MAX_OUVRIER && trouve == 0;i++) {
        if (list[i].NumRegistre == -1) {
            trouve = 1;
            val = list[i].adresse;
        }
    }

    return val;
}
void idExistantOuvrier(IndexOuvrier list[MAX_OUVRIER], long *idInterdit) {
    short i=0;

    while (i < MAX_OUVRIER) {
        *idInterdit = list[i].NumRegistre;
        idInterdit++;
        i++;
    }
}
short verifNumOuvrierDispo(long *idInterdit, long id) {
    short res = 0,i = 0;

    while (i < MAX_OUVRIER && *idInterdit != id) {
        idInterdit++;
        i++;
    }

    if (*idInterdit == id)
        res = 1;
    else
        res = 0;

    return res;
}
short RechercheIndexOuvrier(IndexOuvrier *list,long valChercher) {
    short i = 0, trouve=-1;

    while (i < MAX_OUVRIER && trouve == -1) {
        if (list[i].NumRegistre == valChercher)
            trouve = i;
        else
            i++;
    }

    return trouve;
}
short comparaisonDate(Date dt1,Date dt2) {
    if (dt1.annee < dt2.annee)
        return -1;
    else if (dt1.annee > dt2.annee)
        return 1;
    else {
        if (dt1.mois < dt2.mois)
            return -1;
        else if (dt1.mois > dt2.mois)
            return 1;
        else
        {
            if (dt1.jour < dt2.jour)
                return -1;
            else if (dt1.jour > dt2.jour)
                return 1;
            else
                return 0;
        }
    }
}

void initialisationOuvrier(IndexOuvrier *list) {
    short i=0;
    IndexOuvrier *tmp = list;
    Ouvrier ouvrier;
    FILE *fp;
    fp = fopen(LISTE_OUVRIER,"r+b");

    if (fp==NULL) {
        printf("Ouvrier> Fichier non existant => creation d'un nouveau fichier\n\n");
        fp = fopen(LISTE_OUVRIER,"w+b");

        while (i<MAX_OUVRIER) {
            tmp->adresse = i;
            tmp->NumRegistre = -1;
            i++;
            tmp++;
        }
    } else {
        printf("Ouvrier> Lecture du fichier\n");
        fread(&ouvrier,sizeof(Ouvrier),1,fp);

        while (!feof(fp) && i<MAX_OUVRIER) {
            //printf("%d\t%ld\t%s\t%s\n",i,ouvrier.NumRegistre,ouvrier.nom,ouvrier.prenom);
            tmp->adresse = i;
            tmp->NumRegistre = ouvrier.NumRegistre;
            strcpy(tmp->nom,ouvrier.nom);
            strcpy(tmp->prenom,ouvrier.prenom);
            fread(&ouvrier,sizeof(Ouvrier),1,fp);
            i++;
            tmp++;
        }

        while (feof(fp) && i<MAX_OUVRIER) {
            tmp->adresse = i;
            tmp->NumRegistre = -1;
            i++;
            tmp++;
        }
    }
    fclose(fp);
}
void triIndexOuvrier(IndexOuvrier *listOuvrier) {
    int i, taille = MAX_OUVRIER, tab_en_ordre = 0;

    while (tab_en_ordre == 0) {
        tab_en_ordre = 1;
        for(i = 0; i < taille-1; i++) {
            if (listOuvrier[i].NumRegistre != -1 && listOuvrier[i+1].NumRegistre != -1 && ((strcmp(listOuvrier[i].nom,listOuvrier[i+1].nom) == 1)|| (strcmp(listOuvrier[i].nom,listOuvrier[i+1].nom) == 0 && strcmp(listOuvrier[i].prenom,listOuvrier[i+1].prenom) == 1))) {
                EchangeOuvrier(&listOuvrier[i],&listOuvrier[i+1]);
                tab_en_ordre = 0;
            }
        }
        taille--;
    }
}
void EchangeOuvrier(IndexOuvrier *tmp1,IndexOuvrier *tmp2) {
    IndexOuvrier tmp;

    strcpy(tmp.nom,tmp1->nom);
    strcpy(tmp.prenom,tmp1->prenom);
    tmp.NumRegistre = tmp1->NumRegistre;
    tmp.adresse = tmp1->adresse;

    strcpy(tmp1->nom,tmp2->nom);
    strcpy(tmp1->prenom,tmp2->prenom);
    tmp1->NumRegistre = tmp2->NumRegistre;
    tmp1->adresse = tmp2->adresse;

    strcpy(tmp2->nom,tmp.nom);
    strcpy(tmp2->prenom,tmp.prenom);
    tmp2->NumRegistre = tmp.NumRegistre;
    tmp2->adresse = tmp.adresse;
}

void AjoutOuvrier(IndexOuvrier list[MAX_OUVRIER]) { // Cas 1 = Ajouter un ouvrier
    char pch[MAX_DATE];
    long idInterdit[MAX_OUVRIER];
    int i, vali = 0, erreur = 0;
    Ouvrier tab;
    FILE *fp;
    Date date;

    printf("+---------------------------+\n");
    printf("|     Ajouter un ouvrier    |\n");
    printf("+---------------------------+\n");

    fp = fopen(LISTE_OUVRIER,"r+b");

    if (fp == NULL) {
        printf("Erreur : Ajout de donnees impossible ! \n");
    } else {
        i = verificationPlaceIndexOuvrier(list);
        idExistantOuvrier(list, idInterdit);

        if (i != -1) {
            printf("Adresse: %d\n", i);
            list[i].adresse = i;
            do {
                if (erreur == 1) printf("Erreur : Valeur interdite car deja utilisee ! \n");
                printf("Entrez l\'id de l\'ouvrier a ajouter (de 1 a 99999): ");
                tab.NumRegistre = LireLong(1, 99999);

                if (verifNumOuvrierDispo(idInterdit, tab.NumRegistre) == 0)
                    erreur = 0;
                else
                    erreur = 1;

            } while (erreur == 1);
            list[i].NumRegistre = tab.NumRegistre;

            printf("Nom: ");
            LireNomPropre(tab.nom, MAX_NOM);
            strcpy(list[i].nom, tab.nom);

            printf("Prenom: ");
            LireNomPropre(tab.prenom, MAX_PRENOM);
            strcpy(list[i].prenom, tab.prenom);

            printf("Numero de maison: ");
            int intNumMaison = LireEntier(1, 9999);
            char strNumMaison2 [MAX_RUE] = {}, strNumMaison [MAX_RUE] = ", ";
            itoa(intNumMaison, strNumMaison2, 10);
            strcat(strNumMaison, strNumMaison2);

            printf("Rue: ");
            LireNomPropre(tab.rue, MAX_RUE-6);
            strcat(tab.rue, strNumMaison);

            printf("Code Postal: ");
            tab.CodePostal = LireEntier(1000, 9999);

            printf("Ville: ");
            LireNomPropre(tab.ville, MAX_VILLE);

            printf("Specialite: ");
            LireChaine(tab.specialite, MAX_SPE);

            do {
                printf("Date de naissance:");
                LireChaine(pch,MAX_DATE);
                if (!LireDate(&date, pch) || !ValideDate(&date) || !(date.annee < 2003 ||(date.annee==2003 &&(date.mois<05||(date.mois==05 && date.jour<=10))))) { // Ouvrier de minimum 18 ans
                    vali = 0;
                    printf("Veuillez encoder une date valide\n");
                } else {
                    vali = 1;
                }
            } while (!vali);

            tab.d_naiss.jour = date.jour;
            tab.d_naiss.mois = date.mois;
            tab.d_naiss.annee = date.annee;

            do {
                printf("Date d engagement:");
                LireChaine(pch,MAX_DATE);
                if (!LireDate(&date, pch) || !ValideDate(&date) || !(date.annee > tab.d_naiss.annee+18 ||(date.annee == tab.d_naiss.annee+18 &&(date.mois > tab.d_naiss.mois||(date.mois==tab.d_naiss.mois && date.jour>=tab.d_naiss.jour))))) {
                    vali = 0;
                    printf("Veuillez encoder une date valide\n");
                } else {
                    vali = 1;
                }
            } while (!vali);

            tab.d_engag.jour = date.jour;
            tab.d_engag.mois = date.mois;
            tab.d_engag.annee = date.annee;

            if (fseek(fp, sizeof(Ouvrier)*i, SEEK_SET) == 0) {
                fwrite(&tab, sizeof(Ouvrier), 1, fp);
                printf("L\'ouvrier a bien ete ajoute \n");
            } else {
                printf("Ajout au fichier impossible ! \n");
                list[i].adresse = -1;
                list[i].NumRegistre = -1;
                strcpy(list[i].nom," ");
            }
        } else {
            printf("Plus de place Disponible");
        }
        fclose(fp);
    }
}
void ModifierOuvrier(IndexOuvrier *list) { // Cas 2 = Modifier les informations d'un ouvrier
    char pch[MAX_DATE];
    long choix = 0;
    int i = 0, vali = 0;
    IndexOuvrier *itmp = list;
    FILE *fp;
    Ouvrier tab;
    Date date;

    // system("cls");
    printf("+-----------------------------------------+\n");
    printf("|                                         |\n");
    printf("|            Modifier un ouvrier          |\n");
    printf("|                                         |\n");
    printf("+-----------------------------------------+\n\n");

    printf(" +-------+---------------------------+--------------------------------+\n");
    printf(" |  id   |            nom            |             prenom             |\n");
    printf(" +-------+---------------------------+--------------------------------+\n");

    while (i < MAX_OUVRIER) {
        if (itmp->NumRegistre != -1)
            printf(" | %05ld | %25s | %30s |\n", itmp->NumRegistre, itmp->nom, itmp->prenom);
        i++;
        itmp++;
    }
    printf(" +-------+---------------------------+--------------------------------+\n");

    printf("Entrez l\'id de l\'ouvrier a modifier (de 1 a 99999): ");
    choix = LireLong(1, 99999);

    i = RechercheIndexOuvrier(list, choix);

    if (i!=-1) {
        fp = fopen(LISTE_OUVRIER, "r+b");

        if (fp==NULL) {
            printf("Erreur : Probleme lors de l\'ouverture du fichier ! \n");
        } else {
            fseek(fp, (list[i].adresse)*sizeof(Ouvrier), SEEK_SET);
            fread(&tab,sizeof(Ouvrier),1,fp);

            printf("+--------------------------------------+\n");
            printf("|                                      |\n");
            printf("|            Modifier un ouvrier          |\n");
            printf("|                                      |\n");
            printf("+--------------------------------------+\n\n");
            printf("+----------------------------------------------------------+\n");
            printf("| %25s %30s |\n", tab.nom, tab.prenom);
            printf("+---------------------+------------------------------------+\n");
            printf("| Num Registre        | %34ld |\n", tab.NumRegistre);
            printf("| Specialite          | %34s |\n", tab.specialite);
            printf("| Date Naissance      | %26hd/%hd/%hd |\n", tab.d_naiss.jour, tab.d_naiss.mois, tab.d_naiss.annee);
            printf("| Date Engagement     | %26hd/%hd/%hd |\n", tab.d_engag.jour, tab.d_engag.mois, tab.d_engag.annee);
            printf("| Rue                 | %34s |\n", tab.rue);
            printf("| Code Postal         | %34d |\n", tab.CodePostal);
            printf("| Ville               | %34s |\n", tab.ville);
            printf("+---------------------+------------------------------------+\n");

            printf("1 - Nom\n");
            printf("2 - Prenom\n");
            printf("3 - Specalite\n");
            printf("4 - Date de naissance\n");
            printf("5 - Date d\'engagement\n");
            printf("6 - Rue\n");
            printf("7 - Code Postal\n");
            printf("8 - Ville\n");

            printf("\nChoix: ");
            choix = LireLong(1, 8);

            switch(choix) {
                case 1:
                    printf("Nom: ");
                    LireNomPropre(tab.nom, MAX_NOM);
                    strcpy(list[i].nom, tab.nom);
                    break;

                case 2:
                    printf("Prenom: ");
                    LireNomPropre(tab.prenom, MAX_PRENOM);
                    strcpy(list[i].prenom, tab.prenom);
                    break;

                case 3:
                    printf("Specialite: ");
                    LireChaine(tab.specialite, MAX_SPE);
                    break;

                case 4:
                    do {
                        printf("Date de naissance:");
                        LireChaine(pch,MAX_DATE);
                        if (!LireDate(&date, pch) || !ValideDate(&date) || !(date.annee < tab.d_engag.annee-18 ||(date.annee==tab.d_engag.annee-18 &&(date.mois<tab.d_engag.mois||(date.mois==tab.d_engag.mois && date.jour<=tab.d_engag.jour))))) { // Ouvrier de minimum 18 ans
                            vali = 0;
                            printf("Veuillez encoder une date valide\n");
                        } else {
                            vali = 1;
                        }
                    } while (!vali);

                    tab.d_naiss.jour = date.jour;
                    tab.d_naiss.mois = date.mois;
                    tab.d_naiss.annee = date.annee;
                    break;

                case 5:
                    do {
                        printf("Date d engagement:");
                        LireChaine(pch,MAX_DATE);
                        if (!LireDate(&date, pch) || !ValideDate(&date) || !(date.annee > tab.d_naiss.annee+18 ||(date.annee == tab.d_naiss.annee+18 &&(date.mois > tab.d_naiss.mois||(date.mois==tab.d_naiss.mois && date.jour>=tab.d_naiss.jour))))) {
                            vali = 0;
                            printf("Veuillez encoder une date valide\n");
                        } else {
                            vali = 1;
                        }
                    } while (!vali);

                    tab.d_engag.jour = date.jour;
                    tab.d_engag.mois = date.mois;
                    tab.d_engag.annee = date.annee;
                    break;

                case 6:
                    printf("Numero de maison: ");
                    int intNumMaison = LireEntier(1, 9999);
                    char strNumMaison2 [MAX_RUE] = {}, strNumMaison [MAX_RUE] = ", ";
                    itoa(intNumMaison, strNumMaison2, 10);
                    strcat(strNumMaison, strNumMaison2);

                    printf("Rue: ");
                    LireNomPropre(tab.rue, MAX_RUE-6);
                    strcat(tab.rue, strNumMaison);
                    break;

                case 7:
                    printf("Code Postal: ");
                    tab.CodePostal = LireEntier(1000, 9999);
                    break;

                case 8:
                    printf("Ville: ");
                    LireNomPropre(tab.ville, MAX_VILLE);
                    break;

                default:
                    printf("\nErreur : Aucun choix correspondant detecte ! \n");
            }
            strcpy(list[i].nom, tab.nom);
            fseek(fp, (list[i].adresse)*sizeof(Ouvrier), SEEK_SET);
            fwrite(&tab, sizeof(Ouvrier), 1, fp);
            fclose(fp);
            printf("\n Modification effectue avec succes ! \n");
        }
    }
}
void SuppressionOuvrier(IndexOuvrier list[MAX_OUVRIER]) { // Cas 3 = Supprimer un ouvrier //Nécésite de faire les horaires pour achever le code
    long choix = 0;
    int i = 0, indice;
    IndexOuvrier *tmp = list;
    Ouvrier otmp;
    FILE *fp;

    printf("+------------------------------------------+\n");
    printf("|                                          |\n");
    printf("|          Affichage des ouvriers          |\n");
    printf("|                                          |\n");
    printf("+------------------------------------------+\n\n");
    printf(" +-------+---------------------------+--------------------------------+\n");
    printf(" |  id   |            nom            |             prenom             |\n");
    printf(" +-------+---------------------------+--------------------------------+\n");

    while (i < MAX_OUVRIER) {
        if (tmp->NumRegistre != -1)
            printf(" | %05ld | %25s | %30s |\n", tmp->NumRegistre, tmp->nom, tmp->prenom);
        i++;
        tmp++;
    }
    printf(" +-------+---------------------------+--------------------------------+\n");

    printf("Entrez l\'id de l\'ouvrier a supprimer (de 1 a 99999): ");
    choix = LireLong(1, 99999);

    indice = RechercheIndexOuvrier(list, choix);

    if (indice != -1) {
        fp = fopen(LISTE_OUVRIER,"r+b");

        if (fp==NULL) {
            printf("Erreur : probleme lors de l\'ouverture du fichier: impossible de supprimer un ouvrier ! \n");
        } else {
            fseek(fp, list[indice].adresse*sizeof(Ouvrier), SEEK_SET);
            fread(&otmp, sizeof(Ouvrier), 1, fp);

            printf("+-------------------------------------------+\n");
            printf("|                                           |\n");
            printf("|          Supprimer d\'un ouvrier           |\n");
            printf("|                                           |\n");
            printf("+-------------------------------------------+\n\n");
            printf("+----------------------------------------------------------+\n");
            printf("| %25s %30s |\n", otmp.nom, otmp.prenom);
            printf("+---------------------+------------------------------------+\n");
            printf("| Num Registre        | %34ld |\n", otmp.NumRegistre);
            printf("| Specialite          | %34s |\n", otmp.specialite);
            printf("| Date Naissance      | %26hd/%hd/%hd |\n", otmp.d_naiss.jour, otmp.d_naiss.mois, otmp.d_naiss.annee);
            printf("| Date Engagement     | %26hd/%hd/%hd |\n", otmp.d_engag.jour, otmp.d_engag.mois, otmp.d_engag.annee);
            printf("| Rue                 | %34s |\n", otmp.rue);
            printf("| Code Postal         | %34d |\n", otmp.CodePostal);
            printf("| Ville               | %34s |\n", otmp.ville);
            printf("+---------------------+------------------------------------+\n");

            printf("Etes-vous certain de le supprimer ? (Oui = 1, Non = 2): ");
            choix = LireLong(1,2);

            if (choix == 2) {
                printf("Suppression annulee !\n");
            } else {
                if (verificationUtilisation(otmp.NumRegistre) == 0 || verificationUtilisation(otmp.NumRegistre) == 2) {
                    otmp.NumRegistre = -1;
                    fseek(fp, list[indice].adresse*sizeof(Ouvrier), SEEK_SET);
                    fwrite(&otmp, sizeof(Ouvrier), 1, fp);

                    list[indice].NumRegistre = -1;

                    printf("Suppression effectue avec succes ! \n");
                }
            }
        }
    }
}

void AffichageOuvrierList(IndexOuvrier *list) { // Cas 4 = Afficher la liste des ouvriers
    IndexOuvrier *tmp = list;
    Ouvrier otmp;
    FILE *fp;
    long choix;
    int i = 0;

    printf("+------------------------------------------+\n");
    printf("|                                          |\n");
    printf("|          Affichage des ouvriers          |\n");
    printf("|                                          |\n");
    printf("+------------------------------------------+\n\n");
    printf(" +-------+---------------------------+--------------------------------+\n");
    printf(" |  id   |            nom            |             prenom             |\n");
    printf(" +-------+---------------------------+--------------------------------+\n");

    while (i < MAX_OUVRIER) {
        if (tmp->NumRegistre != -1)
            printf(" | %05ld | %25s | %30s |\n", tmp->NumRegistre, tmp->nom, tmp->prenom);
        i++;
        tmp++;
    }
    printf(" +-------+---------------------------+--------------------------------+\n");

    printf("Voulez-vous des informations detaille ? \n");
    printf("Choix (Oui = 1, Non = 2): ");
    choix = LireLong(1,2);

    if (choix == 1) {
        printf("Entrez l\'id de l\'ouvrier voulu: ");
        choix = LireLong(1, 99999);

        i = RechercheIndexOuvrier(list, choix);

        if (i == -1)
            printf("Erreur : Id incorrect ou jeu non trouve ! \n");
        else {
            fp = fopen(LISTE_OUVRIER, "r+b");

            if (fp==NULL) {
                printf("Erreur : Impossible d'afficher les details: ouverture du fichier impossible ! \n");
            } else {
                fseek(fp, list[i].adresse*sizeof(Ouvrier), SEEK_SET);
                fread(&otmp, sizeof(Ouvrier), 1, fp);

                // system("cls");
                printf("+-------------------------------------------+\n");
                printf("|                                           |\n");
                printf("|          Affichage d\'un ouvrier           |\n");
                printf("|                                           |\n");
                printf("+-------------------------------------------+\n\n");
                printf("+----------------------------------------------------------+\n");
                printf("| %25s %30s |\n", otmp.nom, otmp.prenom);
                printf("+---------------------+------------------------------------+\n");
                printf("| Num Registre        | %34ld |\n", otmp.NumRegistre);
                printf("| Specialite          | %34s |\n", otmp.specialite);
                printf("| Date Naissance      | %26hd/%hd/%hd |\n", otmp.d_naiss.jour, otmp.d_naiss.mois, otmp.d_naiss.annee);
                printf("| Date Engagement     | %26hd/%hd/%hd |\n", otmp.d_engag.jour, otmp.d_engag.mois, otmp.d_engag.annee);
                printf("| Rue                 | %34s |\n", otmp.rue);
                printf("| Code Postal         | %34d |\n", otmp.CodePostal);
                printf("| Ville               | %34s |\n", otmp.ville);
                printf("+---------------------+------------------------------------+\n");
            }
        }
    }
}
void AffichageOuvrierDetails(IndexOuvrier *list,IndexChantier *listChantier) { // Cas 5 = Affichage des ouvriers détaillé
    short i = 0, vali = 0;
    char pch[MAX_DATE];
    IndexOuvrier *tmp = list;
    EstOccupe estOccupe;
    Date date1,date2;
    FILE *fp;
    long choix;

    printf("+------------------------------------------------------------------+\n");
    printf("|                                                                  |\n");
    printf("|                 Affichage des horaires d un ouvrier              |\n");
    printf("|                                                                  |\n");
    printf("+------------------------------------------------------------------+\n\n");
    printf(" +-------+---------------------------+--------------------------------+\n");
    printf(" |  id   |            nom            |             prenom             |\n");
    printf(" +-------+---------------------------+--------------------------------+\n");

    while (i < MAX_OUVRIER) {
        if (tmp->NumRegistre != -1)
            printf(" | %05ld | %25s | %30s |\n", tmp->NumRegistre, tmp->nom, tmp->prenom);
        i++;
        tmp++;
    }
    printf(" +-------+---------------------------+--------------------------------+\n");

    printf("Introduisez l'id de l\'ouvrier voulu: ");
    choix = LireLong(1,99999);

    i = RechercheIndexOuvrier(list,choix);

    if (i == -1) {
        printf("Erreur : Id incorrect ou ouvrier non trouve ! \n");
    } else {
        do {
            printf("Date de debut:");
            LireChaine(pch, MAX_DATE);
            if (!LireDate(&date1, pch) || !ValideDate(&date1)) {
                vali = 0;
                printf("Erreur : Veuillez encoder une date valide ! \n");
            } else {
                vali = 1;
            }
        } while (!vali);

        do {
            printf("Date de fin:");
            LireChaine(pch, MAX_DATE);
            if (!LireDate(&date2, pch) || !ValideDate(&date2) || !(date2.annee > date1.annee || (date2.annee==date1.annee && (date2.mois > date1.mois || (date2.mois==date1.mois && date2.jour>=date1.jour))))) {
                vali = 0;
                printf("Veuillez encoder une date valide et/ou superieur a la date de debut \n");
            } else {
                vali = 1;
            }
        } while (!vali);

        fp = fopen(LISTE_HORAIRE, "rb");

        if (fp != NULL) {
            printf("Liste des chantiers: \n");
            printf(" +-------+---------------------------+\n");
            printf(" |  id   |            nom            |\n");
            printf(" +-------+---------------------------+\n");
            fread(&estOccupe, sizeof(EstOccupe), 1, fp);
            while (!feof(fp)) {
                if (estOccupe.NumRegistre == choix && comparaisonDate(date1,estOccupe.date) <= 0 && comparaisonDate(date2,estOccupe.date) >= 0) {
                    i = RechercheIndexChantier(listChantier, estOccupe.NumChantier);
                    printf(" | %05ld | %25s | \n", listChantier[i].Numchantier, listChantier[i].NomChantier);
                }
                fread(&estOccupe, sizeof(EstOccupe), 1, fp);
            }
            fclose(fp);
            printf(" +-------+---------------------------+\n");
        } else {
            printf("Erreur : Echec de l'ouverture du fichier :( \n");
        }
    }
}

/* Chantier */
int verificationPlaceIndexChantier(IndexChantier list[MAX_CHANTIER]) {
    int i, trouve = 0, val = 0;

    for(i = 0; i < MAX_CHANTIER && trouve == 0; i++) {
        if (list[i].Numchantier == -1) {
            trouve = 1;
            val = list[i].adresse;
        }
    }
    return val;
}
void idExistantChantier(IndexChantier list[MAX_CHANTIER], long *idInterdit) {
    short i=0;

    while (i < MAX_CHANTIER) {
        *idInterdit = list[i].Numchantier;
        idInterdit++;
        i++;
    }
}
short verifNumChantierDispo(long *idInterdit, long id) {
    short i = 0, res = 0;

    while (i < MAX_CHANTIER && *idInterdit != id) {
        idInterdit++;
        i++;
    }

    if (*idInterdit == id)
        res = 1;
    else
        res = 0;

    return res;
}
short RechercheIndexChantier(IndexChantier *list, long valChercher) {
    short i = 0,trouve=-1;

    while (i < MAX_CHANTIER && trouve == -1) {
        if (list[i].Numchantier == valChercher)
            trouve = i;
        else
            i++;
    }
    return trouve;
}

void initialisationChantier(IndexChantier *list) {
    short i=0;
    IndexChantier *tmp = list;
    Chantier chantier;
    FILE *fp;
    fp = fopen(LISTE_CHANTIER,"r+b");

    if (fp==NULL) {
        printf("Chantier>  Fichier non existant => creation d'un nouveau fichier\n\n");
        fp = fopen(LISTE_CHANTIER,"w+b");

        while (i<MAX_CHANTIER) {
            tmp->adresse = i;
            tmp->Numchantier = -1;
            i++;
            tmp++;
        }
    } else {
        printf("Chantier> Lecture du fichier\n");
        fread(&chantier,sizeof(Chantier),1,fp);

        while (!feof(fp) && i<MAX_CHANTIER) {
            tmp->adresse = i;
            tmp->Numchantier = chantier.Numchantier;
            strcpy(tmp->NomChantier,chantier.NomChantier);
            fread(&chantier,sizeof(Chantier),1,fp);
            i++;
            tmp++;
        }

        while (feof(fp) && i<MAX_CHANTIER) {
            tmp->adresse = i;
            tmp->Numchantier = -1;
            i++;
            tmp++;
        }
    }
    fclose(fp);
}
void triIndexChantier(IndexChantier *listChantier) {
    int i, taille = MAX_CHANTIER, tab_en_ordre = 0;

    while (tab_en_ordre == 0) {
        tab_en_ordre = 1;
        for(i = 0; i < taille-1; i++) {
            if (listChantier[i].Numchantier != -1 && listChantier[i+1].Numchantier != -1 && strcmp(listChantier[i].NomChantier,listChantier[i+1].NomChantier) == 1) {
                EchangeChantier(&listChantier[i],&listChantier[i+1]);
                tab_en_ordre = 0;
            }
        }
        taille--;
    }
}
void EchangeChantier(IndexChantier *tmp1, IndexChantier *tmp2) {
    IndexChantier tmp;

    strcpy(tmp.NomChantier,tmp1->NomChantier);
    tmp.Numchantier = tmp1->Numchantier;
    tmp.adresse = tmp1->adresse;

    strcpy(tmp1->NomChantier,tmp2->NomChantier);
    tmp1->Numchantier = tmp2->Numchantier;
    tmp1->adresse = tmp2->adresse;

    strcpy(tmp2->NomChantier,tmp.NomChantier);
    tmp2->Numchantier = tmp.Numchantier;
    tmp2->adresse = tmp.adresse;
}

void AjoutChantier(IndexChantier list[MAX_CHANTIER]) { // Cas 6 = Ajouter un chantier
    short erreur = 0;
    long idInterdit[MAX_CHANTIER];
    int i;
    Chantier tab;
    FILE *fp;

    printf("+----------------------------+\n");
    printf("|     Ajouter un chantier    |\n");
    printf("+----------------------------+\n");

    fp = fopen(LISTE_CHANTIER,"r+b");

    if (fp == NULL) {
        printf("Erreur : Ajout de donnees impossible ! \n");
    } else {
        i = verificationPlaceIndexChantier(list);
        idExistantChantier(list,idInterdit);

        if (i != -1) {
            list[i].adresse = i;
            do {
                if (erreur == 1) printf("Valeur Interdite car deja utilisee !\n");
                printf("Numero de chantier (1 a 99999): ");
                tab.Numchantier = LireLong(1,99999);

                if (verifNumChantierDispo(idInterdit,tab.Numchantier) == 0)
                    erreur = 0;
                else
                    erreur = 1;

            } while (erreur == 1);
            list[i].Numchantier = tab.Numchantier;

            printf("Nom du chantier: ");
            LireNomPropre(tab.NomChantier, MAX_NOMCHAN);
            strcpy(list[i].NomChantier, tab.NomChantier);

            printf("Rue: ");
            LireNomPropre(tab.rue, MAX_RUE);

            printf("Code Postal: ");
            tab.CodePostal = LireEntier(1000, 9999);

            printf("Ville: ");
            LireNomPropre(tab.ville, MAX_VILLE);

            printf("Maitre d\'ouvrage: ");
            LireChaine(tab.MaitreOuvrage, MAX_MAITRE);

            printf("Montant des travaux: ");
            tab.Montant = LireFloat(0,16000000);

            printf("Duree des travaux: ");
            tab.Duree = LireEntier(1, 9999);

            printf("Code Gerant: ");
            LireChaine(tab.CodeGerant, MAX_GERANT);

            if (fseek(fp,sizeof(Chantier)*i, SEEK_SET) == 0) {
                fwrite(&tab,sizeof(Chantier),1,fp);
                printf("Le chantier a bien ete ajoute\n");
            } else {
                printf("Ajout au fichier impossible !\n");
                list[i].adresse = -1;
                list[i].Numchantier = -1;
                strcpy(list[i].NomChantier," ");
            }
        } else {
            printf("Plus de place Disponible");
        }
        fclose(fp);
    }
}

void AffichageChantierList(IndexChantier *list) { // Cas 7 = Afficher la liste des chantiers
    long choix = 0;
    short i = 0;
    IndexChantier *tmp = list;
    Chantier otmp;
    FILE *fp;

    printf("+-------------------------------------------+\n");
    printf("|                                           |\n");
    printf("|          Affichage des chantiers          |\n");
    printf("|                                           |\n");
    printf("+-------------------------------------------+\n\n");
    printf(" +-------+---------------------------+\n");
    printf(" |  id   |            nom            |\n");
    printf(" +-------+---------------------------+\n");
    while (i < MAX_CHANTIER) {
        if (tmp->Numchantier != -1)
            printf(" | %05ld | %25s | \n", tmp->Numchantier, tmp->NomChantier);
        i++;
        tmp++;
    }
    printf(" +-------+---------------------------+\n");

    printf("Voulez-vous des informations detaille ?\n");
    printf("Choix (Oui = 1, Non = 2): ");
    choix = LireLong(1,2);

    if (choix == 1) {
        printf("Entrez l\'id du chantier : ");
        choix = LireLong(1,99999);

        i = RechercheIndexChantier(list,choix);

        if (i == -1) {
            printf("Erreur : Id incorrect !\n");
        } else {
            fp = fopen(LISTE_CHANTIER,"r+b");

            if (fp==NULL) {
                printf("Impossible d'afficher les details: ouverture du fichier impossible !");
            } else {
                fseek(fp, list[i].adresse*sizeof(Chantier), SEEK_SET);
                fread(&otmp, sizeof(Chantier), 1, fp);

                printf("+--------------------------------------------+\n");
                printf("|                                            |\n");
                printf("|          Affichage d\'un chantier           |\n");
                printf("|                                            |\n");
                printf("+--------------------------------------------+\n\n");
                printf("+-------------------------------------------------------------+\n");
                printf("|                                   %25s |\n", otmp.NomChantier);
                printf("+------------------------+------------------------------------+\n");
                printf("| Num de chantier        | %34ld |\n", otmp.Numchantier);
                printf("| Rue                    | %34s |\n", otmp.rue  );
                printf("| Code Postal            | %34d |\n", otmp.CodePostal);
                printf("| Ville                  | %34s |\n", otmp.ville);
                printf("| Maitre d ouvrage       | %34s |\n", otmp.MaitreOuvrage);
                printf("| Montant                | %34.2f |\n", otmp.Montant);
                printf("| Duree                  | %34d |\n", otmp.Duree);
                printf("| Code Gerant            | %34s |\n", otmp.CodeGerant);
                printf("+------------------------+------------------------------------+\n");
            }

        }
    }
}
void AffichageOuvrierPourChantier(IndexChantier *listChantier,IndexOuvrier *listOuvrier) { // Cas 8 = Affichage des ouvriers pour un chantier
    long choix = 0;
    short i = 0;
    IndexChantier *tmp = listChantier;
    Chantier otmp;
    FILE *fp;
    EstOccupe estOccupe;

    printf("+------------------------------------------------------------------+\n");
    printf("|                                                                  |\n");
    printf("|      Affichage des ouvriers ayant travaille sur un chantier      |\n");
    printf("|                                                                  |\n");
    printf("+------------------------------------------------------------------+\n\n");
    printf(" +-------+---------------------------+\n");
    printf(" |  id   |            nom            |\n");
    printf(" +-------+---------------------------+\n");
    while (i < MAX_CHANTIER){
        if (tmp->Numchantier != -1)
            printf(" | %05ld | %25s | \n", tmp->Numchantier, tmp->NomChantier);
        i++;
        tmp++;
    }
    printf(" +-------+---------------------------+\n");

    printf("Introduisez l'id du chantier voulu: ");
    choix = LireLong(1,99999);

    i = RechercheIndexChantier(listChantier,choix);

    if (i == -1) {
        printf("Erreur : Id incorrect ! \n");
    } else {
        fp = fopen(LISTE_HORAIRE, "rb");

        if (fp != NULL) {
            printf("Liste des ouvriers: \n");
            printf(" +-------+---------------------------+--------------------------------+\n");
            printf(" |  id   |            nom            |             prenom             |\n");
            printf(" +-------+---------------------------+--------------------------------+\n");
            fread(&estOccupe,sizeof(EstOccupe), 1, fp);
            while (!feof(fp)) {
                if (estOccupe.NumChantier == choix){
                    i = RechercheIndexOuvrier(listOuvrier, estOccupe.NumRegistre);
                    printf(" | %05ld | %25s | %30s |\n", listOuvrier[i].NumRegistre, listOuvrier[i].nom, listOuvrier[i].prenom);
                }
                fread(&estOccupe, sizeof(EstOccupe), 1, fp);
            }
            fclose(fp);
            printf(" +-------+---------------------------+--------------------------------+\n");
        } else {
            printf("Erreur : Echec de l'ouverture du fichier :( \n");
        }
    }
}

/* Horaire */
short verificationUnicite(EstOccupe *tab) {
    short res = 0;
    FILE *fp = fopen(LISTE_HORAIRE, "r+b");
    EstOccupe tmp;

    if (fp != NULL) {
        fread(&tmp,sizeof(EstOccupe), 1, fp);
        while (!feof(fp) && res != 1)
        {
            if (tmp.NumRegistre == tab->NumRegistre && tmp.NumChantier == tab->NumChantier && tmp.date.jour == tab->date.jour && tmp.date.mois == tab->date.mois && tmp.date.annee == tab->date.annee) {
                printf("Donnees deja enregistree !");
                res = 1;
            }
            fread(&tmp,sizeof(EstOccupe), 1, fp);
        }
        fclose(fp);
    } else {
        res = 0;
    }
    return res;
}
void ecrirePartieFichiers(EstOccupe *tab) {
    FILE *fp = fopen(LISTE_HORAIRE, "a+b");

    if (fp != NULL) {
        fseek(fp,0,SEEK_END);
        fwrite(tab, sizeof(EstOccupe), 1, fp);
        fclose(fp);
    } else {
        printf("Erreur : probleme lors de l'ouverture du fichier ! \n");
    }
}

void AjoutHoraire(IndexChantier *listChantier,IndexOuvrier *listOuvrier) {
    char pch[MAX_DATE];
    long choix, tmpL, idInterdit[MAX_OUVRIER];
    short i = 0, indice = -1, nbreJ, trouve, vali = 0;
    Date date;
    EstOccupe tmp;
    IndexChantier *tabChantier = listChantier;
    IndexOuvrier *tabOuvrier = listOuvrier;

    printf("+-------------------------------------------+\n");
    printf("|                                           |\n");
    printf("|          Affichage des chantiers          |\n");
    printf("|                                           |\n");
    printf("+-------------------------------------------+\n\n");
    printf(" +-------+---------------------------+\n");
    printf(" |  id   |            nom            |\n");
    printf(" +-------+---------------------------+\n");

    while (i < MAX_CHANTIER) {
        if (tabChantier->Numchantier != -1)
            printf(" | %05ld | %25s | \n", tabChantier->Numchantier, tabChantier->NomChantier);
        i++;
        tabChantier++;
    }
    printf(" +-------+---------------------------+\n");

    printf("Entrez l\'id d\'un chantier : ");
    choix = LireLong(1,99999);

    indice = RechercheIndexChantier(listChantier, choix) ;

    if (indice != -1) {
        tmp.NumChantier = choix;

        do {
            printf("Date du chantier: ");
            LireChaine(pch, MAX_DATE);
            if (!LireDate(&date, pch) || !ValideDate(&date)) {
                vali = 0;
                printf("Erreur : Veuillez encoder une date valide ! \n");
            } else {
                vali = 1;
            }
        } while (!vali);

        tmp.date.jour = date.jour;
        tmp.date.mois = date.mois;
        tmp.date.annee = date.annee;

        printf("Heures: ");
        LireHeure(tmp.heures, 3, 10); // Source temps de travail maximum : https://emploi.belgique.be/fr/themes/international/detachement/conditions-de-travail-respecter-en-cas-de-detachement-en-1

        printf("+------------------------------------------+\n");
        printf("|                                          |\n");
        printf("|          Affichage des ouvriers          |\n");
        printf("|                                          |\n");
        printf("+------------------------------------------+\n\n");
        printf(" +-------+---------------------------+--------------------------------+\n");
        printf(" |  id   |            nom            |             prenom             |\n");
        printf(" +-------+---------------------------+--------------------------------+\n");

        i = 0;
        while (i < MAX_OUVRIER) {
            if (tabOuvrier->NumRegistre != -1)
                printf(" | %5ld | %25s | %30s |\n", tabOuvrier->NumRegistre, tabOuvrier->nom, tabOuvrier->prenom);
            i++;
            tabOuvrier++;
        }
        printf(" +-------+---------------------------+--------------------------------+\n");

        idExistantOuvrier(listOuvrier,idInterdit);

        do {
            printf("ID de l\'ouvrier (-1 pour annuler): ");
            tmpL = LireLong(-1,99999);
        } while ((tmpL != -1 && verifNumOuvrierDispo(idInterdit,tmpL) != 1) || tmpL == 0);

        if (tmpL >= 0) {
            tmp.NumRegistre = tmpL;

            if (verificationUnicite(&tmp) == 0) {
                ecrirePartieFichiers(&tmp);
                i++;
            } else {
                printf("Erreur : Problème unicite des donnees => Annulation de l\'action en cours ! \n");
            }
        } else {
            printf("Ajout annule !\n");
        }
    } else {
        printf("Erreur : Pas de chantier trouve ! \n");
    }
}

void AffichageHoraires() {
    int i;
    EstOccupe tmp;
    FILE *fp;

    printf("+---------------------------------------+\n");
    printf("|                                       |\n");
    printf("|         Affichage des horaires        |\n");
    printf("|                                       |\n");
    printf("+---------------------------------------+\n\n");

    fp = fopen(LISTE_HORAIRE, "rb");

    if (fp != NULL) {
        printf("+--------------+-------------+------------+-------+\n");
        printf("| NumRegistre  | NumChantier |    Date    | Temps |\n");
        printf("+--------------+-------------+------------+-------+\n");
        fread(&tmp, sizeof(EstOccupe), 1, fp);

        while (!feof(fp)) {
            printf("| %12ld |", tmp.NumRegistre);
            printf(" %11ld |", tmp.NumChantier);
            printf(" %02hd/%02hd/%04hd ", tmp.date.jour, tmp.date.mois, tmp.date.annee);
            printf("| %s |\n", tmp.heures);
            fread(&tmp,sizeof(EstOccupe), 1, fp);
        }
        printf("+--------------+-------------+------------+-------+\n");
        fclose(fp);
    } else {
        printf("Erreur : Echec de l'ouverture du fichier :( \n");
    }
}