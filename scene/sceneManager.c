#include "sceneManager.h"

#include "gamescene.h"
#include "menu.h"
Scene* scene = NULL;
void create_scene(SceneType type) {
    switch (type) {
        case Menu_L:
            scene = New_Menu(Menu_L);
            break;
        case GameScene_L:
            scene = New_GameScene(GameScene_L);
            break;
        default:
            break;
    }
}
