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


// dimensions de l'ecran
#define DEFAULT_SCREEN_WIDTH 1080
#define DEFAULT_SCREEN_HEIGHT 720

// nombre de cases par defaut de la grid
#define DEFAULT_WIDTH 15 // en largeur donc nb de colonnes
#define DEFAULT_HEIGHT 10 // en hauteur donc nombre de lignes

// positions par defaut du curseur et donc du joueur
#define DEFAULT_LIGNE 0
#define DEFAULT_COLONNE 0


// HP par defaut du joueur
#define DEFAULT_PLAYER_HP 59

// HP par defaut des ennemis
#define DEFAULT_ENEMY_HP 10

// duree par defaut d'une animation de mouvement (UP, DOWN etc...)
#define DEFAULT_DUREE_ANIMATION 141

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

    void setPosition(const position &p)
    {
        ligne = p.ligne;
        colonne = p.colonne;
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




// scene 3D
class scene3D
{
public:

    irr::IrrlichtDevice *device;
    is::IMeshSceneNode *node;
    scene::ITriangleSelector *selector; // pour les collisions

    scene3D(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh_bsp)
        :device(device)
    {
        node = device->getSceneManager()->addOctreeSceneNode(mesh_bsp->getMesh(0), nullptr, -1, 1024);
        // Translation pour que nos personnages soient dans le décor
        node->setPosition(core::vector3df(0,-104,0));

        selector = device->getSceneManager()->createOctreeTriangleSelector(node->getMesh(), node);
        node->setTriangleSelector(selector);
    }


    void addCollisionToPerson(is::IAnimatedMeshSceneNode *perso)
    {
        const core::aabbox3d<f32>& box = perso->getBoundingBox();
        core::vector3df radius = box.MaxEdge - box.getCenter();
        scene::ISceneNodeAnimator *anim;
        anim = device->getSceneManager()->createCollisionResponseAnimator(selector,
                                                      perso,  // Le noeud que l'on veut gérer
                                                      radius, // "rayons" de la caméra
                                                      ic::vector3df(0, -10, 0),  // gravité
                                                      ic::vector3df(0, 0, 0));  // décalage du centre

        perso->addAnimator(anim);
    }

    void addCollisionToCamera(scene::ICameraSceneNode *camera, ic::vector3df radius)
    {
        scene::ISceneNodeAnimator *animcam;
        animcam = device->getSceneManager()->createCollisionResponseAnimator(selector,
                                                     camera,
                                                     radius,
                                                     ic::vector3df(0, 0, 0),  // gravité
                                                     ic::vector3df(0, 0, 0));  // décalage du centre

        camera->addAnimator(animcam);
    }
};



#endif


