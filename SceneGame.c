// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"

// ゲームシーン　初期化
void Game_Init(void)
{
    printf("Game_Init() ");
}

// ゲームシーン　更新
void Game_Update(void)
{
    printf("Game_Update() ");
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
    printf("Game_Draw()\n");
}

// ゲームシーン　VSync
void Game_VSync(void)
{
    printf(" Game_VSync()\n");
}

// ゲームシーン　クリア（終了）
void Game_Clear(void)
{
    printf("**Game_Clear()\n");
}
