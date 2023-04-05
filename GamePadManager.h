// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームパッドマネージャーのヘッダファイル である。
// GamePadManager.h
#pragma once
#ifndef __GAMEPADMANAGER_H__
#define __GAMEPADMANAGER_H__

#include <stdio.h>
#include "mylib.h"

#ifdef __cplusplus
extern "C" {
#endif

// マクロ定義や関数プロトタイプ宣言をここに記述する
void GamePadManager_Init(void);
void GamePadManager_Update(void);
unsigned short GamePadManager_GetPad(void);
unsigned short GamePadManager_GetTrigger(void);


#ifdef __cplusplus
}
#endif

#endif // __GAMEPADMANAGER_H__
