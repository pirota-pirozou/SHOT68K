// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// このコードは、サンプルプログラムの起動部分 main.c である。
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <doslib.h>
#include <iocslib.h>
#include "CF_MACRO.h"
#include "types.h"
#include "mylib.h"
#include "BMPLoad256.h"
#include "CMSprite.h"
#include "zmsclib.h"
#include "GamePadManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"

//#define PAD_TEST		// コメントを外すとゲームパッドの値表示テスト
static int usp;			// スーパーバイザーモードのスタックポインタ
int main_sp;			// main.c のスタックポインタ

// 常駐データ
pBMPFILE256 pBmpBackGround = NULL;		// BMPファイルデータ（ゲーム背景）
pBMPFILE256 pBmpBackTitle = NULL;		// BMPファイルデータ（タイトル）

// シーンの登録テーブル
static const SSceneWork sceneTable[] =
 {
	{ Title_Init, Title_Update, Title_Draw, Title_VSync, Title_Clear },		// タイトルシーン
	{ Game_Init, Game_Update, Game_Draw, Game_VSync, Game_Clear }			// ゲームシーン
};

#ifdef __cplusplus
extern "C" {
#endif

// プロトタイプ宣言
BOOL load_pat_data(void);
BOOL load_title_data(void);
BOOL load_gamebg_data(void);
void PRG_QUIT(void);

#ifdef __cplusplus
}
#endif

/* GCC のコンパイルオプションを退避 */
#pragma GCC push_options

/* GCC のスタック一括補正を無効化（この後の pragma GCC pop_options で再び有効化される）*/
#pragma GCC optimize("-fno-defer-pop")

/// @brief メイン関数
/* main */
int main(int argc, char *argv[])
{
	allmem();						// mallocの初期化
	srand((unsigned)time(NULL));	// 乱数の初期化

	usp = B_SUPER(0);				// スーパーバイザーモードのスタックポインタを取得
	setup_vector();					// 各種ベクタの設定

	asm volatile("move.l sp,%0" : "=r"(main_sp));	// main.c のスタックポインタを取得

	int keep = zmsc_keepchk();		// ZMSCの常駐チェック
	if (keep == 0)					// 常駐していない場合
	{
		printf("Z-MUSICが常駐していません。\n");
		// 常駐してなくても動くようになっている
//		PRG_QUIT();					// プログラム終了
	}
	zmsc_init();					// ZMSCライブラリの初期化

	screen_init();					// 画面初期化

	gcls(0);						// グラフィック画面クリア０
	gcls(1);						// グラフィック画面クリア１

	BOOL bSuccess;
	bSuccess = load_pat_data();		// パターンデータの読み込み
	// データの読み込みに失敗したら終了
	if (!bSuccess)
	{
		goto FORCE_QUIT;
	}

	bSuccess = load_title_data();	// タイトル画面のデータ読み込み
	// データの読み込みに失敗したら終了
	if (!bSuccess)
	{
		goto FORCE_QUIT;
	}

	bSuccess = load_gamebg_data();	// ゲーム背景画面のデータ読み込み
	// データの読み込みに失敗したら終了
	if (!bSuccess)
	{
		goto FORCE_QUIT;
	}

//	printf("画面が切り替わりました。\n");
//	INKEY();				// キー入力待ち

	CM_sprite_on();			// スプライト表示管理ＯＮ

	// ゲームパッドマネージャーの初期化
	GamePadManager_Init();

	// シーンマネージャーの初期化
	SceneManager_Init();
	SceneManager_Regist(sceneTable);

	// シーンの切り替え
	SceneManager_ChangeScene(SCENE_ID_TITLE);

	// ゲームパッドのテスト
#ifdef PAD_TEST
	unsigned short pad = 0;
	unsigned char strtmp[128];
#endif

	// メインループテスト
	for (;;)
	{
		if (BITSNS(0) & 2) 	// ESCキーで抜ける
		{
			break;
		}
		GamePadManager_Update();	// ゲームパッドの更新

#ifdef PAD_TEST
		pad = GamePadManager_GetPad();
		sprintf(strtmp, "PAD=%02X", pad);
		CM_bg_puts(strtmp, 0, 8, 1);
#endif

		Scene_Update();		// シーンの更新
		Scene_Draw();		// シーンの描画
		// VSyncの待ち
//		vsync();
		CM_vsync();			// VSync待ち
		Scene_VSync();		// シーンのVSync後の処理
	}
	Scene_Clear();			// ゲームループから抜けるときシーンのクリア

	// プログラム終了
	// プログラムのAbortアドレスを強引にここに設定する
FORCE_QUIT:
	asm volatile("bra _PRG_QUIT");	// 行儀悪いが、強制終了
//	PRG_QUIT();
	// ここには到達しない
__UNREACHABLE__;
	return 0;
}

///////////////////////////////////
/// @brief プログラム終了
///////////////////////////////////
void PRG_QUIT(void)
{
	// main.c のスタックポインタを復元
//	asm volatile("addq.l #4,sp");	// スタック捨てる（行儀悪い）
	asm volatile("move.l %0,sp" : /* 出力 */
								: /* 入力 */ "d"(main_sp));

	CM_sprite_off();		// スプライト表示管理ＯＦＦ

	screen_restore();		// DOS画面復帰

	zmsc_fadeout();				// 音楽フェードアウト


	if (usp > 0)
	{
		B_SUPER(usp);				// ユーザーモードへ復帰
	}

	// 背景画面のメモリ解放チェック
	if (pBmpBackGround != NULL)
	{
		// メモリ解放
		free(pBmpBackGround);
		pBmpBackGround = NULL;
	}
	// タイトル画面のメモリ解放チェック
	if (pBmpBackTitle != NULL)
	{
		// メモリ解放
		free(pBmpBackTitle);
		pBmpBackTitle = NULL;
	}

	//
	printf("プログラムの実行を終了しました。\n");
	exit(0);
}

/* GCC のコンパイルオプションを復活 */
#pragma GCC pop_options

/// @brief スプライトパターン・パレットデータの読み込み
/// スプライトパターンとパレットデータを読み込み定義する
/// 引数: なし
/// @retval TRUE:成功 FALSE:失敗
BOOL load_pat_data(void)
{
	FILE *fp = NULL;
	pPX2FILE px2buf = NULL;
	BOOL result = TRUE;

	// PX2ファイルの読み込み
	do
	{
		px2buf = (pPX2FILE)malloc(sizeof(PX2FILE));
		if (px2buf == NULL)
		{
			printf("px2buf: メモリが確保できません。\n");
			result = FALSE;
			break;
		}
		// PX2ファイルの読み込み
		fp = fopen("shoot.px2", "rb");
		if (fp == NULL)
		{
			printf("スプライトファイルが開けません。\n");
			result = FALSE;
			break;
		}
		int a = fread(px2buf, sizeof(PX2FILE), 1, fp);
		if (a != 1)
		{
			printf("スプライトファイルの読み込みに失敗しました。\n");
			result = FALSE;
			break;
		}
		// スプライトパターンとパレットの定義
		CM_def_px2(px2buf);
		break;
	} while (0);

	if (fp != NULL)
	{
		// ファイルクローズ
		fclose(fp);
		fp = NULL;
	}
	if (px2buf != NULL)
	{
		// メモリ解放
		free(px2buf);
		px2buf = NULL;
	}

	return result;
}

/// @brief BMPファイルを読み込む
/// @param filename ファイル名
/// @param bmpdata 読み込んだBMPデータへのポインタ
/// @retval TRUE:成功 FALSE:失敗
BOOL load_bmp_data(char* filename, pBMPFILE256* bmp_data)
{
	FILE *fp = NULL;
	pBMPFILE256 pBMP = NULL;
	BOOL result = TRUE;

	// BMPファイルの読み込み
	do
	{
		pBMP = LoadBMP256(filename);
		if (pBMP == NULL)
		{
			printf("pBMP: メモリが確保できません。\n");
			result = FALSE;
			break;
		}
		*bmp_data = pBMP;
	} while (0);

	return result;
}

// ゲーム背景のBMPを読み込む
// 引数: なし
// Return: TRUE:成功 FALSE:失敗
BOOL load_gamebg_data(void)
{
	return load_bmp_data("space.bmp", &pBmpBackGround);
}

// タイトル画面のBMPを読み込む
// 引数: なし
// Return: TRUE:成功 FALSE:失敗
BOOL load_title_data(void)
{
	return load_bmp_data("title.bmp", &pBmpBackTitle);
}
