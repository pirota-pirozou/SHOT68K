// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃R�[�h�{�� �ł���B
// Path: SceneGame.c
//

#include "SceneGame.h"
#include "ObjManager.h"
#include "BMPLoad256.h"

#define PAUSE_X 13
#define PAUSE_Y 15

#define DEADLINE_Y  232             // �G�̐N���ʒu

#define BGDRAW_FLG_SCORE 0x0001    // �X�R�A�̏��������t���O
#define BGDRAW_FLG_HISCORE 0x0002  // �n�C�X�R�A�̏��������t���O
#define BGDRAW_FLG_LEFT 0x0004  // �c�@�̏��������t���O
#define BGDRAW_FLG_STAGE 0x0008  // �X�e�[�W�̏��������t���O
#define BGDRAW_FLG_GAMEOVER 0x0010  // �Q�[���I�[�o�[���������t���O

int score = 0;                  // �X�R�A
int hiscore = 0;                // �n�C�X�R�A

static BOOL bPause = FALSE;     // �|�[�Y�t���O
static uint32 bgDraw_flg;       // BG���������t���O
static pSObj pObjPlayer;        // �v���C���[�̃I�u�W�F�N�g
static pSObj pObjBullet;        // �v���C���[�̒e�̃I�u�W�F�N�g

static int enemy_left;          // �G�̎c�萔

// �Q�[����Ԃ̊Ǘ��p
enum
{
    STATUS_NORMAL,              // �i�ʏ�j�Q�[����
    STATUS_MISS,                // �~�X
    STATUS_CLEAR,               // �X�e�[�W�N���A
    STATUS_GAMEOVER,            // �Q�[���I�[�o�[
    STATUS_MAX                  // ��Ԃ̍ő�l
};
static int status;              // �Q�[�����
static int status_count;        // ��ԑJ�ڃJ�E���^

// �G�̓���@�Ǘ��p
static int enemy_move_idx;      // �G�̓���C���f�b�N�X
static int16 enemy_move_vx;     // �G�̓������
static int16 enemy_wall_touch;  // �G�̕ǐڐG�t���O
static int stage;               // �X�e�[�W
static int player_left;         // �c�@

// �v���g�^�C�v�錾
static void SetPause(BOOL);
static void addScore(int);
static void setupEnemies(void);
static void to_next_stage(void);
static void obj_clear_all(void);
static void initStage(void);
static void setGameOver(void);
static void enemiesMoveDown(int);

// �Q�[���V�[���@������
void Game_Init(void)
{
    OBJManager_Init();          // �I�u�W�F�N�g�}�l�[�W���[�̏�����
    CM_sprite_clr();            // �X�v���C�g�N���A
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR

    // �r�b�g�}�b�v�t�@�C���̓ǂݍ���
    PutBMP256("space.bmp");

    CM_bg_puts("SCORE", 0, 0, 1);
    CM_bg_puts("HI", 19, 0, 1);
    CM_bg_puts("LEFT", 0, 31, 1);
    CM_bg_puts("STAGE", 31-9, 31, 1);
    bPause = FALSE;             // �|�[�Y�t���O���N���A
    score = 0;                  // �X�R�A���N���A
    stage = 1;                  // �X�e�[�W�͂P
    player_left = 3;            // �c�@�͂R
    pObjBullet = NULL;          // �v���C���[�̒e�̃I�u�W�F�N�g���N���A
    // BG���������t���O���Z�b�g
    bgDraw_flg = BGDRAW_FLG_SCORE | BGDRAW_FLG_HISCORE | BGDRAW_FLG_LEFT | BGDRAW_FLG_STAGE;

    // �Q�[����Ԃ�������
    status = STATUS_NORMAL;
    status_count = 0;

    // �G�̓����������
    enemy_move_vx = 1;
    enemy_wall_touch = 0;

    // �v���C���[�̐���
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
    // �G�̐���
    setupEnemies();

    // �G�̓���C���f�b�N�X��������
    enemy_move_idx = ObjManager_FindEnemyIdx();
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

    ObjManager_Update();    // �I�u�W�F�N�g�}�l�[�W���[�̍X�V

    // �Q�[����Ԃ̍X�V
    switch (status)
    {
    case STATUS_NORMAL:
        // �i�ʏ�j�Q�[����
        if (enemy_move_idx < 0) break;
        // �G�̓�����X�V�B�P�L������������
        pSObj pObj = ObjManager_GetObj(enemy_move_idx);
        // TODO: �G�̓�����X�V����
        pObj->x += (enemy_move_vx * 4);
//        pObj->x +=(pObj->vx * 4);
        if (pObj->x < 8)
        {
            // ���[�ɓ��B������
            pObj->x = 8;
            // ���씽�]
//            pObj->vx = 1;
            enemy_move_vx = 1;
            enemy_wall_touch = 1;
        }
        else if (pObj->x > 240)
        {
            // �E�[�ɓ��B������
            pObj->x = 240;
            // ���씽�]
//            pObj->vx = -1;
            enemy_move_vx = -1;
            enemy_wall_touch = 1;
        }
        else
        {
            // �ǂɐڐG���Ă��Ȃ�
//            enemy_wall_touch = 0;
        }
        // �L�����G�ؑ�
        pObj->anm_idx ^= 1;
        // �����G���n�ʂɍ~�肽��
        if (pObj->y >= DEADLINE_Y)
        {
            // �v���C���[���j
            ObjManager_Make(OBJ_ID_PEFFECT, pObjPlayer->x, pObjPlayer->y);
            // �v���C���[������
            ObjManager_Destroy(pObjPlayer);
            // �Q�[���I�[�o�[��
            setGameOver();
            break;
        }
        // ���̃L������
        int next_idx = ObjManager_FindEnemyNextIdx(enemy_move_idx);
        if (next_idx < enemy_move_idx)
        {
            // �S�Ă̓G�̓��삪�I�������
            if (enemy_wall_touch)
            {
                // �ǂɐڐG������
                // �G�S�̂����Ɉړ�
                enemy_wall_touch = 0;
//                enemiesMoveDown(8);
            }
        }
        else
        {
            // �ǂɃ^�b�`������L���������Ɉړ�
            if (enemy_wall_touch)
            {
                pObj->y += 8;
            }

        }
        enemy_move_idx = next_idx;
        break;
    case STATUS_MISS:
        // �~�X
        if ((--status_count) <= 0)
        {
            // ���t���[���o�߂�����Q�[���I�[�o�[
            status = STATUS_GAMEOVER;
            status_count = 60*3;
        }
        break;
    case STATUS_CLEAR:
        // �X�e�[�W�N���A
        if ((--status_count) <= 0)
        {
            // ���t���[���o�߂����玟�̃X�e�[�W�֑J��
            to_next_stage();
        }
        break;
    case STATUS_GAMEOVER:
        // �Q�[���I�[�o�[
        if ((--status_count) <= 0)
        {
            //  ���t���[���o�߂�����^�C�g���V�[���֑J��
            SceneManager_ChangeScene(SCENE_ID_TITLE);
        }
        break;
    default:
        // �����ɂ͗��Ȃ�
        __UNREACHABLE__;
    }

    // �f�o�b�O�p
    if (pad_trg & PAD_A)
    {
        // A�{�^���Ń^�C�g���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// �Q�[���V�[���@�`��
void Game_Draw(void)
{
//    CM_bg_puts("GAME_DRAW()", 0, 2, 1);
    ObjManager_Draw();      // �I�u�W�F�N�g�}�l�[�W���[�̕`��
}

// �Q�[���V�[���@VSync
void Game_VSync(void)
{
    char strtmp[128];
    if (bgDraw_flg & BGDRAW_FLG_SCORE)
    {
        // �X�R�A�̏�������
        sprintf(strtmp, "%08d", score);
        CM_bg_puts(strtmp, 6, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_SCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_HISCORE)
    {
        // �n�C�X�R�A�̏�������
        sprintf(strtmp, "%08d", hiscore);
        CM_bg_puts(strtmp, 22, 0, 1);
        bgDraw_flg &= ~BGDRAW_FLG_HISCORE;
    }
    if (bgDraw_flg & BGDRAW_FLG_LEFT)
    {
        // �c�@�̏�������
        int left = player_left - 1;
        if (left < 0) left = 0;
        sprintf(strtmp, "%02d", left);
        CM_bg_puts(strtmp, 5, 31, 1);
        bgDraw_flg &= ~BGDRAW_FLG_LEFT;
    }
    if (bgDraw_flg & BGDRAW_FLG_STAGE)
    {
        // �c�@�̏�������
        sprintf(strtmp, "%02d", stage);
        CM_bg_puts(strtmp, 28, 31, 1);
        bgDraw_flg &= ~BGDRAW_FLG_STAGE;
    }
    if (bgDraw_flg & BGDRAW_FLG_GAMEOVER)
    {
        // �Q�[���I�[�o�[�̏�������
        CM_bg_puts("GAME OVER", 11, 16, 1);
        bgDraw_flg &= ~BGDRAW_FLG_GAMEOVER;
    }

//    CM_bg_puts("GAME_VSYNC()", 0, 3, 1);
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

    // �ړ�����
    int16 vx = 0;
    if ((pad & PAD_START)==PAD_START)
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
    // �ړ�
    pObj->x = clamp(pObj->x+vx, 8, 256-16-8);

    // �e���˔���
    if (pad_trg & PAD_B)
    {
        // ���@�����G�t�F�N�g�̃e�X�g
//         ObjManager_Make(OBJ_ID_PEFFECT,
//            pObj->x, pObj->y-32);


        // B�{�^���Œe����
        if (pObjBullet == NULL)
        {
            pObjBullet = ObjManager_Make(OBJ_ID_PBULLET,
                pObj->x, pObj->y-2);
        }
    }

}

/////////////////////////////////
/// ���@�e�̍X�V����
/////////////////////////////////
/// @brief ���@�e�̍X�V����
/// @param pObj ���@�e�̃I�u�W�F�N�g
/// @return �Ȃ�
void ObjFunc_PBullet(pSObj pObj)
{
    pObj->y -= 8;
    if (pObj->y <= 0)
    {
        // ��ʊO�ɏo�������
        ObjManager_Destroy(pObj);
        pObjBullet = NULL;          // �o���t���O���N���A
    }

    // ���@�e�ƓG�Ƃ̓����蔻��
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObjEnemy = ObjManager_GetObj(i);
        if (pObjEnemy->id >= OBJ_ID_ENEMY1 &&
            pObjEnemy->id <= OBJ_ID_ENEMY3)
        {
            // �G�Ƃ̓�����`�F�b�N
            int16 cx = abs(pObj->x - pObjEnemy->x);
            int16 cy = abs(pObj->y - pObjEnemy->y);

            if (cx < 8 && cy < 8)
            {
                // ��������
                ObjManager_Destroy(pObj);      // ���@�e������
                pObjBullet = NULL;             // �o���t���O���N���A
                //  �G�����G�t�F�N�g
                ObjManager_Make(OBJ_ID_EEFFECT,
                    pObjEnemy->x, pObjEnemy->y);
                // �G���Ƃ̃X�R�A���Z
                int pts = 0;
                switch (pObjEnemy->id)
                {
                case OBJ_ID_ENEMY1:
                    pts = 100;
                    break;
                case OBJ_ID_ENEMY2:
                    pts = 200;
                    break;
                case OBJ_ID_ENEMY3:
                    pts = 300;
                    break;
                default:
                    // �����ɂ͗��Ȃ��͂�
                    __UNREACHABLE__;
                }
                ObjManager_Destroy(pObjEnemy);  // �G������
                addScore(pts);                  // �X�R�A���Z
                enemy_left--;                   // �G�̎c�������炷
                if (enemy_left <= 0)
                {
                    // �G�̎c�萔��0�ɂȂ�����X�e�[�W�N���A
                    status = STATUS_CLEAR;
                    status_count = 60*3;        // ��ʑJ�ڂ܂ł̎���
                }
                break;
            }
        }
    }

}

/////////////////////////////////
/// @brief �G�e�̍X�V����
/////////////////////////////////
/// @param pObj �G�e�̃I�u�W�F�N�g
/// @return �Ȃ�
void ObjFunc_EBullet(pSObj pObj)
{
    pObj->y += 4;           // ���Ɉړ�
    if (pObj->y >= 248)
    {
        // ��ʊO�ɏo�������
        ObjManager_Destroy(pObj);
        return;
    }

    // ���@�ƓG�e�Ƃ̓����蔻��
    if (pObjPlayer == NULL)
    {
        // ���@�����ł��Ă���ꍇ�͓����蔻�肵�Ȃ�
        return;
    }

    // �G�Ƃ̓�����`�F�b�N
    int16 cx = abs(pObj->x - pObjPlayer->x);
    int16 cy = abs(pObj->y - pObjPlayer->y);

    if (cx < 12 && cy < 12)
    {
        // ��������
        //  ���@�����G�t�F�N�g
        ObjManager_Make(OBJ_ID_PEFFECT,
            pObjPlayer->x, pObjPlayer->y);
        ObjManager_Destroy(pObj);      // �G�e������
        ObjManager_Destroy(pObjPlayer); // �v���C���[������
        pObjPlayer = NULL;             // �v���C���[�t���O���N���A
    }
}


/////////////////////////////////
/// �G�����G�t�F�N�g�̍X�V����
/////////////////////////////////
/// @brief �G�����G�t�F�N�g�̍X�V����
/// @param pObj �G�����G�t�F�N�g�̃I�u�W�F�N�g
/// @return �Ȃ�
void ObjFunc_EEffect(pSObj pObj)
{
    // �A�j���[�V��������
    pObj->anm_cou++;
    if (pObj->anm_cou >= pObj->anm_spd)
    {
        pObj->anm_cou = 0;
        pObj->anm_idx++;
        if (pObj->anm_idx >= pObj->anm_num)
        {
            // �A�j���[�V�����I��
            ObjManager_Destroy(pObj);
        }
    }
}

//////////////////////////////////////
/// @brief �|�[�Y�t���O�̐ݒ�
/// @param[in] flg �|�[�Y�t���O
/// @return �Ȃ�
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
/// @return �Ȃ�
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
/// @brief �G�̏����z�u
//////////////////////////////////////
static void setupEnemies(void)
{
    // �G�̏����z�u
    enemy_left = 0;
    for (int i = 0; i < 5; i++)
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
        for (int j = 0; j < 11; j++)
        {
            int16 _x = (j * 20) + 24;
            int16 _y = (i * 24) + 24;

            pSObj pObj = ObjManager_Make(_id, _x, _y);
            enemy_left++;
        }
    }
}

//////////////////////////////////////
/// @brief �G�S�̂����ɉ��~������
//////////////////////////////////////
/// @param add ���~�h�b�g��
static void enemiesMoveDown(int add)
{
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        if (pObj->id == OBJ_ID_ENEMY1 ||
            pObj->id == OBJ_ID_ENEMY2 ||
            pObj->id == OBJ_ID_ENEMY3)
        {
            pObj->y += add;
        }
    }
}

//////////////////////////////////////
/// @brief ���̃X�e�[�W��
//////////////////////////////////////
static void to_next_stage(void)
{
    stage++;
    if (stage > 99) stage = 99;

    bgDraw_flg |= BGDRAW_FLG_STAGE;

    // �X�e�[�W�̏�����
    initStage();
}

//////////////////////////////////////
/// @brief �X�e�[�W�̏�����
//////////////////////////////////////
static void initStage(void)
{
    // �I�u�W�F�N�g�̑S����
     obj_clear_all();

    // �V���Ƀv���C���[�̐���
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);

    // �G�̏����z�u
    setupEnemies();

    // �Q�[����Ԃ̏�����
    status = STATUS_NORMAL;

    // �G�̓����������
    enemy_move_vx = 1;
    enemy_wall_touch = 0;

}

//////////////////////////////////////
/// @brief �I�u�W�F�N�g�̑S����
//////////////////////////////////////
static void obj_clear_all(void)
{
    // �I�u�W�F�N�g�̑S����
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        ObjManager_Destroy(pObj);
    }
    // ���@�e�̃|�C���^���N���A
    pObjBullet = NULL;
}

//////////////////////////////////////
/// @brief �Q�[���I�[�o�[��Ԃ�
//////////////////////////////////////
static void setGameOver(void)
{
    // �Q�[���I�[�o�[��Ԃ�
    status_count = 240;
    status = STATUS_GAMEOVER;
    player_left = 0;
    bgDraw_flg |= (BGDRAW_FLG_GAMEOVER | BGDRAW_FLG_LEFT);
}