/**
 * Author : Cédric KUASSIVI
 *
 * MyEventReceiver.cpp :
 *
 * Il s'agit de la classe principale de gestion des events (appuie sur touches du clavier, etc...)
 *
**/

#include "MyEventReceiver.hpp"



bool MyEventReceiver::OnEvent(const irr::SEvent &event)
  {



    /********** Clavier et souris ***********/
    //if (!grid ) return false;

    bool isCombat = true; // par la suite il s'agira d'un attribut qu'on mettra a 1 quand le combat commence
    if (isCombat)
    {
        switch (event.EventType)
        {
        case irr::EET_KEY_INPUT_EVENT:
            return keyboard_combat(event);
        case irr::EET_MOUSE_INPUT_EVENT:
            return mouse(event);
        default:;
        }
    }




    return false;
  }




bool MyEventReceiver::keyboard_combat(const irr::SEvent &event)
{


    /** Clavier **/
    if (event.EventType == EET_KEY_INPUT_EVENT) // event de type clavier
    {
        if(event.KeyInput.PressedDown) // touche appuyee
        {
            switch (event.KeyInput.Key)
            {
                case KEY_ESCAPE: // echap
                  exit(0);
                case KEY_KEY_Z: // haut
                  act = HAUT;
                  break;
                case KEY_KEY_S: // bas
                  act = BAS;
                  break;
                case KEY_KEY_D: // droite
                  act = DROITE;
                  break;
                case KEY_KEY_Q: // gauche
                  act = GAUCHE;
                  break;
                case KEY_KEY_A: // reset
                  act = RESET;
                  break;
                case KEY_KEY_M: // validate
                  act = VALIDATE;
                  break;
                case KEY_KEY_W: // pour tester les nouvelles features
                  //act = DEBUG;
                  gmngr->step();
                  std::cout << "ok" << std::endl;
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





    grid->mouvement(act); // le mouvement correspondant est realise


  return false;
}







/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool MyEventReceiver::mouse(const irr::SEvent &event)
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
        irr::core::vector3df rotation = cam1->getRotation();
//        irr::core::vector3df rotation = grid->myGrid->getGridNode(0)->getRotation();
        rotation.Y -= (event.MouseInput.X - old_x);
        old_x = event.MouseInput.X;
        old_y = event.MouseInput.Y;
//        grid->myGrid->getGridNode(0)->setRotation(rotation);
        cam1->setRotation(rotation);
      }
      break;
    default:
      ;
  }

  return false;
}





MyEventReceiver::MyEventReceiver():
    perso1(nullptr), button_pressed(false)
{}




