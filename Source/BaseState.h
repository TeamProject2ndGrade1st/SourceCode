#pragma once
#include <vector>
class BaseFriend;

class FriendState
{
public:
	FriendState(BaseFriend* _friend, const char* name) :owner(_friend), name(name) {}
	 virtual ~FriendState() {}

	 //ステートに入った時に呼ばれる
	 virtual void Enter() = 0;

	 //ステートで毎フレーム呼ばれる
	 virtual void Execute() = 0;

	 //ステートから出るときに呼ばれる
	 virtual void Exit() = 0;

	 virtual void DrawDebug() {}

protected:
	BaseFriend* owner;
	const char* name;
};


class HierarchicalState : public FriendState
{
public:
	HierarchicalState(BaseFriend* _friend) :FriendState(_friend, "hierarchical") {}
	virtual ~HierarchicalState() {}

	virtual void Enetr() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;

	virtual void SetSubState(int newState);
	virtual void ChangeSubState(int newState);
	virtual void RegisterSubState(FriendState* state);
	virtual FriendState* SetSubState() { return subState; }
	virtual int GetSubStateIndex();

protected:
	std::vector<FriendState*> subStatePool;
	FriendState* subState = nullptr;
};
