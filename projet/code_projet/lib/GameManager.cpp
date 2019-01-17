#include <iostream>

#include "GameManager.hpp"

GameManager::GameManager(irr::IrrlichtDevice *device)
    :device(device), isCombat(1), isPromenade(0)
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
        std::cout << "... GameManager::getPlayer() : Le joueur a ete bien recupere ..." << std::endl;
        return j1[0];
    }
    std::cout << "... GameManager::getPlayer() : Aucun joueur present ! Vous avez recupere un pointeur NULL ! ..." << std::endl;
    return NULL;
}


void GameManager::playAnimation(Action act, scene::IAnimatedMeshSceneNode *perso)
{
    switch (act) {
    case VALIDATE:
        animator( 1 /*int nombreCasesHorizontales*/, -2 /*int nombreCasesVerticales*/, perso);
        break;
    case NOTHING:
        if (device->getTimer()->getTime() - this->currentAnimationTime >= DEFAULT_DUREE_ANIMATION)
            perso->setMD2Animation(is::EMAT_STAND);
        break;
    default:
        if (device->getTimer()->getTime() - this->currentAnimationTime >= DEFAULT_DUREE_ANIMATION)
            perso->setMD2Animation(is::EMAT_STAND);
        break;
    }
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
                          << "      la fonction GameManager::removeEnemy(id) permet de le retirer proprement de la scene..." << std::endl;                return 1;
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
            std::cout << "... GameManager::getEnemy() : l'ennemi[" << id << "] a ete bien recupere ..." << std::endl;
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
            g->myGrid->translation(getPlayer()->node->getPosition() - core::vector3df(5, 25, 5));
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






/** gestionnaire de jeu **/

void GameManager::combat(irr::ITimer *Timer)
{
    addGridMapping(DEFAULT_WIDTH, DEFAULT_HEIGHT, Timer);
}









void GameManager::promenade(irr::ITimer *Timer)
{}






void GameManager::sceneRenderer(irr::ITimer *Timer)
{
    if (isCombat && !isPromenade)
        combat(Timer);
    if (isPromenade && !isCombat)
        combat(Timer);
    else
        std::cout << "GameManager::sceneRenderer() : Il y a un gros probleme ! ce cas de figure ne devrait pas etre possible" << std::endl;
}
