// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ＯＢＪマネージャーのヘッダファイル である。
//
#pragma once
#ifndef __OBJMANAGER_H__
#define __OBJMANAGER_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <doslib.h>
#include <iocslib.h>
#include "types.h"

#include "GamePadManager.h"
#include "SceneManager.h"
#include "CMSprite.h"

// オブジェクトの最大数
#define OBJ_MAX 128

// switch-case で到達しないマクロ
#ifdef __GNUC__
#define __UNREACHABLE__ __builtin_unreachable()
#endif

#ifdef _MSC_VER
#define __UNREACHABLE__ __assume(0)
#endif

// オブジェクトIDの定義
enum
{
    OBJ_ID_EMPTY = 0,
    OBJ_ID_PLAYER,
    OBJ_ID_PBULLET,
    OBJ_ID_EBULLET,
    OBJ_ID_ENEMY1,
    OBJ_ID_ENEMY2,
    OBJ_ID_ENEMY3,
    OBJ_ID_EEFFECT,
    OBJ_ID_PEFFECT,
    OBJ_ID_MAX
};

// 構造体の定義

/// @brief オブジェクトの基本構造体
/// @note 128バイトになるようにパディングを入れている

typedef struct _tObj SObj;
typedef void (*ObjFuncPtr)(SObj *);

/// @brief オブジェクトの基本構造体
/// @note 全部のオブジェクトでこの構造体を使用する
struct _tObj
{
    uint16 id;              // オブジェクトID
    int16 x;                // X座標
    int16 y;                // Y座標
    uint16 pat;             // パターン番号
    uint16 plane;           // プレーン番号
    uint16 pad0;
    ObjFuncPtr Update;      // 更新関数
    ObjFuncPtr Draw;        // 描画関数
    uint16 anm_spd;         // アニメーション速度
    uint16 anm_num;         // アニメーション枚数
    uint16 anm_cou;         // アニメーショタイミングカウンタ
    uint16 anm_idx;         // アニメーションインデックス

    uint8  padding[128-28]; // 128バイトになるようにパディング
};
typedef SObj* pSObj;

extern pSObj g_pObj; // オブジェクトのポインタ

/////////////////////////////////
// ＯＢＪマネージャー：取得処理
/////////////////////////////////
/// @brief オブジェクトのポインタを取得する
/// @param i オブジェクトのインデックス
/// @retval オブジェクトのポインタ
FORCE_INLINE pSObj ObjManager_GetObj(int i)
{
    return &g_pObj[i];
}

// プロトタイプ宣言
void OBJManager_Init();
pSObj ObjManager_Make(int id, int x, int y);
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();

void ObjManager_Destroy(pSObj pObj);

int ObjManager_FindEnemyIdx(void);
int ObjManager_FindEnemyNextIdx(int);

#endif // __OBJMANAGER_H__
