// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、タイトルシーンのヘッダファイル である。
//
#pragma once
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "SceneManager.h"

// プロトタイプ宣言
void Title_Init(void);
// タイトルシーン　更新
void Title_Update(void);
// タイトルシーン　描画
void Title_Draw(void);
// タイトルシーン　VSync
void Title_VSync(void);
// タイトルシーン　クリア（終了）
void Title_Clear(void);

#endif // __SCENE_TITLE_H__
