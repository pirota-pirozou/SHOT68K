// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃R�[�h�{�� �ł���B
// Path: SceneGame.c
//

#include "SceneGame.h"

// �Q�[���V�[���@������
void Game_Init(void)
{
    printf("Game_Init() ");
}

// �Q�[���V�[���@�X�V
void Game_Update(void)
{
    printf("Game_Update() ");
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
    printf("Game_Draw()\n");
}

// �Q�[���V�[���@VSync
void Game_VSync(void)
{
    printf(" Game_VSync()\n");
}

// �Q�[���V�[���@�N���A�i�I���j
void Game_Clear(void)
{
    printf("**Game_Clear()\n");
}
