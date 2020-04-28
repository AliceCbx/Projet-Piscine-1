#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Graphe::Graphe(std::string nomFichiertopo,std::string nomFichierpond)              //constructeur
{
    std::ifstream ifs{nomFichiertopo};              //lecture du fichier
    if (!ifs)
        std::cout<<"pb d'ouverture ou nom du fichier\n";


    ifs >> m_orient;                            //lecture de l'orientation
    if ( ifs.fail() )
        std::cout << "pb de lecture orientation\n";

    double ordre;
    ifs >> ordre;                               //lecture ordre
    if ( ifs.fail() )
        std::cout << "pb de lecture ordre\n";

    for(int i=0; i<ordre; ++i)                  //lecture sommet
    {
        m_sommets.push_back( new Sommet{ifs} );     //création d'un nouveau sommet
    }

    double taille;
    ifs >> taille;                              //lecture de la taille(nombre d'arêtes)
    if ( ifs.fail() )
        std::cout<<"pb de lecture taille\n";

    double idT,idP;
    int num1,num2;
    std::string ligne;

    for (int i=0; i<taille; ++i)
    {
        ifs >> idT >> num1 >> num2;                          //lecture des ID des arêtes

        if(nomFichierpond != "vide")                        //si ouverture d'une ponderation
        {
            std::ifstream ifs2{nomFichierpond};              //lecture du fichier
            if (!ifs2)
                std::cout << "pb d'ouverture ou nom du fichier ponderation\n";

            double taille2;
            ifs2 >> taille2;                              //lecture de la taille(nombre d'arêtes)
            if ( ifs.fail() )
                std::cout << "pb de lecture taille\n";

            if(taille != taille2)
            {
                std::cout << "Probleme au niveau du nombre d'arete, pas la meme taille\n";
            }

            double poids;
            for (int i=0; i<taille; ++i)
            {
                ifs2 >> idP >> poids;                 //lecture des ID des arêtes et du poids de chacune

                if(idT == idP)                      //si mm arete
                {
                    m_aretes.push_back( new Arete(idT,num1,num2,poids) );                       //création d'une nouvelle arête

                    m_sommets[num1]->ajouterSucc(std::make_pair(m_sommets[num2],poids));        //ajout d'un successeur avec son poids

                    ///si le graphe n'est pas orienté
                    ///si num2 est successeur de num1, num1 est successeur de num2
                    if(!m_orient)
                        m_sommets[num2]->ajouterSucc(std::make_pair(m_sommets[num1],poids));
                }
            }
        }
        else
        {
            double poids = 0;

            m_aretes.push_back( new Arete(idT,num1,num2,poids) );

                    m_sommets[num1]->ajouterSucc(std::make_pair(m_sommets[num2],poids));        //ajout d'un successeur avec son poids

                    ///si le graphe n'est pas orienté
                    ///si num2 est successeur de num1, num1 est successeur de num2
                    if(!m_orient)
                        m_sommets[num2]->ajouterSucc(std::make_pair(m_sommets[num1],poids));
        }
    }
}

Graphe::~Graphe()       //destructeur
{
    for (auto s : m_sommets)
        delete s;
}

void Graphe::afficher() const                   //fonction d'affichage du fichier
{
    if(m_orient == 0)
        std::cout << std::endl << "Graphe non oriente\n";
    else
        std::cout << "Graphe oriente\n";


    std::cout << "Ordre : " << m_sommets.size() << std::endl;
    std::cout << "Liste sommets " << std::endl;

    for(const auto s : m_sommets)
    {
        s->afficherVoisins();                   //appelle du sspg pour afficher les successeurs à un sommet
        std::cout << std::endl;
    }
}

void Graphe::afficherGrapheSvg(Svgfile* svgout) const           //affichage du graphe en SVG
{
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->afficherSommetSvg(svgout);
    }

double x1,x2,y1,y2;

    for(size_t i=0; i<m_aretes.size(); ++i)
    {
                x1 = m_sommets[m_aretes[i]->get_Num1()]->getX();
                y1 = m_sommets[m_aretes[i]->get_Num1()]->getY();

                x2 = m_sommets[m_aretes[i]->get_Num2()]->getX();
                y2 = m_sommets[m_aretes[i]->get_Num2()]->getY();

        svgout->addLine(x1*100,y1*100,x2*100,y2*100,"black");
        svgout->addText( ((x1*100)+(x2*100))/2 , ((y1*100)+(y2*100))/2 , m_aretes[i]->get_Poids() , "purple");
    }
}
