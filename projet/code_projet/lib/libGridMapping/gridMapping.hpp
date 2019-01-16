/**
 * Author : Cédric KUASSIVI
 *
 * gridMapping.hpp :
 *
 * Header du fichier gridMapping.cpp
 *
**/

#pragma once

#ifndef GRID_MAPPING_HPP
#define GRID_MAPPING_HPP

#include "globalHeader.hpp"




class gridMapping
{
public:
    // width et height : dimensions de la grid
    int width;
    int height;

    // curseur : position actuelle du curseur sur la grid
    position curseur;

    // attribut qui contiendra les positions (ligne et colonne) de tous les obstacles de la scene 2D
    obstacle barrier;

    // node pour l'affichage 3D
    gridMesh *myGrid;

    // joueur
    player j1;

    // pour le curseur qui blink
    irr::ITimer *timerCursorBlink;

    // enemy
    std::vector<enemy> mechant;

    // variable inutile (juste pour le debug)
    // pour verifier que l ennemi n est pas dans la mm case qu 'un autre ennemi
    position pos_debug = position(alea(1, DEFAULT_HEIGHT - 1), alea(1, DEFAULT_WIDTH - 1));
    //position pos_debug = position(0, 0);

    public:

    // pour le curseur qui blink
    bool isBlinking;

    /** constructeurs **/
    gridMapping(position curseur, int width, int height, is::ISceneManager *smgr, ITimer *timerCursorBlink); // constructeur

    /** Pour le debug **/
    std::string printCurseur(bool display); // affichage curseur
    void printInfo(); // affichage des infos de la classe

    /** Autres **/
    void setCurseur(position curseur); //pour modifier la position du curseur
    void mouvement(gridAction act); // pour effectuer un mouvement / action (droite / gauche / haut / bas / etc...)
    bool isObstacle(position p, const obstacle &obs); // renvoie 1 s'il y a un obstacle a la position p
    bool isObstacle(position p, const obstacle &obs, bool val); // juste pour le debug

    // ajout d'obstacles
    void addObstacle(position p);
    void addObstacle(std::vector<position> vec);

    // suppression d'obstacles
    void removeObstacle(position p);

    // pour faire clignoter le curseur
    void curseurBlink(bool b)
    {
        if (b)
        {
            if(timerCursorBlink->getTime() >= DEFAULT_BLINK_DELAY) // periode de blink
            {
                // si le curseur etait en wireframe, il devient rempli et inversement
                myGrid->getGridNode(curseur)->setWireframe();

                // timer reinitialise
                timerCursorBlink->setTime(0);
            }
        }
        else if (!b)
            myGrid->getGridNode(curseur)->setWireframe(false);
    }


    void makeCurseurBlink(bool val)
    {
        // pour faire blinker le curseur, il faut que isBlinking = true
        // voir la fonction void mouvement(gridAction act); pour mieux comprendre
        isBlinking = val;
        curseurBlink(isBlinking);
    }


    // une animation 3d quelconque de la grille (pour prouver qu'on est bien en 3d)
    void animate(is::ISceneManager *smgr)
    {
        for (unsigned int k = 0; k < myGrid->mesh.size(); k++)
        {
            scene::ISceneNodeAnimator* anim =
                    smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));
            if(anim)
            {
                myGrid->getGridNode(k)->addAnimator(anim);
                anim->drop();
                anim = 0;
            }
        }
    }

    /** gestion de l'ennemi **/

    // ajout d'ennemis
    bool addEnemy(position p);
    // gestion de l'affichage de l'ennemi sur la grid
    bool setEnemyCursor(int i, position &p_i);
    // gestion de la suppression des donnee de l'ennemi mort
    void enemyDied(int i);
};



#endif
