// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"

// ゲームシーン　初期化
void Game_Init(void)
{
//    printf("Game_Init() ");
    OBJManager_Init();      // オブジェクトマネージャーの初期化
    BGTEXTCL(1, 0x0100);	// BGTEXT1 CLR
}

// ゲームシーン　更新
void Game_Update(void)
{
//    printf("Game_Update() ");
    CM_bg_puts("GAME_UPDATE()", 0, 0, 1);
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if (pad_trg & PAD_A)
    {
        // Aボタンでタイトルシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// ゲームシーン　描画
void Game_Draw(void)
{
//    printf("Game_Draw()\n");
    CM_bg_puts("GAME_DRAW()", 0, 1, 1);
    CM_sp_set(0, 50, 50, 0x0141, 3);
//					*ｄ１＝スプライトプレーン番号（０～１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

// ゲームシーン　VSync
void Game_VSync(void)
{
    CM_bg_puts("GAME_VSYNC()", 0, 2, 1);
//    printf(" Game_VSync()\n");
}

// ゲームシーン　クリア（終了）
void Game_Clear(void)
{
    ObjManager_End();       // オブジェクトマネージャーの終了
}
