# Projet Museum 3D - Synthèse d'Image

## Description
Ce projet est un musée virtuel 3D interactif développé en OpenGL avec C++. Le joueur peut se déplacer librement dans le musée et observer différents objets exposés.

## Fonctionnalités Obligatoires Implémentées ✅

### 1. Animation
- **Rotation continue** des objets centraux du musée
- **Oscillation verticale** pour certains exhibits (mouvement haut/bas)
- **Rotations différenciées** : chaque objet a son propre style d'animation
- **Lumière dynamique** qui tourne autour de la scène

### 2. Interaction avec l'utilisateur
- **Déplacement WASD** :
  - `W` : Avancer
  - `S` : Reculer
  - `A` : Gauche
  - `D` : Droite
- **Contrôle de la caméra à la souris** (vue FPS)
- **Molette de la souris** : Zoom in/out
- **ESC** : Quitter l'application
- **Hauteur fixe** : La caméra reste à hauteur d'yeux (1.8m) comme un visiteur réel

### 3. Éclairage
- **Lumière ambiante** : Illumination de base du musée (30%)
- **Lumière diffuse** : Éclairage directionnel des objets (50%)
- **Lumière spéculaire** : Reflets brillants sur les surfaces (70%)
- **Lumière dynamique** qui se déplace dans le musée pour créer une atmosphère vivante

## Fonctionnalités Bonus 🌟

### Multiples Exhibits
- 5 objets exposés positionnés stratégiquement dans le musée
- Chaque objet a sa propre animation unique
- Textures variées pour différencier les exhibits

### Environnement du Musée
- Sol du musée avec texture
- Atmosphère sombre et réaliste (couleur de fond appropriée)
- Espace large pour explorer

### Système de Caméra Avancé
- Caméra First-Person fluide
- Contrainte de pitch pour éviter les retournements
- Vitesse de mouvement réaliste
- Sensibilité de souris ajustable

## Structure du Projet

```
Projet/
├── main.cpp                    # Code principal du musée
├── CMakeLists.txt             # Configuration de compilation
├── build_run.bat              # Script de compilation et exécution
├── include/
│   ├── Camera.hpp             # Système de caméra FPS
│   ├── glad/                  # OpenGL loader
│   └── KHR/
├── shader/shader/
│   ├── SimpleVertexShader.vertexshader    # Vertex shader avec lighting
│   ├── SimpleFragmentShader.fragmentshader # Fragment shader avec Phong
│   └── shader.cpp/hpp         # Chargeur de shaders
├── model/
│   ├── objload.cpp/hpp        # Loader OBJ
│   └── torus.obj              # Modèle 3D
├── textures/
│   └── *.raw                  # Fichiers de textures
└── src/
    └── glad.c                 # GLAD implementation
```

## Compilation et Exécution

### Méthode 1 : Script automatique
```bash
cd Projet
build_run.bat
```

### Méthode 2 : Manuel
```bash
cd Projet
mkdir build
cd build
cmake ..
mingw32-make
cd ..
build/main.exe
```

## Contrôles

| Touche/Action | Fonction |
|---------------|----------|
| W | Avancer |
| S | Reculer |
| A | Aller à gauche |
| D | Aller à droite |
| Souris | Regarder autour |
| Molette | Zoom |
| ESC | Quitter |

## Technologies Utilisées

- **OpenGL 3.3 Core Profile**
- **GLFW 3.4** - Gestion de fenêtre et inputs
- **GLM** - Mathématiques 3D
- **GLAD** - OpenGL loader
- **C++17**

## Modèle de Phong Lighting

Le projet utilise le modèle de Phong complet :

```glsl
Lighting = Ambient + Diffuse + Specular
```

- **Ambient** (0.3) : Lumière ambiante du musée
- **Diffuse** (0.5) : Lumière directionnelle dépendante de l'angle
- **Specular** (0.7) : Reflets brillants (exposant = 50)

## Améliorations Possibles

### Options à considérer :
1. **Multiple textures par objet** (texture mapping avancé)
2. **Skybox** pour l'environnement du musée
3. **Collision detection** pour ne pas traverser les objets
4. **Shadows** (shadow mapping)
5. **Particules** pour effets atmosphériques
6. **Sons ambiants** du musée
7. **UI/HUD** avec informations sur les exhibits
8. **Normal mapping** pour plus de détails
9. **Post-processing** (bloom, SSAO)
10. **Différents modèles OBJ** (dinosaures, armures, etc.)

## Idées d'Objets à Ajouter

Pour enrichir votre musée, vous pouvez ajouter :
- **Fossiles de dinosaures** (crâne de T-Rex, squelettes)
- **Armures médiévales** (chevalier, samurai)
- **Statues antiques** (grecques, romaines)
- **Artefacts égyptiens** (sarcophages, bustes)
- **Objets d'art moderne** (sculptures abstraites)
- **Vitrines** avec socles

## Critères d'Évaluation

### ✅ L'idée de la scène
Un musée interactif est original et permet de montrer plusieurs concepts techniques tout en restant cohérent visuellement.

### ✅ La qualité du rendu
- Éclairage Phong complet
- Textures multiples
- Animations fluides
- Atmosphère réaliste

### ✅ Les options ajoutées
- Système de caméra FPS sophistiqué
- Animations variées et dynamiques
- Lumière mobile pour ambiance vivante
- Multiples exhibits avec comportements uniques

## Auteurs

Projet réalisé dans le cadre du module "Synthèse d'Image"

## License

Projet académique - Usage éducatif uniquement
