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

struct _tObj
{
    uint16 id;              // オブジェクトID
    uint16 x;               // X座標
    uint16 y;               // Y座標
    uint16 pat;             // パターン番号
    uint16 plane;           // プレーン番号
    uint16 pad0;
    ObjFuncPtr Update;      // 更新関数
    ObjFuncPtr Draw;        // 描画関数

    uint8  padding[128-20]; // 128バイトになるようにパディング
};
typedef SObj* pSObj;

/// @brief プレイヤーの構造体
typedef struct
{
    uint16 id;              // オブジェクトID
    uint16 x;               // X座標
    uint16 y;               // Y座標
    uint16 pat;             // パターン番号
    uint16 plane;           // プレーン番号
    uint16 pad0;

    ObjFuncPtr Update;      // 更新関数
    ObjFuncPtr Draw;        // 描画関数

    uint8  padding[128-20]; // 128バイトになるようにパディング
} SObjPlayer, *pSObjPlayer;

/// @brief プレイヤー弾の構造体
typedef struct
{
    uint16 id;              // オブジェクトID
    uint16 x;               // X座標
    uint16 y;               // Y座標
    uint16 pat;             // パターン番号
    uint16 plane;           // プレーン番号
    uint16 pad0;

    ObjFuncPtr Update;      // 更新関数
    ObjFuncPtr Draw;        // 描画関数

    uint8  padding[128-20]; // 128バイトになるようにパディング
} SObjPBullet, *pSObjPBullet;

// プロトタイプ宣言
void OBJManager_Init();
pSObj ObjManager_Make(int id, int x, int y);
void ObjManager_Update();
void ObjManager_Draw();
void ObjManager_End();



void ObjManager_Destroy(pSObj pObj);


#endif // __OBJMANAGER_H__
