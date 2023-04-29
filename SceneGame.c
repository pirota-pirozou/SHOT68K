// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃R�[�h�{�� �ł���B
// Path: SceneGame.c
//

#include "SceneGame.h"
#include "ObjManager.h"
#include "BMPLoad256.h"

#define PAUSE_X 13                      // �|�[�Y�̕\���ʒu�w
#define PAUSE_Y 15                      // �|�[�Y�̕\���ʒu�x

#define DEADLINE_Y  232                 // �G�̐N���ʒu

#define ENEMY_SETUP_COL   11            // �G�̏����z�u��
#define ENEMY_SETUP_ROW   5             // �G�̏����z�u�s��

#define BGDRAW_FLG_SCORE 0x0001         // �X�R�A�̏��������t���O
#define BGDRAW_FLG_HISCORE 0x0002       // �n�C�X�R�A�̏��������t���O
#define BGDRAW_FLG_LEFT 0x0004          // �c�@�̏��������t���O
#define BGDRAW_FLG_STAGE 0x0008         // �X�e�[�W�̏��������t���O
#define BGDRAW_FLG_GAMEOVER 0x0010      // �Q�[���I�[�o�[���������t���O

extern pBMPFILE256 pBmpBackGround;	    // BMP�t�@�C���f�[�^�i�Q�[���w�i�j

int score = 0;                          // �X�R�A
int hiscore = 0;                        // �n�C�X�R�A

static BOOL bPause = FALSE;             // �|�[�Y�t���O
static uint32 bgDraw_flg;               // BG���������t���O
static pSObj pObjPlayer;                // �v���C���[�̃I�u�W�F�N�g
static pSObj pObjBullet;                // �v���C���[�̒e�̃I�u�W�F�N�g

static int enemy_left;                  // �G�̎c�萔

// �Q�[����Ԃ̊Ǘ��p
enum
{
    STATUS_NORMAL,                      // �i�ʏ�j�Q�[����
    STATUS_INVASION,                    // �N��
    STATUS_MISS,                        // �~�X�i���@����j
    STATUS_CLEAR,                       // �X�e�[�W�N���A
    STATUS_GAMEOVER,                    // �Q�[���I�[�o�[
    STATUS_MAX                          // ��Ԃ̍ő�l
};
static int status;                      // �Q�[�����
static int status_count;                // ��ԑJ�ڃJ�E���^

// �G�̓���@�Ǘ��p
static int enemy_move_idx;              // �G�̓���C���f�b�N�X
static uint16 enemy_touch_wall;         // �G���ǂɐG�ꂽ�t���O
static int stage;                       // �X�e�[�W
static int player_left;                 // �c�@

// �U������GObj�������Ă���e�[�u��
static pSObj enemy_atack_tbl[ENEMY_SETUP_COL];

// �v���g�^�C�v�錾
static void SetPause(BOOL);
static void addScore(int);
static void setupEnemies(void);
static void to_next_stage(void);
static void obj_clear_all(void);
static void obj_clear_ebullet(void);
static void initStage(void);
static void readyPlayer(void);
static void setGameOver(void);
static void setPlayerMiss(void);
static void enemiesMoveDown(int);

/// @brief �Q�[���V�[���@������
void Game_Init(void)
{
    OBJManager_Init();          // �I�u�W�F�N�g�}�l�[�W���[�̏�����
    CM_sprite_clr();            // �X�v���C�g�N���A
    BGTEXTCL(1, 0x0100);	    // BGTEXT1 CLR
    // �O���t�B�b�N��ʂO�̃N���A
    gcls(0);
    // �r�b�g�}�b�v�t�@�C���̕\��
    PutBMPMemory256(pBmpBackGround);

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

    // �v���C���[�̐���
    readyPlayer();
    initStage();                // �X�e�[�W�̏�����
}

/// @brief �G�̒e�𔭎˂���
static void EBullet_Make(void)
{
    pSObj pObj = NULL;
    pSObj pEBullet = NULL;
    // �G�̒e�𔭎�
    int col = rand() % ENEMY_SETUP_COL;
    for (int i=0; i<ENEMY_SETUP_COL; i++)
    {
        pObj = enemy_atack_tbl[col];
        if (pObj == NULL)
        {
            col++;
            col %= ENEMY_SETUP_COL;
            continue;
        }
        else
        {
            pObj = enemy_atack_tbl[col];
            break;
        }
    }
    if (pObj == NULL) return;               // �G�����݂��Ȃ���΃��^�[��

    // �G�e�𐶐�
//    printf("EBullet_Make: x=%d, y=%d\n",pObj->x, pObj->y+8);
    pEBullet = ObjManager_Make(OBJ_ID_EBULLET, pObj->x, pObj->y+8);
}

/// @brief �Q�[���V�[���@�X�V
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
        // �G�̓�����X�V����
        switch (pObj->stat)
        {
            // �ʏ퓮��
            case 0:
                pObj->x += (pObj->vx * 4);
                if (pObj->vx < 0 && pObj->x <= 12)
                {
                    // ���[�ɓ��B������
                    pObj->x = 12;
                    enemy_touch_wall |= 0x01;   // ���ǂɐG�ꂽ�t���O���Z�b�g
                }
                if (pObj->vx > 0 && pObj->x >= 240)
                {
                    // �E�[�ɓ��B������
                    pObj->x = 240;
                    enemy_touch_wall |= 0x02;   // �E�ǂɐG�ꂽ�t���O���Z�b�g
                }
                break;

            // ���Ɉړ����č��E���]
            case 1:
                // ���Ɉړ�
                // ���씽�]
                pObj->vx = -pObj->vx;
                pObj->y += pObj->vy;
                pObj->stat = 0;
                break;

            // ���B���Ȃ�
            default:
                __UNREACHABLE__;
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
        // �f�o�b�O�\��
//        char strtmp[128];
//        sprintf(strtmp, "NEXT_IDX=%3d", next_idx);
//        CM_bg_puts(strtmp, 0, 1, 1);
        if (next_idx >= 0)
        {
            // �����œG�̃V���b�g�𔭎�
            if ((rand() % 256) < 5)
            {
                // 5/256�̊m���œG�e�𔭎�
                EBullet_Make();
            }

            // ���̃L������������
            if (next_idx <= enemy_move_idx)
            {
                // �S�ẴL�����������I�������

                // ���씽�]�`�F�b�N
                if (enemy_touch_wall & 0x01)
                {
                    // ���ǂɐG��Ă�����
                    enemiesMoveDown(8);
                    enemy_touch_wall &= ~0x01;
                }

                if (enemy_touch_wall & 0x02)
                {
                    // �E�ǂɐG��Ă�����
                    enemiesMoveDown(8);
                    enemy_touch_wall &= ~0x02;
                }
            }
        }
        enemy_move_idx = next_idx;
        break;
    case STATUS_INVASION:
        // �N��
        if ((--status_count) <= 0)
        {
            // ���t���[���o�߂�����Q�[���I�[�o�[
            status = STATUS_GAMEOVER;
            status_count = 60*3;
        }
        break;
    case STATUS_MISS:
        // �N��
        if ((--status_count) <= 0)
        {
            // �c�@���炷
            player_left--;
            bgDraw_flg |= BGDRAW_FLG_LEFT;
            if (player_left <= 0)
            {
                // �v���C���[�̎c�@���Ȃ��Ȃ���
                // �Q�[���I�[�o�[��Ԃ�
                setGameOver();
            }
            else
            {
                // ���t���[���o�߂����烊�g���C
                status = STATUS_NORMAL;
                // ���g���C����
                readyPlayer();      // �v���C���[�쐬
                obj_clear_ebullet();    // �G�e������
            }
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
/*
    // �f�o�b�O�p
    if (pad_trg & PAD_A)
    {
        // A�{�^���Ń^�C�g���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
 */
}

/// @brief �Q�[���V�[���@�`��
void Game_Draw(void)
{
    ObjManager_Draw();      // �I�u�W�F�N�g�}�l�[�W���[�̕`��
}

/// @brief �Q�[���V�[���@VSync
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
}

/// @brief �Q�[���V�[���@�N���A�i�I���j
void Game_Clear(void)
{
    ObjManager_End();       // �I�u�W�F�N�g�}�l�[�W���[�̏I��
}

// ObjManager����Ă΂��֐��́ASceneGame ���Ɉړ������B

/////////////////////////////////
// �v���C���[�̍X�V����
/////////////////////////////////
/// @brief �v���C���[�̍X�V����
/// @param pObj �v���C���[�̃I�u�W�F�N�g
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
    if (pad_trg & (PAD_B | PAD_A) )
    {
        // A,B�{�^���Œe����
        if (pObjBullet == NULL)
        {
            pObjBullet = ObjManager_Make(OBJ_ID_PBULLET,
                pObj->x, pObj->y-2);
        }
    }

}

/////////////////////////////////
/// @brief �U��������̍ŉ��s�̓G��T��
/////////////////////////////////
/// @param row �U�������s
/// @param col �U��������
static pSObj SearchNextAttackEnemy(int row, int col)
{
    pSObj pObj = NULL;
    int _row = -1;
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObjEnemy = ObjManager_GetObj(i);
        if (pObjEnemy->id >= OBJ_ID_ENEMY1 &&
            pObjEnemy->id <= OBJ_ID_ENEMY3)
        {
            if (pObjEnemy->col == col)
            {
                if (pObjEnemy->row > _row)
                {
                    // ��ԉ��̓G��������
                    pObj = pObjEnemy;
                    _row = pObjEnemy->row;
                }
            }
        }
    }
    return pObj;
}


/////////////////////////////////
/// @brief ���@�e�̍X�V����
/////////////////////////////////
/// @param pObj ���@�e�̃I�u�W�F�N�g
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
                    pts = 10;
                    break;
                case OBJ_ID_ENEMY2:
                    pts = 20;
                    break;
                case OBJ_ID_ENEMY3:
                    pts = 30;
                    break;
                default:
                    // �����ɂ͗��Ȃ��͂�
                    __UNREACHABLE__;
                }
                // ���ł�����G���U������G��������H
                int col = pObjEnemy->col;
                int row = pObjEnemy->row;
                ObjManager_Destroy(pObjEnemy);  // �G������
                if (enemy_atack_tbl[col] == pObjEnemy)
                {
                    // �U������G�����ł�������A
                    pSObj pObjTmp = SearchNextAttackEnemy(row, col);
                    // ������̎��̓G���U������G�ɂ���
                    enemy_atack_tbl[col] = pObjTmp;
/*
                    if (pObjTmp != NULL)
                    {
                        // �U������G��������A���X�V
                        char strtmp[128];
                        sprintf(strtmp, "NEXT_ATK=%2d %2d", pObjTmp->col, pObjTmp->row);
                        CM_bg_puts(strtmp, 0, 1, 1);
                    }
                    else
                    {
                        CM_bg_puts("NEXT_ATK=NONE   ", 0, 1, 1);
                    }
 */
                }

                //
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
void ObjFunc_EBullet(pSObj pObj)
{
    pObj->y += 1;           // ���Ɉړ�
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
        // �v���C���[�~�X������
        setPlayerMiss();
    }
}


/////////////////////////////////
/// @brief �G�����G�t�F�N�g�̍X�V����
/////////////////////////////////
/// @param pObj �G�����G�t�F�N�g�̃I�u�W�F�N�g
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
    enemy_left = 0;             // �G�̎c�萔
    int sy = 24;

    sy += ((stage-1) * 8);      // �X�e�[�W�ɂ���ď���Y�ʒu��ς���
//    sy = 112;
    if (sy > 112) sy = 112;     // ������݂���

    for (int i = 0; i < ENEMY_SETUP_ROW; i++)
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
        for (int j = 0; j < ENEMY_SETUP_COL; j++)
        {
            int16 _x = (j * 20) + 24;
            int16 _y = (i * 24) + sy;

            pSObj pObj = ObjManager_Make(_id, _x, _y);
            pObj->row = i;      // �G�̍s�ԍ�
            pObj->col = j;      // �G�̗�ԍ�
            pObj->stat = 0;     // �G�̏��

            if (i == (ENEMY_SETUP_ROW-1))
            {
                // �ŉ��i�̓G�̂ݒe����
                enemy_atack_tbl[j] = pObj;
            }

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
            pObj->stat = 1; // ���~�t���O�𗧂Ă�
            pObj->vy = add; // ���~�h�b�g����ݒ�
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
    // �G�̏����z�u
    setupEnemies();

    // �G�̓���C���f�b�N�X��������
    enemy_move_idx = ObjManager_FindEnemyIdx();
    enemy_touch_wall = 0;       // �G���ǂɐG�ꂽ�t���O���N���A

    // �Q�[����Ԃ̏�����
    status = STATUS_NORMAL;
}

//////////////////////////////////////
/// @brief �v���C���[�̏�����
//////////////////////////////////////
static void readyPlayer(void)
{
    pObjPlayer = ObjManager_Make(OBJ_ID_PLAYER, 128, 256-16);
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
/// @brief �G�̒e�̑S����
//////////////////////////////////////
static void obj_clear_ebullet(void)
{
    for (int i = 0; i < OBJ_MAX; i++)
    {
        pSObj pObj = ObjManager_GetObj(i);
        if (pObj->id == OBJ_ID_EBULLET)
        {
            ObjManager_Destroy(pObj);
        }
    }
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

//////////////////////////////////////
/// @brief �v���C���[���~�X����
//////////////////////////////////////
static void setPlayerMiss(void)
{
    // �~�X��Ԃ�
    status_count = 180;
    status = STATUS_MISS;
}