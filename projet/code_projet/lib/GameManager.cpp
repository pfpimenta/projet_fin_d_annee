#include <iostream>

#include "gui_ids.h"
#include "GameManager.hpp"

GameManager::GameManager(irr::IrrlichtDevice *device)
    :device(device), isCombat(0), isPromenade(1)
{
    smgr = this->device->getSceneManager();

    if (getPlayer() != NULL)tempPlayer3DPosition = getPlayer()->node->getPosition();
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


            mechant[index]->node->setRotation(core::vector3df(90, 0, 90));
            if (getPlayer() != NULL) mechant[index]->node->setPosition(core::vector3df(mechant[index]->node->getPosition().X,
                                                                                       getPlayer()->node->getPosition().Y - 25,
                                                                                       mechant[index]->node->getPosition().Z));

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
                                                       getGridMapping()->myGrid->getGridNode(0)->getPosition().Y + std::max(DEFAULT_HEIGHT, DEFAULT_WIDTH) * DEFAULT_GRID_NODE_SIZE * 0.5,
                                                       0);

                irr::core::vector3df translationCamCombat(getGridMapping()->myGrid->getGridNode(0)->getPosition().X - 40 * DEFAULT_HEIGHT,
                                                          0,
                                                          (getGridMapping()->myGrid->getGridNode(0)->getPosition().Z - DEFAULT_GRID_NODE_SIZE * DEFAULT_WIDTH/2));

                camera->setPosition(camCombatPosition + translationCamCombat);

                irr::core::vector3df target(getGridMapping()->myGrid->getGridNode(0)->getPosition().X + DEFAULT_GRID_NODE_SIZE * DEFAULT_HEIGHT/2,
                                          getGridMapping()->myGrid->getGridNode(0)->getPosition().Y,
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

            irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(nullptr,
                                                                                100,         // Vitesse de rotation
                                                                                0,          // Vitesse de déplacement
                                                                                -1,          // Identifiant
                                                                                nullptr, 0,  // Table de changement de touches
                                                                                true      // Pas de possibilité de voler
                                                                                );          // Vitesse saut

            //irr::scene::ICameraSceneNode *camera = smgr->addCameraSceneNode(getPlayer()->node, core::vector3df(0, 0, 0), core::vector3df(0,0,0));
            irr::core::vector3d<float> pos = getPlayer()->node->getPosition();
            //pos.Y -= 27;
            camera->setPosition(pos);
            // ajout collision a la camera en mode jeu libre si la map 3D est chargee
//            if (getMapScene3D() != NULL) getMapScene3D()->addCollisionToCamera(camera, ic::vector3df(30, 50, 30));

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
        getGridMapping()->j1.pos = getPlayer()->p;
        break;

    case RESET: // reset
        getPlayer()->node->setPosition((core::vector3df(0, 25, 0)
                            + getGridMapping()->myGrid->getGridNode(0)->getPosition()
                            + core::vector3df(-getGridMapping()->j1.pos.ligne * DEFAULT_GRID_NODE_SIZE,
                                              0,
                                              -getGridMapping()->j1.pos.colonne * DEFAULT_GRID_NODE_SIZE)));

        break;


    case ATTACK:
        //for(unsigned int k = 0; k < mechant.size(); k++)
        getPlayer()->node->setMD2Animation(is::EMAT_CROUCH_ATTACK);
        for (auto &k : enemyID)
        {
            if(getEnemy(k) != NULL)
            {
                if(getEnemy(k)->p.isNear(getPlayer()->p))
                {
                    if (getEnemy(k)->HP >=  damage)
                    {
                        getEnemy(k)->HP -= damage;
                    }
                    else
                    {
                        getEnemy(k)->HP = 0;
                    }
                }
            }
            animPlayer(0, RESET);
            endPlayerTurn = true;
        }
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
        if (getGridMapping() != NULL)
        {
            if (voieLibre){
                getPlayer()->node->setMD2Animation(is::EMAT_STAND);
                animator(0, 1, perso);
            }
        }
        break;
    case DOWN: // bas
        if (getGridMapping() != NULL)
        {
            if (voieLibre)
            {
                getPlayer()->node->setMD2Animation(is::EMAT_STAND);
                animator(0, -1, perso);
            }
        }
        break;
    case RIGHT: // droite
        if (getGridMapping() != NULL)
        {
            if (voieLibre)
            {
                getPlayer()->node->setMD2Animation(is::EMAT_STAND);
                animator(1, 0, perso);
            }
        }
        break;
    case LEFT: // gauche
        if (getGridMapping() != NULL)
        {
            if (voieLibre)
            {
                getPlayer()->node->setMD2Animation(is::EMAT_STAND);
                animator(-1, 0, perso);
            }
        }
        break;
    case ATTACK: // attack
        //animATTACK(perso);
        break;


    case NOTHING:
        perso->setMD2Animation(is::EMAT_STAND);
        break;
    default:
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
  itemWindow = gui->addWindow(ic::rect<s32>(10, 70, DEFAULT_SCREEN_WIDTH - 10, DEFAULT_SCREEN_HEIGHT - 30), false, L"items");
  itemWindow->setDraggable(false);
  itemWindow->setDrawTitlebar(false);
  iv::ITexture *fondItem = device->getVideoDriver()->getTexture(L"data/potions/fondItem.png");
  gui->addImage(fondItem, ic::position2d<s32>(0, 0), true, itemWindow);
  itemWindow->setVisible(false);

  //fenetre a afficher quand un item  est recupere
//  windowItemRecovered = gui->addWindow(ic::rect<s32>(10, 70, DEFAULT_SCREEN_WIDTH - 10, DEFAULT_SCREEN_HEIGHT - 30), false, L"items");
//  windowItemRecovered->setVisible(false);

//  windowUltimeItemmessage = gui->addMessageBox(L"", L"Félicitation, vous avez ramassé tous les objets\n"
//                                                    "présents ce qui a réveillé un pouvoir enfoui à \n"
//                                                    "                 l'origine de ces lieux!");
//  windowUltimeItemmessage->setVisible(false);

//  windowUltimeItemRecovered = gui->addMessageBox(L"", L"Vous obtenez les légendaires lames du chaos!");
//  windowUltimeItemRecovered->setVisible(false);
  windowItemRecovered = gui->addWindow(ic::rect<s32>(DEFAULT_SCREEN_WIDTH/2 - 250, DEFAULT_SCREEN_HEIGHT/2 - 125,
                                                     DEFAULT_SCREEN_WIDTH/2 + 250, DEFAULT_SCREEN_HEIGHT/2 + 125),
                                       false, L"items recovered");
  windowItemRecovered->setDraggable(false);
  windowItemRecovered->setDrawTitlebar(false);
  iv::ITexture *fondWindowItemRecovered = device->getVideoDriver()->getTexture(L"data/menu/messagePotion.png");
  gui->addImage(fondWindowItemRecovered, ic::position2d<s32>(0, 0), true, windowItemRecovered);
  windowItemRecovered->setVisible(false);
  windowItemRecovered->setDrawBackground(false);

  windowUltimeItemmessage = gui->addWindow(ic::rect<s32>(DEFAULT_SCREEN_WIDTH/2 - 250, DEFAULT_SCREEN_HEIGHT/2 - 125,
                                                     DEFAULT_SCREEN_WIDTH/2 + 250, DEFAULT_SCREEN_HEIGHT/2 + 125),
                                       false, L"indice");
  windowUltimeItemmessage->setDraggable(false);
  windowUltimeItemmessage->setDrawTitlebar(false);
  iv::ITexture *fondWindowUltimeItemmessage = device->getVideoDriver()->getTexture(L"data/menu/messageIndice.png");
  gui->addImage(fondWindowUltimeItemmessage, ic::position2d<s32>(0, 0), true, windowUltimeItemmessage);
  windowUltimeItemmessage->setVisible(false);
  windowUltimeItemmessage->setDrawBackground(false);

  windowUltimeItemRecovered = gui->addWindow(ic::rect<s32>(DEFAULT_SCREEN_WIDTH/2 - 250, DEFAULT_SCREEN_HEIGHT/2 - 75,
                                                     DEFAULT_SCREEN_WIDTH/2 + 250, DEFAULT_SCREEN_HEIGHT/2 + 75),
                                       false, L"lames du chaos");
  windowUltimeItemRecovered->setDraggable(false);
  windowUltimeItemRecovered->setDrawTitlebar(false);
  iv::ITexture *fondWindowUltimeItemRecovered = device->getVideoDriver()->getTexture(L"data/menu/messageLamesDuChaos.png");
  gui->addImage(fondWindowUltimeItemRecovered, ic::position2d<s32>(0, 0), true, windowUltimeItemRecovered);
  windowUltimeItemRecovered->setVisible(false);
  windowUltimeItemRecovered->setDrawBackground(false);

  // ecran titre (pour commencer le jeu)
  ecranTitre = gui->addWindow(ic::rect<s32>(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT), false);
  ecranTitre->setDrawTitlebar(false);
  ecranTitre->setDraggable(false);
  iv::ITexture *name = device->getVideoDriver()->getTexture(L"data/menu/ecrantitre.png");
  iv::ITexture *start = device->getVideoDriver()->getTexture(L"data/menu/startbutton.png");
  gui->addImage(name, ic::position2d<s32>(0, 0), true, ecranTitre);
  ig::IGUIButton *startButton = gui->addButton(ic::rect<s32>(DEFAULT_SCREEN_WIDTH/2 - 150, DEFAULT_SCREEN_HEIGHT/2 - 50,
                                                             DEFAULT_SCREEN_WIDTH/2 + 150, DEFAULT_SCREEN_HEIGHT/2 + 50), ecranTitre, WINDOW_BUTTON);

  startButton->setUseAlphaChannel(true);
  startButton->setDrawBorder(false);
  startButton->setImage(start);
  startButton->setScaleImage(true);


}





/** map 3D **/
bool GameManager::addMapScene3D(is::IAnimatedMesh *mesh_bsp)
{
    if(mapScene3D.size() == 0)
    {
        scene3D *scene = new scene3D(device, mesh_bsp);

        // on ajoute une collision au joueur s'il existe
        if (getPlayer() != NULL)
        {
            scene->addCollisionToPerson(getPlayer()->node);
        }

        // on ajoute une collision aux ennemis s'ils existent
//        for (unsigned int i = 0; i < mechant.size(); i++)
        for (auto &i : enemyID)
        {
            if (getEnemy(i) != NULL)
                scene->addCollisionToPerson(getEnemy(i)->node);
        }

        // on ajoute une collision au joueur s'il existe
//        if (getCameraJeuLibre() != NULL) scene->addCollisionToCamera(getCameraJeuLibre(), ic::vector3df(30, 50, 30));

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
//        for (unsigned int i = 0; i < mechant.size(); i++)
        for (auto &i : enemyID)
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



/** objets et coffres **/
void GameManager::parametreChest(is::IAnimatedMesh *meshChest)
{
    iv::IVideoDriver  *driver = device->getVideoDriver();

    for (int k = 0; k < NB_CHEST + 1; k++)
    {
        chest[k] = smgr->addAnimatedMeshSceneNode(meshChest);
        chest[k]->setMaterialFlag(iv::EMF_LIGHTING, false);
        chest[k]->setMaterialTexture(0, driver->getTexture("data/coffre/diffuse.tga"));
    }
  //  const core::aabbox3d<f32>& boxChest = chest[0]->getBoundingBox();
  //  core::vector3df radiusChest = boxChest.MaxEdge - boxChest.getCenter();
  //  //on applique le poids et les colisions aux coffres
  //  for (int k = 0; k < 1; k++)
  //  {
  //      scene::ITriangleSelector *selectorChest = smgr->createOctreeTriangleSelector(node->getMesh(), node);
  //      node->setTriangleSelector(selectorChest);

  //      scene::ISceneNodeAnimator *animChest
  //                                = smgr->createCollisionResponseAnimator(selectorChest,
  //                                                                       chest[k],  // Le noeud que l'on veut gérer
  //                                                                       radiusChest, // "rayons" de la caméra
  //                                                                       ic::vector3df(0, -10, 0),  // gravité
  //                                                                       ic::vector3df(0, 0, 0));  // décalage du centre
  //      chest[k]->addAnimator(animChest);
  //  }
    chest[0]->setPosition(ic::vector3df(1066.48, -264, 101.24));
    items[0] = driver->getTexture("data/potions/petitePotion.png");
    idItem[0] = ITEM_1;

    chest[1]->setPosition(ic::vector3df(775.95, -8, -1767.98));
    items[1] = driver->getTexture("data/potions/petitePotion.png");
    idItem[1] = ITEM_2;

    chest[2]->setPosition(ic::vector3df(624.709, -183.975, -1358.84));
    chest[2]->setRotation(ic::vector3df(0, -44, 0));
    items[2] = driver->getTexture("data/potions/petitePotion.png");
    idItem[2] = ITEM_3;

    chest[3]->setPosition(ic::vector3df(424.272, 432.025, -966));
    chest[3]->setRotation(ic::vector3df(0, 90, 0));
    items[3] = driver->getTexture("data/potions/petitePotion.png");
    idItem[3] = ITEM_4;

    chest[4]->setPosition(ic::vector3df(-504, 568.025, 698.047));
    chest[4]->setRotation(ic::vector3df(0, 180, 0));
    items[4] = driver->getTexture("data/potions/moyennePotion.png");
    idItem[4] = ITEM_5;

    chest[5]->setPosition(ic::vector3df(263.147, 568.017, 609.818));
    chest[5]->setRotation(ic::vector3df(0, 180, 0));
    items[5] = driver->getTexture("data/potions/moyennePotion.png");
    idItem[5] = ITEM_6;

    chest[6]->setPosition(ic::vector3df(582.848, 952.02, 179.737));
    chest[6]->setRotation(ic::vector3df(0, -90, 0));
    items[6] = driver->getTexture("data/potions/grossePotion.png");
    idItem[6] = ITEM_7;

    items[7] = driver->getTexture("data/potions/lamesDuChaos.png");
    idItem[7] = ITEM_8;

}

bool GameManager::openChest(is::IAnimatedMeshSceneNode *perso, irr::ITimer *Timer)
{
    int epsilon = 125;
    for (unsigned int k = 0; k < NB_CHEST + 1; k++)
    {
        if (chest[k] != NULL && perso != NULL && chest[k]->isVisible()) // pour eviter les erreurs de segmentations
        {

                if (    (core::abs_(perso->getPosition().X - chest[k]->getPosition().X)) <= epsilon
                        &&   (core::abs_(perso->getPosition().Y - chest[k]->getPosition().Y)) <= epsilon
                        &&   (core::abs_(perso->getPosition().Z - chest[k]->getPosition().Z)) <= epsilon
                        &&   interaction )

                {
                    //actualisation de l inventaire
                            interaction = false;
                            itemWinTime = Timer->getTime();
                            int ligne = nbObjetTrouve>3;
                            if (ligne ==0)
                                itemsButton[nbObjetTrouve] = device->getGUIEnvironment()->addButton(ic::rect<s32>(nbObjetTrouve%4*(DEFAULT_SCREEN_WIDTH - 20)/4 + 10, ligne*(DEFAULT_SCREEN_HEIGHT - 100)/2 + 45,
                                                                                          (nbObjetTrouve%4 + 1)*(DEFAULT_SCREEN_WIDTH - 20)/4 - 10, (ligne + 1)*(DEFAULT_SCREEN_HEIGHT - 100)/2),
                                                                                          itemWindow, idItem[nbObjetTrouve]);
                            else
                                itemsButton[nbObjetTrouve] = device->getGUIEnvironment()->addButton(ic::rect<s32>(nbObjetTrouve%4*(DEFAULT_SCREEN_WIDTH - 20)/4 + 10, ligne*(DEFAULT_SCREEN_HEIGHT - 100)/2,
                                                                                          (nbObjetTrouve%4 + 1)*(DEFAULT_SCREEN_WIDTH - 20)/4 - 10, (ligne + 1)*(DEFAULT_SCREEN_HEIGHT - 100)/2),
                                                                                          itemWindow, idItem[nbObjetTrouve]);

                            itemsButton[nbObjetTrouve]->setUseAlphaChannel(true);itemsButton[nbObjetTrouve]->setDrawBorder(false);
                            itemsButton[nbObjetTrouve]->setImage(items[nbObjetTrouve]);itemsButton[nbObjetTrouve]->setScaleImage(true);
                            indiceItemRecovered[nbObjetTrouve] = k;
                            nbObjetTrouve++;

                            chest[k]->setVisible(false);
                            if(nbObjetTrouve == 8)
                                windowUltimeItemRecovered->setVisible(true);
                            else if(nbObjetTrouve == 7)
                                windowUltimeItemmessage->setVisible(true);
                            else
                                windowItemRecovered->setVisible(true);

                            return true;
                }
        }
    }
    return false;
}


/** mini boss **/
void GameManager::createMiniBoss()
{
    //is::IAnimatedMesh* mesh = getPlayer()->node->getMesh();
    is::IAnimatedMesh* mesh = smgr->getMesh("data/splice/splice.DAE");

    // pour resize le mesh
    is::IMeshManipulator *meshManip = smgr->getMeshManipulator();
    float fact = 0.2;
    meshManip->scaleMesh(mesh,core::vector3df(fact, fact, fact));

    for (int k=0; k<3; k++)
    {
        miniBoss[k] = smgr->addAnimatedMeshSceneNode(mesh);
        miniBoss[k]->setRotation(ic::vector3df(-90, 0, 0));
        miniBoss[k]->setMaterialFlag(iv::EMF_LIGHTING, false);
        miniBoss[k]->setMD2Animation(is::EMAT_STAND);
        miniBoss[k]->setMaterialTexture(0, device->getVideoDriver()->getTexture("data//splice/splice.png"));
    }

    ic::vector3df translation(0, -25, 0);
    miniBoss[0]->setRotation(ic::vector3df(-90, 90, 0));
    miniBoss[0]->setPosition(ic::vector3df(864.375, 721.751, 132.37) + translation);

    miniBoss[1]->setRotation(ic::vector3df(-90, 180, 0));
    miniBoss[1]->setPosition(ic::vector3df(477.741, 609.751, 394.089) + translation);

    miniBoss[2]->setPosition(ic::vector3df(1.33853, 977.751, 186.854) + translation);
}

void GameManager::isVersusMiniboss(is::IAnimatedMeshSceneNode *perso, ITimer* timer)
{
    int epsilon = 60;
    for (unsigned int k = 0; k < 3; k++)
    {
        if (miniBoss[k] != NULL && perso != NULL && miniBoss[k]->isVisible()) // pour eviter les erreurs de segmentations
        {

                if (    (core::abs_(perso->getPosition().X - miniBoss[k]->getPosition().X)) <= epsilon
                        &&   (core::abs_(perso->getPosition().Y - miniBoss[k]->getPosition().Y)) <= epsilon
                        &&   (core::abs_(perso->getPosition().Z - miniBoss[k]->getPosition().Z)) <= epsilon)

                {
                    miniBoss[k]->setVisible(false);

                    versusMiniBoss = true;
                    startCombat(timer);

                }
        }
    }
}


/** game over screen **/
void GameManager::addGameOverScreen()
{
    ig::IGUIImage *gameOverBox = device->getGUIEnvironment()->addImage(ic::rect<s32>(0, 0,  DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)); gameOverBox->setScaleImage(true);
    irr::video::ITexture *gameOverText = device->getVideoDriver()->getTexture("./data/maps/gameOver.png");
    gameOverBox->setImage(gameOverText);
}

/** Victory screen **/
void GameManager::addVictoryScreen()
{
    ig::IGUIImage *victoryBox = device->getGUIEnvironment()->addImage(ic::rect<s32>(0, 0,  DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)); victoryBox->setScaleImage(true);
    irr::video::ITexture *victoryText = device->getVideoDriver()->getTexture("./data/maps/victory.png");
    victoryBox->setImage(victoryText);
}



/** gestionnaire de jeu **/


bool GameManager::isSomeoneAtPosition(int ligne, int colonne){


//    for (unsigned int k = 0; k < mechant.size(); k++)
    for (auto &k : enemyID)
    {
        if (getEnemy(k) != NULL)
        {
            if(position(ligne, colonne) == getEnemy(k)->p ){
                // il y a un ennemi dans cette position
                return true;
            }
        }

    }


    if (getPlayer() != NULL) // pour eviter les erreurs de segmentations
    {
            if(position(ligne, colonne) == getPlayer()->p){
                // il y a un ennemi dans cette position
                return true;
            }
    }

    if (getGridMapping() != NULL) // pour eviter les erreurs de segmentations
    {

        for (unsigned int k = 0; k < getGridMapping()->barrier.pos.size(); k++)
        {
            if(getGridMapping()->barrier.pos[k] == position(ligne, colonne)){
                // il y a un obstacle a cette position
                return true;
            }
        }
    }

    return false;
}


void GameManager::executerAction(int enemyIndex, QTableAction a){
  //float attack_damage;
//  int pos_x = this->getEnemy(enemyIndex)->p.colonne; // colonne == x
//  int pos_y = this->getEnemy(enemyIndex)->p.ligne; // ligne == y
  position gridPosition(0, 0);

  if(getEnemy(enemyIndex) != NULL)  gridPosition = this->getEnemy(enemyIndex)->p;

  // faire l'action
  switch(a){
    case QUP:
          gridPosition = gridPosition + position(-1, 0);
          if(getEnemy(enemyIndex) != NULL)
          {
              getEnemy(enemyIndex)->node->setMD2Animation(is::EMAT_STAND);
              if(getGridMapping()->setEnemyCursor(enemyIndex, gridPosition))
              {
                  animator(0, 1, this->getEnemy(enemyIndex)->node);
                  this->getEnemy(enemyIndex)->p = gridPosition;
                  std::cout << "enemy up" << std::endl;
              }
          }
          break;
    case QDOWN:
          gridPosition = gridPosition + position(1, 0);
          if(getEnemy(enemyIndex) != NULL)
          {
              getEnemy(enemyIndex)->node->setMD2Animation(is::EMAT_STAND);
              if(getGridMapping()->setEnemyCursor(enemyIndex, gridPosition))
              {
                  animator(0, -1, this->getEnemy(enemyIndex)->node);
                  this->getEnemy(enemyIndex)->p = gridPosition;
                  std::cout << "enemy down" << std::endl;
              }
          }
          break;
    case QLEFT:
          gridPosition = gridPosition + position(0, -1);
          if(getEnemy(enemyIndex) != NULL)
          {
              getEnemy(enemyIndex)->node->setMD2Animation(is::EMAT_STAND);
              if(getGridMapping()->setEnemyCursor(enemyIndex, gridPosition))
              {
                  animator(-1, 0, this->getEnemy(enemyIndex)->node);
                  this->getEnemy(enemyIndex)->p = gridPosition;
                  std::cout << "enemy left" << std::endl;
              }
          }
          break;
    case QRIGHT:
          gridPosition = gridPosition + position(0, 1);
          if(getEnemy(enemyIndex) != NULL)
          {
              getEnemy(enemyIndex)->node->setMD2Animation(is::EMAT_STAND);
              if(getGridMapping()->setEnemyCursor(enemyIndex, gridPosition))
              {
                  animator(1, 0, this->getEnemy(enemyIndex)->node);
                  this->getEnemy(enemyIndex)->p = gridPosition;
                  std::cout << "enemy right" << std::endl;
              }
          }
          break;
    case QATTACK:
          if(getEnemy(enemyIndex) != NULL)
          {
              //attack_damage = this->getEnemy(enemyIndex)->getAttackForce();
              getEnemy(enemyIndex)->node->setMD2Animation(is::EMAT_CROUCH_ATTACK);
              std::cout << "enemy attack" << std::endl;
              std::cout << "player HP before attack : " << getPlayer()->HP << std::endl;
              std::cout << "player in attack zone : " << (int)getPlayer()->p.isNear(getEnemy(enemyIndex)->p) << std::endl;

              if(getPlayer()->p.isNear(getEnemy(enemyIndex)->p))
              {
                  int damage = 2;
                  if (getPlayer()->HP >=  damage)
                  {
                      getPlayer()->HP -= damage;
                  }
                  else
                  {
                      getPlayer()->HP = 0;
                  }
              }
              std::cout << "player HP after attack : " << getPlayer()->HP << std::endl;


              //          this->doDamageAroundPoint(pos_x, pos_y, attack_damage);
              //          if(this->verifyDeadLearners()){
              //            // qqn est mort par cet attaque
              //            this->learners[learnerIndex]->killedSomeone();
              //          }
          }
          break;




    default:
          std::cout << "ERROR enemy::move("<<(int)a <<") ne marche pas" << std::endl;
          exit(1);
          break;
  }
}

// a appeler dans le sceneRenderer durant le mode combat
void GameManager::startCombat(irr::ITimer *Timer)
{
    isCombat = 1;
    isPromenade = 0;
    playerTurn = true;
    ennemysTurn = false;
    endPlayerTurn = false;

    tempPlayer3DPosition = getPlayer()->node->getPosition();
    tempPlayer3DRotation = getPlayer()->node->getRotation();

    //animation debut combat
    std::vector<iv::ITexture*> fightVector = loadGif(20, L"data/animations/combat/combat", device->getVideoDriver());
    ig::IGUIImage *fightBox = device->getGUIEnvironment()->addImage(ic::rect<s32>(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
    fightBox->setScaleImage(true);
    playVideo(fightVector, 20, fightBox, device);

    removeMapScene3D();
    removeCameraJeuLibre();
    for (unsigned int k = 0; k < NB_CHEST + 1; k++)
    {
        if (chest[k] != NULL) // pour eviter les erreurs de segmentations
        {
            chest[k]->setVisible(false);
        }
    }

    for (unsigned int k = 0; k < 3; k++)
    {
        if (miniBoss[k] != NULL) // pour eviter les erreurs de segmentations
        {
            miniBoss[k]->setVisible(false);
        }
    }

    addGridMapping(DEFAULT_WIDTH, DEFAULT_HEIGHT, Timer);
    addCameraCombat();

    //////////////// if (isVersusBossFinal) on charge le boss ///////////////////

    // ajout d'ennemis
    std::vector<position> enemyPos;
    position p1(alea(1, DEFAULT_HEIGHT - 1), alea(DEFAULT_WIDTH/2, DEFAULT_WIDTH - 1));
    position p2(alea(1, DEFAULT_HEIGHT - 1), alea(DEFAULT_WIDTH/2, DEFAULT_WIDTH - 1));
    while (p1 == p2)
    {
        p2 = position(alea(1, DEFAULT_HEIGHT - 1), alea(DEFAULT_WIDTH/2, DEFAULT_WIDTH - 1));
    }

    if(isVersusBossFinal)
    {
        enemyPos.push_back(p1);
        //mesh = .... // mesh du boss final
        //textureEnemy = .... // texture du boss final
        int HpBossFinal = 50;
        this->addEnemy(enemyPos[0],
                       HpBossFinal, // HP du boss final
                       mesh, // mesh de l'ennemi
                       textureEnemy, // texture du joueur
                       getPlayer()->node->getPosition()
                       + ic::vector3df(-DEFAULT_GRID_NODE_SIZE * enemyPos[0].ligne, 0, -DEFAULT_GRID_NODE_SIZE * enemyPos[0].colonne)); // positions 3D dans le monde 3D du joueur
        getGridMapping()->addEnemy(enemyPos[0]);

    }
    else if(versusMiniBoss)
    {
        versusMiniBoss = false;
        enemyPos.push_back(p1);
        //mesh = .... // mesh du boss final
        //textureEnemy = .... // texture du boss final
        int HpBossFinal = 20;
        this->addEnemy(enemyPos[0],
                       HpBossFinal, // HP du boss final
                       mesh, // mesh de l'ennemi
                       textureEnemy, // texture du joueur
                       getPlayer()->node->getPosition()
                       + ic::vector3df(-DEFAULT_GRID_NODE_SIZE * enemyPos[0].ligne, 0, -DEFAULT_GRID_NODE_SIZE * enemyPos[0].colonne)); // positions 3D dans le monde 3D du joueur
        getGridMapping()->addEnemy(enemyPos[0]);

    }
    else
    {
        enemyPos.push_back(p1);
        enemyPos.push_back(p2);


        for (unsigned int k = 0; k < enemyPos.size(); k++)
        {
            this->addEnemy(enemyPos[k],
                           DEFAULT_ENEMY_HP, // HP de l'ennemi
                           mesh, // mesh de l'ennemi
                           textureEnemy, // texture du joueur
                           getPlayer()->node->getPosition()
                           + ic::vector3df(-DEFAULT_GRID_NODE_SIZE * enemyPos[k].ligne, 0, -DEFAULT_GRID_NODE_SIZE * enemyPos[k].colonne)); // positions 3D dans le monde 3D du joueur
            getGridMapping()->addEnemy(enemyPos[k]);
        }
    }

    for (unsigned int k = 0; k < mechant.size(); k++ )
    {
        allEnemyDead.push_back(0);
//        if(getEnemy(k) != NULL)
//        {
//            if (getEnemy(k)->HP > 0)
//                allEnemyDead.push_back(false);
//        }
    }

    // load  Q table pour les ennemis
    int num_states = getNumStates();
    this->qTable = new Q_table(num_states, NUM_ACTIONS);
    this->qTable->loadTable("q_table");
    // mettre la q table dans tous les ennemis
    //for (unsigned int k = 0; k < mechant.size(); k++)
    for (auto &k : enemyID)
    {
        if (getEnemy(k) != NULL && getPlayer() != NULL) // pour eviter les erreurs de segmentations
        {
          this->getEnemy(k)->setQTable(this->qTable);
        }
    }

}

// a appeler dans le sceneRenderer durant le mode jeu libre
void GameManager::startPromenade(irr::ITimer *Timer)
{
    isCombat = 0;
    isPromenade = 1;

    // pour reprendre la partie ou on s'est arrete
    if(getPlayer() != NULL)
    {
        getPlayer()->node->setPosition(tempPlayer3DPosition);
        getPlayer()->node->setRotation(tempPlayer3DRotation);
    }
    is::IAnimatedMeshSceneNode *perso = getPlayer()->node;

    bool isNotItem = false;
    bool isNotMiniboss = false;
    int epsilon = 60;

    if(fightMob)
    {
        for (int k = 0; k < NB_CHEST + 1; k++)
        {
            if (chest[k] != NULL) // pour eviter les erreurs de segmentations
            {
                for(int i = 0; i < nbObjetTrouve; i++)
                    if(k == indiceItemRecovered[i])
                        isNotItem = true;
                if(!isNotItem)
                    chest[k]->setVisible(true);
                isNotItem = false;


            }
        }

        for (int k = 0; k < 3; k++)
        {
            for(int i = 0; i < nbMinibosskilled; i++)
                if(k == indiceMinibossKilled[i])
                    isNotMiniboss = true;
            if(!isNotMiniboss)
                miniBoss[k]->setVisible(true);
            isNotMiniboss = false;
        }
    }

    else
    {
        for (unsigned int k = 0; k < 3; k++)
        {
            if (    (core::abs_(perso->getPosition().X - miniBoss[k]->getPosition().X)) <= epsilon
                    &&   (core::abs_(perso->getPosition().Y - miniBoss[k]->getPosition().Y)) <= epsilon
                    &&   (core::abs_(perso->getPosition().Z - miniBoss[k]->getPosition().Z)) <= epsilon)

            {
                indiceMinibossKilled[nbMinibosskilled] = k;
                nbMinibosskilled++;

                //on entre en collision avec un miniboss
                ////combat loop avec  miniboss, si on gagne on obtien une des 3 cles pour le boss ultime
                for (int k = 0; k < NB_CHEST + 1; k++)
                {
                    if (chest[k] != NULL) // pour eviter les erreurs de segmentations
                    {
                        for(int i = 0; i < nbObjetTrouve; i++)
                            if(k == indiceItemRecovered[i])
                                isNotItem = true;
                        if(!isNotItem)
                            chest[k]->setVisible(true);
                        isNotItem = false;


                    }
                }

                for (int k = 0; k < 3; k++)
                {
                    for(int i = 0; i < nbMinibosskilled; i++)
                        if(k == indiceMinibossKilled[i])
                            isNotMiniboss = true;
                    if(!isNotMiniboss)
                        miniBoss[k]->setVisible(true);
                    isNotMiniboss = false;
                }
                cle++;

            }
        }
    }

    removeGridMapping();
    removeCameraCombat();
    addMapScene3D(mesh_bsp);
    addCameraJeuLibre();
}

void GameManager::loopPromenade(irr::ITimer *Timer){
  // est appellee en loop dans le mode jeu libre

    irr::core::vector3d<float> pos = getPlayer()->node->getPosition();
    irr::core::vector3d<float> rotationperso = getPlayer()->node->getRotation();
    pos.Y += 25;
    pos.X += 28 * cos(rotationperso.Y * M_PI / 180.0);
    pos.Z += -28 * sin(rotationperso.Y * M_PI / 180.0);

    getCameraJeuLibre()->setPosition(pos);

    irr::core::vector3df rotationCamera = getCameraJeuLibre()->getRotation();
    rotationperso.Y = rotationCamera.Y - 90;

    getPlayer()->node->setRotation(rotationperso);

    if ((float)getPlayer()->HP + gainHp*(float)(DEFAULT_PLAYER_HP - getPlayer()->HP) -
            ((int)(getPlayer()->HP + (float)gainHp*(DEFAULT_PLAYER_HP - getPlayer()->HP)))!=0.0f)
        getPlayer()->HP += 1 + gainHp*(float)(DEFAULT_PLAYER_HP - getPlayer()->HP);
    else
        getPlayer()->HP += gainHp*(float)(DEFAULT_PLAYER_HP - getPlayer()->HP);

    gainHp = 0;

    isOpenedChest = openChest(getPlayer()->node,Timer);


    if(nbObjetTrouve == 7)
    {
        chest[7]->setPosition(ic::vector3df(1287.93, 308.025, -1309.74));
    }

    if(isOpenedChest)
    {
        isWaiting = true;
    }

    if(isWaiting)
        if((Timer->getTime() - itemWinTime>7000) || clicSouris)
        {
            isWaiting = false;
            //selon les differentes situations, on affiche un message différent
            if(nbObjetTrouve == 8)
                windowUltimeItemRecovered->setVisible(false);
            else if(nbObjetTrouve == 7)
                windowUltimeItemmessage->setVisible(false);
            else
                windowItemRecovered->setVisible(false);

        }
    clicSouris = false;


    //on teste si on est en collision avec un mini boss
    isVersusMiniboss(getPlayer()->node,Timer);

    if (cle == 3)
    {
        // si le joueur est devant la porte du boss et appuie sur M
        if(getPlayer()->node->getPosition().X <= -1668.46 && getPlayer()->node->getPosition().X >=-1745.07
           && getPlayer()->node->getPosition().Y <= 21 && getPlayer()->node->getPosition().Y >= 20
           && getPlayer()->node->getPosition().Z <= -172.217 && getPlayer()->node->getPosition().Z >=-283.837
           && interaction)
        {
            isVersusBossFinal = true;
            startCombat(Timer);
        }
    }

    // combat aleatoire

    float probaFight = 0.0001;
    float randNum = (float)rand() / (float)RAND_MAX;

    if ( randNum < probaFight && onMvt)
    {
        fightMob = true;
        isCombat = 1; isPromenade = 0;
        startCombat(Timer);
    }

    //    ic::vector3df unePosition(967.692, 337.751, -1187.54);

    //    if (getPlayer() != NULL) // pour eviter les erreurs de segmentations
    //    {
    //        if (    (core::abs_(getPlayer()->node->getPosition().X - unePosition.X)) <= this->epsilon
    //                &&   (core::abs_(getPlayer()->node->getPosition().Y - unePosition.Y)) <= this->epsilon
    //                &&   (core::abs_(getPlayer()->node->getPosition().Z - unePosition.Z)) <= this->epsilon  )

    //        {
    //            isCombat = 1; isPromenade = 0;
    //            startCombat(Timer);
    //        }
    //    }




}

void GameManager::loopCombat(irr::ITimer *Timer){
  // est appellee en loop dans le mode combat


    if (getEnemy(0) != NULL) std::cout << "isNear[0] : " << (int)getPlayer()->p.isNear(getEnemy(0)->p) << std::endl;
    if (getEnemy(1) != NULL) std::cout << "isNear[1] : " << (int)getPlayer()->p.isNear(getEnemy(1)->p) << std::endl;

  // pour la prise de decision des ennemis / q table
  int dist_x_pers, dist_y_pers;
  float hp_pers;
  QTableAction a;


  // on vire l'ennemi s'il meurt
  //for (unsigned int k = 0; k < mechant.size(); k++)
  for (auto &k : enemyID)
  {
      if (getEnemy(k) != NULL)
      {
          if(getEnemy(k)->HP <= 0)
          {

              getEnemy(k)->node->setRotation(core::vector3df(90, 0, 90));
              if (getPlayer() != NULL) getEnemy(k)->node->setPosition(core::vector3df(getEnemy(k)->node->getPosition().X,
                                                                                         getPlayer()->node->getPosition().Y - 25,
                                                                                         getEnemy(k)->node->getPosition().Z));


              allEnemyDead[k] = 1;
          }

      }
  }


  // on sort du mode combat s'il n'y a plus d'ennemis

  bool finCombat = false;
  int ctrl = std::accumulate(allEnemyDead.begin(), allEnemyDead.end(), 0);
  if (ctrl == (int)allEnemyDead.size())
  {
      finCombat = true;
  }

  if(finCombat)
  {
      for (auto &k : enemyID)
      {
          if (getEnemy(k) != NULL)
          {
              removeEnemy(k);
          }
      }

      allEnemyDead.erase(allEnemyDead.begin(), allEnemyDead.end());
      isCombat = 0; isPromenade = 1;
      if (isVersusBossFinal)
      {
          if(getPlayer() != NULL)
          {
              removeGridMapping();
              removeCameraCombat();
              removeMapScene3D();
              removeCameraJeuLibre();

              for (auto &k : enemyID)
              {
                  if (getEnemy(k) != NULL)
                  {
                      getEnemy(k)->node->remove();
                      //removeEnemy(k);
                  }
              }
              addVictoryScreen();
          }
      }

      startPromenade(Timer);
  }



  // on affiche un game over si le joueur meurt
  if(getPlayer() != NULL)
  {
      if(getPlayer()->HP <= 0)
      {
          removeGridMapping();
          removeCameraCombat();
          removeMapScene3D();
          removeCameraJeuLibre();

          for (auto &k : enemyID)
          {
              if (getEnemy(k) != NULL)
              {
                  getEnemy(k)->node->remove();
                  //removeEnemy(k);
              }
          }
          addGameOverScreen();
      }


  }


  // DEBUG :
//  if(getPlayer()->p == position(0, 3))
//  {

//      for (auto &k : enemyID)
//      {
//          if (getEnemy(k) != NULL)
//          {
//              removeEnemy(k);
//          }
//      }

//      isCombat = 0; isPromenade = 1;
//      startPromenade(Timer);
//  }



  // tour des Ennemi : choisir une action
  if(this->ennemysTurn && !this->playerTurn)
  {
      std::cout << "enemys turn" << std::endl;

      device->sleep(1000);

      //for (unsigned int k = 0; k < mechant.size(); k++)
      for (auto &k : enemyID)
      {

          if (getEnemy(k) != NULL && getPlayer() != NULL) // pour eviter les erreurs de segmentations
          {
              if (getEnemy(k)->HP > 0)
              {
                  // get informations pour choisir l'action
                  dist_x_pers = this->getPlayer()->p.ligne - this->getEnemy(k)->p.ligne;
                  dist_y_pers = this->getPlayer()->p.colonne - this->getEnemy(k)->p.colonne;
                  hp_pers = this->getPlayer()->HP;

                  std::cout << "DEBUG dist_x_pers : " <<(int)dist_x_pers<< "dist_y_pers : "<<(int)dist_y_pers<< '\n';

                  // choisir l'action de l'ennemi
                  a = this->getEnemy(k)->chooseAction(dist_x_pers, dist_y_pers, hp_pers);

                  std::cout << "DEBUG combat action(" << k << ") " << (int)a << '\n';

                  // executer l'action:
                  this->executerAction(k, a);

              }
              else
              {
                  getEnemy(k)->node->setMD2Animation(is::EMAT_STAND);
              }
          }
      }
      // fin du tour des ennemis
      playerTurn = true;
      ennemysTurn = false;
      // pour reset la position du curseur
      getGridMapping()->mouvementGridPlayer(RESET); // ne marche pas
  }


    // tour du player : attendre une action du player
    if(this->playerTurn && !this->ennemysTurn)
    {
        // ?? TODO
        if(endPlayerTurn)
        {
            // fin du tour du player
            playerTurn = false;
            ennemysTurn = true;
            endPlayerTurn = false;
        }
    }
}

void GameManager::sceneRenderer(irr::ITimer *Timer)
{

    /** variables utiles pour plus tard **/
    iv::IVideoDriver  *driver = device->getVideoDriver();
    ig::IGUIEnvironment *gui  = device->getGUIEnvironment();

    /** chargement de la map **/
    mesh_bsp = smgr->getMesh("cf.bsp");

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

    hpBox->setVisible(false);


    /** conditions initiales avant le debut de la partie **/
    isCombat = 0; isPromenade = 0;




    while(device->run())
    {


        device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0)); // fond noir
        smgr->drawAll();
        gui->drawAll();

//        std::cout << "startGame : " << (int)startGame << std::endl;
//        std::cout << "isCombat : " << (int)isCombat << std::endl;
//        std::cout << "isPromenade : " << (int)isPromenade << std::endl;


        if (startGame)
        {


            /** initialisation de la partie quand on appuie sur start **/
            if (!isCombat && !isPromenade)
            {
                ecranTitre->remove();
                hpBox->setVisible(true);
                isCombat = 0; isPromenade = 1;
                startPromenade(Timer);
            }

            if(getPlayer() != NULL) std::cout << "positions Joueur : x = " << getPlayer()->node->getPosition().X
                                              << ", " << getPlayer()->node->getPosition().Y
                                              << ", " << getPlayer()->node->getPosition().Z
                                              << std::endl;


            // debut de jeu qui passe du mode jeu libre au mode combat lorsque le joueur se trouve aux memes coordonnees que l'ennemi[k]
            // le joueur perd la moitie de sa vie lorsqu'il valide sa position a la case [2, 5] de la gridMapping
            // le jeu rebascule en mode jeu libre lorsqu'on valide la position du joueur a la case [0, 3] de la gridMapping
            //this->epsilon = 10;

            // durant le mode jeu libre
            if ( !isCombat && isPromenade)
            {
                this->loopPromenade(Timer);
            }
            // durant le mode combat
            if ( isCombat && !isPromenade)
            {
                this->loopCombat(Timer);
                if(getPlayer() != NULL) std::cout << "HP Joueur : " << getPlayer()->HP << std::endl;
                if(getEnemy(0) != NULL) std::cout << "HP ennemi[0] : " << getEnemy(0)->HP << std::endl;
                if(getEnemy(1) != NULL) std::cout << "HP ennemi[1] : " << getEnemy(1)->HP << std::endl;

            }

            /** DO NOT EDIT **/

            // pour que les cameras aient la bonne target
//            if (getCameraJeuLibre() != NULL)
//                getCameraJeuLibre()->setTarget(getPlayer()->node->getPosition());

            // faire clignoter le curseur
            if (getGridMapping() != NULL)
                getGridMapping()->makeCurseurBlink(true);

            // actualise la barre de vie en fonction du nombre de HP restant du joueur
            if (getPlayer() != NULL)
                hpBox->setImage(hpVector[getPlayer()->HP]);
            /** *********** **/

        }



        device->getVideoDriver()->endScene();




    }


}
