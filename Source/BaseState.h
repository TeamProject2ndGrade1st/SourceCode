#pragma once
#include <vector>
#include "Argent/Component/BaseActor.h"

class State
{
public:
	State(Argent::Component::BaseActor* chara) :owner(chara) {}
	 virtual ~State() {}

	 //�X�e�[�g�ɓ��������ɌĂ΂��
	 virtual void Enter() = 0;

	 //�X�e�[�g�Ŗ��t���[���Ă΂��
	 virtual void Execute() = 0;

	 //�X�e�[�g����o��Ƃ��ɌĂ΂��
	 virtual void Exit() = 0;

protected:
	Argent::Component::BaseActor* owner;

};
