#include "Arete.h"
#include <fstream>
#include <iostream>

Arete::Arete(double& id, int& num1, int& num2, double& poids)       //constructeur
{
        m_id = id;
        m_num1 = num1;
        m_num2 = num2;
        m_poids = poids;
}

Arete::~Arete()         //destructeur
{}
