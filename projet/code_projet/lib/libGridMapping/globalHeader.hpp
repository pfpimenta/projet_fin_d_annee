/**
 * Author : Cédric KUASSIVI
 *
 * globalHeader.hpp :
 *
 * contient toutes les class, enum et #define  'globaux' et includes utilisees dans tous les fichiers.
 *
 * Tous les fichier .hpp sont inclus ici. Le fichier globalHeader.hpp est donc le seul
 * fichier include dans les autres fichiers de ce sous repertoire.
 *
**/

#pragma once

#ifndef GLOBAL_HEADER_HPP
#define GLOBAL_HEADER_HPP


#include <irrlicht/irrlicht.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>



using namespace irr;


namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;



// nombre de cases par defaut de la grid
#define DEFAULT_WIDTH 20 // en largeur donc nb de colonnes
#define DEFAULT_HEIGHT 10 // en hauteur donc nombre de lignes

// positions par defaut du curseur et donc du joueur
#define DEFAULT_LIGNE 0
#define DEFAULT_COLONNE 0

// taille par defaut du carre elementaire de la grid
#define DEFAULT_GRID_NODE_SIZE 5

// decallage par defaut du carre elementaire par rapport au centre de la fenetre
// on en a besoin au moment de creer les nodes personnalises
// je conseille laisser les valeurs a 0 pour ne pas se faire chier
// si vous voulez translater le carre elementaire ou meme la grille en entier, j'ai
// ecrit une fonction void translation(irr::core::vector3df) permettant de le faire
// facilement.
#define DEFAULT_X_OFFSET 0
#define DEFAULT_Y_OFFSET 0

// delai clignotage curseur
#define DEFAULT_BLINK_DELAY 500 //ms



/** action que le joueur peut effectuer sur la grid **/
enum gridAction {HAUT, BAS, GAUCHE, DROITE, RESET, NOTHING, VALIDATE, DEBUG};


/** action que le joueur peut effectuer en tant qu'objet **/
//enum playerAction {ATTACK};

/** couleurs des grid nodes **/
enum gridColor {PLAYER, SELECTED, UNSELECTED, OBSTACLE, ENNEMY};



// nombre aleatoire entre min et max exclu
// pourra par la suite etre supprime. Je l'utilise pour le debug
static int alea(int min, int max)
{
    return rand()%(max-min+1) + min;
}


/** positions des objets (obstacles, joueur, ennemis, etc ..) et aussi du curseur de la grid **/
struct position
{
    /** attributs **/
    int ligne; //ligne
    int colonne; //colonne

    /** constructeurs **/
    position(int ligne, int colonne){this->ligne = ligne; this->colonne = colonne;}
    position(const position &p){this->ligne = p.ligne; this->colonne = p.colonne;}
    position(){}

    /** pour le debug **/
    std::string printPosition(bool display)
    {
        std::string s = "position : ligne = " + std::to_string(ligne) + "; colonne = " + std::to_string(colonne);
        if (display)(std::cout << s << std::endl);
        return s;
    }

    /** comparaison de 2 positions **/
    bool operator==(const position &p)
    {
        if (ligne == p.ligne && colonne == p.colonne)
            return true;
        return false;
    }

};



/** obstacles (boundaries, ennemis, autres objets) : pour empecher le joueur de se placer a ces endroits **/
struct obstacle
{
    /** attributs **/
    /** /!\ a optimiser pour conserver 1 seule fois les elements **/
    std::vector<position> pos; // toutes les positions des obstacles

    /** constructeurs **/
    obstacle(int width, int heigth)
    {
        // ajout des boundaries comme obstacles par defaut
        for (int k = -1; k <= width ; k++)
        {
            pos.push_back(position(-1, k));
            pos.push_back(position(heigth, k));
        }

        for (int l = -1; l <= heigth; l++)
        {
            pos.push_back(position(l, -1));
            pos.push_back(position(l, width));
        }
    }

};




/** creation d'un node personnalise (inspire du tuto irrlicht custom scene node)  **/
class gridNode : public scene::ISceneNode
{

    core::aabbox3d<f32> Box; //utile pour l'auto-culling du node cree
    video::S3DVertex Vertices[4]; // tableau des vertices
    video::SMaterial Material; // materiau du node

    bool isWireframe; // true => affichage wireframe
    bool isLighting; // false => affichage couleurs

    // positions des vertices
    core::vector3df positions[4];

public:


    // fonction permettant de recuperer une couleur en fonction du type
    video::SColor getCouleur(gridColor color)
    {
        // A / R / G / B
        switch (color) {
        case PLAYER:
            return(video::SColor(0, 0, 206, 209)); //turquoise
            break;
        case UNSELECTED:
            return(video::SColor(0, 255, 255, 255)); //blanc
            break;
        case SELECTED: // pour l'instant non utilisee, je ne sais pas encore quoi en faire
            return(video::SColor(0, 0, 0, 255)); //bleu
            break;
        case ENNEMY:
            return(video::SColor(0, 255, 0, 0)); // rouge
            break;
        case OBSTACLE:
            return(video::SColor(0, 255, 0, 255)); // violet
            break;
        default:
            return(video::SColor(255, 0, 0, 0)); // noir
            break;
        }
    }



    // modifie la couleur du node avec la couleur choisie
    // (tous les points du maillage ont la meme couleur)
    void setCouleur(gridColor color)
    {
        Vertices[0].Color = getCouleur(color);
        Vertices[1].Color = getCouleur(color);
        Vertices[2].Color = getCouleur(color);
        Vertices[3].Color = getCouleur(color);
    }



    // genere automatiquement les positions des vertices du gridNode elementaire
    // le offset correspond au vecteur de translation. Par defaut il est a 0.
    // le sommet haut-gauche du premier node( situe en haut a gauche du maillage complet)
    // sera place au centre de la fenetre a la position (0, 0, 0)
    void generatePosition(int nodeSize, int xOffset, int zOffset)
    {
        positions[0] = core::vector3df(-(float)nodeSize/2.0, 0, +(float)nodeSize/2.0) + core::vector3df(xOffset, 0, zOffset);
        positions[1] = core::vector3df(-(float)nodeSize/2.0, 0, -(float)nodeSize/2.0) + core::vector3df(xOffset, 0, zOffset);
        positions[2] = core::vector3df(+(float)nodeSize/2.0, 0, -(float)nodeSize/2.0) + core::vector3df(xOffset, 0, zOffset);
        positions[3] = core::vector3df(+(float)nodeSize/2.0, 0, +(float)nodeSize/2.0) + core::vector3df(xOffset, 0, zOffset);

//        std::cout << positions[0].X << "  " << positions[0].Y << "  " << positions[0].Z <<std::endl;
//        std::cout << positions[1].X << "  " << positions[1].Y << "  " << positions[1].Z <<std::endl;
//        std::cout << positions[2].X << "  " << positions[2].Y << "  " << positions[2].Z <<std::endl;
//        std::cout << positions[3].X << "  " << positions[3].Y << "  " << positions[3].Z <<std::endl;
    }




    // constructeur
    gridNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, int nodeSize, int xOffset, int zOffset)
        : scene::ISceneNode(parent, mgr, id), isWireframe(true), isLighting(false)
    {
        Material.Wireframe = isWireframe;
        Material.Lighting = isLighting;


        generatePosition(nodeSize, xOffset, zOffset);


        // positions des vertices
        // maintenant qu'on a les positions, il faut generer les vertices avec toutes leurs infos a savoir :
        // leurs positions, leurs normales (toutes initialisees a (1, 1, 1)), leurs couleurs et
        // les coordonnees de textures.
        Vertices[0] = video::S3DVertex(positions[0].X, positions[0].Y, positions[0].Z , // positions xyz
                                       1,1,1, // normales xyz
                                       getCouleur(UNSELECTED), // couleurs rgba
                                       0, 0); // coordonnees texture

        Vertices[1] = video::S3DVertex(positions[1].X, positions[1].Y, positions[1].Z ,
                                       1,1,1,
                                       getCouleur(UNSELECTED),
                                       1, 0);

        Vertices[2] = video::S3DVertex(positions[2].X, positions[2].Y, positions[2].Z ,
                                       1,1,1,
                                       getCouleur(UNSELECTED),
                                       1, 1);

        Vertices[3] = video::S3DVertex(positions[3].X, positions[3].Y, positions[3].Z ,
                                       1,1,1,
                                       getCouleur(UNSELECTED),
                                       0, 1);

        // pour l'auto-culling : present dans le tuto...
        Box.reset(Vertices[0].Pos);
            for (s32 i=1; i<4; ++i)
                Box.addInternalPoint(Vertices[i].Pos);
    }


    // methode pour pouvoir afficher le node : present dans le tuto...
    virtual void OnRegisterSceneNode()
    {
        if (IsVisible)
            SceneManager->registerNodeForRendering(this);

        ISceneNode::OnRegisterSceneNode();
    }


    // methode pour definir comment afficher le node
    virtual void render()
    {
        /**
         * Comme dans opengl, au moment de dessiner le mesh, on peut choisir quelles primitives afficher avec la ligne :
         * driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 1, video::EVT_STANDARD, scene::EPT_QUADS, video::EIT_16BIT);
         *
         * J ai opte pour les QUADS parce qu'on veut afficher des carres vides (wireframe) ou remplis.
         **/

        u16 indices[] = {   0, 1, 2, 3  }; // connexite a prendre en compte
        video::IVideoDriver* driver = SceneManager->getVideoDriver();

        driver->setMaterial(Material);
        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
        driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 1, video::EVT_STANDARD, scene::EPT_QUADS, video::EIT_16BIT);
    }


    // pour modifier l'affichage wireframe du node
    void setWireframe()
    {
        isWireframe = !isWireframe; // si wireframe == true, wireframe = false et inversement
        Material.Wireframe = isWireframe;
    }


    // pour modifier l'affichage wireframe du node
    void setWireframe(bool val)
    {
        isWireframe = val;
        Material.Wireframe = isWireframe;
    }


    // pour modifier l'affichage couleur (ou en noir) du node
    void setLighting()
    {
        isLighting = !isLighting;
        Material.Lighting = isLighting;
    }


    // pour la box de collision : present dans le tuto ...
    virtual const core::aabbox3d<f32>& getBoundingBox() const
        {
            return Box;
        }


    // autres methodes importantes presentes dans le tuto ...
    virtual u32 getMaterialCount() const
    {
        return 1;
    }

    virtual video::SMaterial& getMaterial(u32 i)
    {
        return Material;
    }

};



// classe contenant le maillage (vecteur de carres elementaires)
class gridMesh
{
public:
    std::vector<gridNode*> mesh;

    int width;
    int height;



    // centrage automatique de la grille a la position (0, 0, 0) selon les axes X et Z
    void centrageAuto(int nodeSize)
    {
        core::vector3df t((float)nodeSize/2.0 * (float)(height - 1), 0, -(float)nodeSize/2.0 * (float)(width - 1));
        translation(t);
    }


    // effectuer une translation sur le mesh
    void translation(core::vector3df t)
    {
        for (unsigned int k = 0; k < mesh.size(); k++)
            mesh[k]->setPosition(t);
    }

    // effectuer une rotation sur le mesh
    void rotation(core::vector3df r)
    {
        for (unsigned int k = 0; k < mesh.size(); k++)
            mesh[k]->setRotation(r);
    }


    // constructeur
    // il construit la grille de taille (width x height). Le sommet haut - gauche de la grille se trouve au centre
    // de la fenetre a la posiion (0, 0, 0) d'ou l'interet de faire par la suite un centrageAuto()
    gridMesh(int width, int height, scene::ISceneNode* parent, scene::ISceneManager* smgr, int nodeSize)
    {
        this->width = width;
        this->height = height;

        for (int i = 0; i < this->width; i++)
        {
            for (int j = 0; j < this->height; j++)
            {
                gridNode *gridCase = new gridNode(parent, smgr, (s32)i, nodeSize, -j * nodeSize, i * nodeSize);
                mesh.push_back(gridCase);
            }
        }
    }




    //pour acceder au gridNode a la position p(ligne, colonne)
    gridNode* getGridNode(position p)
    {
        int k = height * p.colonne + p.ligne;
        return mesh[k];
    }
    // ou alors acceder au kieme gridNode
    gridNode* getGridNode(int k)
    {
        return mesh[k];
    }

    // set all the isWireframe values
    void resetWireframe(bool val)
    {
        for (unsigned int k = 0; k < mesh.size(); k++)
            mesh[k]->setWireframe(val);
    }

};



// joueur

class player
{
public:
    position pos;

    gridMesh *theGrid;

    player(int ligne, int colonne, gridMesh *theGrid)
        :pos(ligne, colonne)
    {
        theGrid->getGridNode(pos)->setCouleur(PLAYER);
    }


    player(){}

    void setPosition(const position &p)
    {
        pos.ligne = p.ligne;
        pos.colonne = p.colonne;
    }

};




// enemy

class enemy
{
public:
    position pos;

    bool isAlive;

    gridMesh *theGrid = NULL;

    enemy(int ligne, int colonne, gridMesh *theGrid)
        :pos(ligne, colonne), isAlive(1)
    {
        theGrid->getGridNode(pos)->setCouleur(ENNEMY);
    }


    enemy(){}

    void setPosition(const position &p)
    {
        pos.ligne = p.ligne;
        pos.colonne = p.colonne;
    }
};




#include "gridMapping.hpp"



#endif
