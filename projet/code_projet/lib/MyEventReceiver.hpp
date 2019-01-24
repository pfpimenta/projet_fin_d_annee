#pragma once

#ifndef MY_EVENT_RECEIVER_HPP
#define MY_EVENT_RECEIVER_HPP


#include "GameManager.hpp"


struct MyEventReceiver : irr::IEventReceiver
{




//    // controller
//    irr::SEvent::SJoystickEvent JoystickState; //permet de recuperer l'etat du joystick (touches appuyees, etc...)
//    irr::core::array<irr::SJoystickInfo> joystickInfo; //permet d'avoir toute les infos d'un joystick (nombre de touches, nom du joystick, etc...)




    irr::scene::IAnimatedMeshSceneNode *perso1; //correspondra au perso principal
    irr::scene::ICameraSceneNode *camCombat; //correspondra a la camera pendant le combat

    // pour la souris
    bool button_pressed;
    int  old_x, old_y;
    float speed = 9;

    /** gestion scene 3D **/
    bool isOpened = true;


    // le gameManager pour pouvoir utiliser directement les touches du clavier pour declencher
    // des fonctions de gmngr
    GameManager *gmngr = new GameManager;
    Action act;


    //constructeur(s) :
    MyEventReceiver();



    bool OnEvent(const irr::SEvent &event); //surcharge de la fonction OnEvent() pour ajouter nos propres events

    /** gestion du combat **/
    bool keyboard_combat(const irr::SEvent &event);
    bool mouse_combat(const irr::SEvent &event);

    /** gestion jeu libre **/
    bool keyboard_promenade(const irr::SEvent &event);
    bool mouse_promenade(const irr::SEvent &event);
    bool gui_manage(const irr::SEvent &event);

};

#endif
