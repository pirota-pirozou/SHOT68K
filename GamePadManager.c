// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームパッドマネージャーのソースファイル である。
// GamePadManager.c
//
// このコードは、ゲームパッドの入力を管理する。
//
#include "GamePadManager.h"

// ゲームパッドの値
static unsigned short pad;
static unsigned short pad_bak;
static unsigned short pad_trg;

// ゲームパッドマネージャー　初期化
void GamePadManager_Init(void)
{
    printf("GamePadManager_Init()\n");

    pad = pad_bak = pad_trg = 0;
}

// ゲームパッドマネージャー　更新
void GamePadManager_Update(void)
{
//    printf("GamePadManager_Update()\n");

    // ゲームパッドの値を取得
    pad = (unsigned short) gamepad();

    // ボタンが押された一瞬の状態を記録
    int tmppad = pad;
    pad_trg = ~pad_bak & tmppad;
    pad_bak = tmppad;
}

// ゲームパッドマネージャー　パッド状態取得
unsigned short GamePadManager_GetPad(void)
{
//    printf("GamePadManager_GetPad()\n");
    return pad;
}

// ゲームパッドマネージャー　padのトリガー状態を取得
unsigned short GamePadManager_GetTrigger(void)
{
//    printf("GamePadManager_GetTrigger()\n");
    return pad_trg;
}
