/*
CARTOUCHE DE PRESENTATION 
Date de programmation : 25/02/2021

9_Avalam_Flex_standalone.c_B2
    Nom de l’équipe :  AVALAM FLEX
    Groupe TP : B2
    Numéro de groupe : 14

Description de l'algorithme : 
    standalone.exe :  Jeu à deux joueurs, saisie des coups au clavier, affichage par avalam-refresh
        Permet de jouer à deux joueurs
        Saisie des coups chacun son tour au clavier
        A chaque coup, produit un fichier json avec la position et le score
        Ce fichier sera utilisé par avalam-refresh.html; Le nom du fichier facultatif peut être passé en ligne de commandes ; Un nom par défaut est prévu dans le programme
        Détecte la fin de la partie et affiche le score à l’écran
*/

#include <stdio.h>
#include "../include/avalam.h"
#include <string.h>

int main(int argc, char *argv[])
{
    char fic[] = "../build/web/exemples/refresh-data.js"; //adresse, provisoire
    if(argc == 1)
    {
        standalone(fic);
    }
    else if(argc == 2)
    {
        standalone(argv[1]); //peut causer core dump si adresse fausse
    }
    else{
        printf("erreur trop d'arguments");
    }

}

void standalone(char *fic)
{
    T_Position plateau = getPositionInitiale();
    T_ListeCoups CoupLeg;
    T_Coup coup;
    T_Score score;
    Refresh(fic,plateau);
    plateau.trait = JAU; //jaune commence
    do
    {
        do
        {//On demande au joueur son coup et on récupère la position indiquée
            printf("A %s de jouer\n",COLNAME(plateau.trait));
            printf("Entrer origine\n");
            scanf("%hhd",&coup.origine);
            printf("\nEntrer destination\n");
            scanf("%hhd",&coup.destination);
        }
        while(!estValide(plateau,coup.origine,coup.destination));//On redemmande le coup au joueur si il n'est pas valide 
            plateau = jouerCoup(plateau,coup.origine,coup.destination);

        #ifdef __DEBUG__ //Affichage des valeurs pour le mod __Debug__
            printf("\n__DEBUG__");
            printf("\ncoup origine saisit : %hhd\n",coup.origine);
            printf("coup destination saisit : %hhd\n",coup.destination);
            printf("Nb coup légaux <%d>\n",CoupLeg.nb);
            printf("trait: <%d>\n", plateau.trait);                                 
            printf("Nb points Jaune: <%d>\n", score.nbJ);   
            printf("Nb tour à 5 Jaune: <%d>\n", score.nbJ5);   
            printf("Nb points Rouge: <%d>\n", score.nbR);    
            printf("Nb tour à 5 Rouge: <%d>\n", score.nbR5);
            printf("FIN __DEBUG__");
        #endif
        Refresh(fic,plateau);
        CoupLeg =  getCoupsLegaux(plateau);
        score = evaluerScore(plateau);
        afficherScore(score);
    }
    while(CoupLeg.nb != 0);//fin de partie si plus de coup légaux
    printf("fin de la partie");
}


void Refresh(const char *fic,const T_Position plateau)//opti const
{
	int i=0;
	T_Score score = evaluerScore(plateau); //on get le score pour afficher
	FILE *fp = fopen (fic, "w+");
    fprintf(fp,"%s","traiterJson({\n");  //1ère ligne
	fprintf(fp,"\n	%s:%d,\n",STR_TURN,plateau.trait);
	fprintf(fp,"	%s:%d,\n",STR_SCORE_J,score.nbJ);
	fprintf(fp,"	%s:%d,\n",STR_SCORE_J5,score.nbJ5);//on copie le score des deux joueurs
	fprintf(fp,"	%s:%d,\n",STR_SCORE_R,score.nbR);
	fprintf(fp,"	%s:%d,\n",STR_SCORE_R5,score.nbR5);
	fprintf(fp,"	%s:[\n",STR_COLS);
	for(i=0;i<NBCASES;i++) 
	{
		fprintf(fp,"        {%s:%d,%s:%d},\n",STR_NB,plateau.cols[i].nb,STR_COULEUR,plateau.cols[i].couleur);//motif répété sur NBCASES-1	
	}
	fprintf(fp,"%s","]\n});"); //fin de mon fichier
	fclose(fp);//on ferme
}
