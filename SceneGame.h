// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// ���̃R�[�h�́A�Q�[���V�[���̃w�b�_�t�@�C�� �ł���B
//
#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "SceneManager.h"

// �v���g�^�C�v�錾
void Game_Init(void);
// �Q�[���V�[���@�X�V
void Game_Update(void);
// �Q�[���V�[���@�`��
void Game_Draw(void);
// �Q�[���V�[���@VSync
void Game_VSync(void);
// �Q�[���V�[���@�N���A�i�I���j
void Game_Clear(void);

#endif // __SCENE_GAME_H__
