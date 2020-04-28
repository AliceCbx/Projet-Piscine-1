#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED
#include <vector>
#include <fstream>
#include "svgfile.h"

class Sommet
{
private:
    double m_id,m_x,m_y;
    std::string m_nom;
    std::vector<std::pair<const Sommet*,double>> m_successeurs;           //vecteur de successeurs


public:

    Sommet(std::ifstream& is);

    ~Sommet();

    double getID() const
    {
        return m_id;
    }

    double getX() const
    {
        return m_x;
    }

    double getY() const
    {
        return m_y;
    }

    ///accesseur : pour la liste des successeurs
    const std::vector<std::pair<const Sommet*,double>>& getSuccesseurs() const;

    void ajouterSucc(std::pair<const Sommet*,double> voisin);   //fonction d'ajout de successeur

    void afficherVoisins() const;                               //fonction d'affichage de successeurs

    void afficherSommetSvg(Svgfile* svgout) const;              //fonction d'affichage SVG
};

#endif // SOMMET_H_INCLUDED
