// Path: ObjManager.c
// ���̃R�[�h�́A�T���v���v���O�����̂n�a�i�l�[�W���[���� ObjManager.c �ł���B
// �n�a�i�̍X�V�A�`��̊Ǘ����s��
//
#include "SceneGame.h"
#include "ObjManager.h"

pSObj g_pObj = NULL;             // �I�u�W�F�N�g�̃|�C���^�z��

// �v���g�^�C�v�錾
static volatile void ObjFunc_Null(pSObj pObj);
static void ObjFunc_Draw(pSObj pObj);
static void ObjFunc_DrawAnm(pSObj pObj);

/////////////////////////////////
/// @brief �n�a�i�}�l�[�W���[���̂̏�����
/////////////////////////////////
/// @return �Ȃ�
void OBJManager_Init()
{
    // �I�u�W�F�N�g�̃��������m��
    g_pObj = (pSObj)malloc(sizeof(SObj) * OBJ_MAX);
    if (g_pObj < 0)
    {
        // �m�ێ��s
        // �{���͂����ŃG���[�������s��������͏ȗ�
        return;
    }
    // ���������N���A
    memset((pSObj)g_pObj, 0, sizeof(SObj) * OBJ_MAX);

    // �I�u�W�F�N�g�̏�����
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
/// @brief �n�a�i�}�l�[�W���[�F�X�V����
/////////////////////////////////
/// @return �Ȃ�
void ObjManager_Update()
{
    // �I�u�W�F�N�g�̍X�V
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
/// @brief �n�a�i�}�l�[�W���[�F�`�揈��
/////////////////////////////////
/// @return �Ȃ�
void ObjManager_Draw()
{
    // �I�u�W�F�N�g�̕`��
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
/// @brief �n�a�i�}�l�[�W���[�F�I������
/////////////////////////////////
/// @return �Ȃ�
void ObjManager_End()
{
    // �I�u�W�F�N�g�̃����������
    free(g_pObj);
    g_pObj = NULL;
}

/////////////////////////////////
/// @brief �n�a�i�}�l�[�W���[�F�l������
/////////////////////////////////
/// @brief �I�u�W�F�N�g���쐬����
/// @param _id �I�u�W�F�N�g�̂h�c
/// @param _x  �I�u�W�F�N�g�̂w���W
/// @param _y  �I�u�W�F�N�g�̂x���W
/// @return �I�u�W�F�N�g�̃|�C���^
pSObj ObjManager_Make(int _id, int _x, int _y)
{
    pSObj pObj = NULL;

    // �󂫃I�u�W�F�N�g��T��
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
        // �󂫂��Ȃ�
        return NULL;
    }

    // �I�u�W�F�N�g�̏�����
    pObj->id = _id;
    pObj->x = _x;
    pObj->y = _y;
    pObj->pat = 0;
    pObj->Update = ObjFunc_Null;

    switch (_id)
    {
        // �v���C���[
    case OBJ_ID_PLAYER:
        pObj->pat = 0x0140;         // ���@�p�^�[��
        pObj->Update = ObjFunc_Player;  // �X�V����
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �v���C���[�̒e
    case OBJ_ID_PBULLET:
        pObj->pat = 0x0141;         // ���@�V���b�g�p�^�[��
        pObj->Update = ObjFunc_PBullet;  // �X�V����
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�̒e
    case OBJ_ID_EBULLET:
        pObj->pat = 0x0142;         // �G���ʃV���b�g�p�^�[��
        pObj->Update = ObjFunc_EBullet;  // �X�V����
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�P
    case OBJ_ID_ENEMY1:
        pObj->pat = 0x0143;         // �G�P�p�^�[��
        pObj->anm_spd = 4;          // �A�j���[�V�������x
        pObj->anm_num = 2;          // �A�j���[�V��������
        pObj->anm_cou = 0;
        pObj->anm_idx = 0;
        pObj->vx = 1;
        pObj->Draw = ObjFunc_DrawAnm;  // �`��A�j���p
        break;
        // �G�Q
    case OBJ_ID_ENEMY2:
        pObj->pat = 0x0145;         // �G�Q�p�^�[��
        pObj->anm_spd = 4;          // �A�j���[�V�������x
        pObj->anm_num = 2;          // �A�j���[�V��������
        pObj->anm_cou = 0;
        pObj->anm_idx = 0;
        pObj->vx = 1;
        pObj->Draw = ObjFunc_DrawAnm;  // �`��A�j���p
        break;
        // �G�R
    case OBJ_ID_ENEMY3:
        pObj->pat = 0x0147;         // �G�R�p�^�[��
        pObj->anm_spd =     4;          // �A�j���[�V�������x
        pObj->anm_num = 2;          // �A�j���[�V��������
        pObj->anm_cou = 0;
        pObj->anm_idx = 0;
        pObj->vx = 1;
        pObj->Draw = ObjFunc_DrawAnm;  // �`��A�j���p
        break;
        // �G�̔���
    case OBJ_ID_EEFFECT:
        pObj->pat = 0x0150;         // �G�����p�^�[��
        pObj->anm_spd = 4;          // �A�j���[�V�������x
        pObj->anm_num = 4;          // �A�j���[�V��������
        pObj->anm_cou = 0;
        pObj->anm_idx = 0;
        pObj->Update = ObjFunc_EEffect;  // �A�j���p
        pObj->Draw = ObjFunc_DrawAnm;  // �`��A�j���p
        break;
        // �v���C���[�̔���
    case OBJ_ID_PEFFECT:
        pObj->pat = 0x0154;         // ���@�����p�^�[��
        pObj->anm_spd = 6;          // �A�j���[�V�������x
        pObj->anm_num = 4;          // �A�j���[�V��������
        pObj->anm_cou = 0;
        pObj->anm_idx = 0;
        pObj->Update = ObjFunc_EEffect;  // �A�j���p
        pObj->Draw = ObjFunc_DrawAnm;  // �`��A�j���p
        break;

        // �����ɂ͗��Ȃ��͂�
    default:
        __UNREACHABLE__;
    }

    return pObj;
}

/////////////////////////////////
/// @brief �n�a�i�}�l�[�W���[�FDestroy
/////////////////////////////////
/// @brief �I�u�W�F�N�g��j������
/// @param pObj �I�u�W�F�N�g�̃|�C���^
/// @return �Ȃ�
void ObjManager_Destroy(pSObj pObj)
{
    pObj->id = OBJ_ID_EMPTY;
    pObj->Update = ObjFunc_Null;
    pObj->Draw = ObjFunc_Null;
    CM_sp_set(pObj->plane, pObj->x, pObj->y, pObj->pat, 0); // �X�v���C�g��\��
}

/////////////////////////////////
/// @brief �n�a�i�}�l�[�W���[�F�G�I�u�W�F�N�g��������
/////////////////////////////////
/// @brief �G�I�u�W�F�N�g��������
/// @return �I�u�W�F�N�g�̃C���f�b�N�X
int ObjManager_FindEnemyIdx(void)
{
    // �󂫃I�u�W�F�N�g��T��
    int i;
    int ret = -1;
    for (i = 0; i < OBJ_MAX; i++)
    {
        if (g_pObj[i].id >= OBJ_ID_ENEMY1 &&
            g_pObj[i].id <= OBJ_ID_ENEMY3)
        {
            ret = i;
            break;
        }
    }

    return ret;
}

/////////////////////////////////
/// @brief �n�a�i�}�l�[�W���[�F���̓G�I�u�W�F�N�g��������
/////////////////////////////////
/// @brief ���̓G�I�u�W�F�N�g��������
/// @param idx ���݂̃C���f�b�N�X
/// @return �I�u�W�F�N�g�̃C���f�b�N�X
int ObjManager_FindEnemyNextIdx(int idx)
{
    // �󂫃I�u�W�F�N�g��T��
    int i;
    int n = (idx + 1);
    int ret = -1;
    for (i = 0; i < OBJ_MAX; i++)
    {
        n &= (OBJ_MAX - 1);
        if (g_pObj[n].id >= OBJ_ID_ENEMY1 &&
            g_pObj[n].id <= OBJ_ID_ENEMY3)
        {
            return n;
        }
        n++;
    }

    return -1;
}


/////////////////////////////////
/// @brief �������Ȃ��֐�
/////////////////////////////////
/// @return �Ȃ�
static volatile void ObjFunc_Null(pSObj pObj)
{
}

////////////////////////////////////////
/// @brief �X�v���C�g�̕`��i�f�t�H���g�j
////////////////////////////////////////
/// @param pObj �I�u�W�F�N�g�̃|�C���^
/// @return �Ȃ�
static void ObjFunc_Draw(pSObj pObj)
{
    // �X�v���C�g�\�� X,Y ���W��16,16 ������Ă���̂Œ���
    // �X�v���C�g�̒��S���W 8,8 ���A�������W��0,0 �ɂȂ�悤�ɕ␳
    int x = pObj->x + 16 - 8;
    int y = pObj->y + 16 - 8;

    CM_sp_set(pObj->plane, x, y, pObj->pat, 1);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1

}

////////////////////////////////////////
/// @brief �X�v���C�g�̕`��i�A�j���t���j
////////////////////////////////////////
/// @param pObj �I�u�W�F�N�g�̃|�C���^
/// @return �Ȃ�
static void ObjFunc_DrawAnm(pSObj pObj)
{
    // �X�v���C�g�\�� X,Y ���W��16,16 ������Ă���̂Œ���
    // �X�v���C�g�̒��S���W 8,8 ���A�������W��0,0 �ɂȂ�悤�ɕ␳
    int x = pObj->x + 16 - 8;
    int y = pObj->y + 16 - 8;

    CM_sp_set(pObj->plane, x, y, pObj->pat+pObj->anm_idx, 1);
//					*���P���X�v���C�g�v���[���ԍ��i�O�`�P�Q�V�j
//					*���Q�������W
//					*���R�������W
//					*���S���p�^�[���R�[�h %HR_VR_X_X_COLR_SPATSPAT : ����8bit:�p�^�[�� / 8-12bit:�J���[
//					*���T���v���C�I���e�B %0000_0000_0000_0_0_PR : 00 �\�����Ȃ� / 01 BG0>BG1>SP / 10 BG0>SP>BG1 / 11 SP>BG0>BG1
}
