// Path: ObjManager.c
// ���̃R�[�h�́A�T���v���v���O�����̂n�a�i�l�[�W���[���� ObjManager.c �ł���B
// �n�a�i�̍X�V�A�`��̊Ǘ����s��
//
#include "SceneGame.h"
#include "ObjManager.h"

static pSObj g_pObj = (pSObj)-1;             // �I�u�W�F�N�g�̃|�C���^�z��

// �v���g�^�C�v�錾
static volatile void ObjFunc_Null(pSObj pObj);
static void ObjFunc_Draw(pSObj pObj);

/////////////////////////////////
// �n�a�i�}�l�[�W���[���̂̏�����
/////////////////////////////////
/// @retval �Ȃ�
void OBJManager_Init()
{
    // �I�u�W�F�N�g�̃��������m��
    g_pObj = (pSObj)MALLOC(sizeof(SObj) * OBJ_MAX);
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
// �n�a�i�}�l�[�W���[�F�擾����
/////////////////////////////////
/// @brief �I�u�W�F�N�g�̃|�C���^���擾����
/// @param i �I�u�W�F�N�g�̃C���f�b�N�X
/// @retval �I�u�W�F�N�g�̃|�C���^
pSObj ObjManager_GetObj(int i)
{
    return &g_pObj[i];
}

/////////////////////////////////
// �n�a�i�}�l�[�W���[�F�X�V����
/////////////////////////////////
/// @retval �Ȃ�
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
// �n�a�i�}�l�[�W���[�F�`�揈��
/////////////////////////////////
/// @retval �Ȃ�
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
// �n�a�i�}�l�[�W���[�F�I������
/////////////////////////////////
/// @retval �Ȃ�
void ObjManager_End()
{
    // �I�u�W�F�N�g�̃����������
    MFREE((int)g_pObj);
    g_pObj = (pSObj)-1;
}

/////////////////////////////////
// �n�a�i�}�l�[�W���[�F�l������
/////////////////////////////////
/// @brief �I�u�W�F�N�g���쐬����
/// @param _id �I�u�W�F�N�g�̂h�c
/// @param _x  �I�u�W�F�N�g�̂w���W
/// @param _y  �I�u�W�F�N�g�̂x���W
/// @retval �I�u�W�F�N�g�̃|�C���^
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
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�P
    case OBJ_ID_ENEMY1:
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�Q
    case OBJ_ID_ENEMY2:
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�R
    case OBJ_ID_ENEMY3:
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �G�̔���
    case OBJ_ID_EEFFECT:
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;
        // �v���C���[�̔���
    case OBJ_ID_PEFFECT:
        pObj->Draw = ObjFunc_Draw;  // �`��W��
        break;

        // �����ɂ͗��Ȃ��͂�
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
// �n�a�i�}�l�[�W���[�FDestroy
/////////////////////////////////
/// @brief �I�u�W�F�N�g��j������
/// @param pObj �I�u�W�F�N�g�̃|�C���^
/// @retval �Ȃ�
void ObjManager_Destroy(pSObj pObj)
{
    pObj->id = OBJ_ID_EMPTY;
    pObj->Update = ObjFunc_Null;
    pObj->Draw = ObjFunc_Null;
    CM_sp_set(pObj->plane, pObj->x, pObj->y, pObj->pat, 0); // �X�v���C�g��\��
}

/////////////////////////////////
/// @brief �������Ȃ��֐�
/////////////////////////////////
/// @retval �Ȃ�
static volatile void ObjFunc_Null(pSObj pObj)
{
}

////////////////////////////////////////
/// @brief �X�v���C�g�̕`��i�f�t�H���g�j
////////////////////////////////////////
/// @param pObj �I�u�W�F�N�g�̃|�C���^
/// @retval �Ȃ�
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
