#include "AnimationController.h"

namespace Mus {
	AnimationController::AnimationController(RE::Actor* actor, ControllerConfig config)
	{
		if (!actor)
			return;
		
		elin = actor;
		id = actor->formID;
		name = actor->GetDisplayFullName();
		animConfig = config;
	}

	AnimationController::~AnimationController() 
	{

	}

	void AnimationController::update(RE::Actor* actor) 
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D || !actor->loadedData->data3D.get())
			return;

		elin = actor;

		if (assignEarsNodes())
			updateEarsAnimation();

		if (assignTailsNodes())
			updateTailsAnimation();

		isCannotReadNode(true);
	}

	void AnimationController::updateEarsAnimation()
	{

	}

	void AnimationController::updateTailsAnimation()
	{

	}

	void AnimationController::resetAnimation()
	{

	}

	bool AnimationController::assignEarsNodes()
	{
		isValidNode = true;

		EarL11 = GetVirtualNode(Nodes::EarL11);
		EarR11 = GetVirtualNode(Nodes::EarR11);
		EarL12 = GetVirtualNode(Nodes::EarL12);
		EarR12 = GetVirtualNode(Nodes::EarR12);
		EarL13 = GetVirtualNode(Nodes::EarL13);
		EarR13 = GetVirtualNode(Nodes::EarR13);

		EarL21 = GetVirtualNode(Nodes::EarL21);
		EarR21 = GetVirtualNode(Nodes::EarR21);
		EarL22 = GetVirtualNode(Nodes::EarL22);
		EarR22 = GetVirtualNode(Nodes::EarR22);
		EarL23 = GetVirtualNode(Nodes::EarL23);
		EarR23 = GetVirtualNode(Nodes::EarR23);

		EarL31 = GetVirtualNode(Nodes::EarL31);
		EarR31 = GetVirtualNode(Nodes::EarR31);
		EarL32 = GetVirtualNode(Nodes::EarL32);
		EarR32 = GetVirtualNode(Nodes::EarR32);

		return isValidNode;
	}

	bool AnimationController::assignTailsNodes()
	{
		isValidNode = true;

		TailBone1 = GetVirtualNode(Nodes::TailBone1);
		TailBone2 = GetVirtualNode(Nodes::TailBone2);
		TailBone3 = GetVirtualNode(Nodes::TailBone3);

		TailSpine = GetVirtualNode(Nodes::TailSpine);

		return isValidNode;
	}

	RE::NiAVObject* AnimationController::GetVirtualNode(const RE::BSFixedString nodeName)
	{
		if (!isValidNode)
			return nullptr;

		RE::NiAVObject* node = elin->loadedData->data3D.get();

		RE::NiAVObject* findNode;
		if ((findNode = node->GetObjectByName(nif::GetVirtualNodeName(nodeName))))
			return findNode;
		
		if (std::find(VNodeEntries.begin(), VNodeEntries.end(), nodeName.c_str()) != VNodeEntries.end())
		{
			isCannotReadNode(false);
			logger::debug("{} / {:x} : {} is already attached, but cannot use it", name.c_str(), id, nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		RE::NiAVObject* origNode = node->GetObjectByName(nodeName);
		if (!origNode)
		{
			logger::debug("{} / {:x} : {} is missing", name.c_str(), id, nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		if (!(findNode = nif::addParentToNode(origNode, nif::GetVirtualNodeName(nodeName).c_str())))
		{
			logger::debug("{} / {:x} : {} is not attached on parent node of {}", name.c_str(), id, nif::GetVirtualNodeName(nodeName), nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		logger::info("{} / {:x} : {} is attached on parent node of {}", name.c_str(), id, nif::GetVirtualNodeName(nodeName), nodeName.c_str());
		VNodeEntries.emplace_back(nif::GetVirtualNodeName(nodeName).c_str());
		return findNode;
	}

	void AnimationController::isCannotReadNode(bool isEnd)
	{
		if (isCountUpdated && !isEnd)
			return;

		if (!isCountUpdated && isEnd)
		{
			isCannotReadCount = -1;
		}

		if (!isEnd && isCannotReadCount < 0)
		{
			isCannotReadCount = 3;
		}
		else if (!isEnd && isCannotReadCount > 0)
		{
			isCannotReadCount--;
		}
		else if (!isEnd && isCannotReadCount == 0)
		{
			logger::debug("{} / {:x} : Reset Virtual Nodes", name, id);
			VNodeEntries.clear();
			isCannotReadCount--;
		}

		isCountUpdated = true;
	}
}


