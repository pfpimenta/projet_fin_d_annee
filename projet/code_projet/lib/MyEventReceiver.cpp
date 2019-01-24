#include "MyEventReceiver.hpp"
#include "gui_ids.h"


// constructeur
MyEventReceiver::MyEventReceiver()
    :button_pressed(false)
{}



bool MyEventReceiver::OnEvent(const irr::SEvent &event)
  {
    // Press esc key to exit ...
    if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown && event.KeyInput.Key == KEY_ESCAPE)
        exit(0);



    /********** Clavier et souris ***********/

    /** A l'ecran titre **/
    if (!gmngr->isPromenade && !gmngr->isCombat)
    {
        switch (event.EventType)
        {
            case irr::EET_GUI_EVENT:
              return gui_manage(event);
            default:;
        }
    }

    /** Durant le mode combat **/
    if (gmngr->isCombat && !gmngr->isPromenade)
    {
        if(!gmngr->getGridMapping()) return false;
        switch (event.EventType)
        {
            case irr::EET_KEY_INPUT_EVENT:
                return keyboard_combat(event);
            case irr::EET_MOUSE_INPUT_EVENT:
                return mouse_combat(event);
            case irr::EET_GUI_EVENT:
              return gui_manage(event);
            default:;
        }
    }


    /** Durant le mode jeu libre **/
    if (gmngr->isPromenade && !gmngr->isCombat)
    {
        switch (event.EventType)
        {
            case irr::EET_KEY_INPUT_EVENT:
                return keyboard_promenade(event);
            case irr::EET_MOUSE_INPUT_EVENT:
                return mouse_promenade(event);
            case irr::EET_GUI_EVENT:
              return gui_manage(event);
            default:;
        }
    }


    return false;
  }




/** gestion mode combat **/

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard_combat                                         *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::keyboard_combat(const irr::SEvent &event)
{
        if (event.EventType == EET_KEY_INPUT_EVENT ) // event de type clavier
        {
            if(event.KeyInput.PressedDown) // touche appuyee
            {
                switch (event.KeyInput.Key)
                {
                case KEY_ESCAPE: // echap
                    exit(0);


                case KEY_KEY_Z: // haut
                    act = UP;
                    break;
                case KEY_KEY_S: // bas
                    act = DOWN;
                    break;
                case KEY_KEY_D: // droite
                    act = RIGHT;
                    break;
                case KEY_KEY_Q: // gauche
                    act = LEFT;
                    break;
                case KEY_SPACE: // attack
                    act = ATTACK;
                    break;

                case KEY_KEY_M: // validate
                    act = VALIDATE;
                    gmngr->endPlayerTurn = true;
                    break;
                case KEY_KEY_A: // reset
                    act = RESET;
                    break;


                case KEY_KEY_W: // pour tester les nouvelles features
                    act = DEBUG;
                    break;
                default:;
                }
            }
            else if(!event.KeyInput.PressedDown) // touche relachee
            {
                act = NOTHING;
            }
        }

        else if (event.EventType != EET_KEY_INPUT_EVENT) // si on n'appuie pas sur le clavier
            act = NOTHING;


        bool voieLibre = gmngr->getGridMapping()->mouvementGridPlayer(act);
        gmngr->animPlayer(voieLibre, act);



    return false;
}



/*------------------------------------------------------------------------*\
 * EventReceiver::mouse_combat                                            *
\*------------------------------------------------------------------------*/

// WANTED :
// * Faire tourner la camera tout en conservant la target. Et revenir a la position par defaut
//   quand on lache le clic gauche
//
// * zoomer et dezoomer avec la molette
//
// * revenir a la position par defaut avec clic droit

bool MyEventReceiver::mouse_combat(const irr::SEvent &event)
{
    switch(event.MouseInput.Event)
    {
      case irr::EMIE_RMOUSE_PRESSED_DOWN:
        if(gmngr->getCameraCombat() != NULL && gmngr->getPlayer() != NULL)
        {
            if (gmngr->getGridMapping() != NULL)
            {
                irr::core::vector3df camCombatPosition(0,
                                                       gmngr->getGridMapping()->myGrid->getGridNode(0)->getPosition().Y + std::max(DEFAULT_HEIGHT, DEFAULT_WIDTH) * DEFAULT_GRID_NODE_SIZE * 0.5,
                                                       0);

                irr::core::vector3df translationCamCombat(gmngr->getGridMapping()->myGrid->getGridNode(0)->getPosition().X - 40 * DEFAULT_HEIGHT,
                                                          0,
                                                          (gmngr->getGridMapping()->myGrid->getGridNode(0)->getPosition().Z - DEFAULT_GRID_NODE_SIZE * DEFAULT_WIDTH/2));

                gmngr->getCameraCombat()->setPosition(camCombatPosition + translationCamCombat);
            }
        }
        break;
      case irr::EMIE_LMOUSE_PRESSED_DOWN:
        button_pressed = true;
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
        break;
      case irr::EMIE_LMOUSE_LEFT_UP:
        button_pressed = false;
        break;
      case irr::EMIE_MOUSE_MOVED:
        if (button_pressed)
        {
            if(gmngr->getCameraCombat() != NULL)
            {
                irr::core::vector3df position = gmngr->getCameraCombat()->getPosition();
                position.Z += event.MouseInput.X - old_x;
                position.Y += (event.MouseInput.Y - old_y);
                old_x = event.MouseInput.X;
                old_y = event.MouseInput.Y;
                gmngr->getCameraCombat()->setPosition(position);
            }
        }
        break;
      case irr::EMIE_MOUSE_WHEEL:
        if(gmngr->getCameraCombat() != NULL)
        {
            gmngr->getCameraCombat()->setPosition(core::vector3df(gmngr->getCameraCombat()->getPosition().X + 10 * event.MouseInput.Wheel,
                                                                  gmngr->getCameraCombat()->getPosition().Y,
                                                                  gmngr->getCameraCombat()->getPosition().Z));
        }
        break;
      default:
        ;
  }
  return false;
}



/** gestion mode jeu libre **/

bool MyEventReceiver::keyboard_promenade(const irr::SEvent &event)
{
    static int isMoving = 0;
    bool enabled;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(event.KeyInput.PressedDown)
        {
            irr::core::vector3df position = gmngr->getPlayer()->node->getPosition();
            irr::core::vector3df rotation = gmngr->getPlayer()->node->getRotation();
            switch (event.KeyInput.Key)
            {
              case irr::KEY_ESCAPE:
                exit(0);
              case irr::KEY_KEY_Z:
                position.X += 1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += -1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case irr::KEY_KEY_S:
                position.X += -1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += 1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case KEY_KEY_M: // validate
                gmngr->interaction = true;
                break;
              case irr::KEY_KEY_A:
                speed = (float)(((int)speed )%18) + 3;
                std::cout << "speed = " << speed << std::endl;
                break;
            case irr::KEY_KEY_I:
                gmngr->itemWindow->setVisible(isOpened);
                isOpened = !isOpened;

                enabled = gmngr->getCameraJeuLibre()->isInputReceiverEnabled();
                gmngr->getCameraJeuLibre()->setInputReceiverEnabled(!enabled);
                gmngr->device->getCursorControl()->setVisible(enabled);
                break;
            case irr::KEY_KEY_X:
                enabled = gmngr->getCameraJeuLibre()->isInputReceiverEnabled();
                gmngr->getCameraJeuLibre()->setInputReceiverEnabled(!enabled);
                gmngr->device->getCursorControl()->setVisible(enabled);
                break;
              default:
                isMoving = 0;
            }
            gmngr->getPlayer()->node->setPosition(position);
//            gmngr->getPlayer()->node->setRotation(rotation);
        }

        else if(!event.KeyInput.PressedDown)
        {
            isMoving = 0;
            gmngr->interaction = false;
        }

        if (isMoving == 1)
          gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_RUN);
        if (isMoving == 0)
           gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_STAND);
    }


  return false;
}



/*------------------------------------------------------------------------*\
 * EventReceiver::mouse en promenade                                      *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::mouse_promenade(const irr::SEvent &event)
{
  if (!gmngr->getPlayer()->node) return false;
  switch(event.MouseInput.Event)
  {
    case irr::EMIE_LMOUSE_PRESSED_DOWN:
      button_pressed = true;
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      break;
    case irr::EMIE_LMOUSE_LEFT_UP:
      button_pressed = false;
      gmngr->clicSouris = true;
      break;
    case irr::EMIE_MOUSE_MOVED:
      if (button_pressed)
      {
        irr::core::vector3df rotation = gmngr->getPlayer()->node->getRotation();
        rotation.Y -= (event.MouseInput.X - old_x);
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
        gmngr->getPlayer()->node->setRotation(rotation);
      }
      break;
    default:;
  }

  return false;
}


/*------------------------------------------------------------------------*\
 * fenetre et menu                                                        *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::gui_manage(const irr::SEvent &event)
{
  ig::IGUIContextMenu *menu;
  irr::s32 item;
  irr::s32 id;
  irr::s32 id2;

  switch(event.GUIEvent.EventType)
  {
    // Gestion des menus de la barre de menu
    case ig::EGET_MENU_ITEM_SELECTED:

        menu = (ig::IGUIContextMenu*)event.GUIEvent.Caller;
        item = menu->getSelectedItem();
        id = menu->getItemCommandId(item);

        switch(id)
        {
            case MENU_NEW_GAME:
                // Faire quelque chose ici !
                break;
            case MENU_QUIT:
                exit(0);
            case MENU_COMMANDES:
                gmngr->device->getGUIEnvironment()->addMessageBox(L"Commandes", L"->Mode Jeu libre : "
                                                                                "\n  - Déplacements : Z, S et souris"
                                                                                "\n  - Ouverture du menu des items : I"
                                                                                "\n  - Interaction avec un objet : M"
                                                                                "\n  - Activer/désactiver la caméra fps : X"
                                                                                "\n\n->Mode Combat : "
                                                                                "\n  - Déplacements du perso : Z, S, Q, D"
                                                                                "\n  - Déplacements de la caméra : souris"
                                                                                "\n  - Valider sa position : M"
                                                                                "\n  - Attaquer et passer son tour : ESPACE");
            break;
            case ABOUT:
                gmngr->device->getGUIEnvironment()->addMessageBox(L"A propos", L"Projet de Majeure réalisé par :"
                                                                               " \n FOLETTO PIMENTA Pédro,"
                                                                               " \n KUASSIVI Cédric,"
                                                                               " \n TOSTI Dylan");
                break;
            default:;
        }

        break;


      case ig::EGET_BUTTON_CLICKED:

          id2 = event.GUIEvent.Caller->getID();

          switch(id2)
          {
              case WINDOW_BUTTON:
                  gmngr->startGame = true;
                  //std::cout << "ok" << std::endl;
                  break;

              case ITEM_1:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.4;
                  break;
              case ITEM_2:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.4;
                  break;
              case ITEM_3:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.4;
                  break;
              case ITEM_4:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.4;
                  break;
              case ITEM_5:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.65;
                  break;
              case ITEM_6:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.65;
                  break;
              case ITEM_7:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  gmngr->gainHp = 0.90;
                  break;
              case ITEM_8:
                  gmngr->itemWindow->getElementFromId(id2)->setVisible(false);
                  break;
              default:;
          }

          break;

        default:;

  }
  return false;
}

