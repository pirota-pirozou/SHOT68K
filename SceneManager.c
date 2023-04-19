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
static BOOL isFirst = TRUE;
static int sceneID = -1;
static int nextSceneID = -1;
static const SSceneWork * scenes = NULL;

/////////////////////////////////
/// @brief シーンマネージャー自体の初期化
/////////////////////////////////
void SceneManager_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    SceneManager_Func_VSync = NULL;
    isSceneChanged = FALSE;
    isFirst = TRUE;
    sceneID = nextSceneID = -1;
}

/////////////////////////////////
/// @brief シーンIDの関数に切り替える
/////////////////////////////////
static void SetChangeFunc(int sid)
{
    const SSceneWork* scene = &scenes[sid];
    SceneManager_Func_Init = scene->Init;
    SceneManager_Func_Update = scene->Update;
    SceneManager_Func_Draw = scene->Draw;
    SceneManager_Func_Clear = scene->Clear;
    SceneManager_Func_VSync = scene->VSync;

}

/////////////////////////////////
/// @brief シーンマネージャー：シーン切替
/////////////////////////////////
void SceneManager_ChangeScene(int nextscene)
{
    if (sceneID != nextscene)
    {
        nextSceneID = nextscene;
        // 初回のChangeだったら、Initを呼ぶ
        if (isFirst)
        {
            isFirst = FALSE;
            sceneID = nextSceneID;
            SetChangeFunc(sceneID);
            if (SceneManager_Func_Init != NULL)
            {
                SceneManager_Func_Init();
            }
        }
        else
        {
            isSceneChanged = TRUE;
        }

    }
}

/////////////////////////////////
/// @brief シーンマネージャー：シーン登録
/////////////////////////////////
void SceneManager_Regist(const SSceneWork* scene)
{
    scenes = scene;
}

/////////////////////////////////
/// @brief  シーン：初期化
/////////////////////////////////
void Scene_Init()
{
    if (SceneManager_Func_Init != NULL)
    {
        SceneManager_Func_Init();
    }
}

/////////////////////////////////
/// @brief シーン：更新処理
/////////////////////////////////
void Scene_Update()
{
    if (SceneManager_Func_Update != NULL)
    {
        SceneManager_Func_Update();
    }
}

/////////////////////////////////
/// @brief シーン：描画処理
/////////////////////////////////
void Scene_Draw()
{
    if (SceneManager_Func_Draw != NULL)
    {
        SceneManager_Func_Draw();
    }
    // シーン切替があったら、シーンを切り替える
    if (isSceneChanged)
    {
        isSceneChanged = FALSE;
        Scene_Clear();      // 現在のシーンのクリア処理

        // 次のシーンの関数ポインタをセット
        sceneID = nextSceneID;
        SetChangeFunc(sceneID);

        // 次のシーンの初期化処理
        Scene_Init();
    }

}

/////////////////////////////////
/// @brief シーン：切替処理
/////////////////////////////////
void Scene_Clear()
{
    if (SceneManager_Func_Clear != NULL)
    {
        SceneManager_Func_Clear();
    }
}

/////////////////////////////////
/// @brief シーン：VSync処理
/////////////////////////////////
void Scene_VSync()
{
    if (SceneManager_Func_VSync != NULL)
    {
        SceneManager_Func_VSync();
    }
}
