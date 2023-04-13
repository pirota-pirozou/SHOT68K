// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、タイトルシーンのコード本体 である。
// Path: SceneTitle.c
//

#include "SceneTitle.h"
#include "BMPLoad256.h"

extern int score;                  // スコア
extern int hiscore;                // ハイスコア

extern pBMPFILE256 pBmpTitle;	    // BMPファイルデータ（タイトル）

static int cou;                    // カウンタ

// タイトルシーン　初期化
void Title_Init(void)
{
    char strtmp[128];

    CM_sprite_clr();            // スプライトクリア
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR

    // グラフィック画面０のクリア
    gcls(0);

    sprintf(strtmp, "SCORE %08d", score);
    CM_bg_puts(strtmp, 0, 0, 1);
    sprintf(strtmp, "HI %08d", hiscore);
    CM_bg_puts(strtmp, 19, 0, 1);

    CM_bg_puts("<SAMPLE GAME:SHOT68K>", 4,  6, 1);
    CM_bg_puts("PROGRAMMED BY PIROTA.", 4, 29, 1);

    cou = 0;
}

// タイトルシーン　更新
void Title_Update(void)
{
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if ( (pad_trg & (PAD_A|PAD_B)) ||
         ((pad_trg & PAD_START) == PAD_START) )
    {
        // A,B,STARTボタンでゲームシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_GAME);
    }

    // カウンタをインクリメント
    cou++;
}

// タイトルシーン　描画
void Title_Draw(void)
{
//    CM_bg_puts("TITLE_DRAW()", 0, 2, 1);

//    CM_sp_set(0, 50, 50, 0x0140, 3);
//					*ｄ１＝スプライトプレーン番号（０〜１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

// タイトルシーン　VSync
void Title_VSync(void)
{
    if ((cou & 32)==0)
    {
        CM_bg_puts("PRESS BUTTON TO START.", 4, 16, 1);
    }
    else
    {
        CM_bg_puts("                      ", 4, 16, 1);
    }

}

// タイトルシーン　クリア（終了）
void Title_Clear(void)
{
//    printf("**Title_Clear()\n");
}
