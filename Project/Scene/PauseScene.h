#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
#include <map>
class SceneController;
class MenuUI;
class Input;
class PauseScene :
    public SceneBase
{
public:
    PauseScene(SceneController& controller);
    ~PauseScene();
    //”h¶ƒNƒ‰ƒX‚ÅÀ‘•‚ğÀ‘•
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    virtual void DebugDraw()const override {};
private:
  
};

