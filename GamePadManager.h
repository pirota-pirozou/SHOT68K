// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームパッドマネージャーのヘッダファイル である。
// GamePadManager.h
#pragma once
#ifndef __GAMEPADMANAGER_H__
#define __GAMEPADMANAGER_H__

#include <stdio.h>
#include "CF_MACRO.h"
#include "types.h"
#include "mylib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16 pad;
extern uint16 pad_trg;

// マクロ定義や関数プロトタイプ宣言をここに記述する
void GamePadManager_Init(void);
void GamePadManager_Update(void);

// ゲームパッドマネージャー　パッド状態取得
FORCE_INLINE uint16 GamePadManager_GetPad(void)
{
    return pad;
}

// ゲームパッドマネージャー　padのトリガー状態を取得
FORCE_INLINE uint16 GamePadManager_GetTrigger(void)
{
    return pad_trg;
}

#ifdef __cplusplus
}
#endif

#endif // __GAMEPADMANAGER_H__
