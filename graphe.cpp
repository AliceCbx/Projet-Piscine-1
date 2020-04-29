#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <map>

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
        m_sommets.push_back( new Sommet{ifs} );     //cr�ation d'un nouveau sommet
    }

    double taille;
    ifs >> taille;                              //lecture de la taille(nombre d'ar�tes)
    if ( ifs.fail() )
        std::cout<<"pb de lecture taille\n";

    double idT,idP;
    int num1,num2;
    std::string ligne;

    for (int i=0; i<taille; ++i)
    {
        ifs >> idT >> num1 >> num2;                          //lecture des ID des ar�tes

        if(nomFichierpond != "vide")                        //si ouverture d'une ponderation
        {
            std::ifstream ifs2{nomFichierpond};              //lecture du fichier
            if (!ifs2)
                std::cout << "pb d'ouverture ou nom du fichier ponderation\n";

            double taille2;
            ifs2 >> taille2;                              //lecture de la taille(nombre d'ar�tes)
            if ( ifs.fail() )
                std::cout << "pb de lecture taille\n";

            if(taille != taille2)
            {
                std::cout << "Probleme au niveau du nombre d'arete, pas la meme taille\n";
            }

            double poids;
            for (int i=0; i<taille; ++i)
            {
                ifs2 >> idP >> poids;                 //lecture des ID des ar�tes et du poids de chacune

                if(idT == idP)                      //si mm arete
                {
                    m_aretes.push_back( new Arete(idT,num1,num2,poids) );                       //cr�ation d'une nouvelle ar�te

                    m_sommets[num1]->ajouterSucc(std::make_pair(m_sommets[num2],poids));        //ajout d'un successeur avec son poids

                    ///si le graphe n'est pas orient�
                    ///si num2 est successeur de num1, num1 est successeur de num2
                    if(!m_orient)
                        m_sommets[num2]->ajouterSucc(std::make_pair(m_sommets[num1],poids));
                }
            }
        }
        else
        {
            double poids = 1;

            m_aretes.push_back( new Arete(idT,num1,num2,poids) );

            m_sommets[num1]->ajouterSucc(std::make_pair(m_sommets[num2],poids));        //ajout d'un successeur avec son poids

            ///si le graphe n'est pas orient�
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
        s->afficherVoisins();                   //appelle du sspg pour afficher les successeurs � un sommet
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
        svgout->addText( ((x1*100)+(x2*100))/2, ((y1*100)+(y2*100))/2, m_aretes[i]->get_Poids(), "purple");
    }
}

/*
sous-programme qui affiche une arborescence
params : sommet initial (racine), vecteur de pr�d�cesseur
*/
double Graphe::calculDistance(double num1, double num2, const std::vector<int>& arbre)
{
    int somme=0;

    if(arbre[num2]!=-1)
    {
        size_t j=arbre[num2];

        while(j!=num1)
        {
            j=arbre[j];
        }

        size_t a=num2;

        while(a!=num1)
        {
            for(auto succ: m_sommets[arbre[a]]->getSuccesseurs())
            {
                if(succ.first->getID()==a)
                {
                    somme = somme + succ.second;
                }
            }
            a=arbre[a];
        }
    }
    return somme;
}

std::vector<int> Graphe::rechercheDijkstra(double num_F)   //algorithme de DIJKSTRA
{

    auto cmp = [](std::pair<const Sommet*,double> p1, std::pair<const Sommet*,double> p2)
    {
        return p2.second<p1.second;
    };

    /// d�claration de la file de priorit�
    std::priority_queue<std::pair<const Sommet*,double>,std::vector<std::pair<const Sommet*,double>>,decltype(cmp)> file(cmp);

    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);

    ///pour noter les pr�d�cesseurs : on note les num�ros des pr�d�cesseurs (on pourrait stocker des pointeurs sur ...)
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

        ///pour chaque successeur du sommet d�fil�
        while((!file.empty())&&(couleurs[p.first->getID()] ==1))
        {
            p=file.top();
            file.pop();
        }
        couleurs[p.first->getID()]=1;          //on marque le sommet

        for(auto succ : p.first->getSuccesseurs())          //pour chaque successeur
        {
            if(couleurs[succ.first->getID()] == 0) ///si non marqu�
            {
                if( (poids[p.first->getID()] + succ.second < poids[succ.first->getID()]) || (poids[succ.first->getID()] == -1) ) ///si distance inf�rieur
                {
                    poids[succ.first->getID()] = poids[p.first->getID()] + succ.second;       //on met � jour les distances
                    preds[succ.first->getID()] = p.first->getID();                            //on note le pr�decesseur
                    file.push({succ.first,poids[succ.first->getID()]});                        //on ajoute la pair dans la file
                }
            }
        }
    }
    return preds;
}

/*
parcours BFS
entr�e = le num�ro du sommet initial (on pourrait passer un pointeur sur ...)
retour : le tableau de pr�decesseurs (pour retrouver les chemins)
*/
std::vector<int> Graphe::BFS(int num_s0)const
{
    /// d�claration de la file
    std::queue<const Sommet*> file;

    /// pour le marquage
    std::vector<int> couleurs((int)m_sommets.size(),0);

    ///pour noter les pr�d�cesseurs : on note les num�ros des pr�d�cesseurs (on pourrait stocker des pointeurs sur ...)
    std::vector<int> preds((int)m_sommets.size(),-1);

    ///�tape initiale : on enfile et on marque le sommet initial
    file.push(m_sommets[num_s0]);
    couleurs[num_s0]=1;

    const Sommet* s;

    ///tant que la file n'est pas vide
    while(!file.empty())
    {
        ///on d�file le prochain sommet
        s = file.front();
        file.pop();

        ///pour chaque successeur du sommet d�fil�
        for(auto succ:s->getSuccesseurs())
        {
            if( couleurs[succ.first->getID()] == 0 )         ///s'il n'est pas marqu�
            {
                couleurs[succ.first->getID()] = 1;           ///on le marque
                preds[succ.first->getID()] = s->getID();    ///on note son pr�decesseur (=le sommet d�fil�)
                file.push(succ.first);                        ///on le met dans la file
            }
            couleurs[s->getID()]= 5;                   ///sommet devient noir
        }
    }
    return preds;
}


void Graphe::sauvegarde()     const      //sauvegarde les indices dans un fichier texte
{
    std::ofstream ofs{"sauvegarde.txt"};
    if (!ofs)
        std::cout<<"pb d'ouverture ou nom du fichier\n";

    for(auto s : m_sommets)
    {
        ofs << "Sommet : " << s->getID() ;

        for(const auto p : s->getIndices() )
        {
            ofs << " IndiceNN : " << p.first << " IndiceN : " << p.second << std::endl;
        }
    }
}

void Graphe::proximite(std::string choix2, Graphe g)
{
    double id1,id2;
    double sommedist=0;
    double indiceN,indiceNN;

    //double boucle pour avoir la somme des longueurs des pcc passant de id1 � tous les autres sommets
    if (choix2 == "OUI_P")
    {
        std::cout << std::endl << "PCC avec Dijkstra : "<< std::endl;

        for(id1=0; id1<m_sommets.size() ; ++id1)
        {
            sommedist=0;
            std::vector<int> arbre = g.rechercheDijkstra(id1);      //recherche pcc � partir du sommet id1

            for(id2=0; id2<m_sommets.size(); ++id2)                 //pour tous les autres sommets
            {
                if(id1 != id2)
                {
                    sommedist = sommedist + g.calculDistance(id1,id2,arbre);        //calcul de la longueur entre le sommet id1 et id2
                }
            }

            indiceN = (m_sommets.size() - 1)/sommedist;             //indice normalis�
            indiceNN = 1/sommedist;                                 //indice non normalis�
            m_sommets[id1]->ajouterIndice(std::make_pair(indiceNN,indiceN));

        }
    }
    else
    {
        std::cout << std::endl << "PCC avec BFS : "<< std::endl;

        for(id1=0; id1<m_sommets.size() ; ++id1)
        {
            sommedist=0;
            std::vector<int> arbre = g.BFS(id1);      //recherche pcc � partir du sommet id1

            for(id2=0; id2<m_sommets.size(); ++id2)                 //pour tous les autres sommets
            {
                if(id1 != id2)
                {
                    sommedist = sommedist + g.calculDistance(id1,id2,arbre);        //calcul de la longueur entre le sommet id1 et id2
                }
            }

            indiceN = (m_sommets.size() - 1)/sommedist;             //indice normalis�
            indiceNN = 1/sommedist;                                 //indice non normalis�

            m_sommets[id1]->ajouterIndice(std::make_pair(indiceNN,indiceN));
        }
    }
}

void Graphe::calculCentraliteDegre()            //calcul de la centralit� de degr�
{
    double test=0;
    double idC;

    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->set_idC( idC = 0 );
        m_sommets[i]->set_idC( idC = m_sommets[i]->getSuccesseurs().size() );           //chaque indice correspond au nombre de successeurs

        std::cout << "Indice (non normalise) sommet " << m_sommets[i]->getID() << ": " << m_sommets[i]->get_idC()  << std::endl;
        std::cout << "Indice (normalise) sommet " << m_sommets[i]->getID() << ": " << m_sommets[i]->get_idC()  / (m_sommets.size() - 1) << std::endl << std::endl;

        m_sommets[i]->ajouterIndice(std::make_pair(m_sommets[i]->get_idC(),(m_sommets[i]->get_idC()/(m_sommets.size() - 1))));

        if( test < m_sommets[i]->get_idC() )            //on teste pour savoir quel indice est le plus �lev�
        {
            test = m_sommets[i]->get_idC() ;
        }
    }
    std::cout << "indice centralite degre maximal  : " << test << std::endl;            //affichage de l'indice le plus �lev�
}

void Graphe::calculCentraliteVP()
{
    double idC,indiceNN,indiceN;
    std::vector<double> indiceV;
    double lambda=0, sommeInd=0;
    int test = 0,compt=0;


    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        indiceV.push_back(m_sommets[i]->get_idC());
    }

    do
    {
        for(auto it : m_sommets)                //pour chaque sommet
        {
            for(auto iit : it->getSuccesseurs())         //pour chaque successeur de chaque sommet
            {
                indiceV[compt] = indiceV[compt] +  iit.first->get_idC() ;             //on calcule l'indice des voisins

            }
            ++compt;
        }

        for(size_t i=0; i<indiceV.size(); ++i)
        {
            sommeInd = sommeInd + (indiceV[i] * indiceV[i]);                    //on calcule lambda
        }
        lambda = sqrt(sommeInd);

        for(size_t k=0; k<m_sommets.size(); ++k)                //pour chaque sommet on recalcule l'indice
        {
            indiceNN = (indiceV[k] / lambda); //indice non normalis�
            m_sommets[k]->set_idC( idC = indiceNN);

            indiceN = indiceNN *1;             //indice normalis�

            m_sommets[k]->ajouterIndice(std::make_pair(indiceNN,indiceN));
        }

        test = abs(1 - lambda);
    }
    while (test > lambda);           //on fait �a tant que lambda ne varie pas trop
}

//centralite intermediaire

void Graphe::intermediaire(Graphe g)
{
    int sigst=0; //nombre de PCC entre point s et t
    int sigv=0;  //nombre de PCC entre point s et t passant par v
    std::map< std::vector<int> , double > pcc;
    std::vector<int> recup; //recuperer predecesseur
    int idI;

    ///Mettre tous les indices � 0
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->set_idI(idI=0);
    }

    std::cout << std::endl << "PCC avec BFS ";

    for(size_t i=0; i<m_sommets.size() ; ++i)
    {
        ///appel de la m�thode BFS et r�cup�ration du r�sultat
        recup=g.BFS(i);
        pcc.insert({recup, i});
    }
}
