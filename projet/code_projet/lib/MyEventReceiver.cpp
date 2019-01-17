#include "MyEventReceiver.hpp"



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

    if (gmngr->isCombat)
    {
        switch (event.EventType)
        {
        case irr::EET_KEY_INPUT_EVENT:
            return keyboard_combat(event);
        case irr::EET_MOUSE_INPUT_EVENT:
            return mouse_combat(event);
        default:;
        }
    }


    if (gmngr->isPromenade)
    {
        switch (event.EventType)
        {
        case irr::EET_KEY_INPUT_EVENT:
        default:;
        }
    }


    return false;
  }





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



