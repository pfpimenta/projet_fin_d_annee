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


    /** joueur **/
    bool addPlayer(position p, int HP, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D);
    bool removePlayer();
    bool isPlayerAlive();
    player *getPlayer();


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


    /** Animations **/
    bool isFin = 1;
    int currentAnimationTime;

    void playAnimation(bool voieLibre, Action act, is::IAnimatedMeshSceneNode *perso);
    void animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso);
    void animATTACK(is::IAnimatedMeshSceneNode *perso);
    void animUP(is::IAnimatedMeshSceneNode *perso);
    void animDOWN(is::IAnimatedMeshSceneNode *perso);
    void animLEFT(is::IAnimatedMeshSceneNode *perso);
    void animRIGHT(is::IAnimatedMeshSceneNode *perso);

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







};

#endif
