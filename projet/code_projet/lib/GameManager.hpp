#pragma once

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP


#include "utils.hpp"
#include "libGridMapping/gridMapping.hpp"

class GameManager
{

    //sceneManager (/!\ different du gameManager)
    irr::scene::ISceneManager *smgr;


    // joueur (/!\ il y a toujours un seul joueur; meilleure gestion avec un vecteur)
    std::vector<player*> j1;


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


public:

    //device : correspond exactement au device dans le main
    irr::IrrlichtDevice *device;

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

    void animPlayer(bool voieLibre, Action act)
    {
        switch (act) {
        case VALIDATE:
            playAnimation(voieLibre,  VALIDATE, getPlayer()->node);
            getPlayer()->p.setPosition(getGridMapping()->curseur);
            break;
        default:
            playAnimation(voieLibre,  act, getPlayer()->node);
            break;
        }
    }
    void playAnimation(bool voieLibre, Action act, is::IAnimatedMeshSceneNode *perso);
    void animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso);
    void animATTACK(is::IAnimatedMeshSceneNode *perso);
    void animUP(is::IAnimatedMeshSceneNode *perso);
    void animDOWN(is::IAnimatedMeshSceneNode *perso);
    void animLEFT(is::IAnimatedMeshSceneNode *perso);
    void animRIGHT(is::IAnimatedMeshSceneNode *perso);




    /** fonctions utiles pour la scene 3D **/
    ig::IGUIWindow *window;


    // pour changer la scene
    void parametreScene(bool screenChange, is::IMeshSceneNode *node, is::ISceneManager *smgr, std::vector<is::IAnimatedMesh*> meshVector,
                               scene::ITriangleSelector *selector, scene::ISceneNodeAnimator *anim, is::IAnimatedMeshSceneNode *perso,
                               core::vector3df radius, scene::ISceneNodeAnimator *animcam, scene::ICameraSceneNode* camera);

    // charge un gif
    std::vector<iv::ITexture*> loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver);

    // lire une video
    void playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ig::IGUIImage *box, IrrlichtDevice *device,
                          is::ISceneManager *smgr, ig::IGUIEnvironment *gui, iv::IVideoDriver  *driver);

    // creer le menu
    void create_menu(ig::IGUIEnvironment *gui);

    // construction des differentes fenetres
    void create_window(ig::IGUIEnvironment *gui);


    /** map 3D **/
    bool addMapScene3D();
    bool removeMapScene3D();
    scene3D *getMapScene3D();


    /** gestionnaire de jeu **/
    // pour savoir si on commence un combat
    bool isCombat; // : 1 => le combat commence dans la scene 2D , appelle de la fonction gestion combat()
    bool isPromenade; // : 1 => le joueur se promene dans le monde 3D, appelle de la fonction gestion promenade()
    void combat(irr::ITimer *Timer);
    void promenade(irr::ITimer *Timer);
    void sceneRenderer(ITimer *Timer);



};

#endif
