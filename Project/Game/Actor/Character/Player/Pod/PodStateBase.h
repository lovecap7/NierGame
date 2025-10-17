#pragma once
#include "../../CharacterStateBase.h"
class Actor;
class PodStateBase abstract :
    public CharacterStateBase
{
public:
    PodStateBase(std::weak_ptr<Actor>  pod);
    virtual ~PodStateBase();
protected:
    Vector3 GetPodPos(Vector3 targetPos, Vector3 cameraDir, float upH, float right, float back);
};

