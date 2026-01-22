# Night at the Museum: Interactive 3D Virtual Tour

An immersive 3D museum experience developed using **OpenGL 3.3** for the "Synthèse d'Images" module at **USTHB**. This project explores real-time rendering, dynamic lighting systems, and interactive gameplay mechanics within a high-fidelity virtual environment.

## Technical Specifications

* **Language:** C++17 (Compilateur MinGW-w64 GCC 15.2.0)
* **Graphics API:** OpenGL 3.3 Core Profile via GLAD
* **Libraries:** * **GLFW 3.4.0:** Window management and input handling
* **GLM:** Matrix and vector mathematics
* **Assimp 6.0.3:** Complex 3D model loading (FBX, GLB)
* **stb_image:** Texture decoding


* **Build System:** CMake with automated deployment scripts

## Core Features

* **High-Fidelity Rendering:** Optimized pipeline managing approximately 2.7 million triangles while maintaining a stable 60 FPS.
* **Lighting Engine:** Implementation of the **Phong Reflection Model** (Ambient, Diffuse, Specular) with multiple light types:
* **Flashlight:** A camera-aligned Spotlight (Toggle via F).
* **Static Lights:** Ambient chandelier and soft-light gallery projectors.
* **Interactive Lights:** Context-aware illumination for statues and artifacts.


* **Physics & Navigation:** FPS-style camera with vertical clamping and delta-time synchronization. Collision detection is handled via **Oriented Bounding Boxes (OBB)**.
* **Interactive Gameplay:** Scripted door animations, spatialized audio feedback, and a specialized **Inspection Mode** for detailed object observation.

## Screenshots

### Main Exhibition Hall

The main hall features high-density 3D models including dinosaur fossils and statues, demonstrating the engine's ability to handle complex geometries.

<img width="604" height="338" alt="image" src="https://github.com/user-attachments/assets/cb71f2b9-ec0f-412c-95ad-fcbc22fb0b7c" />

### Lighting and Atmosphere

Demonstration of the dynamic lighting system, showcasing the player's flashlight and the environmental contrast created by the Phong reflection model.

<img width="567" height="315" alt="image" src="https://github.com/user-attachments/assets/125af5d7-706d-46fc-9157-fbb83c7eb0cc" />

### Inspection Mode

A dedicated mode that allows users to lock onto an artifact and rotate it 360 degrees for a detailed view of its topology and textures.
<img width="637" height="358" alt="image" src="https://github.com/user-attachments/assets/657faea7-9d7f-40a9-88f7-d501e02e8a6f" />

## Controls

| Input | Action |
| --- | --- |
| **Z, Q, S, D** | Movement (Forward, Left, Backward, Right) |
| **Mouse** | Camera Look / Orbital rotation in Inspection Mode |
| **F** | Toggle Flashlight |
| **E** | Interact (Open Doors / Activate Statue Lights) |
| **I** | Enter/Exit Inspection Mode |
| **ESC** | Exit Application |

## Installation & Build

1. **Prerequisites:** Ensure CMake and a C++17 compatible compiler are installed.
2. **Clone the repository:**
```bash
git clone https://github.com/username/night-at-the-museum.git

```


3. **Compile:**
```bash
mkdir build && cd build
cmake ..
make

```


4. **Execute:** Run the generated binary or the provided `build_run.bat` script.

---

**Authors:** BENSAFIA Younés, MOUSTARI Abderrahmane, TADJINE Rabah Amine
**Institution:** University of Science and Technology Houari Boumediene (USTHB)
**Faculty:** Electronics and Computer Science (FEI)
**Academic Year:** 2025/2026
