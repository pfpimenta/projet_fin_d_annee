//#include "MyJoystickManager.hpp"


//void MyJoystickManager::animPersoNCam(float xMoveP, float yMoveP, float xMoveC, float yMoveC)
//{
//  MyJoystickManager::animPerso(xMoveP, yMoveP);
//  MyJoystickManager::animCam(xMoveC, yMoveC);
//}



//void MyJoystickManager::animPerso(float xMove, float yMove)
//{

//    irr::core::vector3df position = gmngr->getPlayer()->node->getPosition();
//    irr::core::vector3df rotation = gmngr->getPlayer()->node->getRotation();

//    irr::core::vector3df positionC = gmngr->getCameraJeuLibre()->getTarget();
//    //positionC.normalize();


//    irr::core::vector2df pC(positionC.X, positionC.Z); pC.normalize();
//    irr::core::vector2df p(position.X, position.Z); p.normalize();

//    float anglePC = 180 * acos(pC.X * p.X + pC.Y * p.Y) / M_PI;

//    if (xMove < 0)
//      anglePC *= -1;

//    float eps = 1.0;
//    if (irr::core::equals((float)abs(anglePC - eps), 0.f))
//    {
//      xMove = cos(anglePC * M_PI / 180);
//      yMove = sin(anglePC * M_PI / 180);
//    }

//    std::cout << pC.X << "  " << pC.Y << std::endl;
//    std::cout << p.X << "  " << p.Y << std::endl;



//    std::cout << anglePC << std::endl;


//    float angle = 180 * atan2(yMove, xMove) / M_PI;
//    rotation.Y = 180-angle;

//    position.X -= speedPerso * xMove ;
//    position.Z -= speedPerso * yMove ;

//    // std::cout << position.X << "  " << position.Z << std::endl;



//    gmngr->getPlayer()->node->setRotation(rotation);
//    gmngr->getPlayer()->node->setPosition(position);


//}



//void MyJoystickManager::animCam(float xMove, float yMove)
//{
//    irr::core::vector3df centre = gmngr->getPlayer()->node->getPosition();
//    irr::core::vector3df centreR;
//    centreR = gmngr->getPlayer()->node->getRotation();

//    irr::core::vector3df position = gmngr->getCameraJeuLibre()->getPosition();
//    irr::core::vector3df positionRecup = gmngr->getCameraJeuLibre()->getPosition();



//    float dtheta = 2.5; //en degres (°)

//    float anglex = dtheta * xMove;
//    position.rotateXZBy(anglex, centre);
//    // position.rotateXZBy(anglex, irr::core::vector3df(0, 1, 0));


//    float angley = dtheta * yMove;
//    position.rotateXYBy(angley, centre);
//    // position.rotateXYBy(angley, irr::core::vector3df(0, 0, 1));

//    centreR.Y = anglex;



//    if( !irr::core::equals(xMove, 0.f) ||  !irr::core::equals(yMove, 0.f))
//      {
//        gmngr->getCameraJeuLibre()->setPosition(position);
//        gmngr->getCameraJeuLibre()->setTarget(centre);
//        //perso->setRotation(centreR);
//      }

//    if( !(!irr::core::equals(xMove, 0.f) ||  !irr::core::equals(yMove, 0.f)) )
//      {
//        gmngr->getCameraJeuLibre()->setPosition(positionRecup);
//        gmngr->getCameraJeuLibre()->setTarget(centre);
//        //perso->setRotation(centreR);

//      }

//}











//void MyJoystickManager::xyMoves(const irr::SEvent &event,
//                                        irr::f32 &xMoveP,
//                                        irr::f32 &yMoveP,
//                                        irr::f32 &xMoveC,
//                                        irr::f32 &yMoveC)
//{

//  JoystickState = event.JoystickEvent;

//  const irr::SEvent::SJoystickEvent & joystickData = GetJoystickState();

//  const irr::f32 DEAD_ZONE = 0.05f;

//  //mouvement perso
//  xMoveP =
//      (irr::f32)joystickData.Axis[0] / 32767.f;
//  if(fabs(xMoveP) < DEAD_ZONE)
//      xMoveP = 0.f;

//  yMoveP =
//      (irr::f32)joystickData.Axis[1] / -32767.f;
//  if(fabs(yMoveP) < DEAD_ZONE)
//      yMoveP = 0.f;


//  //mouvement camera

//  // manette PS4 V1

//  xMoveC =
//      (irr::f32)joystickData.Axis[2] / 32767.f;
//  if(fabs(xMoveC) < DEAD_ZONE)
//      xMoveC = 0.f;

//  yMoveC =
//      (irr::f32)joystickData.Axis[3] / -32767.f;
//  if(fabs(yMoveC) < DEAD_ZONE)
//      yMoveC = 0.f;


//  // manette PS4 V2
////  xMoveC =
////      (irr::f32)joystickData.Axis[2] / 32767.f;
////  if(fabs(xMoveC) < DEAD_ZONE)
////      xMoveC = 0.f;

////  yMoveC =
////      (irr::f32)joystickData.Axis[5] / -32767.f;
////  if(fabs(yMoveC) < DEAD_ZONE)
////      yMoveC = 0.f;

//}




//bool MyJoystickManager::myJoysticksActivation(irr::IrrlichtDevice *device)
//  {

//    bool out;

//    if(device->activateJoysticks(joystickInfo))
//    {
//        out = 1;
//        std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

//        for(irr::u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
//        {
//            std::cout << "Joystick " << joystick << ":" << std::endl;
//            std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << std::endl;
//            std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
//            std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;

//            std::cout << "\tHat is: ";

//            switch(joystickInfo[joystick].PovHat)
//            {
//            case irr::SJoystickInfo::POV_HAT_PRESENT:
//                std::cout << "present" << std::endl;
//                break;

//            case irr::SJoystickInfo::POV_HAT_ABSENT:
//                std::cout << "absent" << std::endl;
//                break;

//            case irr::SJoystickInfo::POV_HAT_UNKNOWN:
//            default:
//                std::cout << "unknown" << std::endl;
//                break;
//            }
//        }
//    }
//    else
//    {
//        out = 0;
//        std::cout << "Joystick support is not enabled." << std::endl;
//    }

//    return out;

//  }









//const irr::SEvent::SJoystickEvent & MyJoystickManager::GetJoystickState(void) const
//{
//  return JoystickState;
//}





//void MyJoystickManager::move(float xMoveP, float yMoveP, float xMoveC, float yMoveC)
//{


//  if(!isPerso && !isCamera)
//  {
//    //Si on ne bouge ni la cam ni le perso, alors on ne fait rien
//  }


//  if(isPerso && !isCamera)
//  {
//    //Si on ne bouge que le perso, alors on l anime
//    animPerso(xMoveP, yMoveP);
//  }


//  if(!isPerso && isCamera)
//  {
//    //Si on ne bouge que la cam, alors on l anime
//    animCam(xMoveC, yMoveC);
//  }


//  if(isPerso && isCamera)
//  {
//    //Si on bouge la cam ET le perso, alors on fait ce qu'il faut
//    animPersoNCam(xMoveP, yMoveP, xMoveC, yMoveC);
//  }

//}










//void MyJoystickManager::controllerEvent(const irr::SEvent &event)
//{



//    if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT && event.JoystickEvent.Joystick == 1)
//    {

//        irr::f32 xMoveP = 0.f; // entre -1 et 1
//        irr::f32 yMoveP = 0.f; // entre -1 et 1

//        irr::f32 xMoveC = 0.f; // entre -1 et 1
//        irr::f32 yMoveC = 0.f; // entre -1 et 1

//        static int isMoving = 0;



//        xyMoves(event, xMoveP, yMoveP, xMoveC, yMoveC);



//        if( ( !irr::core::equals(xMoveP, 0.f) ||  !irr::core::equals(yMoveP, 0.f))  &&
//            ( irr::core::equals(xMoveC, 0.f) ||  irr::core::equals(yMoveC, 0.f)) )
//        {
//          // si le perso bouge et que la camera ne bouge pas ...
//          isMoving += 1;
//          isPerso = 1;
//          isCamera = 0;
//        }

//        else if(  ( irr::core::equals(xMoveP, 0.f) ||  irr::core::equals(yMoveP, 0.f))  &&
//              ( !irr::core::equals(xMoveC, 0.f) ||  !irr::core::equals(yMoveC, 0.f))  )
//        {
//          // si la cam bouge et que le perso ne bouge pas ...
//          isMoving = 0;
//          isPerso = 0;
//          isCamera = 1;
//        }

//        else if(  ( !irr::core::equals(xMoveP, 0.f) ||  !irr::core::equals(yMoveP, 0.f))  &&
//              ( !irr::core::equals(xMoveC, 0.f) ||  !irr::core::equals(yMoveC, 0.f)) )
//        {
//          // si la cam ET le perso bouge ...
//          isMoving += 1;
//          isPerso = 1;
//          isCamera = 1;
//        }

//        else
//        {
//          isMoving = 0;
//          isPerso = 0;
//          isCamera = 0;

//          xMoveP = 0;
//          yMoveP = 0;

//          xMoveC = 0;
//          yMoveC = 0;
//        }




//        move(xMoveP, yMoveP, xMoveC, yMoveC);

//        //std::cout << "1 " << std::endl;


//        if (isMoving == 1)
//          gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_RUN);
//        if (isMoving == 0)
//           gmngr->getPlayer()->node->setMD2Animation(irr::scene::EMAT_STAND);


//        if ((irr::u32)GetJoystickState().IsButtonPressed(6))// on augmente la vitesse du perso qd la cond est verifiee
//           speedPerso = 5;
//        if (!(irr::u32)GetJoystickState().IsButtonPressed(6))
//           speedPerso = 1;


//    }
//}







//bool MyJoystickManager::OnEvent(const irr::SEvent &event)
//{
//  return true;
//}






//MyJoystickManager::MyJoystickManager():
//  isPerso(0), speedPerso(1), isCamera(0), translation(0, 20, 50)

//{}
