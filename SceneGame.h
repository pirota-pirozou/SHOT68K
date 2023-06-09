// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのヘッダファイル である。
//
#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "SceneManager.h"

// プロトタイプ宣言
void Game_Init(void);
// ゲームシーン　更新
void Game_Update(void);
// ゲームシーン　描画
void Game_Draw(void);
// ゲームシーン　VSync
void Game_VSync(void);
// ゲームシーン　クリア（終了）
void Game_Clear(void);

// オブジェクトの更新
void ObjFunc_Player(pSObj pObj);
void ObjFunc_PBullet(pSObj pObj);
void ObjFunc_EBullet(pSObj pObj);
void ObjFunc_EEffect(pSObj pObj);



#endif // __SCENE_GAME_H__
