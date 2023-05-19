// Path: ObjManager.c
// このコードは、サンプルプログラムのサウンドマネージャー部分 SoundManager.c である。
// ＯＢＪの更新、描画の管理を行う
//
#include "SoundManager.h"

static pZMDdata bgm_zmd[BGM_MAX];
static pZMDdata se_zmd[SE_MAX];

static const char *bgm_file[BGM_MAX] = {
    "BGM01.ZMD",
};

static const char *se_file[SE_MAX] = {
    "MSE040.zmd",           // 敵の爆発音
    "MSE048.zmd",           // 自機のショット音
    "MSE045.zmd",           // 自機の爆発音
};

/////////////////////////////////
/// @brief ZMDファイルのロード
/////////////////////////////////
/// @return なし
static pZMDdata zmd_load(const char *filename)
{
    pZMDdata zmd;
    FILE *fp;
    int size;
    char strtmp[256];

    sprintf(strtmp, "snd\\%s", filename);
    fp = fopen(strtmp, "rb");
    if (fp == NULL) {
        printf("zmd_load: file open error '%s'\n", strtmp);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    zmd = (pZMDdata)malloc(sizeof(ZMDdata));
    if (zmd == NULL) {
        printf("zmd_load: malloc error\n");
        fclose(fp);
        return NULL;
    }

    zmd->size = size;
    zmd->data = malloc(size);
    if (zmd->data == NULL) {
        printf("zmd_load.data: malloc error\n");
        fclose(fp);
        free(zmd);
        return NULL;
    }

    printf("Loading: '%s' %d bytes read.\n", filename, size);
    fread(zmd->data, size, 1, fp);
    fclose(fp);

    return zmd;
}

/////////////////////////////////
/// @brief サウンドマネージャー自体の初期化
/////////////////////////////////
/// @return なし
void SoundManager_Init()
{
	zmsc_init();					// ZMSCライブラリの初期化

}

/////////////////////////////////
/// @brief サウンドマネージャー　曲・効果音のロード
/////////////////////////////////
/// @return なし
void SoundManager_Load()
{
    int i;

    // BGMのロード
    for (i = 0; i < BGM_MAX; i++) {
        bgm_zmd[i] = zmd_load(bgm_file[i]);
    }
    // SEのロード
    for (i = 0; i < SE_MAX; i++) {
        se_zmd[i] = zmd_load(se_file[i]);
    }
    INKEY();
}

/////////////////////////////////
/// @brief サウンドマネージャー　ＢＧＭの再生
/////////////////////////////////
/// @return なし
void SoundManager_PlayBGM(int no)
{
    pZMDdata zmd = bgm_zmd[no];
    if (zmd->data != NULL) {
        zmsc_play(zmd->data);
    }
}

/////////////////////////////////
/// @brief サウンドマネージャー　効果音の再生
/////////////////////////////////
/// @return なし
void SoundManager_PlaySE(int no)
{
    pZMDdata zmd = se_zmd[no];
    if (zmd->data != NULL) {
        zmsc_seplay(zmd->data);
    }
}


/////////////////////////////////
/// @brief サウンドマネージャー終了
/////////////////////////////////
/// @return なし
void SoundManager_End()
{
    int i;

    // SEの解放
    for (i = (SE_MAX-1); i >= 0; i--) {
        if (se_zmd[i] != NULL) {
            if (se_zmd[i]->data != NULL) {
                free(se_zmd[i]->data);
                se_zmd[i]->data = NULL;
            }
            free(se_zmd[i]);
            se_zmd[i] = NULL;
        }
    }

    // BGMの解放
    for (i = (BGM_MAX-1); i >= 0; i--) {
        if (bgm_zmd[i] != NULL) {
            if (bgm_zmd[i]->data != NULL) {
                free(bgm_zmd[i]->data);
                bgm_zmd[i]->data = NULL;
            }
            free(bgm_zmd[i]);
            bgm_zmd[i] = NULL;
        }
    }

}
