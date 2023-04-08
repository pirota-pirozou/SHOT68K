// Path: ObjManager.c
// このコードは、サンプルプログラムのＯＢＪネージャー部分 ObjManager.c である。
// ＯＢＪの更新、描画の管理を行う
//
#include "ObjManager.h"

static pSObj g_pObj = (pSObj)-1;             // オブジェクトのポインタ配列

// プロトタイプ宣言
static volatile void ObjFunc_Null(pSObj pObj);

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
    pObj->Draw = ObjFunc_Null;

    switch (id)
    {
        // プレイヤー
    case OBJ_ID_PLAYER:
        break;
        // プレイヤーの弾
    case OBJ_ID_PBULLET:
        break;
        // 敵の弾
    case OBJ_ID_EBULLET:
        break;
        // 敵１
    case OBJ_ID_ENEMY1:
        break;
        // 敵２
    case OBJ_ID_ENEMY2:
        break;
        // 敵３
    case OBJ_ID_ENEMY3:
        break;
        // 敵の爆発
    case OBJ_ID_EEFFECT:
        break;
        // プレイヤーの爆発
    case OBJ_ID_PEFFECT:
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
