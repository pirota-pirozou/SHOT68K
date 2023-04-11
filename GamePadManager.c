// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームパッドマネージャーのソースファイル である。
// GamePadManager.c
//
// このコードは、ゲームパッドの入力を管理する。
//
#include "GamePadManager.h"

// ゲームパッドの値
static uint16 pad;
static uint16 pad_bak;
static uint16 pad_trg;

// ゲームパッドマネージャー　初期化
FORCE_INLINE void GamePadManager_Init(void)
{
    pad = pad_bak = pad_trg = 0;
}

// ゲームパッドマネージャー　更新
void GamePadManager_Update(void)
{
    // ゲームパッドの値を取得
    pad = (uint16) gamepad();

    // ボタンが押された一瞬の状態を記録
    int tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}

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
