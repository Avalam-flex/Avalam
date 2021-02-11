#include <stdio.h>
#include "../include/avalam.h"

int main()
{
    T_Position plateau = getPositionInitiale();
    T_ListeCoups CoupLeg;
    T_Coup coup;
    T_Score score;
    int i;
    char *fic;
    fic = "../build/web/exemples/refresh-data.js"; //adresse, provisoire
    Refresh(fic,plateau);
    plateau.trait = JAU; //jaune commence
    do
    {
        printf("A %hhd de jouer\n",plateau.trait);
        printf("Entrer origine\n");
        scanf("%hhd",&coup.origine);
        printf("\nEntrer destination\n");
        scanf("%hhd",&coup.destination);
        plateau = jouerCoup(plateau,coup.origine,coup.destination);
        CoupLeg =  getCoupsLegaux(plateau);
        Refresh(fic,plateau);
    }
    while(CoupLeg.nb != 0 || !estValide(plateau,coup.origine,coup.destination));
    score = evaluerScore(plateau);
    printf("le score de sommet pour les jaunes: %hhd",score.nbJ);
    printf("le score de sommet pour les rouges:%hhd",score.nbR);
}
