// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃R�[�h�{�� �ł���B
// Path: SceneGame.c
//

#include "SceneGame.h"

#define PAUSE_X 13
#define PAUSE_Y 15

#define BGDRAW_FLG_SCORE 0x0001    // �X�R�A�̏��������t���O
#define BGDRAW_FLG_HISCORE 0x0002  // �n�C�X�R�A�̏��������t���O

int score = 0;                  // �X�R�A
int hiscore = 0;                // �n�C�X�R�A

static BOOL bPause = FALSE;     // �|�[�Y�t���O
static uint32 bgDraw_flg;       // BG���������t���O
static pSObj pObjPlayer;        // �v���C���[�̃I�u�W�F�N�g

// �v���g�^�C�v�錾
static void SetPause(BOOL);
static void addScore(int);

// �Q�[���V�[���@������
void Game_Init(void)
{
    OBJManager_Init();          // �I�u�W�F�N�g�}�l�[�W���[�̏�����
    CM_sprite_clr();            // �X�v���C�g�N���A
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR
    CM_bg_puts("SCORE", 0, 0, 1);
    CM_bg_puts("HI", 19, 0, 1);
    bPause = FALSE;             // �|�[�Y�t���O���N���A
    score = 0;                  // �X�R�A���N���A
    bgDraw_flg = BGDRAW_FLG_SCORE | BGDRAW_FLG_HISCORE; // BG���������t���O���Z�b�g

    // �v���C���[�̐���
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
}

// �Q�[���V�[���@�X�V
void Game_Update(void)
{
    // �Q�[���p�b�h�̒l���擾
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if ((pad_trg & PAD_START)==PAD_START)
    {
        // START�{�^���Ń|�[�Y
        SetPause(!bPause);
    }

    if (bPause) return;     // �|�[�Y���͏������Ȃ�

    CM_bg_puts("GAME_UPDATE()", 0, 1, 1);
    ObjManager_Update();    // �I�u�W�F�N�g�}�l�[�W���[�̍X�V

    if (pad_trg & PAD_B)
    {
        // B�{�^���ŃX�R�A�����Z�e�X�g
        addScore(100);
    }
    if (pad_trg & PAD_A)
    {
        // A�{�^���Ń^�C�g���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// �Q�[���V�[���@�`��
void Game_Draw(void)
{
    CM_bg_puts("GAME_DRAW()", 0, 2, 1);
    ObjManager_Draw();      // �I�u�W�F�N�g�}�l�[�W���[�̕`��
//    CM_sp_set(0, 16, 16, 0x0141, 3);

}

// �Q�[���V�[���@VSync
void Game_VSync(void)
{
    char strtmp[128];
    if (bgDraw_flg & BGDRAW_FLG_SCORE)
    {
        // �X�R�A�̏�������
        sprintf(strtmp, "%08d", score);
        CM_bg_puts(strtmp, 7, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_SCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_HISCORE)
    {
        // �n�C�X�R�A�̏�������
        sprintf(strtmp, "%08d", hiscore);
        CM_bg_puts(strtmp, 22, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_HISCORE;
    }
    CM_bg_puts("GAME_VSYNC()", 0, 3, 1);
}

// �Q�[���V�[���@�N���A�i�I���j
void Game_Clear(void)
{
    ObjManager_End();       // �I�u�W�F�N�g�}�l�[�W���[�̏I��
}

// ObjManager����Ă΂��֐��́ASceneGame ���Ɉړ������B

/////////////////////////////////
// �v���C���[�̍X�V����
/////////////////////////////////
// @brief �v���C���[�̍X�V����
// @param pObj �v���C���[�̃I�u�W�F�N�g
void ObjFunc_Player(pSObj pObj)
{
    // �Q�[���p�b�h�̒l���擾
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    // �ړ�
    int16 vx = 0;
    if ((pad_trg & PAD_START)==PAD_START)
    {
        // START�{�^���͖���
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

//////////////////////////////////////
// �|�[�Y�t���O�̐ݒ�
/// @param[in] flg �|�[�Y�t���O
/// @retval �Ȃ�
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
// �X�R�A�̉��Z
/// @param[in] pts ���Z����X�R�A
/// @retval �Ȃ�
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
