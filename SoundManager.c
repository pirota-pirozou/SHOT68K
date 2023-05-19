// Path: ObjManager.c
// ���̃R�[�h�́A�T���v���v���O�����̃T�E���h�}�l�[�W���[���� SoundManager.c �ł���B
// �n�a�i�̍X�V�A�`��̊Ǘ����s��
//
#include "SoundManager.h"

static pZMDdata bgm_zmd[BGM_MAX];
static pZMDdata se_zmd[SE_MAX];

static const char *bgm_file[BGM_MAX] = {
    "BGM01.ZMD",
};

static const char *se_file[SE_MAX] = {
    "MSE040.zmd",           // �G�̔�����
    "MSE048.zmd",           // ���@�̃V���b�g��
    "MSE045.zmd",           // ���@�̔�����
};

/////////////////////////////////
/// @brief ZMD�t�@�C���̃��[�h
/////////////////////////////////
/// @return �Ȃ�
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
/// @brief �T�E���h�}�l�[�W���[���̂̏�����
/////////////////////////////////
/// @return �Ȃ�
void SoundManager_Init()
{
	zmsc_init();					// ZMSC���C�u�����̏�����

}

/////////////////////////////////
/// @brief �T�E���h�}�l�[�W���[�@�ȁE���ʉ��̃��[�h
/////////////////////////////////
/// @return �Ȃ�
void SoundManager_Load()
{
    int i;

    // BGM�̃��[�h
    for (i = 0; i < BGM_MAX; i++) {
        bgm_zmd[i] = zmd_load(bgm_file[i]);
    }
    // SE�̃��[�h
    for (i = 0; i < SE_MAX; i++) {
        se_zmd[i] = zmd_load(se_file[i]);
    }
    INKEY();
}

/////////////////////////////////
/// @brief �T�E���h�}�l�[�W���[�@�a�f�l�̍Đ�
/////////////////////////////////
/// @return �Ȃ�
void SoundManager_PlayBGM(int no)
{
    pZMDdata zmd = bgm_zmd[no];
    if (zmd->data != NULL) {
        zmsc_play(zmd->data);
    }
}

/////////////////////////////////
/// @brief �T�E���h�}�l�[�W���[�@���ʉ��̍Đ�
/////////////////////////////////
/// @return �Ȃ�
void SoundManager_PlaySE(int no)
{
    pZMDdata zmd = se_zmd[no];
    if (zmd->data != NULL) {
        zmsc_seplay(zmd->data);
    }
}


/////////////////////////////////
/// @brief �T�E���h�}�l�[�W���[�I��
/////////////////////////////////
/// @return �Ȃ�
void SoundManager_End()
{
    int i;

    // SE�̉��
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

    // BGM�̉��
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
