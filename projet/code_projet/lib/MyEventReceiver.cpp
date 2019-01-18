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


    if (event.EventType == EET_KEY_INPUT_EVENT) // event de type clavier
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
        gmngr->playAnimation(voieLibre, act, gmngr->getPlayer()->node);



    return false;
}



/*------------------------------------------------------------------------*\
 * EventReceiver::mouse_combat                                            *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::mouse_combat(const irr::SEvent &event)
{
    switch(event.MouseInput.Event)
    {
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
          irr::core::vector3df rotation = gmngr->getPlayer()->node->getRotation();
          rotation.Y -= (event.MouseInput.X - old_x);
          old_x = event.MouseInput.X;
          old_y = event.MouseInput.Y;
          gmngr->getPlayer()->node->setRotation(rotation);
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
    static int hasJumped = 0;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if(event.KeyInput.PressedDown && event.KeyInput.Key == irr::KEY_SPACE)
            hasJumped += 1;
        else if (!event.KeyInput.PressedDown || event.KeyInput.Key == irr::KEY_SPACE)
            hasJumped = 0;


        if(event.KeyInput.PressedDown)
        {
            irr::core::vector3df position = gmngr->getPlayer()->node->getPosition();
            irr::core::vector3df rotation = gmngr->getPlayer()->node->getRotation();
            switch (event.KeyInput.Key)
            {
              case irr::KEY_ESCAPE:
                exit(0);
              case irr::KEY_KEY_Z: // Avance
                position.X += 1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += -1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case irr::KEY_KEY_S: // Recule
                position.X += -1 * cos(rotation.Y * M_PI / 180.0) * speed;
                position.Z += 1 * sin(rotation.Y * M_PI / 180.0) * speed;
                isMoving += 1;
                break;
              case irr::KEY_KEY_D: // Tourne à droite
                rotation.Y += 10;
                isMoving += 1;
                break;
              case irr::KEY_KEY_Q: // Tourne à gauche
                rotation.Y -= 10;
                isMoving += 1;
                break;
              case irr::KEY_KEY_A: // Tourne à gauche
                speed = (float)(((int)speed )%18) + 3;
                std::cout << "speed = " << speed << std::endl;
                break;
            case irr::KEY_KEY_I: // Tourne à gauche
                gmngr->window->setVisible(isOpened);
                isOpened = !isOpened;
                break;
              default:
                isMoving = 0;
            }
            gmngr->getPlayer()->node->setPosition(position);
            gmngr->getPlayer()->node->setRotation(rotation);
        }

        else if(!event.KeyInput.PressedDown)
            isMoving = 0;

        if (isMoving == 1)
          gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_RUN);
        if (isMoving == 0)
           gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_STAND);
        if (hasJumped == 1)
          gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_JUMP);

    }


  return false;
}



/*------------------------------------------------------------------------*\
 * EventReceiver::mouse en promenade                                      *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::mouse_promenade(const irr::SEvent &event)
{
  switch(event.MouseInput.Event)
  {
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
  if (!map3DNode) return false;

  ig::IGUIContextMenu *menu;
  irr::s32 item;
  irr::s32 id;

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
                gmngr->device->getGUIEnvironment()->addMessageBox(L"Commandes", L"En exploration, ....");
                break;
            default:;
        }

        break;

    default:;

  }
  return false;
}



/**************************************************************************\
 * MyEventReceiver::set_map3DNode                                         *
\**************************************************************************/
void MyEventReceiver::set_map3DNode(irr::scene::ISceneNode *n)
{
  map3DNode = n;
}

