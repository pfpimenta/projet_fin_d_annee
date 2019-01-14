/**
 * Author : Cédric KUASSIVI
 *
 * gridMapping.cpp :
 *
 * Il s'agit de la classe principale de gestion de l'affichage 3D de la grille dans laquelle
 * evoluera le combat.
 *
**/

#include "globalHeader.hpp"


/** constructeurs **/

// constructeur
gridMapping::gridMapping(position curseur, int width, int height, scene::ISceneManager *smgr, irr::ITimer *timerCursorBlink)
    :width(width), height(height), curseur(curseur), barrier(width, height), isBlinking(0)
{
    // on cree un nouveau maillage de width colonnes et height colonnes. sur les axes X et Z
    // la taille d'un carre elementaire sera : DEFAULT_GRID_NODE_SIZE
    myGrid = new gridMesh(width, height, smgr->getRootSceneNode(), smgr, DEFAULT_GRID_NODE_SIZE);

    // le curseur aura les proprietes inverses du reste de la grille :
    // si par exemple la grille est en wireframe, le curseur sera plein.
    myGrid->getGridNode(this->curseur)->setWireframe();

    //pour centrer la grille aux coordonnees (0, 0, 0)
    myGrid->centrageAuto(DEFAULT_GRID_NODE_SIZE);

    // le joueur est initialise a la position (DEFAULT_LIGNE, DEFAULT_COLONNE)
    j1 = player(DEFAULT_LIGNE, DEFAULT_COLONNE, myGrid);

    // initialisation du timer pour faire clignoter le curseur
    this->timerCursorBlink = timerCursorBlink;

    //animate(smgr); // pour rassurer les non-croyants qu'on travaille en 3D et non en 2D


}





/** Pour le debug **/
// affichage curseur
std::string gridMapping::printCurseur(bool display)
{
    return curseur.printPosition(display);
}

// affichage des infos de la classe
void gridMapping::printInfo()
{
    std::cout << std::endl;
    std::cout << "***** Debut Infos *****" << std::endl;
    curseur.printPosition(1);
    std::cout << "width : " << width << " | height : " << height << std::endl;
    //isObstacle(curseur, barrier, 1);

    std::cout << "dim barrier: " << barrier.pos.size() << std::endl;

    // affichage de toutes les positions des obstacles et ennemis presents.
//    std::cout << "barrier : ";
//    for (unsigned int k = 0; k < barrier.pos.size(); k++) std::cout<< " ( " << barrier.pos[k].ligne << " , " << barrier.pos[k].colonne << " ) | ";
//    std::cout << std::endl;

    std::cout << "***** Fin Infos *****" << std::endl;
    std::cout << std::endl;
}




/** Autres **/
// modifie la position du curseur de la grid
void gridMapping::setCurseur(position curseur)
{
    this->curseur = curseur;
}

// gere les mouvements du curseur et les actions sur la grid (VALIDATE par ex)
void gridMapping::mouvement(gridAction act)
{
    switch (act) {
    /**
     * Pour le mouvement du curseur, veut une animation :
     * quand le curseur est sur une case, cette case n'est pas en wireframe
     * tandis que le reste des cases est en wireframe (ou l'inverse).
     * Pour se faire, apres avoir actualise la position du curseur, on remet
     * son ancienne position en wireframe et son actuelle position en non-wireframe.
     *
     * Pour gerer le fait qu'on ne puisse pas aller dans une case deja occupee,
     * on verifie que la position du curseur n'est pas parmi les obstacles (variable barrier).
     * Si un obstacle est deja a cette position, on laisse le curseur a sa position actuelle.
     **/
    case DROITE:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        myGrid->getGridNode(curseur)->setWireframe();
        curseur.colonne ++;
        if (isObstacle(curseur, barrier)){ curseur.colonne --;}
        myGrid->getGridNode(curseur)->setWireframe();
        break;
    case GAUCHE:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        myGrid->getGridNode(curseur)->setWireframe();
        curseur.colonne --;
        if (isObstacle(curseur, barrier)){  curseur.colonne ++;}
        myGrid->getGridNode(curseur)->setWireframe();
        break;
    case HAUT:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        myGrid->getGridNode(curseur)->setWireframe();
        curseur.ligne --;
        if (isObstacle(curseur, barrier)){  curseur.ligne ++;}
        myGrid->getGridNode(curseur)->setWireframe();
        break;
    case BAS:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        myGrid->getGridNode(curseur)->setWireframe();
        curseur.ligne ++;
        if (isObstacle(curseur, barrier)){  curseur.ligne --;}
        myGrid->getGridNode(curseur)->setWireframe();
        break;


    /**
     * cette action permet de valider la position du curseur et y placer le joueur.
     * pour se faire, la case dans laquelle se trouvait le joueur est consideree comme inutilisee (UNSELECTED)
     * De plus, la position du joueur est actualisee avec la position du curseur
     * Et a cette position, la case correspondante est coloriee avec la bonne couleur
     **/
    case VALIDATE:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        myGrid->getGridNode(j1.pos)->setCouleur(UNSELECTED);
        j1.setPosition(curseur);
        myGrid->getGridNode(j1.pos)->setCouleur(PLAYER);
        mouvement(RESET);
        break;


    /**
     * On veut que le curseur revienne la ou se trouve le joueur.
     * Pour ne pas se compliquer la tache, on set la position du curseur sur celle du joueur
     * Ensuite, on met toutes les cases a wireframe et on met le curseur a non-wireframe
     **/
    case RESET:
        if (isBlinking)
            curseurBlink(false); // on ne veut pas que le curseur blink a ce moment
        curseur.ligne = j1.pos.ligne;
        curseur.colonne = j1.pos.colonne;
        myGrid->resetWireframe(true); // utiliser une DEFINE pour facilement inverser l'affichage
        myGrid->getGridNode(curseur)->setWireframe();
        break;


    /**
     * On ne fait rien a ce moment
     **/
    case NOTHING:
        if (isBlinking)
            curseurBlink(true); // on veut que le curseur blink a ce moment
        break;


    case DEBUG:
        //enemyDied(0);
        pos_debug = position(alea(1, DEFAULT_HEIGHT - 1), alea(1, DEFAULT_WIDTH - 1));
        setEnemyCursor(0, pos_debug);
        std::cout << pos_debug.ligne << " " << pos_debug.colonne << std::endl;
        break;


    default:
        if (isBlinking)
            curseurBlink(true); // on veut que le curseur blink a ce moment
        break;
    }
}


// pour verifier si le curseur est place sur un obstacle
bool gridMapping::isObstacle(position p, const obstacle &obs)
{
    for (unsigned int i = 0; i < obs.pos.size(); i++)
    {
        if (p == obs.pos[i])
        {
            std::cout << "Obstacle détecté !" << std::endl;
            return true;
        }
    }
    std::cout << "Aucun obstacle détecté" << std::endl;
    return false;
}

// inutile : juste pour le debug
bool gridMapping::isObstacle(position p, const obstacle &obs, bool val)
{
    if (val)
    {
        if (isObstacle(p, obs))
            std::cout << "Obstacle détecté !" << std::endl;
        else
            std::cout << "Aucun obstacle détecté" << std::endl;
    }
    return isObstacle(p, obs);
}



// ajout d'obstacles
void gridMapping::addObstacle(position p)
{
    barrier.pos.push_back(p);
    myGrid->getGridNode(p)->setCouleur(OBSTACLE);
}

void gridMapping::addObstacle(std::vector<position> vec)
{
    for (unsigned k = 0; k < vec.size(); k++)
        addObstacle(vec[k]);
}

// suppression obstacle
void gridMapping::removeObstacle(position p)
{
    std::vector<position>::iterator it;
    it = find (barrier.pos.begin(), barrier.pos.end(), p);
    barrier.pos.erase(it);
    myGrid->getGridNode(p)->setCouleur(UNSELECTED);
}



/** gestion des ennemis **/

// ajout d'ennemis
bool gridMapping::addEnemy(position p)
{
    // on verifie que la position ou on veut ajouter l'ennemi n est pas occupee
    if (!isObstacle(p, barrier) && !(p == j1.pos))
    {
        // on ajoute les ennemis en tant qu'obstacles (dans la variable barrier avec la fct addObstacle())
        // pour ne pas y placer d'autres ennemis, d'autres obstacles, ou le joueur
        addObstacle(p);
        mechant.push_back(enemy(p.ligne, p.colonne, myGrid));

        std::cout << "Ennemi ajoute avec succes" << std::endl;

        return true;
    }
    else
        std::cout << " Impossible d'ajouter cet ennemi. la position : ("
                  << p.ligne << ", " << p.colonne << ") est deja occupee " << std::endl;

    return false;
}

// gestion de l'affichage / deplacement de l'ennemi sur la grid
bool gridMapping::setEnemyCursor(int i /* i = numero de l'ennemi*/, position &p_i)
{

    /**
     * Etape 1 : on verifie que l'ennemi[i] existe
     * Etape 2 : on verifie s'il est vivant
     * Etape 3 : on verifie que la position ou il veut aller n est pas occupee
     *
     * S il n y a pas de probleme, on retire son ancienne position des obstacles
     * Ensuite on set sa nouvelle position qu'on considere alors comme nouvel obstacle
     * Et on met la couleur qui va avec !
     *
     * Sinon, l'ennemi ne bouge pas ! (d'ou le passage par reference "position &p_i")
     **/
    if ((unsigned int )i < mechant.size())
    {
        if(mechant[i].isAlive)
        {
            if (!isObstacle(p_i, barrier) && !(p_i == j1.pos))
            {
                removeObstacle(mechant[i].pos); // on retire l'ancienne position de l'ennemi
                mechant[i].setPosition(p_i);
                addObstacle(p_i); // maj de la nouvelle position de l'ennemi
                myGrid->getGridNode(mechant[i].pos)->setCouleur(ENNEMY);

                std::cout << "position de l'ennemi modifiee avec succes" << std::endl;

                return true;
            }
            else
            {
                p_i.ligne = mechant[i].pos.ligne;
                p_i.ligne = mechant[i].pos.ligne;
            }
        }
    }

    std::cout << "echec de modification de la position" << std::endl;

    return false;
}


// gestion de la suppression des donnee de l'ennemi mort
void gridMapping::enemyDied(int i)
{
    /**
     * Pour tuer l'ennemi i, il faut deja qu'il existe !
     * Ensuite on dit qu'il est mort (isAlive = 0)
     * On ne le considere donc plus comme un obstacle
     * et on le vire de la liste des ennemis
     **/
    if ((unsigned int )i < mechant.size())
    {
        mechant[i].isAlive = 0;
        removeObstacle(mechant[i].pos);

        mechant.erase(mechant.begin() + i);
    }

}
