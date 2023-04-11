// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームパッドマネージャーのソースファイル である。
// GamePadManager.c
//
// このコードは、ゲームパッドの入力を管理する。
//

#include "GamePadManager.h"

// ゲームパッドの値
uint16 pad;
uint16 pad_trg;
static uint16 pad_bak;

// ゲームパッドマネージャー　初期化
void GamePadManager_Init(void)
{
    pad = pad_bak = pad_trg = 0;
}

// ゲームパッドマネージャー　更新
void GamePadManager_Update(void)
{
    // ゲームパッドの値を取得
    pad = (uint16) gamepad();

    // ボタンが押された一瞬の状態を記録
    uint16 tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}
