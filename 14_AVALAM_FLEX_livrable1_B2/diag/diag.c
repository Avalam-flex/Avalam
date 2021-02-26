
/*
CARTOUCHE DE PRESENTATION 
Date de programmation : 25/02/2021

9_Avalam_Flex_standalone.c_B2
    Nom de l’équipe :  AVALAM FLEX
    Groupe TP : B2
    Numéro de groupe : 14

Description de l'algorithme : 
    diag.exe :  Saisie de positions sur la ligne de commande, affichage par avalam-diag
        Permet de passer, en ligne de commandes : un numéro du diagramme ; une position, type “FEN”
        Une fois l’exécutable lancé, saisies au clavier : du nom du fichier à produire [un nom par défaut est prévu] ; d’une chaîne de description [vide par défaut]
        Produit un fichier json avec la position et la description, pouvant être utilisé par avalam-diag
*/

#include <stdio.h>
#include <string.h>
#include "../include/avalam.h"

//MACRO CONSTANTES
#define MAXCAR 200 //Taille fichier redirection
#define MAXFEN 50 //Taille chaine type FEN

//DEBUT main
int main(int argc,char *fen[])
{

//DECLARATION DES VARIABLES
    int etat = 1;
    int i=0,j=0;//Compteurs de tableau
    char FEN2[MAXFEN]=""; //chaine filtrée
    char trait; //chaine pour le trait
    char carac;//chaine de caractère pour la description du plateau
    char fic[MAXCAR] = "../build/web/data/"; //adresse manque le nom du fichier
    char note[MAXCAR]="";//chaine de caractère pour le fichier redirectionné
    char fichier[50] = "diag.js"; //fichier dans lequel sera affiché le plateau
    T_Position plateau;
//FIN DECLARATION

    //S'il n'y a pas 3 arguments 
    if(argc != 3) //error trop de champs
    {   
        //On demmande à l'utilisateur de rsaisir à nouveau sa commande
        printf("Votre commande est invalide, formats corrects : \n<<numéro diagramme>> <<fen>> \n<<numéro diagramme>> <<fen>> < FICHIER\n");
    }
    //Sinon, on lance le programme
    else
    {
    
    fen[2][50] = '\0'; //chaine de 50 carac max, on coupe la fen si trop grande
    printf("Diagramme : %s\n",fen[1]);//Affiche le numéro du diagramme
    printf("Votre chaine : %s \n",fen[2]);//Affiche la chaine de l'utilisateur
    printf("Nom du fichier? (diag.js) par défaut dans ./web/data\n");//Affichage du fichier par défaut 
    fgets(fichier,50,stdin);//On saisie le nom du fichier s'il y en a un 

    if(fichier[0]!= '\n')//Si le fichier a été tapé
    {
        fichier[strlen(fichier)-1] = '\0';//On place un '\0' à la fin du nom
        strcat(fic,fichier);//Concaténation de fic et fichier
    }
    else{ //Le fichier n'a pas été tapé
        strcat(fic,"diag.js"); //fichier = diag d'origine
    }
    printf("Entrer votre description et CTRL D: \n"); //On demande la description que l'utilisateur souhaite ajouter ou non à ce plateau
    do{  //On écrit dans la chaine carac
        carac = fgetc(stdin); //fgetc = caractère par caractère 
        if(carac != '\n' && carac != EOF)//EOF = caractère de fin de chaine
        {//Si on ne passe pas de lignes et que la chaine n'est pas finie3

            note[i] = carac; //alors, on écrit
            i++; //On passe au prochain rang
        }
    }
    while(carac != EOF && strlen(note) <200) ;//ce while permet de faire une description sur plusieurs lignes

    //FORMATAGE TYPE FEN
    i = 0;
    do
    {
        switch(etat)
        {

            case 1 : //Si c'est le caractère est soit : u, d, t, q, c, U, D, T, Q, C  ( || = ou )
                if (fen[2][i]=='u'|| fen[2][i]=='U' || fen[2][i]=='d' || fen[2][i]=='D' || fen[2][i]=='t' || fen[2][i]=='T' || fen[2][i]=='q' || fen[2][i]=='Q' || fen[2][i]=='c' || fen[2][i]=='C')//toutes les lettres connues utilisable
                    {
                        FEN2[j]=fen[2][i];//alors on l'écrit dans la chaine FEN
                        i++;//on passe au prochain caractère
                        j++;//on se prépare à écrire le prochain caractère dans la chaine FEN
                        etat=1;//on reppase dans ce filtre pour voir si le prochain caractère est valide
                    }
                else if (fen[2][i]=='r' || fen[2][i]=='j') //si c'est un trait
                    {
                        trait=fen[2][i]; //on stock le trait dans une chaine à part
                        i++;//on regarde la prochain caractère 
                        etat=1;//on reppase au début du 1er filtre
                    }
                else {etat=2;} //else on check si c'est un nombre
                break;

            case 2 : // On verifie si c'est un chiffre (code ASCII Chiffre - code ASCII de 0)
            //Si c'est un chiffre alors son CODE ASCII - CODE ASCII DE 0 sera >= 0 et <10
                if((fen[2][i]-48)>=0 && (fen[2][i]-48)<10) //verifie si chiffre code ASCII - code ASCII de 0
                {
                        FEN2[j]=fen[2][i];// on l'écrit dans la chaine FEN
                        i++;//on passe au prochain caractère
                        j++;//on se prépare à écrire le prochain caractère dans la chaine FEN
                        etat=1;//on reppase dans le 1er filtre
                }
                //Sinon, ce n'est pas un chiffre
                else {etat=1; i++;}//alors on reppase au 1er filtre
                break;

        }    
    } while (fen[2][i]!='\0');//On filtre jusqu'à la fin de la chaîne de l'utilisateur

    FEN2[j] = ' '; //espace pour le trait à la fin de la chaine FEN
    if( trait!='j' && trait!='r')//si on a pas trouvé de r ou de j dans la chaine de l'utilisateur
    {
        FEN2[j+1]='j'; //choix par défaut : jaune
    }
    else{ //Sinon, on a trouvé un j ou un r
        FEN2[j+1]=trait; //Alors, on l'écrit
    }
    #ifdef __DEBUG__ //MODE DEBUG
        printf("__DEBUG__");
        printf("Nouveau type FEN: <<%s>>\n",FEN2);//affichage de la chaine FEN formatée
    #endif
    plateau = interpreteur(FEN2); //On appelle la fonction interpéteur 
    Refresh_diag(fic,plateau,fen[2],fen[1],note);//On appelle la fonction Refresh_diag
//C'est deux fonctionne sont codée en dehors du main, en dessous.
}
    return 0;//FIN DU PROGRAMME
}




T_Position interpreteur(const char *FEN2){
//Cette fonction a pour but d'interpreter la chaîne type FEN
//Un "u" doit ajouter un pion jaune
//Un "j" ou un "r" doit donner un trait de 1 ou 2
//Si il y a 2 chiffres d'affilé, il faut les concaténer et donner n nombres d'espaces

    int i = 0; //compteur pour les cols du plateau
    int k = 0; //compteur pour la chaine FEN2
    int j;
    T_Position plateau;
    int chx;
    for(j=0;j<NBCASES;j++) //ce for clean le plateau pour n'avoir aucun pion
	{
		plateau.cols[j].couleur=0;//motif répété sur NBCASES-1	
        plateau.cols[j].nb=0;
	}//ici, le plateau est vide.


    //INTERPETATION DES CHIFFRES
    while(FEN2[k]!=' ')//tant que pas d'espace
    {
        //Cette double condition vérifie s'il y a deux chiffres d'affilé, si oui on les regroupe
       
        if((FEN2[k]-48)>=0 && (FEN2[k]-48)<10) //verifie si chiffre: (code ASCII - code ASCII de 0)
        {
            if ((FEN2[k]-48)>=0 && (FEN2[k+1]-48)<10) //verifie si nombre à 2 chiffres
                {
                    i = (((FEN2[k]-48)*10) + (FEN2[k+1]-48) + i); //1 chiffre * 10 + 2e chiffre (ex : 12 = 1 *10 + 2)
                    k = k+2; //On regarde le caractère après le nombre à deux chiffres
                }

            //Si c'est pas un nombre
            else{
                k++; //On regarde la prochain caractère a interpreter
                i = (FEN2[k]-48) + i; //on place les espaces
            }
        }
        else //donc c'est un char (caractère)
        {
        

        //INTERPRETATION DES CARACTERES
        chx = FEN2[k];
        switch(chx){
            //En fonction du code ASCII du caractère on entre dans son case
            //C'est toujours le même principe :
            case 67: //Si c'est un 'C'
                plateau.cols[i].nb = 5; //On associe 5 au nombres de pions pour le colonne
                plateau.cols[i].couleur = ROU; //On donne la couleur rouge au pion
                break;
            case 68://D
                plateau.cols[i].nb = 2;//2 pions pour la colonne
                plateau.cols[i].couleur = ROU;//couleur rouge
                break;
            case 81://Q
                plateau.cols[i].nb = 4;//4 pions pour la colonne
                plateau.cols[i].couleur = ROU;//couleur rouge
                break;
            case 84://T
                plateau.cols[i].nb = 3;//3 pions pour la colonne
                plateau.cols[i].couleur = ROU;//couleur rouge
                break;          
            case 85://U
                plateau.cols[i].nb = 1;//1 pion pour la colonne
                plateau.cols[i].couleur = ROU;//couleur rouge
                break;

                 //Cas pour les rouges terminés

            case 99: //c
                plateau.cols[i].nb = 5;//5 pions pour la colonne
                plateau.cols[i].couleur = JAU; //couleur jaune
                break;
            case 100://d
                plateau.cols[i].nb = 2;//2 pions pour la colonne
                plateau.cols[i].couleur = JAU; //couleur jaune
                break;
            case 111://q
                plateau.cols[i].nb = 4;//4 pions pour la colonne
                plateau.cols[i].couleur = JAU; //couleur jaune
                break;
            case 116://t
                plateau.cols[i].nb = 3;//3 pions pour la colonne
                plateau.cols[i].couleur = JAU; //couleur jaune
                break;          
            case 117://u
                plateau.cols[i].nb = 1;//1 pion pour la colonne
                plateau.cols[i].couleur = JAU; //couleur jaune
                break;

                 //Cas pour les jaunes terminés
        }
        //Quand on est passé dans le switch ; on regarde le prochain caractère
        i++; 
        k++;
        }
    }

    //INTERPRETATION DU TRAIT
    if(FEN2[k+1] == 'j'){//Si le dernier caractère est j
        plateau.trait = 1;//alors trait jaune
    }
    else{//Sinon, il est rouge 
        plateau.trait = 2;//alors trait rouge
    }
    #ifdef __DEBUG__ //MODE DEBUG
        for(int cpt = 0;cpt < NBCASES;cpt++)
        {
            //Affichage de chaque pions par colonne et de leur couleur
            printf("Nb de pions sur la colonne %d: %d\n",cpt+1,plateau.cols[cpt].nb);
            printf("Couleur %d: %d\n",cpt+1,plateau.cols[cpt].couleur);
        }
        printf("FIN __DEBUG__\n");
    #endif
    
    return plateau;
}



void Refresh_diag(const char *fic,const T_Position plateau,const char *FEN,const char *num,const char *notes)
//Cette fonction a but but de rafraichir le tableau de jeu une fois l'interpretation faite afin de simplement voir le plateau avec ces pions
{
    int i=0;
	FILE *fp = fopen (fic, "w+");
    //On affiche tout le nécessaires dans le fichier d'affichage
    fprintf(fp,"%s","traiterJson({\n");  //1ère ligne
    fprintf(fp,"\n	%s:%d,\n",STR_TURN,plateau.trait);//affichage du trait
    fprintf(fp,"	%s:\"%s\",\n",STR_NUMDIAG,num); // affichage du numéro de diagramme
    fprintf(fp,"	%s:\"%s\",\n",STR_NOTES,notes);// affichage de la description
    fprintf(fp,"	%s:\"%s\",\n",STR_FEN,FEN);// affichage de la chaine FEN
    fprintf(fp,"	%s:[\n",STR_COLS);
	for(i=0;i<NBCASES;i++) 
	{
		fprintf(fp,"        {%s:%d,%s:%d},\n",STR_NB,plateau.cols[i].nb,STR_COULEUR,plateau.cols[i].couleur);//motif répété sur NBCASES-1	
	}
	fprintf(fp,"%s","]\n});"); //fin de mon fichier
	fclose(fp);//on ferme
}