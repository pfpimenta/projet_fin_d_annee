#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>


#include <irrlicht/irrlicht.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;


#include "../lib/Q_table.hpp"
#include "../lib/Agent.hpp"
#include "../lib/GameManager.hpp"
#include "../lib/State.hpp"

#include "../lib/MyEventReceiver.hpp"



int main(){

    /** DO NOT EDIT **/

    std::srand(std::time(nullptr)); // use current time as seed for random generator

    // Le gestionnaire d'événements
    MyEventReceiver *receiver = new MyEventReceiver();

    // Création de la fenêtre et du système de rendu.
    IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, receiver);

    iv::IVideoDriver  *driver = device->getVideoDriver();
    is::ISceneManager *smgr = device->getSceneManager();

    irr::ITimer *timerCursorBlink = device->getTimer(); // timer pour le curseur qui blink


    /** ********* **/





//    std::cout << "test main Q_table" << std::endl;
//    // pour tester la Q_table
//    Q_table qtable = Q_table();
//    qtable.printTable();




//cration du gameManager

/** DO NOT EDIT **/
    GameManager *gameManager = new GameManager(20, 10, smgr, timerCursorBlink);

    // /!\ toujours rattacher le receiver au gameManager ET gameManager->grid
    receiver->gmngr = gameManager;
    receiver->grid = gameManager->grid;

/** ******** **/



    // ajout d'obstacles
    std::vector<position> trucs; int nbTruc = 10;
    for(int k = 0; k < nbTruc; k++)
    {
        trucs.push_back(position(alea(1, DEFAULT_HEIGHT - 1), alea(1, DEFAULT_WIDTH - 1)));
        std::cout << trucs[k].printPosition(0) << std::endl;
    }
    gameManager->grid->addObstacle(trucs);


    // ajout d'ennemis
    Agent personnage_test = Agent(2, 5);
    gameManager->addPersonnage(&personnage_test);
    Agent personnage_test2 = Agent(5, 2);
    gameManager->addPersonnage(&personnage_test2);
    Agent personnage_test3 = Agent(2, 2);
    gameManager->addPersonnage(&personnage_test3);




//    // training test
//    gameManager.train();





    // tester le jeu avec l'affichage au terminal
    // while(true){
    //     std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clear
    //     gameManager.printGrid();
    //     gameManager.printHPs();
    //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' ); // wait for key
    //     gameManager.step();
    // }









    /** Ajout d'une camera **/
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,0));
    camera->setPosition(ic::vector3df(0, - std::max(DEFAULT_HEIGHT, DEFAULT_WIDTH) * DEFAULT_GRID_NODE_SIZE / 1.5, 0));
    receiver->cam1 = camera;




    while(device->run())
    {
        driver->beginScene(true, true, iv::SColor(255, 0, 0, 0)); // fond noir

        camera->setTarget(core::vector3df(0, 0, 0));

        // Dessin de la scène :
        smgr->drawAll();


        // debug : verifier que l'ennemi se deplace vraiment
//        std::cout << "perso pedro : ";
//        std::cout << gameManager->personnages[0]->pos_x << " "
//                << gameManager->personnages[0]->pos_y  << std::endl;
//        std::cout << "perso ced : ";
//        std::cout << gameManager->grid->mechant[0].pos.ligne << " "
//                << gameManager->grid->mechant[0].pos.colonne  << std::endl;


        gameManager->printHPs();


        // pour faire clignoter le curseur
        gameManager->makeCurseurBlink(true);


        driver->endScene();


    }

    device->drop();


    return 0;
}

