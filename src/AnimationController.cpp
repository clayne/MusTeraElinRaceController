#include "AnimationController.h"

namespace Mus {
	AnimationController::AnimationController(RE::Actor* actor, ControllerConfig config)
		: elin(actor)
		, id(actor->formID)
		, name(actor->GetDisplayFullName())
		, animConfig(config) {
	}

	AnimationController::~AnimationController() {
	}

	void AnimationController::update(RE::Actor* actor) 
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D || !actor->loadedData->data3D.get())
			return;

		elin = actor;

		if (assignEarsNodes())
			updateEarsAnimation();

		if (assignTailNodes())
			updateTailAnimation();

		isCannotReadNode(true);
	}

	void AnimationController::updateEarsAnimation()
	{
		updateEarsNodes();
	}

	void AnimationController::updateTailAnimation()
	{
		updateTailNodes();
	}

	void AnimationController::resetEarsAnimation()
	{
		en.EarL11->local.translate = emptyPoint;
		en.EarR11->local.translate = emptyPoint;
		en.EarL12->local.translate = emptyPoint;
		en.EarR12->local.translate = emptyPoint;
		en.EarL13->local.translate = emptyPoint;
		en.EarR13->local.translate = emptyPoint;

		en.EarL21->local.translate = emptyPoint;
		en.EarR21->local.translate = emptyPoint;
		en.EarL22->local.translate = emptyPoint;
		en.EarR22->local.translate = emptyPoint;
		en.EarL23->local.translate = emptyPoint;
		en.EarR23->local.translate = emptyPoint;

		en.EarL31->local.translate = emptyPoint;
		en.EarR31->local.translate = emptyPoint;
		en.EarL32->local.translate = emptyPoint;
		en.EarR32->local.translate = emptyPoint;

		en.EarL11->local.rotate = emptyRotate;
		en.EarR11->local.rotate = emptyRotate;
		en.EarL12->local.rotate = emptyRotate;
		en.EarR12->local.rotate = emptyRotate;
		en.EarL13->local.rotate = emptyRotate;
		en.EarR13->local.rotate = emptyRotate;

		en.EarL21->local.rotate = emptyRotate;
		en.EarR21->local.rotate = emptyRotate;
		en.EarL22->local.rotate = emptyRotate;
		en.EarR22->local.rotate = emptyRotate;
		en.EarL23->local.rotate = emptyRotate;
		en.EarR23->local.rotate = emptyRotate;

		en.EarL31->local.rotate = emptyRotate;
		en.EarR31->local.rotate = emptyRotate;
		en.EarL32->local.rotate = emptyRotate;
		en.EarR32->local.rotate = emptyRotate;
	}

	void AnimationController::resetTailAnimation()
	{
		en.TailBone1->local.translate = emptyPoint;
		en.TailBone2->local.translate = emptyPoint;
		en.TailBone3->local.translate = emptyPoint;

		en.TailSpine->local.translate = emptyPoint;

		en.TailBone1->local.rotate = emptyRotate;
		en.TailBone2->local.rotate = emptyRotate;
		en.TailBone3->local.rotate = emptyRotate;

		en.TailSpine->local.rotate = emptyRotate;
	}

	bool AnimationController::assignEarsNodes()
	{
		isValidNode = true;

		en.EarL11 = GetVirtualNode(Nodes::EarL11);
		en.EarR11 = GetVirtualNode(Nodes::EarR11);
		en.EarL12 = GetVirtualNode(Nodes::EarL12);
		en.EarR12 = GetVirtualNode(Nodes::EarR12);
		en.EarL13 = GetVirtualNode(Nodes::EarL13);
		en.EarR13 = GetVirtualNode(Nodes::EarR13);

		en.EarL21 = GetVirtualNode(Nodes::EarL21);
		en.EarR21 = GetVirtualNode(Nodes::EarR21);
		en.EarL22 = GetVirtualNode(Nodes::EarL22);
		en.EarR22 = GetVirtualNode(Nodes::EarR22);
		en.EarL23 = GetVirtualNode(Nodes::EarL23);
		en.EarR23 = GetVirtualNode(Nodes::EarR23);

		en.EarL31 = GetVirtualNode(Nodes::EarL31);
		en.EarR31 = GetVirtualNode(Nodes::EarR31);
		en.EarL32 = GetVirtualNode(Nodes::EarL32);
		en.EarR32 = GetVirtualNode(Nodes::EarR32);

		return isValidNode;
	}

	bool AnimationController::assignTailNodes()
	{
		isValidNode = true;

		en.TailBone1 = GetVirtualNode(Nodes::TailBone1);
		en.TailBone2 = GetVirtualNode(Nodes::TailBone2);
		en.TailBone3 = GetVirtualNode(Nodes::TailBone3);

		en.TailSpine = GetVirtualNode(Nodes::TailSpine);

		return isValidNode;
	}

	void AnimationController::updateEarsNodes()
	{
		updateNode(en.EarL11);
		updateNode(en.EarL12);
		updateNode(en.EarL13);
		updateNode(en.EarR11);
		updateNode(en.EarR12);
		updateNode(en.EarR13);

		updateNode(en.EarL21);
		updateNode(en.EarL22);
		updateNode(en.EarL23);
		updateNode(en.EarR21);
		updateNode(en.EarR22);
		updateNode(en.EarR23);

		updateNode(en.EarL31);
		updateNode(en.EarL32);
		updateNode(en.EarR31);
		updateNode(en.EarR32);
	}

	void AnimationController::updateTailNodes()
	{
		updateNode(en.TailBone1);
		updateNode(en.TailBone2);
		updateNode(en.TailBone3);

		updateNode(en.TailSpine);
	}

	void AnimationController::updateNode(RE::NiAVObject* node)
	{
		nif::nodeTaskInterface::GetSingleton().AddnewNodeTask(node, true, false);
	}

	RE::NiAVObject* AnimationController::GetVirtualNode(RE::BSFixedString nodeName)
	{
		if (!isValidNode)
			return nullptr;

		RE::NiAVObject* rootnode = elin->Get3D();
		if (!rootnode)
			return nullptr;

		RE::NiAVObject* findNode;
		if (findNode = rootnode->IGetObjectByName(nif::GetVirtualNodeName(nodeName)); findNode)
			return findNode;
		
		if (std::find(VNodeEntries.begin(), VNodeEntries.end(), nif::GetVirtualNodeName(nodeName).c_str()) != VNodeEntries.end())
		{
			isCannotReadNode(false);
			logger::debug("{} / {:x} : {} is already attached, but cannot use it", name, id, nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		RE::NiAVObject* origNode;
		if (origNode = rootnode->IGetObjectByName(nodeName); !origNode)
		{
			logger::debug("{} / {:x} : {} is missing", name, id, nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		if (findNode = nif::addParentToNode(origNode, nif::GetVirtualNodeName(nodeName).c_str()); !findNode)
		{
			logger::debug("{} / {:x} : {} is not attached on parent node of {}", name, id, nif::GetVirtualNodeName(nodeName).c_str(), nodeName.c_str());
			isValidNode = false;
			return nullptr;
		}

		logger::info("{} / {:x} : {} is attached on parent node of {}", name, id, nif::GetVirtualNodeName(nodeName).c_str(), nodeName.c_str());
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


