#include "../lib/utils.hpp"
#include "../lib/GameManager.hpp"
#include "../lib/MyEventReceiver.hpp"
#include "../lib/Trainer.hpp"

// DEFAULT_PATH_TO_DATA "../projet/code_projet/data/" // compilation depuis le CMakeLists.txt
// DEFAULT_PATH_TO_DATA "./data/" // compilation depuis le makefile



void interface(); // $ ./projetFA
void mode_train(); // $ ./projetFA train




int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator


    if(argc == 1)
    {
        std::cout << "... Player mode (interface) ..." << std::endl;
        std::cout << "    N'oubliez pas de mettre le dossier data au bon endroit !" << std::endl;
        interface();
    }

    else if (argc == 2)
    {
        char* param = argv[1];

        if(strcmp(param,"train")==0)
        {
            std::cout << "...mode TRAIN ..." << '\n';
            mode_train();
        }

        else
        {
            std::cout << "ERROR : parametre non reconnu" << '\n';
            std::exit(EXIT_FAILURE);
        }
    }

    else
    {
        std::cout << "ERROR : trop de parametres " << '\n';
        std::exit(EXIT_FAILURE);
    }


    return 0;
}







// entrainement des ennemis
void mode_train()
{
  Trainer* trainer = new Trainer(10, 15);
  //trainer->train();
  trainer->train_v2();
}












// execution du main avec interface irrlicht
void interface()
{

    /** DO NOT EDIT **/

    // Le gestionnaire d'événements
    MyEventReceiver *receiver = new MyEventReceiver();

    // Création de la fenêtre et du système de rendu.
    irr::IrrlichtDevice *device = createDevice(irr::video::EDT_OPENGL,
                                              irr::core::dimension2d<u32>(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT),
                                              16, false, false, false, receiver);

    irr::ITimer *Timer = device->getTimer();
    irr::video::IVideoDriver  *driver = device->getVideoDriver();
    irr::scene::ISceneManager *smgr = device->getSceneManager();



    //creation du gameManager
    GameManager *gameManager = new GameManager(device);
    receiver->gmngr = gameManager; // /!\ toujours rattacher le receiver au gameManager

    /** ************* **/




    // Ajout de l'archive qui contient entre autres un niveau complet
    device->getFileSystem()->addFileArchive("./data/maps/cf.pk3");

    // chargement du mesh (puisqu'il sera utilise plusieurs fois)
    irr::scene::IAnimatedMesh *mesh = smgr->getMesh("./data/tris/tris.md2");

    // chargement des textures
    irr::video::ITexture* textureJoueur = driver->getTexture("./data/tris/blue_texture.pcx");
    irr::video::ITexture* textureEnemy = driver->getTexture("./data/tris/red_texture.pcx");

    gameManager->mesh = mesh;
    gameManager->textureEnemy = textureEnemy;



    // ajout d'un joueur
    gameManager->addPlayer(position(DEFAULT_LIGNE, DEFAULT_COLONNE), // positions 2D dans la grille quand le combat commencera (peut etre modifie plus tard avant le debut du combat)
                           DEFAULT_PLAYER_HP, // HP du joueur
                           mesh, // mesh du joueur
                           textureJoueur, // texture du joueur
                           ic::vector3df(0, 0, 0)); // positions 3D dans le monde 3D du joueur
    gameManager->getPlayer()->node->setPosition(ic::vector3df(1290.93, 388.025, -1334.74));
    gameManager->tempPlayer3DPosition = gameManager->getPlayer()->node->getPosition();



    // ajouts d'ennemis

    //    gameManager->addEnemy(position(2, 5),
    //                           DEFAULT_ENEMY_HP, // HP de l'ennemi
    //                           mesh, // mesh de l'ennemi
    //                           textureEnemy, // texture du joueur
    //                           ic::vector3df(131.569, -78.2488, -898.676)); // positions 3D dans le monde 3D du joueur


//    gameManager->addEnemy(position(5, 3),
//                           DEFAULT_ENEMY_HP, // HP de l'ennemi
//                           mesh, // mesh de l'ennemi
//                           textureEnemy, // texture du joueur
//                           ic::vector3df(-DEFAULT_GRID_NODE_SIZE * 5, 0, -DEFAULT_GRID_NODE_SIZE * 3)); // positions 3D dans le monde 3D du joueur






    gameManager->sceneRenderer(Timer);


    device->drop();




}
