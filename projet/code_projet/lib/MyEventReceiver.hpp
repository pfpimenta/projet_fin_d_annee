#pragma once

#ifndef MY_EVENT_RECEIVER_HPP
#define MY_EVENT_RECEIVER_HPP


#include "GameManager.hpp"


struct MyEventReceiver : irr::IEventReceiver
{


    irr::scene::IAnimatedMeshSceneNode *perso1; //correspondra au perso principal
    irr::scene::ICameraSceneNode *camCombat; //correspondra a la camera pendant le combat

    bool button_pressed;
    int  old_x, old_y;
    float speed = 3;



    // le gameManager pour pouvoir utiliser directement les touches du clavier pour declencher
    // des fonctions de gmngr
    GameManager *gmngr = new GameManager;
    Action act;


    //constructeur(s) :
    MyEventReceiver();



    bool OnEvent(const irr::SEvent &event); //surcharge de la fonction OnEvent() pour ajouter nos propres events

    bool keyboard_combat(const irr::SEvent &event);
    bool mouse_combat(const irr::SEvent &event);

};

#endif
