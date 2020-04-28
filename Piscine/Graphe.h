#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED
#include <vector>
#include "Sommet.h"
#include "Arete.h"
#include "svgfile.h"


class Graphe
{
private:
    std::vector<Sommet*> m_sommets;
    std::vector<Arete*> m_aretes;
    int m_orient;

public:
    Graphe(std::string nomFichiertopo,std::string nomFichierpond);      //constructeur

    ~Graphe();                          //destructeur

    void afficher() const;
    void afficherGrapheSvg(Svgfile* svgout) const;
};


#endif // GRAPHE_H_INCLUDED
