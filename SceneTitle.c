// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、タイトルシーンのコード本体 である。
// Path: SceneTitle.c
//

#include "SceneTitle.h"

// タイトルシーン　初期化
void Title_Init(void)
{
    printf("Title_Init() ");
}

// タイトルシーン　更新
void Title_Update(void)
{
    printf(" Title_Update() ");
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if (pad_trg & PAD_A)
    {
        // Aボタンでゲームシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_GAME);
    }
}

// タイトルシーン　描画
void Title_Draw(void)
{
    printf("Title_Draw()\n");
    CM_sp_set(0, 50, 50, 0x0140, 3);
//					*ｄ１＝スプライトプレーン番号（０～１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

// タイトルシーン　VSync
void Title_VSync(void)
{
    printf(" Title_VSync()\n");
}

// タイトルシーン　クリア（終了）
void Title_Clear(void)
{
    printf("**Title_Clear()\n");
}
