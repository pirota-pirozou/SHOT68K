// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"
#include "ObjManager.h"

#define PAUSE_X 13
#define PAUSE_Y 15

#define BGDRAW_FLG_SCORE 0x0001    // スコアの書き換えフラグ
#define BGDRAW_FLG_HISCORE 0x0002  // ハイスコアの書き換えフラグ

int score = 0;                  // スコア
int hiscore = 0;                // ハイスコア

static BOOL bPause = FALSE;     // ポーズフラグ
static uint32 bgDraw_flg;       // BG書き換えフラグ
static pSObj pObjPlayer;        // プレイヤーのオブジェクト
static pSObj pObjBullet;        // プレイヤーの弾のオブジェクト

// プロトタイプ宣言
static void SetPause(BOOL);
static void addScore(int);

// ゲームシーン　初期化
void Game_Init(void)
{
    OBJManager_Init();          // オブジェクトマネージャーの初期化
    CM_sprite_clr();            // スプライトクリア
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR
    CM_bg_puts("SCORE", 0, 0, 1);
    CM_bg_puts("HI", 19, 0, 1);
    bPause = FALSE;             // ポーズフラグをクリア
    score = 0;                  // スコアをクリア
    pObjBullet = NULL;          // プレイヤーの弾のオブジェクトをクリア
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

//    CM_bg_puts("GAME_UPDATE()", 0, 1, 1);
    ObjManager_Update();    // オブジェクトマネージャーの更新
/*
    if (pad_trg & PAD_B)
    {
        // Bボタンでスコアを加算テスト
        addScore(100);
    }
 */
    if (pad_trg & PAD_A)
    {
        // Aボタンでタイトルシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// ゲームシーン　描画
void Game_Draw(void)
{
//    CM_bg_puts("GAME_DRAW()", 0, 2, 1);
    ObjManager_Draw();      // オブジェクトマネージャーの描画
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
//    CM_bg_puts("GAME_VSYNC()", 0, 3, 1);
}

// ゲームシーン　クリア（終了）
void Game_Clear(void)
{
    ObjManager_End();       // オブジェクトマネージャーの終了
}

// ObjManagerから呼ばれる関数は、SceneGame 内に移動した。

/////////////////////////////////
// プレイヤーの更新処理
/////////////////////////////////
// @brief プレイヤーの更新処理
// @param pObj プレイヤーのオブジェクト
void ObjFunc_Player(pSObj pObj)
{
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    // 移動判定
    int16 vx = 0;
    if ((pad_trg & PAD_START)==PAD_START)
    {
        // STARTボタンは無効
    }
    else if (pad & PAD_LEFT)
    {
        vx = -2;
    }
    else if (pad & PAD_RIGHT)
    {
        vx = 2;
    }
    // 移動
    pObj->x = clamp(pObj->x+vx, 8, 256-16-8);

    // 弾発射判定
    if (pad_trg & PAD_B)
    {
        // 自機爆発エフェクトのテスト
//         ObjManager_Make(OBJ_ID_PEFFECT,
//            pObj->x, pObj->y-32);


        // Bボタンで弾発射
        if (pObjBullet == NULL)
        {
            pObjBullet = ObjManager_Make(OBJ_ID_PBULLET,
                pObj->x, pObj->y-2);
        }
    }

}

/////////////////////////////////
/// 自機弾の更新処理
/////////////////////////////////
/// @brief 自機弾の更新処理
/// @param pObj 自機弾のオブジェクト
/// @retval なし
void ObjFunc_PBullet(pSObj pObj)
{
    pObj->y -= 8;
    if (pObj->y <= 0)
    {
        // 画面外に出たら消滅
        ObjManager_Destroy(pObj);
        pObjBullet = NULL;          // 出現フラグもクリア
    }

    // 自機弾と敵との当たり判定
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObjEnemy = ObjManager_GetObj(i);
        if (pObjEnemy->id >= OBJ_ID_ENEMY1 &&
            pObjEnemy->id <= OBJ_ID_ENEMY3)
        {
            // 敵との当たりチェック
            int16 cx = abs(pObj->x - pObjEnemy->x);
            int16 cy = abs(pObj->y - pObjEnemy->y);

            if (cx < 16 && cy < 16)
            {
                // 当たった
                ObjManager_Destroy(pObj);      // 自機弾を消滅
                pObjBullet = NULL;             // 出現フラグもクリア
                ObjManager_Destroy(pObjEnemy); // 敵を消滅
                //  敵爆発エフェクト
                ObjManager_Make(OBJ_ID_EEFFECT,
                    pObjEnemy->x, pObjEnemy->y);
                // 敵ごとのスコア加算
                int pts = 0;
                switch (pObjEnemy->id)
                {
                case OBJ_ID_ENEMY1:
                    pts = 100;
                    break;
                case OBJ_ID_ENEMY2:
                    pts = 200;
                    break;
                case OBJ_ID_ENEMY3:
                    pts = 300;
                    break;
                default:
                    // ここには来ないはず
                    __UNREACHABLE__;
                }
                addScore(pts);                  // スコア加算
                break;
            }
        }
    }

}

/////////////////////////////////
/// 敵爆発エフェクトの更新処理
/////////////////////////////////
/// @brief 敵爆発エフェクトの更新処理
/// @param pObj 敵爆発エフェクトのオブジェクト
/// @retval なし
void ObjFunc_EEffect(pSObj pObj)
{
    // アニメーション処理
    pObj->anm_cou++;
    if (pObj->anm_cou >= pObj->anm_spd)
    {
        pObj->anm_cou = 0;
        pObj->anm_idx++;
        if (pObj->anm_idx >= pObj->anm_num)
        {
            // アニメーション終了
            ObjManager_Destroy(pObj);
        }
    }
}

//////////////////////////////////////
/// @brief ポーズフラグの設定
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
