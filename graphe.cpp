#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

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

std::vector<int> Graphe::rechercheDijkstra(double num_F)   //algorithme de DIJKSTRA
{

    auto cmp = [](std::pair<const Sommet*,double> p1, std::pair<const Sommet*,double> p2)
    {
        return p2.second<p1.second;
    };

    /// déclaration de la file de priorité
    std::priority_queue<std::pair<const Sommet*,double>,std::vector<std::pair<const Sommet*,double>>,decltype(cmp)> file(cmp);

    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);

    ///pour noter les prédécesseurs : on note les numéros des prédécesseurs (on pourrait stocker des pointeurs sur ...)
    std::vector<int> preds((int)m_sommets.size(),-1);

    ///pour la distance
    std::vector<int> poids((int)m_sommets.size(),-1);


    ///initialisation
    poids[num_F]=0;
    file.push({m_sommets[num_F],0});

    std::pair<const Sommet*,double> p;

    while(!file.empty())
    {
        ///on marque le sommet s avec le plus petit poids
        p = file.top();
        file.pop();

        ///pour chaque successeur du sommet défilé
        while((!file.empty())&&(couleurs[p.first->getID()] ==1))
        {
            p=file.top();
            file.pop();
        }
        couleurs[p.first->getID()]=1;          //on marque le sommet

        for(auto succ : p.first->getSuccesseurs())          //pour chaque successeur
        {
            if(couleurs[succ.first->getID()] == 0) ///si non marqué
            {
                if( (poids[p.first->getID()] + succ.second < poids[succ.first->getID()]) || (poids[succ.first->getID()] == -1) ) ///si distance inférieur
                {
                    poids[succ.first->getID()] = poids[p.first->getID()] + succ.second;       //on met à jour les distances
                    preds[succ.first->getID()] = p.first->getID();                            //on note le prédecesseur
                    file.push({succ.first,poids[succ.first->getID()]});                        //on ajoute la pair dans la file
                }
            }
        }
    }
    return preds;
}


/*
sous-programme qui affiche une arborescence
params : sommet initial (racine), vecteur de prédécesseur
*/
void Graphe::afficher_parcours(double num1, double num2, const std::vector<int>& arbre)
{

    if(arbre[num2]!=-1)
    {
        std::cout<<num2<<" <-- ";
        size_t j=arbre[num2];

        while(j!=num1)
        {
            std::cout<<j<<" <-- ";
            j=arbre[j];
        }
        std::cout<<j<<std::endl;

        size_t a=num2;
        int somme=0;

        while(a!=num1)
        {
            for(auto succ: m_sommets[arbre[a]]->getSuccesseurs())
            {
                if(succ.first->getID()==a)
                {
                    std::cout << succ.second << " + ";
                    somme = somme + succ.second;
                }
            }
            a=arbre[a];
        }
        std::cout << "somme : " << somme;
    }
}

/*
parcours BFS
entrée = le numéro du sommet initial (on pourrait passer un pointeur sur ...)
retour : le tableau de prédecesseurs (pour retrouver les chemins)
*/
std::vector<int> Graphe::BFS(int num_s0)const
{
    /// déclaration de la file
    std::queue<const Sommet*> file;

    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);

    ///pour noter les prédécesseurs : on note les numéros des prédécesseurs (on pourrait stocker des pointeurs sur ...)
    std::vector<int> preds((int)m_sommets.size(),-1);

    ///étape initiale : on enfile et on marque le sommet initial
    file.push(m_sommets[num_s0]);
    couleurs[num_s0]=1;

    const Sommet* s;

    ///tant que la file n'est pas vide
    while(!file.empty())
    {
        ///on défile le prochain sommet
        s = file.front();
        file.pop();

        ///pour chaque successeur du sommet défilé
        for(auto succ:s->getSuccesseurs())
        {
            if( couleurs[succ.first->getID()] == 0 )         ///s'il n'est pas marqué
            {
                couleurs[succ.first->getID()] = 1;           ///on le marque
                preds[succ.first->getID()] = s->getID();    ///on note son prédecesseur (=le sommet défilé)
                file.push(succ.first);                        ///on le met dans la file
            }
            couleurs[s->getID()]= 5;                   ///sommet devient noir
        }
    }
    return preds;
}
