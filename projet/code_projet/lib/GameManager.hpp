#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP


#include "utils.hpp"
#include "enemy.hpp"
#include "State.hpp"
#include "libGridMapping/gridMapping.hpp"

class GameManager
{
    //sceneManager (/!\ different du gameManager)
    irr::scene::ISceneManager *smgr;

    // joueur (/!\ il y a toujours un seul joueur; meilleure gestion avec un vecteur)
    std::vector<player*> j1;
    core::vector3df tempPlayer3DPosition;

    // ennemis
    std::vector<enemy*> mechant;
    std::vector<int> enemyID; // pour bien gerer les ennemis ajoutes

    // gridMapping (/!\ il y a toujours un seul gridMapping; meilleure gestion avec un vecteur)
    std::vector<gridMapping*> grid;

    // map 3D  (/!\ il y a toujours une seule map 3D (pour l'instant celle de Mario); meilleure gestion avec un vecteur)
    std::vector<scene3D*> mapScene3D;

    // cameras
    std::vector<scene::ICameraSceneNode*> cameraCombat;
    std::vector<scene::ICameraSceneNode*> cameraJeuLibre;

    // Q table pour les ennemis
    Q_table* qTable;

    // pour la hitbox
    float epsilon = 50;




public:
    //device : correspond exactement au device dans le main
    irr::IrrlichtDevice *device;

    irr::scene::IAnimatedMesh *mesh; //mesh 3D des persos
    irr::video::ITexture* textureEnemy;

    // constructeur
    GameManager(IrrlichtDevice *device);
    GameManager():isCombat(0), isPromenade(1){} // penser a initialiser de la meme facon isCombat & isPromenade dans les 2 constructeurs


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


    /** cameras **/
    // combat
    bool addCameraCombat();
    bool removeCameraCombat();
    scene::ICameraSceneNode *getCameraCombat();

    // jeu libre
    bool addCameraJeuLibre();
    bool removeCameraJeuLibre();
    scene::ICameraSceneNode *getCameraJeuLibre();


    /** Animations **/
    bool isFin = 1;
    int currentAnimationTime;
    int currentAnimationTimeEnemy;

    void animEnemy(int id, bool voieLibre, Action act);
    void animPlayer(bool voieLibre, Action act);
    void playAnimation(bool voieLibre, Action act, is::IAnimatedMeshSceneNode *perso);
    void animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso);
    void animATTACK(is::IAnimatedMeshSceneNode *perso);
    void animUP(is::IAnimatedMeshSceneNode *perso);
    void animDOWN(is::IAnimatedMeshSceneNode *perso);
    void animLEFT(is::IAnimatedMeshSceneNode *perso);
    void animRIGHT(is::IAnimatedMeshSceneNode *perso);




    /** fonctions utiles pour la scene 3D **/

    ig::IGUIWindow *itemWindow; //fenetre des items (inventaire)
    ig::IGUIWindow *windowItemRecovered; //fenetre a afficher quand un item  est recupere
    ig::IGUIWindow *ecranTitre; // ecran titre (pour commencer le jeu)
    bool startGame = false; // mis a true quand on appuie sur start

    std::vector<iv::ITexture*> loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver); // charge un gif
    void playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ig::IGUIImage *box, IrrlichtDevice *device); // lire une video
    void createMenu(ig::IGUIEnvironment *gui); // creer le menu
    void createItemWindow(ig::IGUIEnvironment *gui); // construction de la fenetre des items


    /** map 3D **/
    bool addMapScene3D();
    bool removeMapScene3D();
    scene3D *getMapScene3D();

    /** game over screen **/
    void addGameOverScreen();


    /** gestionnaire de jeu **/
    // pour savoir si on commence un combat
    bool isCombat; // : 1 => le combat commence dans la scene 2D , appelle de la fonction gestion combat()
    bool isPromenade; // : 1 => le joueur se promene dans le monde 3D, appelle de la fonction gestion promenade()

    // flags pour le combat tour par tour
    bool playerTurn, ennemysTurn, endPlayerTurn;

    std::vector<bool> allEnemyDead; // pour savoir quand sortir du mode combat
    bool isSomeoneAtPosition(int ligne, int colonne);
    void executerAction(int enemyIndex, QTableAction a);
    void startCombat(irr::ITimer *Timer);
    void startPromenade(irr::ITimer *Timer);
    void loopPromenade(irr::ITimer *Timer);
    void loopCombat(irr::ITimer *Timer);
    void loopJeuLibre(irr::ITimer *Timer);
    void sceneRenderer(ITimer *Timer);



};

#endif
