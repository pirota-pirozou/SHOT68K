// Path: SceneManager.c
// ���̃R�[�h�́A�T���v���v���O�����̃V�[���}�l�[�W���[���� SceneManager.c �ł���B
// �V�[���̍X�V�A�`��A�ؑւ̊Ǘ����s��
//
#include "SceneManager.h"

static FuncPtr0 SceneManager_Func_Init = NULL;
static FuncPtr0 SceneManager_Func_Update = NULL;
static FuncPtr0 SceneManager_Func_Draw = NULL;
static FuncPtr0 SceneManager_Func_Clear = NULL;
static FuncPtr0 SceneManager_Func_VSync = NULL;
static BOOL isSceneChanged = FALSE;

static pSSceneWork scenes = NULL;

/////////////////////////////////
// �V�[���}�l�[�W���[���̂̏�����
/////////////////////////////////
void SceneManager_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    isSceneChanged = FALSE;
}

/////////////////////////////////
// �V�[���}�l�[�W���[�F�V�[���ؑ�
/////////////////////////////////
void SceneManager_ChangeScene(int nextscene)
{
//    self->nextScene = scene;
    isSceneChanged = TRUE;
}

/////////////////////////////////
// �V�[���}�l�[�W���[�F�V�[���o�^
/////////////////////////////////
void SceneManager_Regist(pSSceneWork scene)
{
    scenes = scene;
}

/////////////////////////////////
// �V�[���F������
/////////////////////////////////
void Scene_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    isSceneChanged = FALSE;
}

/////////////////////////////////
// �V�[���F�X�V����
/////////////////////////////////
void Scene_Update()
{
    if (SceneManager_Func_Update != NULL)
    {
        SceneManager_Func_Update();
    }
}

/////////////////////////////////
// �V�[���F�`�揈��
/////////////////////////////////
void Scene_Draw()
{
    if (SceneManager_Func_Draw != NULL)
    {
        SceneManager_Func_Draw();
    }
}

/////////////////////////////////
// �V�[���F�ؑ֏���
/////////////////////////////////
void Scene_Clear()
{
    if (SceneManager_Func_Clear != NULL)
    {
        SceneManager_Func_Clear();
    }
}

/////////////////////////////////
// �V�[���FVSync����
/////////////////////////////////
void Scene_VSync()
{
    if (SceneManager_Func_VSync != NULL)
    {
        SceneManager_Func_VSync();
    }
}
