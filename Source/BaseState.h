#pragma once
#include <vector>
#include "Argent/Component/BaseActor.h"

class State
{
public:
	State(Argent::Component::BaseActor* chara) :owner(chara) {}
	 virtual ~State() {}

	 //ステートに入った時に呼ばれる
	 virtual void Enter() = 0;

	 //ステートで毎フレーム呼ばれる
	 virtual void Execute() = 0;

	 //ステートから出るときに呼ばれる
	 virtual void Exit() = 0;

protected:
	Argent::Component::BaseActor* owner;

};
