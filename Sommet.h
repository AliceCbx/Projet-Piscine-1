#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED
#include <vector>
#include <fstream>
#include "svgfile.h"


class Sommet
{
private:
    double m_id,m_x,m_y,m_idC;
    std::string m_nom;
    std::vector<std::pair<const Sommet*,double>> m_successeurs;           //vecteur de successeurs
    std::vector<std::pair<double,double>> m_indices;

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

     double get_idC() const
    {
        return m_idC;
    }

    void set_idC(double idC)
    {
        m_idC=idC;
    }

    ///accesseur : pour la liste des successeurs
    const std::vector<std::pair<const Sommet*,double>>& getSuccesseurs() const;
    const std::vector<std::pair<double,double>>& getIndices() const;

    void ajouterSucc(std::pair<const Sommet*,double> voisin);   //fonction d'ajout de successeur
    void ajouterIndice(std::pair<double,double> indice);

    void afficherVoisins() const;                               //fonction d'affichage de successeurs

    void afficherSommetSvg(Svgfile* svgout) const;              //fonction d'affichage SVG


};

#endif // SOMMET_H_INCLUDED
