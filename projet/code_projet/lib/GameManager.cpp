#include <iostream>

#include "gui_ids.h"
#include "GameManager.hpp"

GameManager::GameManager(irr::IrrlichtDevice *device)
    :device(device), isCombat(0), isPromenade(1)
{
    smgr = this->device->getSceneManager();
}




/**  joueur **/

// addPlayer ajoute le joueur a la scene 3D et configure les parametre utiles pour la scene 2D et :
// renvoie 1 si tout s'est bien passe
// renvoie 0 si un joueur est deja de la partie
bool GameManager::addPlayer(position p, int HP, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, core::vector3df pos3D)
{
    if( j1.size() == 0 )
    {
        player *joueur = new player(p, HP, smgr, mesh, texture, pos3D, j1.size());
        joueur->node->setRotation(ic::vector3df(0, 90, 0));

        if( getGridMapping() != NULL) joueur->p.setPosition(getGridMapping()->curseur);

        // ajout de collision au joueur si la map 3D est chargee
        if (getMapScene3D() != NULL )getMapScene3D()->addCollisionToPerson(joueur->node);

        j1.push_back(joueur);
        std::cout << "... GameManager::addPlayer : joueur ajoute avec succes !..." << std::endl;
        return 1;
    }
    std::cout << "... GameManager::addPlayer : joueur non ajoute ! Un autre joueur est deja de la partie ..." << std::endl;
    return 0;
}


// removePlayer retire le joueur de la scene 3D et par la meme occasion de la scene 2D et :
// renvoie 1 si tout s'est bien passe
// renvoie 0 si aucun joueur n'est present
bool GameManager::removePlayer()
{
    if( j1.size() == 1 )
    {
        j1[0]->node->remove();
        j1.erase(j1.begin());
        std::cout << "... GameManager::removePlayer : joueur retire avec succes !..." << std::endl;
        return 1;
    }
    std::cout << "... GameManager::removePlayer : joueur non retire ! Aucun joueur n'est present ..." << std::endl;
    return 0;
}


// isPlayerAlive renvoie :
// 0 si aucun joueur n'est ajoute ou si le joueur meurt (HP <= 0)
// 1 si le joueur est en vie (HP > 0)
bool GameManager::isPlayerAlive()
{
    if ( j1.size() == 1 )
    {
        if(j1[0]->isAlive())
        {
            std::cout << "... GameManager::isPlayerAlive : Le joueur est vivant ..." << std::endl;
            return 1;
        }
        else if(!j1[0]->isAlive())
        {
            std::cout << "... GameManager::isPlayerAlive : Le joueur n'est pas vivant mais est toujours affiche" << std::endl
                      << "      la fonction GameManager::removePlayer() permet de le retirer proprement de la scene..." << std::endl;
            return 0;
        }
    }
    else
        std::cout << "... GameManager::isPlayerAlive : Aucun joueur n'est de la partie,"
                  << " donc non le joueur n'est pas vivant ..." << std::endl;
    return 0;
}

// getPlayer pointe vers le joueur si le joueur existe (j1.size() == 1 && tant qu'on n'a pas removePlayer())
player *GameManager::getPlayer()
{
    if (j1.size() == 1)
    {
        //std::cout << "... GameManager::getPlayer() : Le joueur a ete bien recupere ..." << std::endl;
        return j1[0];
    }
    std::cout << "... GameManager::getPlayer() : Aucun joueur present ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}











/**  Enemy **/

// addEnemy ajoute l'ennemi a la scene 3D et configure les parametre utiles pour la scene 2D et :
// renvoie 1 si tout s'est bien passe
// renvoie 0 sinon
bool GameManager::addEnemy(position p, int HP, irr::scene::IAnimatedMesh *mesh, irr::video::ITexture* texture, ic::vector3df pos3D)
{
    if( true ) // on pourra rajouter une condition sur le nombre max d'ennemis
    {
        enemy *vilain = new enemy(p, HP, smgr, mesh, texture, pos3D, mechant.size());

        // ajout de collision a l'ennemi si la map 3D est chargee
        if (getMapScene3D() != NULL )getMapScene3D()->addCollisionToPerson(vilain->node);

        mechant.push_back(vilain);
        enemyID.push_back(vilain->id);
        std::cout << "... GameManager::addEnemy : Ennemi ajoute avec succes !..." << std::endl;
        return 1;
    }
    std::cout << "... GameManager::addEnemy : C'est vraiment chelou qu'il y ait une erreur ..." << std::endl;
    return 0;
}


// removeEnemy retire l'ennemi[i] de la scene 3D et par la meme occasion de la scene 2D et :
// renvoie 1 si tout s'est bien passe
// renvoie 0 s'il y a eu un probleme
bool GameManager::removeEnemy(int id)
{
    if (mechant.size() != 0)
    {
        auto it = std::find (enemyID.begin(), enemyID.end(), id);
        if (it < enemyID.end())
        {
            auto index = std::distance(enemyID.begin(), it);
            enemyID.erase(it);
            mechant[index]->node->remove();
            mechant.erase(mechant.begin() + index);
            std::cout << "... GameManager::removeEnemy() : l'ennemi[" << id << "] retire avec succes ..." << std::endl;
            return 1;
        }
        else if (it >= enemyID.end())
        {
            std::cout << "... GameManager::removeEnemy() : L'ennemi[" << id << "] n'existe pas ! ..." << std::endl;
            return 0;
        }
    }
    std::cout << "... GameManager::removeEnemy() : Aucun ennemi present ! ..." << std::endl;
    return 0;
}



// isEnemyAlive(i) renvoie :
// 0 si aucun ennemi n'est ajoute ou si l'ennemi[i] meurt (HP <= 0)
// 1 si l'ennemi[i] existe (mechant.size() != 0 && tant qu'on n'a pas removeEnemy(i)) et ennemi[i].HP > 0
bool GameManager::isEnemyAlive(int id)
{
    if (mechant.size() != 0)
    {
        auto it = std::find (enemyID.begin(), enemyID.end(), id);
        if (it < enemyID.end())
        {
            auto index = std::distance(enemyID.begin(), it);
            if (mechant[index]->isAlive())
            {
                std::cout << "... GameManager::isEnemyAlive() : l'ennemi[" << id << "] est vivant ..." << std::endl;
                return 1;
            }
            if (!mechant[index]->isAlive())
            {
                std::cout << "... GameManager::isEnemyAlive : l'ennemi[" << id << "] n'est pas vivant mais est toujours affiche" << std::endl
                          << "      la fonction GameManager::removeEnemy(id) permet de le retirer proprement de la scene..." << std::endl;
                return 1;
            }
        }
        else if (it >= enemyID.end())
        {
            std::cout << "... GameManager::isEnemyAlive() : L'ennemi[" << id << "] n'existe pas ! ..." << std::endl;
            return 0;
        }
    }
    std::cout << "... GameManager::isEnemyAlive() : Aucun ennemi present ! ..." << std::endl;
    return 0;
}



// getEnemy(i) pointe vers l'ennemi[i] si l'ennemi[i] existe (j1.size() != 0 && tant qu'on n'a pas removeEnemy(i))
enemy *GameManager::getEnemy(int id)
{
    if (mechant.size() != 0)
    {
        auto it = std::find (enemyID.begin(), enemyID.end(), id);
        if (it < enemyID.end())
        {
            auto index = std::distance(enemyID.begin(), it);
            //std::cout << "... GameManager::getEnemy() : l'ennemi[" << id << "] a ete bien recupere ..." << std::endl;
            return mechant[index];
        }
        else if (it >= enemyID.end())
        {
            std::cout << "... GameManager::getEnemy() : L'ennemi[" << id << "] n'existe pas ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
            return NULL;
        }
    }
    std::cout << "... GameManager::getEnemy() : Aucun ennemi present ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}





/** gridMapping **/
bool GameManager::addGridMapping(int width, int height, ITimer *Timer)
{
    if( grid.size() == 0 )
    {
        if (j1.size() == 1)
        {
            gridMapping *g = new gridMapping(getPlayer()->p, width, height, smgr, Timer);
            g->myGrid->translation(getPlayer()->node->getPosition() - core::vector3df(0, 25, -DEFAULT_GRID_NODE_SIZE/8));

            if( getPlayer() != NULL) getPlayer()->p.setPosition(g->j1.pos);

            grid.push_back(g);
            std::cout << "... GameManager::addGridMapping : GridMapping ajoute avec succes ! Il est egalement affiche ..." << std::endl;
            isCombat = 1; isPromenade = 0;
            return 1;
        }
        else if (j1.size() != 1 || !j1[0]->isAlive())
        {
            std::cout << "... GameManager::addGridMapping : GridMapping non ajoute ! joueur non present ou mort ..." << std::endl;
            return 0;
        }
    }
    std::cout << "... GameManager::addGridMapping : GridMapping non ajoute ! Un autre GridMapping est deja present et affiche ..." << std::endl;
    return 0;
}


bool GameManager::removeGridMapping()
{
    if( grid.size() == 1 )
    {
        if( getPlayer() != NULL) getPlayer()->p.setPosition(position(0, 0));
        for (unsigned int k = 0; k < grid[0]->myGrid->mesh.size(); k++)
            grid[0]->myGrid->mesh[k]->remove();
        grid.erase(grid.begin());
        std::cout << "... GameManager::removeGridMapping() : Le gridMapping a ete bien efface ..." << std::endl;
        isCombat = 0; isPromenade = 1;
        return 1;
    }

    std::cout << "... GameManager::removeGridMapping() : Aucun gridMapping present !  ..." << std::endl;
    return 0;
}



gridMapping *GameManager::getGridMapping()
{
    if( grid.size() == 1 )
    {
        //std::cout << "... GameManager::getGridMapping() : Le gridMapping a ete bien recupere ..." << std::endl;
        return grid[0];
    }
    //std::cout << "... GameManager::getGridMapping() : Aucun gridMapping present ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}









/** cameras **/
// combat
bool GameManager::addCameraCombat()
{
    if( cameraCombat.size() == 0 )
    {
        if (cameraJeuLibre.size() == 0)
        {
            irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0,0,0));

            if(getGridMapping() != NULL && getPlayer() != NULL)
            {
                irr::core::vector3df camCombatPosition(0,
                                                       -getPlayer()->node->getPosition().Y + std::max(DEFAULT_HEIGHT, DEFAULT_WIDTH) * DEFAULT_GRID_NODE_SIZE * 0.25,
                                                       0);

                irr::core::vector3df translationCamCombat(getGridMapping()->myGrid->getGridNode(0)->getPosition().X - 40 * DEFAULT_HEIGHT,
                                                          0,
                                                          (getGridMapping()->myGrid->getGridNode(0)->getPosition().Z - DEFAULT_GRID_NODE_SIZE * DEFAULT_WIDTH/2));

                camera->setPosition(camCombatPosition + translationCamCombat);

                irr::core::vector3df target(getGridMapping()->myGrid->getGridNode(0)->getPosition().X + DEFAULT_GRID_NODE_SIZE * DEFAULT_HEIGHT/2,
                                          0,
                                          (getGridMapping()->myGrid->getGridNode(0)->getPosition().Z - DEFAULT_GRID_NODE_SIZE * DEFAULT_WIDTH/2));

                camera->setTarget(target);
            }

            cameraCombat.push_back(camera);
            std::cout << "... GameManager::addCameraCombat : CameraCombat ajoutee avec succes !  ..." << std::endl;
            isCombat = 1; isPromenade = 0;
            return 1;
        }
        else if (cameraJeuLibre.size() != 0)
        {
            std::cout << "... GameManager::addCameraCombat : CameraCombat non ajoutee ! cameraJeuLibre deja presente.  ..."
                      << "     Utilisez la fonction GameManager::removeCameraJeuLibre(), puis reessayez."
                      << std::endl;
            return 0;
        }
    }
    std::cout << "... GameManager::addCameraCombat : CameraCombat non ajoutee ! Une CameraCombat est deja presente ..." << std::endl;
    return 0;
}


bool GameManager::removeCameraCombat()
{
    if( cameraCombat.size() == 1 )
    {
        cameraCombat[0]->remove();
        cameraCombat.erase(cameraCombat.begin());
        std::cout << "... GameManager::removeCameraCombat() : La CameraCombat a ete bien effacee ..." << std::endl;
        isCombat = 0; isPromenade = 0;
        return 1;
    }

    std::cout << "... GameManager::removeCameraCombat() : Aucune CameraCombat presente !  ..." << std::endl;
    return 0;
}



scene::ICameraSceneNode *GameManager::getCameraCombat()
{
    if( cameraCombat.size() == 1 )
    {
        //std::cout << "... GameManager::getCameraCombat() : La CameraCombat a ete bien recupere ..." << std::endl;
        return cameraCombat[0];
    }
    //std::cout << "... GameManager::getCameraCombat() : Aucune CameraCombat presente ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}



// JeuLibre
bool GameManager::addCameraJeuLibre()
{
    if( cameraJeuLibre.size() == 0 )
    {
        if (cameraCombat.size() == 0)
        {
            if (getPlayer() == NULL)
            {
                std::cout << "... GameManager::addCameraJeuLibre : CameraJeuLibre non ajoutee ! Aucun joueur present !  ..." << std::endl;
                return 0;
            }
            irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNode(getPlayer()->node, core::vector3df(0, 0, 0), core::vector3df(0,0,0));
            camera->setPosition(ic::vector3df(-50, 30, 0));
            // ajout collision a la camera en mode jeu libre si la map 3D est chargee
            if (getMapScene3D() != NULL) getMapScene3D()->addCollisionToCamera(camera, ic::vector3df(30, 50, 30));

            cameraJeuLibre.push_back(camera);
            isCombat = 0; isPromenade = 1;
            std::cout << "... GameManager::addCameraJeuLibre : CameraJeuLibre ajoutee avec succes !  ..." << std::endl;
            return 1;
        }
        else if (cameraCombat.size() != 0)
        {
            std::cout << "... GameManager::addCameraJeuLibre : CameraJeuLibre non ajoutee ! cameraCombat deja presente.  ..."
                      << "     Utilisez la fonction GameManager::removeCameraCombat(), puis reessayez."
                      << std::endl;
            return 0;
        }
    }
    std::cout << "... GameManager::addCameraJeuLibre : CameraJeuLibre non ajoutee ! Une CameraJeuLibre est deja presente ..." << std::endl;
    return 0;
}


bool GameManager::removeCameraJeuLibre()
{
    if( cameraJeuLibre.size() == 1 )
    {
        cameraJeuLibre[0]->remove();
        cameraJeuLibre.erase(cameraJeuLibre.begin());
        std::cout << "... GameManager::removeCameraJeuLibre() : La CameraJeuLibre a ete bien effacee ..." << std::endl;
        isCombat = 0; isPromenade = 0;
        return 1;
    }

    std::cout << "... GameManager::removeCameraJeuLibre() : Aucune CameraJeuLibre presente !  ..." << std::endl;
    return 0;
}



scene::ICameraSceneNode *GameManager::getCameraJeuLibre()
{
    if( cameraJeuLibre.size() == 1 )
    {
        //std::cout << "... GameManager::getCameraJeuLibre() : La CameraJeuLibre a ete bien recupere ..." << std::endl;
        return cameraJeuLibre[0];
    }
    //std::cout << "... GameManager::getCameraJeuLibre() : Aucune CameraJeuLibre presente ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}





/** Animations **/

void GameManager::animEnemy(int id, bool voieLibre, Action act)
{
    switch (act) {
    case VALIDATE: // passer la main au joueur
        break;
    case RESET: // reset
        break;

    default: // dans la voie libre il faut verifier que la position 2D ou se rend l'ennemi est differente de celle du joueur
        if (getEnemy(id) != NULL) playAnimation(voieLibre,  act, getEnemy(id)->node);
        break;
    }
}

void GameManager::animPlayer(bool voieLibre, Action act)
{
    switch (act) {
    case VALIDATE:
        playAnimation(voieLibre,  VALIDATE, getPlayer()->node);
        getPlayer()->p.setPosition(getGridMapping()->curseur);
        break;

    case RESET: // reset
        getPlayer()->node->setPosition((core::vector3df(0, 25, 0)
                            + getGridMapping()->myGrid->getGridNode(0)->getPosition()
                            + core::vector3df(-getGridMapping()->j1.pos.ligne * DEFAULT_GRID_NODE_SIZE,
                                              0,
                                              -getGridMapping()->j1.pos.colonne * DEFAULT_GRID_NODE_SIZE)));

        break;
    default:
        playAnimation(voieLibre,  act, getPlayer()->node);
        break;
    }
}


void GameManager::playAnimation(bool voieLibre, Action act, scene::IAnimatedMeshSceneNode *perso)
{
    switch (act) {
    case VALIDATE:
        break;
    case RESET: // reset
        break;



    case UP: // haut
        if (voieLibre)
            animator(0, 1, perso);
        //animUP(getPlayer()->node);
        break;
    case DOWN: // bas
        if (voieLibre)
            animator(0, -1, perso);
        //animDOWN(getPlayer()->node);
        break;
    case RIGHT: // droite
        if (voieLibre)
            animator(1, 0, perso);
        //animRIGHT(getPlayer()->node);
        break;
    case LEFT: // gauche
        if (voieLibre)
            animator(-1, 0, perso);
        //animLEFT(getPlayer()->node);
        break;
    case ATTACK: // attack
        animATTACK(perso);
        break;


    case NOTHING:
        if (device->getTimer()->getTime() - currentAnimationTime >= DEFAULT_DUREE_ANIMATION )
            perso->setMD2Animation(is::EMAT_STAND);
        break;
    default:
        if (device->getTimer()->getTime() - currentAnimationTime >= DEFAULT_DUREE_ANIMATION )
            perso->setMD2Animation(is::EMAT_STAND);
        break;
    }
}

void GameManager::animator(int nombreCasesHorizontales, int nombreCasesVerticales, is::IAnimatedMeshSceneNode *perso)
{
    ic::vector3df depart = perso->getPosition();
    ic::vector3df arrivee = depart + ic::vector3df(nombreCasesVerticales * DEFAULT_GRID_NODE_SIZE,
                                                   0,
                                                   - nombreCasesHorizontales * DEFAULT_GRID_NODE_SIZE);
    perso->setPosition(arrivee);
    perso->setRotation(ic::vector3df(0, atan2(nombreCasesHorizontales, nombreCasesVerticales) * 180 / M_PI, 0));
}


void GameManager::animATTACK(is::IAnimatedMeshSceneNode *perso)
{
    currentAnimationTime = device->getTimer()->getTime();
    perso->setMD2Animation(is::EMAT_CROUCH_ATTACK);
}

void GameManager::animUP(is::IAnimatedMeshSceneNode *perso)
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

void GameManager::animDOWN(is::IAnimatedMeshSceneNode *perso)
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

void GameManager::animLEFT(is::IAnimatedMeshSceneNode *perso)
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

void GameManager::animRIGHT(is::IAnimatedMeshSceneNode *perso)
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








/** fonctions utiles pour la scene 3D **/

// charger animation
std::vector<iv::ITexture*> GameManager::loadGif(int nbFrame, std::wstring nomGeneral, iv::IVideoDriver *driver)
{
    std::vector<iv::ITexture*> Vector;
    std::wstring numFrame;
    std::wstring nomComplet;

    iv::ITexture *Texture;

    for(int i = 1; i<=nbFrame ; i++)
    {
      numFrame = std::to_wstring(i) + L".png";
      nomComplet = nomGeneral + numFrame;
      Texture = driver->getTexture(nomComplet.c_str());
      Vector.push_back(Texture);
    }
    return Vector;
}



// lecture d une video
void GameManager::playVideo(std::vector<iv::ITexture*> frameVector, int nbFrame, ig::IGUIImage *box, IrrlichtDevice *device)
{
    int currentFrame = 0;
    while(currentFrame<nbFrame)
    {
        box->setImage(frameVector[currentFrame]);
        currentFrame++;
        std::cout << currentFrame << std::endl;
        smgr->drawAll();
        device->getGUIEnvironment()->drawAll();
        device->getVideoDriver()->endScene();
        device->sleep(100);
    }
    box->remove();
}


// creer le menu
void GameManager::createMenu(ig::IGUIEnvironment *gui)
{
  ig::IGUIContextMenu *submenu;

  // une entrée principale :
  ig::IGUIContextMenu *menu = gui->addMenu();
  menu->addItem(L"Menu", -1, true, true);

  menu->addItem(L"A propos", -1, true, true);

  submenu = menu->getSubMenu(1);
  submenu->addItem(L"A propos", ABOUT);

  // Le contenu du menu :
  submenu = menu->getSubMenu(0);
  submenu->addItem(L"New game...", MENU_NEW_GAME);
  submenu->addSeparator();
  submenu->addItem(L"Commandes", MENU_COMMANDES);
  submenu->addSeparator();
  submenu->addItem(L"Quit", MENU_QUIT);



}


// creation de la fenetre des items
void GameManager::createItemWindow(ig::IGUIEnvironment *gui)
{
  // La fenetre des items
  itemWindow = gui->addWindow(ic::rect<s32>(420,25, 620,460), false, L"items");
  itemWindow->setVisible(false);
}





/** map 3D **/
bool GameManager::addMapScene3D()
{

    if(mapScene3D.size() == 0)
    {
        scene3D *scene = new scene3D(device, smgr->getMesh("mario.bsp"));

        // on ajoute une collision au joueur s'il existe
        if (getPlayer() != NULL) scene->addCollisionToPerson(getPlayer()->node);

        // on ajoute une collision aux ennemis s'ils existent
        for (unsigned int i = 0; i < mechant.size(); i++)
        {
            if (getEnemy(i) != NULL)
                scene->addCollisionToPerson(getEnemy(i)->node);
        }

        // on ajoute une collision au joueur s'il existe
        if (getCameraJeuLibre() != NULL) scene->addCollisionToCamera(getCameraJeuLibre(), ic::vector3df(30, 50, 30));

        mapScene3D.push_back(scene);
        std::cout << "... GameManager::addMapScene3D() : MapScene3D ajoutee avec succes ! ..." << std::endl;
        return 1;
    }
    std::cout << "... GameManager::addMapScene3D() : MapScene3D non ajoutee ! elle est deja presente et affichee !  ..." << std::endl;
    return 0;
}


bool GameManager::removeMapScene3D()
{
    if(mapScene3D.size() == 1)
    {
        if (getPlayer() != NULL) getPlayer()->node->removeAnimators();
        // on ajoute une collision aux ennemis s'ils existent
        for (unsigned int i = 0; i < mechant.size(); i++)
        {
            if (getEnemy(i) != NULL)
                getEnemy(i)->node->removeAnimators();
        }

        // on ajoute une collision au joueur s'il existe
        if (getCameraJeuLibre() != NULL) getCameraJeuLibre()->removeAnimators();

        mapScene3D[0]->node->remove();
        mapScene3D.erase(mapScene3D.begin());
        itemWindow->setVisible(false);
        std::cout << "... GameManager::removeMapScene3D() : MapScene3D retiree avec succes ! ..." << std::endl;
        return 1;
    }
    std::cout << "... GameManager::removeMapScene3D() : Impossible de retirer MapScene3D. La scene 3D n'est pas presente !  ..."
              << std::endl;
    return 0;
}

scene3D *GameManager::getMapScene3D()
{
    if(mapScene3D.size() == 1)
    {
        std::cout << "... GameManager::getMapScene3D() : La scene 3D a ete bien recuperee !  ..." << std::endl;
        return mapScene3D[0];
    }
    std::cout << "... GameManager::getMapScene3D() : La scene 3D n'est pas presente ! Vous avez recupere un pointeur NULL !  ..."
              << std::endl;
    return NULL;
}










/** gestionnaire de jeu **/

// a appeler dans le sceneRenderer durant le mode combat
void GameManager::combat(irr::ITimer *Timer)
{
    isCombat = 1;
    isPromenade = 0;

    //animation debut combat
    std::vector<iv::ITexture*> fightVector = loadGif(20, L"data/animations/combat/combat", device->getVideoDriver());
    ig::IGUIImage *fightBox = device->getGUIEnvironment()->addImage(ic::rect<s32>(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
    fightBox->setScaleImage(true);
    playVideo(fightVector, 20, fightBox, device);

    removeMapScene3D();
    removeCameraJeuLibre();
    addGridMapping(DEFAULT_WIDTH, DEFAULT_HEIGHT, Timer);
    addCameraCombat();

    // Q table for the ennemis
    int num_states = getNumStates();
    this->qTable = new Q_table(num_states, NUM_ACTIONS);
    this->qTable->loadTable("test_table");
    // mettre la q table dans tous les ennemis
    for (unsigned int k = 0; k < mechant.size(); k++)
    {
        if (getEnemy(k) != NULL && getPlayer() != NULL) // pour eviter les erreurs de segmentations
        {
          this->getEnemy(k)->setQTable(this->qTable);
        }
    }
}


// a appeler dans le sceneRenderer durant le mode jeu libre
void GameManager::promenade(irr::ITimer *Timer)
{
    isCombat = 0;
    isPromenade = 1;
    removeGridMapping();
    removeCameraCombat();
    addMapScene3D();
    addCameraJeuLibre();
}



void GameManager::sceneRenderer(irr::ITimer *Timer)
{

    /** variables utiles pour plus tard **/
    iv::IVideoDriver  *driver = device->getVideoDriver();
    ig::IGUIEnvironment *gui  = device->getGUIEnvironment();

    /** police de caractere **/
    ig::IGUISkin* skin = gui->getSkin();
    ig::IGUIFont* font = gui->getFont("data/menu/fontlucida.png");
    skin->setFont(font);

    /** initialisation du menu et de la fenetre des items **/
    createMenu(gui);
    createItemWindow(gui);

    /** initialisation de la barre de vie du joueur **/
    ig::IGUIImage *hpBox = gui->addImage(ic::rect<s32>(10,25,  0.28 * DEFAULT_SCREEN_WIDTH,40)); hpBox->setScaleImage(true);
    std::vector<iv::ITexture*> hpVector = loadGif(/*nbFrameHp*/60, /*path*/ L"data/perso/hp/health", driver);
    if (getPlayer() != NULL)
        hpBox->setImage(hpVector[getPlayer()->HP]);
    else
        hpBox->setImage(hpVector[59]);

    // pour la prise de decision des ennemis / q table
    int dist_x_pers, dist_y_pers;
    float hp_pers;
    QTableAction a;


    /** initialisation de la partie **/
    promenade(Timer);

    while(device->run())
    {


        device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0)); // fond noir
        smgr->drawAll();
        gui->drawAll();



        // debut de jeu qui passe du mode jeu libre au mode combat lorsque le joueur se trouve aux memes coordonnees que l'ennemi[k]
        // le joueur perd la moitie de sa vie lorsqu'il valide sa position a la case [2, 5] de la gridMapping
        // le jeu rebascule en mode jeu libre lorsqu'on valide la position du joueur a la case [0, 3] de la gridMapping
        float epsilon = 10;

        for (unsigned int k = 0; k < mechant.size(); k++)
        {
            if (getEnemy(k) != NULL && getPlayer() != NULL) // pour eviter les erreurs de segmentations
            {

                // durant le mode jeu libre
                if ( !isCombat && isPromenade )
                {
                    if (    (core::abs_(getPlayer()->node->getPosition().X - getEnemy(k)->node->getPosition().X)) <= epsilon
                            &&   (core::abs_(getPlayer()->node->getPosition().Y - getEnemy(k)->node->getPosition().Y)) <= epsilon
                            &&   (core::abs_(getPlayer()->node->getPosition().Z - getEnemy(k)->node->getPosition().Z)) <= epsilon  )

                    {
                        isCombat = 1; isPromenade = 0;
                        combat(Timer);
                    }
                }

                // durant le mode combat
                if ( isCombat && !isPromenade )
                {
                    // get informations pour choisir l'action
                    dist_x_pers = this->getPlayer()->node->getPosition().X - this->getEnemy(k)->node->getPosition().X;
                    dist_y_pers = this->getPlayer()->node->getPosition().Y - this->getEnemy(k)->node->getPosition().Y;
                    hp_pers = this->getPlayer()->HP;

                    // choisir l'action de l'ennemi
                    a = this->getEnemy(k)->chooseAction(dist_x_pers, dist_y_pers, hp_pers);


                    //std::cout << "DEBUG combat action("<<k<<") " <<(int)a<< '\n';

                    if(getPlayer()->p == position(0, 3))
                    {
                        isCombat = 0; isPromenade = 1;
                        promenade(Timer);
                    }
                    if(getPlayer()->p == position(2, 5))
                    {
                        getPlayer()->HP = DEFAULT_PLAYER_HP / 2;
                    }
                }

            }
        }


        /** DO NOT EDIT **/

        // pour que les cameras aient la bonne target
        if (getCameraJeuLibre() != NULL)
            getCameraJeuLibre()->setTarget(getPlayer()->node->getPosition());

        // faire clignoter le curseur
        if (getGridMapping() != NULL)
            getGridMapping()->makeCurseurBlink(true);

        // actualise la barre de vie en fonction du nombre de HP restant du joueur
        if (getPlayer() != NULL)
            hpBox->setImage(hpVector[getPlayer()->HP]);

        device->getVideoDriver()->endScene();


        /** *********** **/

    }


}
