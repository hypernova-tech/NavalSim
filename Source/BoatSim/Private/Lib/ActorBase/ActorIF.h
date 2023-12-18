// MyInterface.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorIF.generated.h"

UINTERFACE(MinimalAPI)
class UActorIF : public UInterface
{
    GENERATED_BODY()


};

class AActor;


class IActorIF
{
    GENERATED_BODY()

private:

    AActor* pOwner;

protected:

public:
    virtual void SetBaseActor(AActor* p_actor) ;
    virtual void SetEnabled(bool val);
    virtual bool GetEnabled();
};