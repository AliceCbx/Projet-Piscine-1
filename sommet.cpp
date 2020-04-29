#include "Sommet.h"
#include <vector>
#include <iostream>
#include <fstream>

Sommet::Sommet(std::ifstream& is)           //constructeur
{
    is >> m_id >> m_nom >> m_x >> m_y;
    if ( is.fail() )
        std::cout << "Probleme lecture sommet\n";
}

Sommet::~Sommet()               //destructeur
{}

///accesseur : pour la liste des successeurs
const std::vector<std::pair<const Sommet*,double>>& Sommet::getSuccesseurs() const
{
    return m_successeurs;
}

const std::vector<std::pair<double,double>>& Sommet::getIndices() const
{
    return m_indices;
}


void Sommet::ajouterSucc(std::pair<const Sommet*,double> voisin)        //on ajoute un successeur
{
    m_successeurs.push_back(voisin);
}

void Sommet::ajouterIndice(std::pair<double,double> indice)        //on ajoute un successeur
{
    m_indices.push_back(indice);
}

void Sommet::afficherVoisins() const                                    //on affiche les voisins
{
    std::cout << "sommet " << m_id << " : " ;
    for(const auto s : m_successeurs)
        std::cout << " " << s.first->getID();
}

void Sommet::afficherSommetSvg(Svgfile* svgout) const           //fonction d'affichage d'un sommet en SVG
{
    svgout->addDisk(m_x*100,m_y*100,5,"green");

    svgout->addText((m_x*100)-5,(m_y*100)-10,m_nom,"red");
}
