// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"

#define PAUSE_X 13
#define PAUSE_Y 15

#define BGDRAW_FLG_SCORE 0x0001    // スコアの書き換えフラグ
#define BGDRAW_FLG_HISCORE 0x0002  // ハイスコアの書き換えフラグ

int score = 0;                  // スコア
int hiscore = 0;                // ハイスコア

static BOOL bPause = FALSE;     // ポーズフラグ
static uint32 bgDraw_flg;       // BG書き換えフラグ
static pSObj pObjPlayer;        // プレイヤーのオブジェクト

// プロトタイプ宣言
static void SetPause(BOOL);
static void addScore(int);

// ゲームシーン　初期化
void Game_Init(void)
{
    OBJManager_Init();      // オブジェクトマネージャーの初期化
    BGTEXTCL(1, 0x0100);	// BGTEXT1 CLR
    CM_bg_puts("SCORE", 0, 0, 1);
    CM_bg_puts("HI", 19, 0, 1);
    bPause = FALSE;         // ポーズフラグをクリア
    score = 0;              // スコアをクリア
    bgDraw_flg = BGDRAW_FLG_SCORE | BGDRAW_FLG_HISCORE; // BG書き換えフラグをセット

    // プレイヤーの生成
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
}

// ゲームシーン　更新
void Game_Update(void)
{
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if ((pad_trg & PAD_START)==PAD_START)
    {
        // STARTボタンでポーズ
        SetPause(!bPause);
    }

    if (bPause) return;     // ポーズ中は処理しない

    CM_bg_puts("GAME_UPDATE()", 0, 1, 1);
    ObjManager_Update();    // オブジェクトマネージャーの更新

    if (pad_trg & PAD_B)
    {
        // Bボタンでスコアを加算テスト
        addScore(100);
    }
    if (pad_trg & PAD_A)
    {
        // Aボタンでタイトルシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// ゲームシーン　描画
void Game_Draw(void)
{
    CM_bg_puts("GAME_DRAW()", 0, 2, 1);
    ObjManager_Draw();      // オブジェクトマネージャーの描画
//    CM_sp_set(0, 16, 16, 0x0141, 3);

}

// ゲームシーン　VSync
void Game_VSync(void)
{
    char strtmp[128];
    if (bgDraw_flg & BGDRAW_FLG_SCORE)
    {
        // スコアの書き換え
        sprintf(strtmp, "%08d", score);
        CM_bg_puts(strtmp, 7, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_SCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_HISCORE)
    {
        // ハイスコアの書き換え
        sprintf(strtmp, "%08d", hiscore);
        CM_bg_puts(strtmp, 22, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_HISCORE;
    }
    CM_bg_puts("GAME_VSYNC()", 0, 3, 1);
}

// ゲームシーン　クリア（終了）
void Game_Clear(void)
{
    ObjManager_End();       // オブジェクトマネージャーの終了
}

//////////////////////////////////////
// ポーズフラグの設定
/// @param[in] flg ポーズフラグ
/// @retval なし
//////////////////////////////////////
static void SetPause(BOOL flg)
{
    bPause = flg;
    if (bPause)
    {
        CM_bg_puts("PAUSE", PAUSE_X, PAUSE_Y, 1);
    }
    else
    {
        CM_bg_puts("     ", PAUSE_X, PAUSE_Y, 1);
    }
}

//////////////////////////////////////
// スコアの加算
/// @param[in] pts 加算するスコア
/// @retval なし
//////////////////////////////////////
static void addScore(int pts)
{
    score += pts;
    bgDraw_flg |= BGDRAW_FLG_SCORE;
    if (score > 99999999) score = 99999999;
    if (score > hiscore)
    {
        hiscore = score;
        bgDraw_flg |= BGDRAW_FLG_HISCORE;
    }
}
