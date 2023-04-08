// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、シーンマネージャーのヘッダファイル である。
//
#pragma once
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"
#include "GamePadManager.h"
#include "ObjManager.h"
#include "CMSprite.h"

typedef void (*FuncPtr0)(void);         // 関数ポインタの型定義

// シーンの登録ワーク型
typedef struct
 {
    FuncPtr0 Init;
    FuncPtr0 Update;
    FuncPtr0 Draw;
    FuncPtr0 VSync;
    FuncPtr0 Clear;
} SSceneWork, *pSSceneWork;

// 最大シーン数
//#define SCENE_MAX 10

// シーンID
enum
{
    SCENE_ID_TITLE = 0,
    SCENE_ID_GAME,
    SCENE_ID_RESULT,
    SCENE_ID_MAX
};

// プロトタイプ宣言
void SceneManager_Init(void);
void SceneManager_ChangeScene(int);
void SceneManager_Regist(pSSceneWork scene);
void Scene_Init(void);
void Scene_Update(void);
void Scene_Draw(void);
void Scene_VSync(void);
void Scene_Clear(void);

#endif // __SCENEMANAGER_H__
