#pragma once
#include <vector>
class BaseFriend;

class State
{
public:
	State(BaseFriend* _friend, const char* name) :owner(_friend), name(name) {}
	 virtual ~State() {}

	 //ステートに入った時に呼ばれる
	 virtual void Enter() = 0;

	 //ステートで毎フレーム呼ばれる
	 virtual void Execute() = 0;

	 //ステートから出るときに呼ばれる
	 virtual void Exit() = 0;

protected:
	BaseFriend* owner;
	const char* name;
};


class HierarchicalState : public State
{
public:
	HierarchicalState(BaseFriend* _friend) :State(_friend, "hierarchical") {}
	virtual ~HierarchicalState() {}

	virtual void Enetr() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;

	virtual void SetSubState(int newState);
	virtual void ChangeSubState(int newState);
	virtual void RegisterSubState(State* state);
	virtual State* SetSubState() { return subState; }
	virtual int GetSubStateIndex();

protected:
	std::vector<State*> subStatePool;
	State* subState = nullptr;
};
