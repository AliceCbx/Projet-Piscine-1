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
    //std::vector<int> m_nbchemin;
    //int m_nb;

public:
    Graphe(std::string nomFichiertopo,std::string nomFichierpond);      //constructeur

    ~Graphe();                          //destructeur

    void afficher() const;
    void afficherGrapheSvg(Svgfile* svgout) const;
    int afficher_parcours1(size_t num,const std::vector<int>& arbre);

    //proximité
    std::vector <int> rechercheDijkstra (double num_F);
    void proximite(std::string choix2,Graphe g);
    int afficher_parcours(double num1, double num2, const std::vector<int>& arbre);

    //Bfs
    std::vector<int> BFS(int num_s0)const;


    //sauvegarde
    void sauvegarde()     const;

    //degre
    void calculCentraliteDegre();
    //VP
    void calculCentraliteVP();




};


#endif // GRAPHE_H_INCLUDED
