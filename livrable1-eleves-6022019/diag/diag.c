#include <stdio.h>
#include <string.h>
#include "../include/avalam.h"

int main()
{
    char tab[50];
    int etat = 1;
    int i;
    do
    {
    printf("entrez votre fen");
    fgets(tab,50,stdin);
    while(tab[i] != '\0')
    {
        i++ ;
        //add filtres
    }
    if(tab[i-1]!= "r") printf("chaîne fausse");
    }
    while(etat == 1);


    return 0;
}