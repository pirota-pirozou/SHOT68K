// Path: ObjManager.c
// このコードは、サンプルプログラムのＯＢＪネージャー部分 ObjManager.c である。
// ＯＢＪの更新、描画の管理を行う
//
#include "ObjManager.h"

static pSObj g_pObj = (pSObj)-1;             // オブジェクトのポインタ配列

// プロトタイプ宣言
static volatile void ObjFunc_Null(pSObj pObj);
static void ObjFunc_Draw(pSObj pObj);

static void ObjFunc_Player(pSObj pObj);


/////////////////////////////////
// ＯＢＪマネージャー自体の初期化
/////////////////////////////////
void OBJManager_Init()
{
    // オブジェクトのメモリを確保
    g_pObj = (pSObj)MALLOC(sizeof(SObj) * OBJ_MAX);
    if (g_pObj < 0)
    {
        // 確保失敗
        // 本当はここでエラー処理を行うが今回は省略
        return;
    }
    // メモリをクリア
    memset((pSObj)g_pObj, 0, sizeof(SObj) * OBJ_MAX);

    // オブジェクトの初期化
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = &g_pObj[i];
        pObj->id = OBJ_ID_EMPTY;
        pObj->plane = i;
        pObj->Update = ObjFunc_Null;
        pObj->Draw = ObjFunc_Null;
    }
}

/////////////////////////////////
// ＯＢＪマネージャー：更新処理
/////////////////////////////////
void ObjManager_Update()
{
    // オブジェクトの更新
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = &g_pObj[i];
        if (pObj->id != OBJ_ID_EMPTY)
        {
            pObj->Update(pObj);
        }
    }
}

/////////////////////////////////
// ＯＢＪマネージャー：描画処理
/////////////////////////////////
void ObjManager_Draw()
{
    // オブジェクトの描画
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = &g_pObj[i];
        if (pObj->id != OBJ_ID_EMPTY)
        {
            pObj->Draw(pObj);
        }
    }
}

/////////////////////////////////
// ＯＢＪマネージャー：終了処理
/////////////////////////////////
void ObjManager_End()
{
    // オブジェクトのメモリを解放
    MFREE((int)g_pObj);
    g_pObj = (pSObj)-1;
}

/////////////////////////////////
// ＯＢＪマネージャー：Ｍａｋｅ
/////////////////////////////////
pSObj ObjManager_Make(int id, int x, int y)
{
    pSObj pObj = NULL;

    // 空きオブジェクトを探す
    int i;
    for (i = 0; i < OBJ_MAX; i++)
    {
        if (g_pObj[i].id == OBJ_ID_EMPTY)
        {
            pObj = &g_pObj[i];
            break;
        }
    }
    if (i >= OBJ_MAX)
    {
        // 空きがない
        return NULL;
    }

    // オブジェクトの初期化
    pObj->id = id;
    pObj->x = x;
    pObj->y = y;
    pObj->pat = 0;
    pObj->Update = ObjFunc_Null;

    switch (id)
    {
        // プレイヤー
    case OBJ_ID_PLAYER:
        pObj->pat = 0x0140;         // 自機パターン
        pObj->Update = ObjFunc_Player;  // 更新処理
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // プレイヤーの弾
    case OBJ_ID_PBULLET:
        pObj->pat = 0x0141;         // 自機ショットパターン
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // 敵の弾
    case OBJ_ID_EBULLET:
        pObj->pat = 0x0142;         // 敵共通ショットパターン
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // 敵１
    case OBJ_ID_ENEMY1:
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // 敵２
    case OBJ_ID_ENEMY2:
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // 敵３
    case OBJ_ID_ENEMY3:
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // 敵の爆発
    case OBJ_ID_EEFFECT:
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;
        // プレイヤーの爆発
    case OBJ_ID_PEFFECT:
        pObj->Draw = ObjFunc_Draw;  // 描画標準
        break;

        // ここには来ないはず
    default:
#ifdef __GNUC__
        /* GNU C Compiler specific */
        __builtin_unreachable();
#endif
#ifdef _MSC_VER
        /* Microsoft Visual C++ specific */
        __assume(0);
#endif
    }

    return pObj;
}

/////////////////////////////////
// ＯＢＪマネージャー：Destroy
/////////////////////////////////
void ObjManager_Destroy(pSObj pObj)
{
    pObj->id = OBJ_ID_EMPTY;
    pObj->Update = ObjFunc_Null;
    pObj->Draw = ObjFunc_Null;
    CM_sp_set(pObj->plane, pObj->x, pObj->y, pObj->pat, 0); // スプライト非表示
}

/////////////////////////////////
// 何もしない関数
/////////////////////////////////
static volatile void ObjFunc_Null(pSObj pObj)
{
}

/////////////////////////////////
// スプライトの描画（デフォルト）
/////////////////////////////////
static void ObjFunc_Draw(pSObj pObj)
{
    // スプライト表示 X,Y 座標が16,16 ずつずれているので注意
    // スプライトの中心座標 8,8 が、引数座標の0,0 になるように補正
    int x = pObj->x + 16 - 8;
    int y = pObj->y + 16 - 8;

    CM_sp_set(pObj->plane, x, y, pObj->pat, 1);
//					*ｄ１＝スプライトプレーン番号（０～１２７）
//					*ｄ２＝ｘ座標
//					*ｄ３＝ｙ座標
//					*ｄ４＝パターンコード %HR_VR_X_X_COLR_SPATSPAT : 下位8bit:パターン / 8-12bit:カラー
//					*ｄ５＝プライオリティ %0000_0000_0000_0_0_PR : 00 表示しない / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

/////////////////////////////////
// プレイヤーの更新処理
/////////////////////////////////
// @brief プレイヤーの更新処理
// @param pObj プレイヤーのオブジェクト
static void ObjFunc_Player(pSObj pObj)
{
    // ゲームパッドの値を取得
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

//    CM_bg_puts("PLAYER()", 0, 4, 1);

    // 移動
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
    pObj->x = clamp(pObj->x+vx, 8, 256-16-8);

}
