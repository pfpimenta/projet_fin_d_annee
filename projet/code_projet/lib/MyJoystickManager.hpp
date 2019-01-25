#pragma once

#ifndef MY_JOYSTICK_MANAGER_HPP
#define MY_JOYSTICK_MANAGER_HPP


#include "GameManager.hpp"

struct MyJoystickManager : irr::IEventReceiver

{


private:


  //attributs :
  irr::SEvent::SJoystickEvent JoystickState; //permet de recuperer l'etat du joystick (touches appuyees, etc...)

  irr::core::array<irr::SJoystickInfo> joystickInfo; //permet d'avoir toute les infos d'un joystick (nombre de touches, nom du joystick, etc...)

  //methodes :


public:

  //attributs :

  //perso
  bool isPerso;
  GameManager *gmngr = new GameManager;

  float speedPerso; //vitesse du perso

  //camera
  bool isCamera;
  irr::core::vector3df translation; //position de cam par rapport a perso




  //methodes :
  void animPersoNCam(float xMoveP, float yMoveP, float xMoveC, float yMoveC);
  const irr::SEvent::SJoystickEvent & GetJoystickState(void) const;
  bool myJoysticksActivation(irr::IrrlichtDevice *device);

  void xyMoves(const irr::SEvent &event,
                       irr::f32 &xMoveP,
                       irr::f32 &yMoveP,
                       irr::f32 &xMoveC,
                       irr::f32 &yMoveC);


  void animPerso(float xMove, float yMove);

  void animCam(float xMove, float yMove);

  void move(float xMoveP, float yMoveP, float xMoveC, float yMoveC);

  void controllerEvent(const irr::SEvent &event);

  bool OnEvent(const irr::SEvent &event);


  //constructeur(s) :
  MyJoystickManager();

};





#endif
