/**
 * Author : Cédric KUASSIVI
 *
 * MyEventReceiver.hpp :
 *
 * Header du fichier MyEventReceiver.cpp
 *
**/


#pragma once

#ifndef MY_EVENT_RECEIVER_HPP
#define MY_EVENT_RECEIVER_HPP


#include "./libGridMapping/gridMapping.hpp"



struct MyEventReceiver : irr::IEventReceiver
{


public:

    irr::scene::IAnimatedMeshSceneNode * perso1; //correspondra au perso principal

    irr::scene::ICameraSceneNode * cam1; //correspondra a la camera principale

    bool button_pressed;
    int  old_x, old_y;
    float speed = 3;

    //bool isCombat = false; // a decommenter plus tard pour le basculement vers le combat


    // action a realiser lorsqu'on appuie sur une touche
    gridAction act;

    //la grid qui sera modifiee en fonction des touches appuyees
    gridMapping *grid;



    //constructeur(s) :
    MyEventReceiver();



    //methodes :
    bool OnEvent(const irr::SEvent &event); //surcharge de la fonction OnEvent() pour ajouter nos propres events

    bool keyboard_combat(const irr::SEvent &event);
    bool mouse(const irr::SEvent &event);



};

#endif
