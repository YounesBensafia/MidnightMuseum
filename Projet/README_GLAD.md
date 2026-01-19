# Configuration GLAD pour TP3

## Étapes à suivre :

### 1. Télécharger GLAD
1. Allez sur https://glad.dav1d.de/
2. Configurez comme suit :
   - **Language**: C/C++
   - **Specification**: OpenGL
   - **gl**: Version 3.3 (ou supérieure, ex: 4.6)
   - **Profile**: Core
   - Cochez **Generate a loader**
3. Cliquez sur **GENERATE**
4. Téléchargez le fichier ZIP

### 2. Extraire et copier les fichiers GLAD

Après avoir extrait le ZIP téléchargé, vous aurez cette structure :
```
glad/
  ├── include/
  │   ├── glad/
  │   │   └── glad.h
  │   └── KHR/
  │       └── khrplatform.h
  └── src/
      └── glad.c
```

**Copiez les fichiers comme suit :**

- Copiez le contenu du dossier `glad/include/glad/` → dans `tp3/include/glad/`
  - Vous devriez avoir : `tp3/include/glad/glad.h`

- Copiez le contenu du dossier `glad/include/KHR/` → dans `tp3/include/KHR/`
  - Vous devriez avoir : `tp3/include/KHR/khrplatform.h`

- Copiez le fichier `glad/src/glad.c` → dans `tp3/src/`
  - Vous devriez avoir : `tp3/src/glad.c`

### 3. Structure finale du projet TP3

Après avoir copié tous les fichiers, votre projet devrait avoir cette structure :
```
tp3/
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
├── src/
│   └── glad.c
├── shader/
│   └── shader/
│       ├── shader.cpp
│       ├── shader.hpp
│       ├── SimpleFragmentShader.fragmentshader
│       └── SimpleVertexShader.vertexshader
├── build/
├── main.cpp
├── CMakeLists.txt
└── build_run.bat
```

### 4. Compiler et exécuter

Une fois les fichiers GLAD copiés, exécutez simplement :
```
build_run.bat
```

### 5. Vérification

Si tout fonctionne correctement, vous devriez voir dans la console :
```
OpenGL Version: <votre version OpenGL>
```

## Utilisation dans le code

GLAD doit être inclus **AVANT** GLFW :
```cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
```

Après avoir créé le contexte OpenGL avec GLFW, initialisez GLAD :
```cpp
glfwMakeContextCurrent(window);

if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
{
    printf("Failed to initialize GLAD\n");
    return -1;
}
```

Maintenant vous avez accès à toutes les fonctions OpenGL 3+ !
