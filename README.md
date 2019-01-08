# Projet_fin_d_annee


December 14, 2018   ->    January 25, 2019


</br>

### Nom du projet:

Projet de majeure Image, modélisation et informatique (IMI) à [CPE Lyon](https://www.cpe.fr/ingenieur-sciences-du-numerique/majeure-image-modelisation-informatique/).

</br>

### Auteurs:

* Pedro FOLETTO PIMENTA
* Cédric KUASSIVI
* Dylan TOSTI


</br>

### Description du projet:

#### Contexte:
Le projet de majeure est inscrit dans la formation d'ingénieur à CPE Lyon, et sa validation joue un rôle important dans l'obtention du diplôme d'ingénieur en sciences du numérique. Ce projet s'étend sur 100 heures durant le mois de Janvier.

Le projet s'effectue par groupe de 2 ou 3 personnes toutes spécialisées dans le même domaine (dans notre cas: Image, modélisation et informatique).

Il s'agit lors du projet de réaliser une application en rapport avec notre spécialité et qui applique directement ou indirectement une (ou plusieurs) thématiques abordée(s) tout au long de notre spécialisation et de notre formation d'ingénieur. Ainsi, notre application doit s’inscrire dans les thématiques: de l’acquisition, du traitement ou de l’analyse et la manipulation de données numériques dont l’application principale concerne les domaines de l’analyse et de la synthèse d’images.


#### Présentation du projet:
Il s'agit d'un jeu vidéo de type combat, dont les ennemis sont des IA.

Le joueur se déplace dans un environnement en 3D. Lorsqu'il entre dans le champ d'attaque d'un ennemi, le jeu est basculé dans une scène 2D. Le joueur n'a pas la possibilité de fuir lorsque le combat est engagé. Le jeu est de nouveau basculé dans la scène 3D lorsque tous les ennemis sont vaincus.

Lorsque le combat est engagé, à tour de rôle, le joueur puis les ennemis réalisent des actions (attaquer, se soigner, se déplacer). Les actions des ennemis seront prédites par reinforcement learning.


</br>

### Etat du projet:

En cours de développement ...


</br>

### Documentation:

#### Compilation et exécution du code:

  _Remarque : Si vous souhaitez compiler le code juste pour le tester, il est conseillé d'utiliser la méthode 2 (utilisation du makefile fourni).
  Si vous souhaitez modifier le code et tester les modifications, il est conseillé d'utiliser Qt pour lancer le CMakeLists.txt fourni (méthode 1.2)._


1. Avec le CMakeLists.txt :
  * Possibilité 1 (**non recommandée**): Se placer dans le répertoire ./projet/ (où se touve le fichier CMakeLists.txt) puis exécuter les commandes suivantes dans un terminal :
  ```
  $ cmake . #pour generer le makefile dans le meme repertoire
  $ make #afin de compiler le code
  ```
  Pour compiler le code puis la commande :
  ```
  $ ./projetFA
  ```
  Pour lancer l'application.

  * Possibilité 2 (**recommandée** pour modifier le code et tester les modifications): Lancer le CMakeLists.txt avec QTcreator et compiler et exécuter le code avec Qt.


2. Avec le makefile (**recommandée** pour une simple exécution):

  Il suffit de se placer dans le répertoire ./projet/code_projet/ puis exécuter la commande :

  ```
  $ make #afin de compiler le code
  ```

  Pour compiler le code puis la commande :
  ```
  $ ./projetFA
  ```
  Pour lancer l'application.


#### Comment jouer (touches à appuyer pour les actions):

A compléter ...

#### Rapport de bugs:

Bugs non fixés à résoudre, etc... (A compléter ...)
