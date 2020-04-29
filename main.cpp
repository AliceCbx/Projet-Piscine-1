#include <iostream>
#include "Graphe.h"
#include "Svgfile.h"
#include <string>

void menu()             //menu
{
    int fin=0;
    std::string nom = "vide", nom2 = "vide";
    int choix1;
    std::string choix2;

    do
    {
        std::cout << std::endl << "------- MENU -------" << std::endl;
        std::cout << std::endl << "1. Charger un graphe\n"
                  "2. Changer le systeme de ponderation d'un graphe\n"
                  "3. Calculer l'indice de centralite\n"
                  "4. Tester la vulnerabilite\n"
                  "5. Quitter le menu" << std::endl;

        std::cout << std::endl << "Choisissez une option !" << std::endl;               //choix de l'option

        std::cin >> choix1;                         //récupération du choix utilisateur

        while((choix1 > 5)|| (choix1 < 0))          //blindage
        {
            std::cout << "Ne correspond pas a un choix possible\n"
                      "Veuillez ressaisir votre choix" << std::endl;

            std::cin >> choix1;
        }

        if((nom == "vide")&&(nom2 == "vide"))                    //vérification chargement de fichiers
        {
            std::cout << "Il n'y a pas de graphe deja charge\n"
                      "Veuillez en charger un avant toutes actions\n\n"
                      "Veuillez choisir le nom du fichier (topologie) en .txt " << std::endl;

            std::cin >> nom;
        }

        Graphe g{nom,nom2};         //ouverture du fichier

        Svgfile svgout;

        switch(choix1)
        {
        case 1:                         //choix 1 : chargement d'une pondération
        {
            std::cout << "Voulez-vous ouvrir un nouveau fichier (OUI_G/OUI_P/NON) ?\n";
            std::cin >> choix2;

            while((choix2 != "OUI_P")&&(choix2 != "NON")&&(choix2 != "OUI_G"))
            {
                std::cout << "Ne correspond pas a un choix possible\n"
                          "Veuillez ressaisir votre choix" << std::endl;

                std::cin >> choix2;
            }

            if(choix2 == "OUI_G")
            {
                std::cout << "Vous souhaitez charger un graphe\n"
                          "Veuillez choisir le nom du fichier (topologie) en .txt" << std::endl;
                std::cin >> nom;
            }

            if(choix2 == "OUI_P")
            {
                std::cout << "Vous souhaitez charger une ponderation\n"
                          "Veuillez choisir le nom du fichier (ponderation) en .txt" << std::endl;
                std::cin >> nom2;
            }
            else
                nom2 = "vide";

            Graphe g{nom,nom2};     //ouverture du fichier
            g.afficher();
            g.afficherGrapheSvg(&svgout);
        }
        break;

        case 2:
        {
            std::cout << "Vous souhaitez charger une nouvelle ponderation\n"
                      "Veuillez choisir le nom du fichier (ponderation) en .txt" << std::endl;

            std::cin >> nom2;

            Graphe p{nom,nom2};
            p.afficherGrapheSvg(&svgout);
        }
        break;

        case 3:
        {
            std::cout <<"Centralité degre:"<< std::endl;
            //g.calculCentraliteDegre();
            g.sauvegarde();

            std::cout <<"Centralité VP:"<< std::endl;
            //g.calculCentraliteVP();
            g.sauvegarde();

            std::cout <<"Centralité proximite:"<< std::endl;
            g.proximite(choix2,g);
            std::cout  << "ok5"<< std::endl;
        }
        break;

        case 4:
            std::cout << "4_OK" << std::endl;
            break;

        case 5:
        {
            fin = 1;
        }

        break;

        }
    }
    while(fin != 1);
}



int main()
{

    menu();         //appel du menu

    return 0;
}
