// Path: ObjManager.c
// このコードは、サンプルプログラムのＯＢＪネージャー部分 ObjManager.c である。
// ＯＢＪの更新、描画の管理を行う
//
#include "SceneGame.h"
#include "ObjManager.h"

static pSObj g_pObj = (pSObj)-1;             // オブジェクトのポインタ配列

// プロトタイプ宣言
static volatile void ObjFunc_Null(pSObj pObj);
static void ObjFunc_Draw(pSObj pObj);

/////////////////////////////////
// ＯＢＪマネージャー自体の初期化
/////////////////////////////////
/// @retval なし
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
// ＯＢＪマネージャー：取得処理
/////////////////////////////////
/// @brief オブジェクトのポインタを取得する
/// @param i オブジェクトのインデックス
/// @retval オブジェクトのポインタ
pSObj ObjManager_GetObj(int i)
{
    return &g_pObj[i];
}

/////////////////////////////////
// ＯＢＪマネージャー：更新処理
/////////////////////////////////
/// @retval なし
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
/// @retval なし
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
/// @retval なし
void ObjManager_End()
{
    // オブジェクトのメモリを解放
    MFREE((int)g_pObj);
    g_pObj = (pSObj)-1;
}

/////////////////////////////////
// ＯＢＪマネージャー：Ｍａｋｅ
/////////////////////////////////
/// @brief オブジェクトを作成する
/// @param _id オブジェクトのＩＤ
/// @param _x  オブジェクトのＸ座標
/// @param _y  オブジェクトのＹ座標
/// @retval オブジェクトのポインタ
pSObj ObjManager_Make(int _id, int _x, int _y)
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
    pObj->id = _id;
    pObj->x = _x;
    pObj->y = _y;
    pObj->pat = 0;
    pObj->Update = ObjFunc_Null;

    switch (_id)
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
        pObj->Update = ObjFunc_PBullet;  // 更新処理
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
/// @brief オブジェクトを破棄する
/// @param pObj オブジェクトのポインタ
/// @retval なし
void ObjManager_Destroy(pSObj pObj)
{
    pObj->id = OBJ_ID_EMPTY;
    pObj->Update = ObjFunc_Null;
    pObj->Draw = ObjFunc_Null;
    CM_sp_set(pObj->plane, pObj->x, pObj->y, pObj->pat, 0); // スプライト非表示
}

/////////////////////////////////
/// @brief 何もしない関数
/////////////////////////////////
/// @retval なし
static volatile void ObjFunc_Null(pSObj pObj)
{
}

////////////////////////////////////////
/// @brief スプライトの描画（デフォルト）
////////////////////////////////////////
/// @param pObj オブジェクトのポインタ
/// @retval なし
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
