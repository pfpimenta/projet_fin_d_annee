#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP


#include "utils.hpp"
#include "libGridMapping/gridMapping.hpp"

class GameManager
{

    //sceneManager (/!\ different du gameManager)
    irr::scene::ISceneManager *smgr;
    //device
    irr::IrrlichtDevice *device;

    // joueur (/!\ il y a toujours un seul joueur; meilleure gestion avec un vecteur)
    std::vector<player*> j1;
    std::vector<Action> playerAnimToDo;

    // ennemis
    std::vector<enemy*> mechant;
    std::vector<int> enemyID; // pour bien gerer les ennemis ajoutes


    // gridMapping (/!\ il y a toujours un seul gridMapping; meilleure gestion avec un vecteur)
    std::vector<gridMapping*> grid;


public:
    // constructeur
    GameManager(IrrlichtDevice *device);
    GameManager():isCombat(1), isPromenade(0){}


    /** Animations **/
    int currentAnimationTime;


//    void doThis(is::IAnimatedMeshSceneNode *perso)
//    {
//        std::vector<Action> toDo = {RIGHT, RIGHT, LEFT, DOWN, LEFT};
//        animator(toDo[0], perso);
//        for (unsigned int i = 1; i < toDo.size(); i++)
//        {
//            if (device->getTimer()->getTime() - this->currentAnimationTime >= DEFAULT_DUREE_ANIMATION)
//                animator(toDo[i], perso);
//        }
//    }


    void animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso)
    {

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
            }
        }

        if (nombreCasesVerticales != 0 && nombreCasesHorizontales != 0)
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



            depart = perso->getPosition();
            arrivee = depart + ic::vector3df(0, 0, - nombreCasesHorizontales * DEFAULT_GRID_NODE_SIZE);
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

        }
    }



    void animUP(is::IAnimatedMeshSceneNode *perso)
    {
        ic::vector3df depart = perso->getPosition();
        ic::vector3df arrivee = depart + ic::vector3df(DEFAULT_GRID_NODE_SIZE, 0, 0);
        perso->setRotation(ic::vector3df(0, 0, 0));
        is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                       arrivee, // position d'arrivee
                                                                       DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                       false); // recommencer l'animation en boucle ?
        currentAnimationTime = device->getTimer()->getTime();
        perso->addAnimator(anim);
        anim->drop();
        perso->setMD2Animation(is::EMAT_RUN);

    }

    void animDOWN(is::IAnimatedMeshSceneNode *perso)
    {
        ic::vector3df depart = perso->getPosition();
        ic::vector3df arrivee = depart + ic::vector3df(-DEFAULT_GRID_NODE_SIZE, 0, 0);
        perso->setRotation(ic::vector3df(0, 180, 0));
        is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                       arrivee, // position d'arrivee
                                                                       DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                       false); // recommencer l'animation en boucle ?
        currentAnimationTime = device->getTimer()->getTime();
        perso->addAnimator(anim);
        anim->drop();
        perso->setMD2Animation(is::EMAT_RUN);
    }

    void animLEFT(is::IAnimatedMeshSceneNode *perso)
    {
        ic::vector3df depart = perso->getPosition();
        ic::vector3df arrivee = depart + ic::vector3df(0, 0, DEFAULT_GRID_NODE_SIZE);
        perso->setRotation(ic::vector3df(0, -90, 0));
        is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                       arrivee, // position d'arrivee
                                                                       DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                       false); // recommencer l'animation en boucle ?
        currentAnimationTime = device->getTimer()->getTime();
        perso->addAnimator(anim);
        anim->drop();
        perso->setMD2Animation(is::EMAT_RUN);
    }

    void animRIGHT(is::IAnimatedMeshSceneNode *perso)
    {
        ic::vector3df depart = perso->getPosition();
        ic::vector3df arrivee = depart + ic::vector3df(0, 0, -DEFAULT_GRID_NODE_SIZE);
        perso->setRotation(ic::vector3df(0, 90, 0));
        is::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(depart, // position de depart
                                                                       arrivee, // position d'arrivee
                                                                       DEFAULT_DUREE_ANIMATION, // duree de l'animation en ms
                                                                       false); // recommencer l'animation en boucle ?
        currentAnimationTime = device->getTimer()->getTime();
        perso->addAnimator(anim);
        anim->drop();
        perso->setMD2Animation(is::EMAT_RUN);
    }









    /** joueur **/
    bool addPlayer(position p, int HP, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D);
    bool removePlayer();
    bool isPlayerAlive();
    player *getPlayer();
    void playAnimation(Action act, is::IAnimatedMeshSceneNode *perso);

    /** ennemis **/
    bool addEnemy(position p, int HP, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D);
    bool removeEnemy(int id);
    bool isEnemyAlive(int id);
    enemy *getEnemy(int id);


    /** gridMapping **/
    bool addGridMapping(int width, int height, ITimer *Timer);
    bool removeGridMapping();
    gridMapping *getGridMapping();


    /** gestionnaire de jeu **/
    // pour savoir si on commence un combat
    bool isCombat; // : 1 => le combat commence dans la scene 2D , appelle de la fonction gestion combat()
    bool isPromenade; // : 1 => le joueur se promene dans le monde 3D, appelle de la fonction gestion promenade()
    void combat(irr::ITimer *Timer);
    void promenade(irr::ITimer *Timer);
    void sceneRenderer(ITimer *Timer);
};

#endif
