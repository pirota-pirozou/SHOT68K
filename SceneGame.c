// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"
#include "ObjManager.h"
#include "BMPLoad256.h"

#define PAUSE_X 13
#define PAUSE_Y 15

#define DEADLINE_Y  232             // 敵の侵略位置

#define BGDRAW_FLG_SCORE 0x0001    // スコアの書き換えフラグ
#define BGDRAW_FLG_HISCORE 0x0002  // ハイスコアの書き換えフラグ
#define BGDRAW_FLG_LEFT 0x0004  // 残機の書き換えフラグ
#define BGDRAW_FLG_STAGE 0x0008  // ステージの書き換えフラグ
#define BGDRAW_FLG_GAMEOVER 0x0010  // ゲームオーバー書き換えフラグ

int score = 0;                  // スコア
int hiscore = 0;                // ハイスコア

static BOOL bPause = FALSE;     // ポーズフラグ
static uint32 bgDraw_flg;       // BG書き換えフラグ
static pSObj pObjPlayer;        // プレイヤーのオブジェクト
static pSObj pObjBullet;        // プレイヤーの弾のオブジェクト

static int enemy_left;          // 敵の残り数

// ゲーム状態の管理用
enum
{
    STATUS_NORMAL,              // （通常）ゲーム中
    STATUS_MISS,                // ミス
    STATUS_CLEAR,               // ステージクリア
    STATUS_GAMEOVER,            // ゲームオーバー
    STATUS_MAX                  // 状態の最大値
};
static int status;              // ゲーム状態
static int status_count;        // 状態遷移カウンタ

// 敵の動作　管理用
static int enemy_move_idx;      // 敵の動作インデックス
static int16 enemy_move_vx;     // 敵の動作方向
static int16 enemy_wall_touch;  // 敵の壁接触フラグ
static int stage;               // ステージ
static int player_left;         // 残機

// プロトタイプ宣言
static void SetPause(BOOL);
static void addScore(int);
static void setupEnemies(void);
static void to_next_stage(void);
static void obj_clear_all(void);
static void initStage(void);
static void setGameOver(void);
static void enemiesMoveDown(int);

// ゲームシーン　初期化
void Game_Init(void)
{
    OBJManager_Init();          // オブジェクトマネージャーの初期化
    CM_sprite_clr();            // スプライトクリア
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR

    // ビットマップファイルの読み込み
    PutBMP256("space.bmp");

    CM_bg_puts("SCORE", 0, 0, 1);
    CM_bg_puts("HI", 19, 0, 1);
    CM_bg_puts("LEFT", 0, 31, 1);
    CM_bg_puts("STAGE", 31-9, 31, 1);
    bPause = FALSE;             // ポーズフラグをクリア
    score = 0;                  // スコアをクリア
    stage = 1;                  // ステージは１
    player_left = 3;            // 残機は３
    pObjBullet = NULL;          // プレイヤーの弾のオブジェクトをクリア
    // BG書き換えフラグをセット
    bgDraw_flg = BGDRAW_FLG_SCORE | BGDRAW_FLG_HISCORE | BGDRAW_FLG_LEFT | BGDRAW_FLG_STAGE;

    // ゲーム状態を初期化
    status = STATUS_NORMAL;
    status_count = 0;

    // 敵の動作を初期化
    enemy_move_vx = 1;
    enemy_wall_touch = 0;

    // プレイヤーの生成
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
    // 敵の生成
    setupEnemies();

    // 敵の動作インデックスを初期化
    enemy_move_idx = ObjManager_FindEnemyIdx();
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

    ObjManager_Update();    // オブジェクトマネージャーの更新

    // ゲーム状態の更新
    switch (status)
    {
    case STATUS_NORMAL:
        // （通常）ゲーム中
        if (enemy_move_idx < 0) break;
        // 敵の動作を更新。１キャラずつ動かす
        pSObj pObj = ObjManager_GetObj(enemy_move_idx);
        // TODO: 敵の動作を更新する
        pObj->x += (enemy_move_vx * 4);
//        pObj->x +=(pObj->vx * 4);
        if (pObj->x < 8)
        {
            // 左端に到達したら
            pObj->x = 8;
            // 動作反転
//            pObj->vx = 1;
            enemy_move_vx = 1;
            enemy_wall_touch = 1;
        }
        else if (pObj->x > 240)
        {
            // 右端に到達したら
            pObj->x = 240;
            // 動作反転
//            pObj->vx = -1;
            enemy_move_vx = -1;
            enemy_wall_touch = 1;
        }
        else
        {
            // 壁に接触していない
//            enemy_wall_touch = 0;
        }
        // キャラ絵切替
        pObj->anm_idx ^= 1;
        // もし敵が地面に降りたら
        if (pObj->y >= DEADLINE_Y)
        {
            // プレイヤー爆破
            ObjManager_Make(OBJ_ID_PEFFECT, pObjPlayer->x, pObjPlayer->y);
            // プレイヤーを消す
            ObjManager_Destroy(pObjPlayer);
            // ゲームオーバーへ
            setGameOver();
            break;
        }
        // 次のキャラへ
        int next_idx = ObjManager_FindEnemyNextIdx(enemy_move_idx);
        if (next_idx < enemy_move_idx)
        {
            // 全ての敵の動作が終わったら
            if (enemy_wall_touch)
            {
                // 壁に接触したら
                // 敵全体を下に移動
                enemy_wall_touch = 0;
//                enemiesMoveDown(8);
            }
        }
        else
        {
            // 壁にタッチしたらキャラを下に移動
            if (enemy_wall_touch)
            {
                pObj->y += 8;
            }

        }
        enemy_move_idx = next_idx;
        break;
    case STATUS_MISS:
        // ミス
        if ((--status_count) <= 0)
        {
            // 一定フレーム経過したらゲームオーバー
            status = STATUS_GAMEOVER;
            status_count = 60*3;
        }
        break;
    case STATUS_CLEAR:
        // ステージクリア
        if ((--status_count) <= 0)
        {
            // 一定フレーム経過したら次のステージへ遷移
            to_next_stage();
        }
        break;
    case STATUS_GAMEOVER:
        // ゲームオーバー
        if ((--status_count) <= 0)
        {
            //  一定フレーム経過したらタイトルシーンへ遷移
            SceneManager_ChangeScene(SCENE_ID_TITLE);
        }
        break;
    default:
        // ここには来ない
        __UNREACHABLE__;
    }

    // デバッグ用
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
        CM_bg_puts(strtmp, 6, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_SCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_HISCORE)
    {
        // ハイスコアの書き換え
        sprintf(strtmp, "%08d", hiscore);
        CM_bg_puts(strtmp, 22, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_HISCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_LEFT)
    {
        // 残機の書き換え
        int left = player_left - 1;
        if (left < 0) left = 0;
        sprintf(strtmp, "%02d", left);
        CM_bg_puts(strtmp, 5, 31, 1);
        bgDraw_flg &= ~BGDRAW_FLG_LEFT;
    }
    if (bgDraw_flg & BGDRAW_FLG_STAGE)
    {
        // 残機の書き換え
        sprintf(strtmp, "%02d", stage);
        CM_bg_puts(strtmp, 28, 31, 1);
        bgDraw_flg &= ~BGDRAW_FLG_STAGE;
    }
    if (bgDraw_flg & BGDRAW_FLG_GAMEOVER)
    {
        // ゲームオーバーの書き換え
        CM_bg_puts("GAME OVER", 11, 16, 1);
        bgDraw_flg &= ~BGDRAW_FLG_GAMEOVER;
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
    if ((pad & PAD_START)==PAD_START)
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
/// @return なし
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

            if (cx < 8 && cy < 8)
            {
                // 当たった
                ObjManager_Destroy(pObj);      // 自機弾を消滅
                pObjBullet = NULL;             // 出現フラグもクリア
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
                ObjManager_Destroy(pObjEnemy);  // 敵を消滅
                addScore(pts);                  // スコア加算
                enemy_left--;                   // 敵の残数を減らす
                if (enemy_left <= 0)
                {
                    // 敵の残り数が0になったらステージクリア
                    status = STATUS_CLEAR;
                    status_count = 60*3;        // 画面遷移までの時間
                }
                break;
            }
        }
    }

}

/////////////////////////////////
/// @brief 敵弾の更新処理
/////////////////////////////////
/// @param pObj 敵弾のオブジェクト
/// @return なし
void ObjFunc_EBullet(pSObj pObj)
{
    pObj->y += 4;           // 下に移動
    if (pObj->y >= 248)
    {
        // 画面外に出たら消滅
        ObjManager_Destroy(pObj);
        return;
    }

    // 自機と敵弾との当たり判定
    if (pObjPlayer == NULL)
    {
        // 自機が消滅している場合は当たり判定しない
        return;
    }

    // 敵との当たりチェック
    int16 cx = abs(pObj->x - pObjPlayer->x);
    int16 cy = abs(pObj->y - pObjPlayer->y);

    if (cx < 12 && cy < 12)
    {
        // 当たった
        //  自機爆発エフェクト
        ObjManager_Make(OBJ_ID_PEFFECT,
            pObjPlayer->x, pObjPlayer->y);
        ObjManager_Destroy(pObj);      // 敵弾を消滅
        ObjManager_Destroy(pObjPlayer); // プレイヤーを消滅
        pObjPlayer = NULL;             // プレイヤーフラグもクリア
    }
}


/////////////////////////////////
/// 敵爆発エフェクトの更新処理
/////////////////////////////////
/// @brief 敵爆発エフェクトの更新処理
/// @param pObj 敵爆発エフェクトのオブジェクト
/// @return なし
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
/// @return なし
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
/// @return なし
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

//////////////////////////////////////
/// @brief 敵の初期配置
//////////////////////////////////////
static void setupEnemies(void)
{
    // 敵の初期配置
    enemy_left = 0;
    for (int i = 0; i < 5; i++)
    {
        uint16 _id;
        switch (i)
        {
        case 0:
            _id = OBJ_ID_ENEMY3;
            break;
        case 1:
        case 2:
            _id = OBJ_ID_ENEMY2;
            break;
        case 3:
        case 4:
            _id = OBJ_ID_ENEMY1;
            break;
        default:
            __UNREACHABLE__;
        }
        for (int j = 0; j < 11; j++)
        {
            int16 _x = (j * 20) + 24;
            int16 _y = (i * 24) + 24;

            pSObj pObj = ObjManager_Make(_id, _x, _y);
            enemy_left++;
        }
    }
}

//////////////////////////////////////
/// @brief 敵全体を下に下降させる
//////////////////////////////////////
/// @param add 下降ドット数
static void enemiesMoveDown(int add)
{
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        if (pObj->id == OBJ_ID_ENEMY1 ||
            pObj->id == OBJ_ID_ENEMY2 ||
            pObj->id == OBJ_ID_ENEMY3)
        {
            pObj->y += add;
        }
    }
}

//////////////////////////////////////
/// @brief 次のステージへ
//////////////////////////////////////
static void to_next_stage(void)
{
    stage++;
    if (stage > 99) stage = 99;

    bgDraw_flg |= BGDRAW_FLG_STAGE;

    // ステージの初期化
    initStage();
}

//////////////////////////////////////
/// @brief ステージの初期化
//////////////////////////////////////
static void initStage(void)
{
    // オブジェクトの全消去
     obj_clear_all();

    // 新たにプレイヤーの生成
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);

    // 敵の初期配置
    setupEnemies();

    // ゲーム状態の初期化
    status = STATUS_NORMAL;

    // 敵の動作を初期化
    enemy_move_vx = 1;
    enemy_wall_touch = 0;

}

//////////////////////////////////////
/// @brief オブジェクトの全消去
//////////////////////////////////////
static void obj_clear_all(void)
{
    // オブジェクトの全消去
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        ObjManager_Destroy(pObj);
    }
    // 自機弾のポインタをクリア
    pObjBullet = NULL;
}

//////////////////////////////////////
/// @brief ゲームオーバー状態へ
//////////////////////////////////////
static void setGameOver(void)
{
    // ゲームオーバー状態へ
    status_count = 240;
    status = STATUS_GAMEOVER;
    player_left = 0;
    bgDraw_flg |= (BGDRAW_FLG_GAMEOVER | BGDRAW_FLG_LEFT);
}