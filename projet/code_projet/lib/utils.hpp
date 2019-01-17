#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP



#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

#include <irrlicht/irrlicht.h>


using namespace std;
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;



// nombre de cases par defaut de la grid
#define DEFAULT_WIDTH 15 // en largeur donc nb de colonnes
#define DEFAULT_HEIGHT 10 // en hauteur donc nombre de lignes

// positions par defaut du curseur et donc du joueur
#define DEFAULT_LIGNE 0
#define DEFAULT_COLONNE 0


// HP par defaut du joueur
#define DEFAULT_PLAYER_HP 10

// HP par defaut des ennemis
#define DEFAULT_ENEMY_HP 10

// duree par defaut d'une animation de mouvement (UP, DOWN etc...)
#define DEFAULT_DUREE_ANIMATION 5

// actions du joueur ou de l'ennemi
enum Action {UP, DOWN, LEFT, RIGHT, ATTACK, VALIDATE, RESET, NOTHING, DEBUG};


/** positions des objets (obstacles, joueur, ennemis, etc ..) **/
class position
{
public:
    /** attributs **/
    int ligne; //ligne
    int colonne; //colonne

    /** constructeurs **/
    position(int ligne, int colonne){this->ligne = ligne; this->colonne = colonne;}
    position(const position &p){this->ligne = p.ligne; this->colonne = p.colonne;}
    position(){}

    /** pour le debug **/
    void printPosition()
    {
        std::cout << ".. Printing position infos" << std::endl;
        std::string s = "position : ligne = " + std::to_string(ligne) + "; colonne = " + std::to_string(colonne);
        std::cout << s << std::endl;
        std::cout << ".. End position infos" << std::endl;
    }

    /** comparaison de 2 positions **/
    bool operator==(const position &p)
    {
        if (ligne == p.ligne && colonne == p.colonne)
            return true;
        return false;
    }
};


class objet
{
public:
    position p;
    int HP;
    int id; // identifiant de l'objet

    objet(position p, int HP, int id)
        :p(p), HP(HP), id(id)
    {}

    bool isAlive(){return HP > 0;}

    void infos()
    {
        std::cout << std::endl;
        std::cout << "... Printing objet infos" << std::endl;
        std::string s = "HP  = " + std::to_string(HP) + "; isAlive() = " + std::to_string(isAlive());
        std::cout << "id : " << id << std::endl;
        std::cout << s << std::endl;
        p.printPosition();
        std::cout << "... End objet infos" << std::endl;
        std::cout << std::endl;
    }

};


class person : public objet
{
public:
    is::IAnimatedMeshSceneNode *node;

    // constructeur sans modele 3D
    person(position p, int HP, int id)
        :objet(p, HP, id)
    {}


    // constructeur avec model 3D
    person(position p, int HP, irr::scene::ISceneManager *smgr, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D, int id)
        :objet(p, HP, id)
    {
        node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setMaterialFlag(iv::EMF_LIGHTING, false);
        node->setMD2Animation(is::EMAT_STAND);
        node->setMaterialTexture(0, texture);
        node->setPosition(pos3D);
    }


    virtual std::string personAction(Action act)
    {
        switch (act)
        {
            case UP:
                return ("UP");
                break;
            case DOWN:
                return ("DOWN");
                break;
            case LEFT:
                return ("LEFT");
                break;
            case RIGHT:
                return ("RIGHT");
                break;
            case ATTACK:
                return ("ATTACK");
                break;
            default:
                return ("Nothing to do ...");
                break;
        }
        return ("Nothing to do ...");
    }

    /** pour le debug **/
    void infos()
    {
        std::cout << std::endl;
        std::cout << "... Printing person infos" << std::endl;
        this->objet::infos();
        std::cout << ".. Printing 3D position" << std::endl;
        std::cout << "3D positiion : x = " << node->getPosition().X << " , y = " << node->getPosition().Y
                  << " , z = " << node->getPosition().Z  << std::endl;
        std::cout << ".. End 3D position" << std::endl;

        std::cout << "... End person infos" << std::endl;
        std::cout << std::endl;
    }
};


class player : public person
{
public:
    // constructeur sans model 3D
    player(position p, int HP, int id)
        :person(p, HP, id)
    {}

    // constructeur avec model 3D
    player(position p, int HP, irr::scene::ISceneManager *smgr, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D, int id)
        :person(p, HP, smgr, mesh, texture, pos3D, id)
    {}

    virtual std::string personAction(Action act)
    {
        switch (act)
        {
            case UP:
                std::cout << "PLAYER UP" << std::endl;
                break;
            case DOWN:
                std::cout << "PLAYER DOWN" << std::endl;
                break;
            case LEFT:
                std::cout << "PLAYER LEFT" << std::endl;
                break;
            case RIGHT:
                std::cout << "PLAYER RIGHT" << std::endl;
                break;
            case ATTACK:
                std::cout << "PLAYER ATTACK" << std::endl;
                break;
            default:
                std::cout << "PLAYER Nothing to do" << std::endl;
                break;
        }
        return ("Done");
    }
};



class enemy : public person
{
public:
    // constructeur sans model 3D
    enemy(position p, int HP, int id)
        :person(p, HP, id)
    {}


    // constructeur avec model 3D
    enemy(position p, int HP, irr::scene::ISceneManager *smgr, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D, int id)
        :person(p, HP, smgr, mesh, texture, pos3D, id)
    {}

    virtual std::string personAction(Action act)
    {
        switch (act)
        {
            case UP:
                std::cout << "ENEMY UP" << std::endl;
                break;
            case DOWN:
                std::cout << "ENEMY DOWN" << std::endl;
                break;
            case LEFT:
                std::cout << "ENEMY LEFT" << std::endl;
                break;
            case RIGHT:
                std::cout << "ENEMY RIGHT" << std::endl;
                break;
            case ATTACK:
                std::cout << "ENEMY ATTACK" << std::endl;
                break;
            default:
                std::cout << "ENEMY Nothing to do" << std::endl;
                break;
        }
        return ("Done");
    }
};




#endif

















// Backup animation du mouvement des persos a mettre dans gameManager

/** void GameManager::animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso)
{

    isFin = 0;

    if (nombreCasesVerticales != 0 || nombreCasesHorizontales != 0)
    {
        if (nombreCasesVerticales != 0)
        {
            ic::vector3df depart = perso->getPosition();
            ic::vector3df arrivee = depart + ic::vector3df(nombreCasesVerticales * DEFAULT_GRID_NODE_SIZE, 0, 0);
            if (nombreCasesVerticales > 0)
                perso->setRotation(ic::vector3df(0, 0, 0));
            else
                perso->setRotation(ic::vector3df(0, 180, 0));
            is::ISceneNodeAnimator *anim1 = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                            arrivee, // position d'arrivee
                                                                            abs(nombreCasesVerticales) * DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                            false); // recommencer l'animation en boucle ?
            currentAnimationTime = device->getTimer()->getTime();
            perso->addAnimator(anim1);
            anim1->drop();
            perso->setMD2Animation(is::EMAT_RUN);

            isFin = 1;
        }

        if (nombreCasesHorizontales != 0)
        {
            ic::vector3df depart = perso->getPosition();
            ic::vector3df arrivee = depart + ic::vector3df(0, 0, - nombreCasesHorizontales * DEFAULT_GRID_NODE_SIZE);
            if (nombreCasesHorizontales > 0)
                perso->setRotation(ic::vector3df(0, 90, 0));
            else
                perso->setRotation(ic::vector3df(0, -90, 0));
            is::ISceneNodeAnimator *anim2 = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                            arrivee, // position d'arrivee
                                                                            abs(nombreCasesHorizontales) * DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                            false); // recommencer l'animation en boucle ?
            currentAnimationTime = device->getTimer()->getTime();
            perso->addAnimator(anim2);
            anim2->drop();
            perso->setMD2Animation(is::EMAT_RUN);

            isFin = 1;
        }
    }

    if (nombreCasesVerticales != 0 && nombreCasesHorizontales != 0)
    {
        bool isFin1 = 0;
        bool isFin2 = 1;
        isFin = 0;

        static int c1 = 0;
        static int c2 = 0;


        while(!isFin)
        {
            if (!isFin1 && isFin2)
            {
                ic::vector3df depart = perso->getPosition();
                ic::vector3df arrivee = depart + ic::vector3df(nombreCasesVerticales * DEFAULT_GRID_NODE_SIZE, 0, 0);

                is::ISceneNodeAnimator *anim1 = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                                arrivee, // position d'arrivee
                                                                                abs(nombreCasesVerticales) * DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                                false); // recommencer l'animation en boucle ?


                if (nombreCasesVerticales > 0)
                    perso->setRotation(ic::vector3df(0, 0, 0));
                else
                    perso->setRotation(ic::vector3df(0, 180, 0));



                currentAnimationTime = device->getTimer()->getTime();


                perso->setMD2Animation(is::EMAT_RUN);
                perso->addAnimator(anim1);

                c1 ++;
//                    std::cout << "perso->getPosition().X = " << perso->getPosition().X << ";   arrivee.X = " << arrivee.X << std::endl;


                if (irr::core::equals(perso->getPosition().X, arrivee.X) && irr::core::equals(perso->getPosition().Y, arrivee.Y) && irr::core::equals(perso->getPosition().Z, arrivee.Z))
                {
                    isFin1 = 1;
                    isFin2 = 0;
                    anim1->drop();

                }


            }



//                if (isFin1 && !isFin2)
//                {
//                    ic::vector3df depart = perso->getPosition();
//                    ic::vector3df arrivee = depart + ic::vector3df(0, 0, - nombreCasesHorizontales * DEFAULT_GRID_NODE_SIZE);

//                    is::ISceneNodeAnimator *anim2 = smgr->createFlyStraightAnimator(depart, // position de depart
//                                                                                    arrivee, // position d'arrivee
//                                                                                    abs(nombreCasesHorizontales) * DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
//                                                                                    false); // recommencer l'animation en boucle ?


//                    while (c2 < 30000)
//                    {
//                        c2 ++;

//                        if (nombreCasesHorizontales > 0)
//                            perso->setRotation(ic::vector3df(0, 90, 0));
//                        else
//                            perso->setRotation(ic::vector3df(0, -90, 0));


//                        currentAnimationTime = device->getTimer()->getTime();


//                        perso->setMD2Animation(is::EMAT_RUN);
//                        perso->addAnimator(anim2);
//                        anim2->drop();

//                        std::cout << c2 << std::endl;
//                    }
//                    isFin1 = 1;
//                    isFin2 = 1;
//                    perso->addAnimator(anim2);
//                }


            isFin = isFin1 && 1;
            std::cout << " isFin ? " << (int)isFin << " ; isFin1 ? " << (int)isFin1 << " ; isFin2 ? " << (int)isFin2 << std::endl;

        }
    }


}

**/
