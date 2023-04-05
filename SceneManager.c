// Path: SceneManager.c
// このコードは、サンプルプログラムのシーンマネージャー部分 SceneManager.c である。
// シーンの更新、描画、切替の管理を行う
//
#include "SceneManager.h"

static FuncPtr0 SceneManager_Func_Init = NULL;
static FuncPtr0 SceneManager_Func_Update = NULL;
static FuncPtr0 SceneManager_Func_Draw = NULL;
static FuncPtr0 SceneManager_Func_Clear = NULL;
static FuncPtr0 SceneManager_Func_VSync = NULL;
static BOOL isSceneChanged = FALSE;

static pSSceneWork scenes = NULL;

/////////////////////////////////
// シーンマネージャー自体の初期化
/////////////////////////////////
void SceneManager_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    isSceneChanged = FALSE;
}

/////////////////////////////////
// シーンマネージャー：シーン切替
/////////////////////////////////
void SceneManager_ChangeScene(int nextscene)
{
//    self->nextScene = scene;
    isSceneChanged = TRUE;
}

/////////////////////////////////
// シーンマネージャー：シーン登録
/////////////////////////////////
void SceneManager_Regist(pSSceneWork scene)
{
    scenes = scene;
}

/////////////////////////////////
// シーン：初期化
/////////////////////////////////
void Scene_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    isSceneChanged = FALSE;
}

/////////////////////////////////
// シーン：更新処理
/////////////////////////////////
void Scene_Update()
{
    if (SceneManager_Func_Update != NULL)
    {
        SceneManager_Func_Update();
    }
}

/////////////////////////////////
// シーン：描画処理
/////////////////////////////////
void Scene_Draw()
{
    if (SceneManager_Func_Draw != NULL)
    {
        SceneManager_Func_Draw();
    }
}

/////////////////////////////////
// シーン：切替処理
/////////////////////////////////
void Scene_Clear()
{
    if (SceneManager_Func_Clear != NULL)
    {
        SceneManager_Func_Clear();
    }
}

/////////////////////////////////
// シーン：VSync処理
/////////////////////////////////
void Scene_VSync()
{
    if (SceneManager_Func_VSync != NULL)
    {
        SceneManager_Func_VSync();
    }
}
