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
static BOOL isFirst = TRUE;
static int sceneID = -1;
static int nextSceneID = -1;
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
    isFirst = TRUE;
    sceneID = nextSceneID = -1;
}

/////////////////////////////////
// �V�[��ID�̊֐��ɐ؂�ւ���
/////////////////////////////////
static void SetChangeFunc(int id)
{

}

/////////////////////////////////
// �V�[���}�l�[�W���[�F�V�[���ؑ�
/////////////////////////////////
void SceneManager_ChangeScene(int nextscene)
{
    if (sceneID != nextscene)
    {
        nextSceneID = nextscene;
        pSSceneWork scene = &scenes[nextSceneID];
        SceneManager_Func_Init = scene->Init;
        SceneManager_Func_Update = scene->Update;
        SceneManager_Func_Draw = scene->Draw;
        SceneManager_Func_Clear = scene->Clear;
        SceneManager_Func_VSync = scene->VSync;
        isSceneChanged = TRUE;
        // �����Change��������AInit���Ă�
        if (isFirst)
        {
            isFirst = FALSE;
            if (SceneManager_Func_Init != NULL)
            {
                SceneManager_Func_Init();
            }
        }
    }
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
    // �V�[���ؑւ���������A�V�[����؂�ւ���
    if (isSceneChanged)
    {
        isSceneChanged = FALSE;
        Scene_Clear();      // ���݂̃V�[���̃N���A����
        SceneManager_ChangeScene(nextSceneID);
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
