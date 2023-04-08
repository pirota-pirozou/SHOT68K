// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃R�[�h�{�� �ł���B
// Path: SceneGame.c
//

#include "SceneGame.h"

// �Q�[���V�[���@������
void Game_Init(void)
{
//    printf("Game_Init() ");
    OBJManager_Init();      // �I�u�W�F�N�g�}�l�[�W���[�̏�����
    BGTEXTCL(1, 0x0100);	// BGTEXT1 CLR
}

// �Q�[���V�[���@�X�V
void Game_Update(void)
{
//    printf("Game_Update() ");
    CM_bg_puts("GAME_UPDATE()", 0, 0, 1);
    ObjManager_Update();    // �I�u�W�F�N�g�}�l�[�W���[�̍X�V
    // �Q�[���p�b�h�̒l���擾
    unsigned short pad = GamePadManager_GetPad();
    unsigned short pad_trg = GamePadManager_GetTrigger();

    if (pad_trg & PAD_A)
    {
        // A�{�^���Ń^�C�g���V�[���֑J��
        SceneManager_ChangeScene(SCENE_ID_TITLE);
    }
}

// �Q�[���V�[���@�`��
void Game_Draw(void)
{
//    printf("Game_Draw()\n");
    CM_bg_puts("GAME_DRAW()", 0, 1, 1);
    ObjManager_Draw();      // �I�u�W�F�N�g�}�l�[�W���[�̕`��
    CM_sp_set(0, 16, 16, 0x0141, 3);

}

// �Q�[���V�[���@VSync
void Game_VSync(void)
{
    CM_bg_puts("GAME_VSYNC()", 0, 2, 1);
//    printf(" Game_VSync()\n");
}

// �Q�[���V�[���@�N���A�i�I���j
void Game_Clear(void)
{
    ObjManager_End();       // �I�u�W�F�N�g�}�l�[�W���[�̏I��
}
