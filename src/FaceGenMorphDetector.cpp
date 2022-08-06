#include "FaceGenMorphDetector.h"

namespace Mus
{
	FaceGenMorphDetector::FaceGenMorphDetector(RE::Actor* actor)
		: elin(actor)
		, id(actor->formID)
		, name(actor->GetDisplayFullName()) {

		beforeTime = clock();
		if (id == 0x14 || id == 0x7)
			cooldown = 4000; //4sec wait
		else
			cooldown = Config::GetSingleton().GetSetting().GetFeature().GetEmotionScanCooldown();

		ActiveLimit = Config::GetSingleton().GetSetting().GetFeature().GetEmotionActiveLimit();
	}

	FaceGenMorphDetector::~FaceGenMorphDetector()
	{

	}

	void FaceGenMorphDetector::update(RE::Actor* actor)
	{
		if (!isValidMorphNodes)
			return;

		if (clock() - beforeTime < cooldown)
			return;
		else
			beforeTime = clock();

		elin = actor;

		CurrentActiveFaceGenMorphType = IGetActiveMorphType();
		CurrentActiveFaceGenMorphValue = IGetActiveMorphValue();

		if (PlayerRun)
		{
			if (isValidMorphNodes = GetFaceNodeNameList(); !isValidMorphNodes)
				logger::error("can't get emotion headparts : {:x} {}", id, name);
			PlayerRun = false;
			isNeedUpdatedMorph = true;
			cooldown = Config::GetSingleton().GetSetting().GetFeature().GetEmotionScanCooldown();
		}

		if (isNeedUpdatedMorph)
			if (!SendFaceGenMorphEvent())
				logger::error("can't send the event : {:x} {}", id, name);

		if (isNeedUpdatedMorph || IsRaceSexMenu.load())
			if (!updateEmotionMorphNodes())
				logger::error("can't update the emotion effects : {:x} {}", id, name);

		isNeedUpdatedMorph = false;
	}

	std::uint32_t FaceGenMorphDetector::IGetActiveMorphType()
	{
		std::uint32_t expression = std::to_underlying(expression_type::mood_neutral);

		RE::BSFaceGenAnimationData* expressionData = elin->GetFaceGenAnimationData();
		if (!expressionData)
			return std::to_underlying(expression_type::mood_neutral);

		for (std::uint32_t index = 0; index < expressionData->expressionKeyFrameResult.count; ++index) {
			if (expressionData->expressionKeyFrameResult.values[index] > expressionData->expressionKeyFrameResult.values[expression]) {
				expression = index;
			}
		}

		if (GetActiveMorphType() != expression)
			isNeedUpdatedMorph = true;

		logger::trace("Get active morph type : {} {}", expression, magic_enum::enum_name(expression_type(expression)));
		return expression;
	}

	std::uint8_t FaceGenMorphDetector::IGetActiveMorphValue()
	{
		RE::BSFaceGenAnimationData* expressionData = elin->GetFaceGenAnimationData();
		if (!expressionData)
			return 0;

		//RE::BSSpinLockGuard locker(expressionData->lock);

		std::uint32_t type = GetActiveMorphType();
		std::uint8_t value = 0;

		if (type < expressionData->expressionKeyFrameResult.count)
			value = std::lround(expressionData->expressionKeyFrameResult.values[type] * 100.0f);

		if (value < ActiveLimit && CurrentActiveFaceGenMorphValue >= ActiveLimit)
			isNeedUpdatedMorph = true;
		else if (value >= ActiveLimit && CurrentActiveFaceGenMorphValue < ActiveLimit)
			isNeedUpdatedMorph = true;

		logger::trace("Get active morph value : {}", value);
		return value;
	}

	bool FaceGenMorphDetector::updateEmotionMorphNodes()
	{
		if (!isValidMorphNodes)
			return false;

		bool isVisible = true;
		if (GetActiveMorphValue() < ActiveLimit)
			isVisible = false;

		RE::BSFaceGenNiNode* FaceNode = elin->GetFaceNodeSkinned();

		if (!FaceNode)
			return false;

		std::uint8_t findType = GetEmotionSimpleType();


		for (auto& list : FaceNodeNameList)
		{	
			RE::BSFixedString nodeName;
			nif::setBSFixedString(nodeName, list.first.c_str());
			RE::NiAVObject* node = FaceNode->IGetObjectByName(nodeName);
			if (!node)
			{
				logger::debug("{:x} {} Can't found the node : {}", id, name, nodeName.c_str());
				continue;
			}

			std::uint8_t type = list.second;
			if (type == findType)
			{
				SetAlphaEmotionMorph(node, isVisible);
				logger::trace("{:x} {} update node {} to {}", id, name, nodeName.c_str(), (isVisible ? "visible" : "invisible"));
			}
			else
			{
				SetAlphaEmotionMorph(node, false);
				logger::trace("{:x} {} update node {} to invisible", id, name, nodeName.c_str());
			}
		}

		return true;
	}

	bool FaceGenMorphDetector::SetAlphaEmotionMorph(RE::NiAVObject* obj, bool visible)
	{
		if (!obj)
			return false;

		logger::trace("{:x} {} : try set alpha of the face node...", id, name);

		RE::BSGeometry* a_geometry = reinterpret_cast<RE::BSGeometry*>(obj);

		using State = RE::BSGeometry::States;

		auto effect = a_geometry->GetGeometryRuntimeData().properties[State::kEffect].get();
		if (!effect)
			return false;

		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
		if (!lightingShader)
			return false;

		auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
		if (!material)
			return false;

		material->materialAlpha = visible ? 1.0f : 0.0f;
		logger::trace("{:x} {} : set alpha of the face node {}", id, name, (visible ? 1.0f : 0.0f));
		return true;
	}

	std::uint8_t FaceGenMorphDetector::GetEmotionSimpleType()
	{
		auto type = GetActiveMorphType();

		if (type >= std::to_underlying(expression_type::Total))
			return std::to_underlying(expression_type_simple::Natural);
		else if (type >= std::to_underlying(expression_type::combat_anger))
			return std::to_underlying(expression_type_simple::Anger);
		else if (type > std::to_underlying(expression_type::mood_neutral))
			return (type - std::to_underlying(expression_type::mood_neutral));
		else if (type < std::to_underlying(expression_type::mood_neutral))
			return type;

		return std::to_underlying(expression_type_simple::Natural);
	}

	bool FaceGenMorphDetector::SendFaceGenMorphEvent()
	{
		RE::BSTEventSource<SKSE::ModCallbackEvent>* modEvent = SKSE::GetModCallbackEventSource();

		if (modEvent)
		{
			FaceGenMorphEvent.sender = elin;
			float numArg = 1.0f;
			if (GetActiveMorphValue() < ActiveLimit)
				numArg = 0.0f;
			FaceGenMorphEvent.numArg = numArg;
			std::string_view strArg = magic_enum::enum_name(expression_type(GetActiveMorphType()));
			nif::setBSFixedString(FaceGenMorphEvent.strArg, strArg.data());

			modEvent->SendEvent(&FaceGenMorphEvent);
			logger::trace("EmotionEvent Send : {:x} {} {} {}", id, name, numArg, strArg.data());
		}
		else
			return false;

		return true;
	}

	bool FaceGenMorphDetector::GetFaceNodeNameList()
	{
		bool result = false;
		logger::info("Get {:x} {} Facenode list...", id, name);

		RE::BSFaceGenNiNode* FaceNode = elin->GetFaceNodeSkinned();

		if (!FaceNode)
			return result;

		RE::NiNode* node = FaceNode->AsNode();
		if (!node)
			return result;

		for (auto& child : node->GetChildren())
		{
			auto obj = child.get();
			if (!obj || !obj->name.c_str())
				continue;
			
			std::string nodeName = lowLetter(obj->name.c_str());
			logger::debug("{:x} {} : found the Face node {}", id, name, nodeName.c_str());
			for (std::uint8_t index = 0; index < std::to_underlying(expression_type_simple::Total); index++)
			{
				expression_type_simple type = expression_type_simple(index);
				std::string typestr = lowLetter(magic_enum::enum_name(type).data());
				if (nodeName.find(typestr) != std::string::npos)
				{
					FaceNodeNameList.emplace(nodeName, index);
					logger::info("{:x} {} : insert the Face node {} on the list", id, name, nodeName.c_str());
					result = true;
					continue;
				}
			}
		}

		if (!result)
			logger::error("Can't get {:x} {} Facenode list", id, name);
		
		return result;
	}

	void FaceGenMorphDetector::PlayerReset(RE::Actor* actor)
	{
		if (!actor)
			return;

		elin = actor;

		cooldown = 2000; //2sec wait
		PlayerRun = true;
		isNeedUpdatedMorph = true;
	}
}