# Museum 3D - Separated Room Structure

## Overview
The museum code has been refactored into separate files for each room to allow team members to work independently on different areas of the museum.

## File Structure

### Header Files (include/)
- **Room1.hpp** - Main entrance room with showcases, fossils, and effigy
- **Hallway.hpp** - Connecting hallway between Room 1 and Room 2
- **Room2.hpp** - Second exhibition room with Buddha statue
- **MuseumScene.hpp** - Main scene coordinator that manages all rooms

### Source Files (src/)
- **Room1.cpp** - Implementation of Room 1 (walls, floor, ceiling, models, collisions)
- **Hallway.cpp** - Implementation of Hallway (walls, ceiling, collisions)
- **Room2.cpp** - Implementation of Room 2 (walls, floor, ceiling, Buddha, collisions)
- **MuseumScene.cpp** - Main scene coordinator implementation

## Working on Individual Rooms

### Room 1 (Main Entrance)
**Files to edit:** `src/Room1.cpp` and `include/Room1.hpp`

**Contains:**
- 6 glass showcases (3 on left, 3 on right)
- Central fossils display
- Effigy statue on left front showcase
- North wall with doorway to hallway
- South, East, West walls
- Collision detection for fossils and showcases

**Key methods:**
- `renderWalls()` - Modify wall placement/appearance
- `renderShowcases()` - Modify showcase positions/appearance
- `renderFossils()` - Modify fossil model rendering
- `renderEffigy()` - Modify effigy rendering
- `checkCollision()` - Modify collision boundaries

### Hallway (Connecting Passage)
**Files to edit:** `src/Hallway.cpp` and `include/Hallway.hpp`

**Contains:**
- East and West hallway walls
- Hallway ceiling
- Collision detection for hallway boundaries

**Key methods:**
- `renderWalls()` - Modify hallway wall placement
- `renderFloorAndCeiling()` - Modify hallway ceiling
- `checkCollision()` - Modify collision boundaries

### Room 2 (Buddha Exhibition)
**Files to edit:** `src/Room2.cpp` and `include/Room2.hpp`

**Contains:**
- Large Buddha triad statue on north wall
- Room walls with doorway from hallway
- Ceiling
- Collision detection for room boundaries

**Key methods:**
- `renderWalls()` - Modify wall placement/appearance
- `renderBuddha()` - Modify Buddha statue rendering
- `renderFloorAndCeiling()` - Modify ceiling
- `checkCollision()` - Modify collision boundaries

## Shared Components

### MuseumScene
Coordinates all rooms and handles:
- Shared floor texture (spans all rooms)
- Lighting system (6 corner lights total)
- Flashlight rendering
- Collision checking across all rooms

## Git Workflow for Team Collaboration

### Branch Strategy
Each team member should work on a separate branch:
```bash
# For Room 1
git checkout -b yourname/room1

# For Hallway
git checkout -b yourname/hallway

# For Room 2  
git checkout -b yourname/room2
```

### Making Changes
1. Edit only YOUR assigned room files
2. Test your changes locally
3. Commit frequently with clear messages:
```bash
git add src/Room1.cpp include/Room1.hpp
git commit -m "Added new exhibit to Room 1"
git push origin yourname/room1
```

### Avoiding Conflicts
- **DO NOT** modify files for rooms you're not assigned to
- **DO NOT** modify MuseumScene unless coordinating with team
- **DO** communicate when changing shared resources (textures, models)

### Merging
When ready to integrate:
1. Pull latest main branch
2. Test your branch
3. Create pull request
4. Have another team member review
5. Merge to main

## Building the Project

```bash
cd Projet/build
cmake ..
mingw32-make
```

## Adding New Models to Your Room

### Example: Adding a new exhibit to Room 1

1. Load the model in `Room1::init()`:
```cpp
void Room1::init() {
    // Existing code...
    newExhibitModel = rm.loadFBXModel("model/new_exhibit.glb");
}
```

2. Add the model member to `Room1.hpp`:
```cpp
private:
    FBXModel newExhibitModel;
```

3. Create a render method in `Room1.cpp`:
```cpp
void Room1::renderNewExhibit(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    // Rendering code here
}
```

4. Call it in `Room1::render()`:
```cpp
void Room1::render(...) {
    // Existing renders...
    renderNewExhibit(view, projection, shaderProgram);
}
```

## Collision Detection

Each room has its own `checkCollision(vec3 newPos)` method. To add new collision zones:

1. Add a helper method (e.g., `checkNewObjectCollision`)
2. Call it in the main `checkCollision()` method
3. Return `true` if collision detected

Example:
```cpp
bool Room1::checkCollision(const vec3& newPos) {
    return checkFossilCollision(newPos) || 
           checkWallCollision(newPos) || 
           checkTableCollision(newPos) ||
           checkNewObjectCollision(newPos);  // Your new object
}
```

## Questions or Issues?

If you need to modify shared components (MuseumScene, lighting, shared floor), coordinate with the team first!
