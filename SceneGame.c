// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、ゲームシーンのコード本体 である。
// Path: SceneGame.c
//

#include "SceneGame.h"
#include "ObjManager.h"
#include "BMPLoad256.h"

#define PAUSE_X 13                      // ポーズの表示位置Ｘ
#define PAUSE_Y 15                      // ポーズの表示位置Ｙ

#define DEADLINE_Y  232                 // 敵の侵略位置

#define ENEMY_SETUP_COL   11            // 敵の初期配置列数
#define ENEMY_SETUP_ROW   5             // 敵の初期配置行数

#define BGDRAW_FLG_SCORE 0x0001         // スコアの書き換えフラグ
#define BGDRAW_FLG_HISCORE 0x0002       // ハイスコアの書き換えフラグ
#define BGDRAW_FLG_LEFT 0x0004          // 残機の書き換えフラグ
#define BGDRAW_FLG_STAGE 0x0008         // ステージの書き換えフラグ
#define BGDRAW_FLG_GAMEOVER 0x0010      // ゲームオーバー書き換えフラグ

extern pBMPFILE256 pBmpBackGround;	    // BMPファイルデータ（ゲーム背景）

int score = 0;                          // スコア
int hiscore = 0;                        // ハイスコア

static BOOL bPause = FALSE;             // ポーズフラグ
static uint32 bgDraw_flg;               // BG書き換えフラグ
static pSObj pObjPlayer;                // プレイヤーのオブジェクト
static pSObj pObjBullet;                // プレイヤーの弾のオブジェクト

static int enemy_left;                  // 敵の残り数

// ゲーム状態の管理用
enum
{
    STATUS_NORMAL,                      // （通常）ゲーム中
    STATUS_INVASION,                    // 侵略
    STATUS_MISS,                        // ミス（自機やられ）
    STATUS_CLEAR,                       // ステージクリア
    STATUS_GAMEOVER,                    // ゲームオーバー
    STATUS_MAX                          // 状態の最大値
};
static int status;                      // ゲーム状態
static int status_count;                // 状態遷移カウンタ

// 敵の動作　管理用
static int enemy_move_idx;              // 敵の動作インデックス
static uint16 enemy_touch_wall;         // 敵が壁に触れたフラグ
static int stage;                       // ステージ
static int player_left;                 // 残機

// 攻撃する敵Objが入っているテーブル
static pSObj enemy_atack_tbl[ENEMY_SETUP_COL];

// プロトタイプ宣言
static void SetPause(BOOL);
static void addScore(int);
static void setupEnemies(void);
static void to_next_stage(void);
static void obj_clear_all(void);
static void obj_clear_ebullet(void);
static void initStage(void);
static void readyPlayer(void);
static void setGameOver(void);
static void setPlayerMiss(void);
static void enemiesMoveDown(int);

/// @brief ゲームシーン　初期化
void Game_Init(void)
{
    OBJManager_Init();          // オブジェクトマネージャーの初期化
    CM_sprite_clr();            // スプライトクリア
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR
    // グラフィック画面０のクリア
    gcls(0);
    // ビットマップファイルの表示
    PutBMPMemory256(pBmpBackGround);

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

    // プレイヤーの生成
    readyPlayer();
    initStage();                // ステージの初期化
}

/// @brief 敵の弾を発射する
static void EBullet_Make(void)
{
    pSObj pObj = NULL;
    pSObj pEBullet = NULL;
    // 敵の弾を発射
    int col = rand() % ENEMY_SETUP_COL;
    for (int i=0; i<ENEMY_SETUP_COL; i++)
    {
        pObj = enemy_atack_tbl[col];
        if (pObj == NULL)
        {
            col++;
            col %= ENEMY_SETUP_COL;
            continue;
        }
        else
        {
            pObj = enemy_atack_tbl[col];
            break;
        }
    }
    if (pObj == NULL) return;               // 敵が存在しなければリターン

    // 敵弾を生成
//    printf("EBullet_Make: x=%d, y=%d\n",pObj->x, pObj->y+8);
    pEBullet = ObjManager_Make(OBJ_ID_EBULLET, pObj->x, pObj->y+8);
}

/// @brief ゲームシーン　更新
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
        // 敵の動作を更新する
        switch (pObj->stat)
        {
            // 通常動作
            case 0:
                pObj->x += (pObj->vx * 4);
                if (pObj->vx < 0 && pObj->x <= 12)
                {
                    // 左端に到達したら
                    pObj->x = 12;
                    enemy_touch_wall |= 0x01;   // 左壁に触れたフラグをセット
                }
                if (pObj->vx > 0 && pObj->x >= 240)
                {
                    // 右端に到達したら
                    pObj->x = 240;
                    enemy_touch_wall |= 0x02;   // 右壁に触れたフラグをセット
                }
                break;

            // 下に移動して左右反転
            case 1:
                // 下に移動
                // 動作反転
                pObj->vx = -pObj->vx;
                pObj->y += pObj->vy;
                pObj->stat = 0;
                break;

            // 到達しない
            default:
                __UNREACHABLE__;
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
        // デバッグ表示
//        char strtmp[128];
//        sprintf(strtmp, "NEXT_IDX=%3d", next_idx);
//        CM_bg_puts(strtmp, 0, 1, 1);
        if (next_idx >= 0)
        {
            // 乱数で敵のショットを発射
            if ((rand() % 256) < 5)
            {
                // 5/256の確率で敵弾を発射
                EBullet_Make();
            }

            // 次のキャラがいたら
            if (next_idx <= enemy_move_idx)
            {
                // 全てのキャラが動き終わったら

                // 動作反転チェック
                if (enemy_touch_wall & 0x01)
                {
                    // 左壁に触れていたら
                    enemiesMoveDown(8);
                    enemy_touch_wall &= ~0x01;
                }

                if (enemy_touch_wall & 0x02)
                {
                    // 右壁に触れていたら
                    enemiesMoveDown(8);
                    enemy_touch_wall &= ~0x02;
                }
            }
        }
        enemy_move_idx = next_idx;
        break;
    case STATUS_INVASION:
        // 侵略
        if ((--status_count) <= 0)
        {
            // 一定フレーム経過したらゲームオーバー
            status = STATUS_GAMEOVER;
            status_count = 60*3;
        }
        break;
    case STATUS_MISS:
        // 侵略
        if ((--status_count) <= 0)
        {
            // 残機減らす
            player_left--;
            bgDraw_flg |= BGDRAW_FLG_LEFT;
            if (player_left <= 0)
            {
                // プレイヤーの残機がなくなった
                // ゲームオーバー状態へ
                setGameOver();
            }
            else
            {
                // 一定フレーム経過したらリトライ
                status = STATUS_NORMAL;
                // リトライ処理
                readyPlayer();      // プレイヤー作成
                obj_clear_ebullet();    // 敵弾を消す
            }
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
/*
    // デバッグ用
    if (pad_trg & PAD_A)
    {
        // Aボタンでタイトルシーンへ遷移
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
 */
}

/// @brief ゲームシーン　描画
void Game_Draw(void)
{
    ObjManager_Draw();      // オブジェクトマネージャーの描画
}

/// @brief ゲームシーン　VSync
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
}

/// @brief ゲームシーン　クリア（終了）
void Game_Clear(void)
{
    ObjManager_End();       // オブジェクトマネージャーの終了
}

// ObjManagerから呼ばれる関数は、SceneGame 内に移動した。

/////////////////////////////////
// プレイヤーの更新処理
/////////////////////////////////
/// @brief プレイヤーの更新処理
/// @param pObj プレイヤーのオブジェクト
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
    if (pad_trg & (PAD_B | PAD_A) )
    {
        // A,Bボタンで弾発射
        if (pObjBullet == NULL)
        {
            pObjBullet = ObjManager_Make(OBJ_ID_PBULLET,
                pObj->x, pObj->y-2);
        }
    }

}

/////////////////////////////////
/// @brief 攻撃した列の最下行の敵を探す
/////////////////////////////////
/// @param row 攻撃した行
/// @param col 攻撃した列
static pSObj SearchNextAttackEnemy(int row, int col)
{
    pSObj pObj = NULL;
    int _row = -1;
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObjEnemy = ObjManager_GetObj(i);
        if (pObjEnemy->id >= OBJ_ID_ENEMY1 &&
            pObjEnemy->id <= OBJ_ID_ENEMY3)
        {
            if (pObjEnemy->col == col)
            {
                if (pObjEnemy->row > _row)
                {
                    // 一番下の敵を見つけた
                    pObj = pObjEnemy;
                    _row = pObjEnemy->row;
                }
            }
        }
    }
    return pObj;
}


/////////////////////////////////
/// @brief 自機弾の更新処理
/////////////////////////////////
/// @param pObj 自機弾のオブジェクト
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
                    pts = 10;
                    break;
                case OBJ_ID_ENEMY2:
                    pts = 20;
                    break;
                case OBJ_ID_ENEMY3:
                    pts = 30;
                    break;
                default:
                    // ここには来ないはず
                    __UNREACHABLE__;
                }
                // 消滅させる敵が攻撃する敵だったら？
                int col = pObjEnemy->col;
                int row = pObjEnemy->row;
                ObjManager_Destroy(pObjEnemy);  // 敵を消滅
                if (enemy_atack_tbl[col] == pObjEnemy)
                {
                    // 攻撃する敵を消滅させたら、
                    pSObj pObjTmp = SearchNextAttackEnemy(row, col);
                    // 同じ列の次の敵を攻撃する敵にする
                    enemy_atack_tbl[col] = pObjTmp;
/*
                    if (pObjTmp != NULL)
                    {
                        // 攻撃する敵がいたら、情報更新
                        char strtmp[128];
                        sprintf(strtmp, "NEXT_ATK=%2d %2d", pObjTmp->col, pObjTmp->row);
                        CM_bg_puts(strtmp, 0, 1, 1);
                    }
                    else
                    {
                        CM_bg_puts("NEXT_ATK=NONE   ", 0, 1, 1);
                    }
 */
                }

                //
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
void ObjFunc_EBullet(pSObj pObj)
{
    pObj->y += 1;           // 下に移動
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
        // プレイヤーミス処理へ
        setPlayerMiss();
    }
}


/////////////////////////////////
/// @brief 敵爆発エフェクトの更新処理
/////////////////////////////////
/// @param pObj 敵爆発エフェクトのオブジェクト
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
    enemy_left = 0;             // 敵の残り数
    int sy = 24;

    sy += ((stage-1) * 8);      // ステージによって初期Y位置を変える
//    sy = 112;
    if (sy > 112) sy = 112;     // 下限を設ける

    for (int i = 0; i < ENEMY_SETUP_ROW; i++)
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
        for (int j = 0; j < ENEMY_SETUP_COL; j++)
        {
            int16 _x = (j * 20) + 24;
            int16 _y = (i * 24) + sy;

            pSObj pObj = ObjManager_Make(_id, _x, _y);
            pObj->row = i;      // 敵の行番号
            pObj->col = j;      // 敵の列番号
            pObj->stat = 0;     // 敵の状態

            if (i == (ENEMY_SETUP_ROW-1))
            {
                // 最下段の敵のみ弾発射
                enemy_atack_tbl[j] = pObj;
            }

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
            pObj->stat = 1; // 下降フラグを立てる
            pObj->vy = add; // 下降ドット数を設定
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
    // 敵の初期配置
    setupEnemies();

    // 敵の動作インデックスを初期化
    enemy_move_idx = ObjManager_FindEnemyIdx();
    enemy_touch_wall = 0;       // 敵が壁に触れたフラグをクリア

    // ゲーム状態の初期化
    status = STATUS_NORMAL;
}

//////////////////////////////////////
/// @brief プレイヤーの初期化
//////////////////////////////////////
static void readyPlayer(void)
{
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
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
/// @brief 敵の弾の全消去
//////////////////////////////////////
static void obj_clear_ebullet(void)
{
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        if (pObj->id == OBJ_ID_EBULLET)
        {
            ObjManager_Destroy(pObj);
        }
    }
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

//////////////////////////////////////
/// @brief プレイヤーがミスした
//////////////////////////////////////
static void setPlayerMiss(void)
{
    // ミス状態へ
    status_count = 180;
    status = STATUS_MISS;
}