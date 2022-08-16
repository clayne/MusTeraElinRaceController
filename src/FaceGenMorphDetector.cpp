#include "FaceGenMorphDetector.h"

namespace Mus
{
	FaceGenMorphDetector::FaceGenMorphDetector(RE::Actor* actor, std::uint8_t EmotionEffectActiveThreshold)
		: elin(actor)
		, id(actor->formID)
		, name(actor->GetDisplayFullName()) {

		beforeTime = clock();
		cooldown = Config::GetSingleton().GetEmotionScanDelay();
		ActiveThreshold = EmotionEffectActiveThreshold;
		isEnable = Config::GetSingleton().GetEnableEmotion();
		isEyes = Config::GetSingleton().GetEmotionEyes();
		isIcons = Config::GetSingleton().GetEmotionIcons();
		isTears = Config::GetSingleton().GetEmotionTears();
		isOverlay = Config::GetSingleton().GetEmotionHeadOverlay();
		isPlayerOnly = Config::GetSingleton().GetEmotionHeadOverlayOnlyPlayer();

		if (!isPlayer(id))
		{
			PlayerRun = false;
			if (isValidMorphNodes = GetFaceNodeNameList(); !isValidMorphNodes && !PlayerRun)
				logger::error("can't get emotion headparts : {:x} {}", id, name);
		}
		else
			PlayerFirstTime = clock();
	}

	FaceGenMorphDetector::~FaceGenMorphDetector()
	{

	}

	void FaceGenMorphDetector::update(RE::Actor* actor)
	{
		if (!IsPlayerElin.load() && isPlayer(id))
			return;

		if (!isValidMorphNodes && !PlayerRun)
			return;

		if (cooldown > 0)
		{
			if (clock() - beforeTime < cooldown && !PlayerRun)
				return;
			else
				beforeTime = clock();
		}

		elin = actor;

		CurrentActiveFaceGenMorphType = IGetActiveMorphType();
		CurrentActiveFaceGenMorphValue = IGetActiveMorphValue();
		
		if (PlayerRun)
		{
			if (clock() - PlayerFirstTime >= PlayerFirstCooldown)
			{
				logger::info("Player headpart scanned and updated while RaceSexMenu loading is done");
				PlayerRun = false;
			}
			if (isValidMorphNodes = GetFaceNodeNameList(); !isValidMorphNodes)
				logger::error("can't get emotion headparts : {:x} {}", id, name);
			isNeedUpdatedMorph = true;
		}

		if (isNeedSendEvent)
			if (!SendFaceGenMorphEvent())
				logger::error("can't send the event : {:x} {}", id, name);

		if (isNeedUpdatedMorph || (IsRaceSexMenu.load() && isPlayer(id) && IsPlayerElin.load()))
			if (!updateEmotionMorphNodes())
				logger::error("can't update the emotion effects : {:x} {}", id, name);
		
		isNeedSendEvent = false;
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
		{
			isNeedUpdatedMorph = true;
			isNeedSendEvent = true;
		}

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

		if (value < ActiveThreshold && CurrentActiveFaceGenMorphValue >= ActiveThreshold)
		{
			isNeedSendEvent = true;
			isNeedUpdatedMorph = true;
		}
		else if (value >= ActiveThreshold && CurrentActiveFaceGenMorphValue < ActiveThreshold)
		{
			isNeedSendEvent = true;
			isNeedUpdatedMorph = true;
		}

		logger::trace("Get active morph value : {}", value);
		return value;
	}

	bool FaceGenMorphDetector::updateEmotionMorphNodes()
	{
		if (!isValidMorphNodes)
			return false;

		bool isVisible = true;
		if (GetActiveMorphValue() < ActiveThreshold)
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
				if (IsMatchName(list.first, "Icon"))
				{
					if (IsMatchName(list.first, "Sad"))
						isVisible = isTears;
					else
						isVisible = isIcons;
				}
				else if (IsMatchName(list.first, "eye"))
				{
					isVisible = isEyes;
				}

				auto alphalist = FaceNodeAlphaList.find(list.first);
				float alpha = 0.0f;
				if (alphalist != FaceNodeAlphaList.end())
					alpha = isVisible ? alphalist->second : 0.0f;
				else
					alpha = isVisible ? 1.0f : 0.0f;

				SetAlphaEmotionMorph(node, alpha);
				logger::trace("{:x} {} update node {} to {}", id, name, nodeName.c_str(), (isVisible ? "visible" : "invisible"));
			}
			else
			{
				SetAlphaEmotionMorph(node, 0.0f);
				logger::trace("{:x} {} update node {} to invisible", id, name, nodeName.c_str());
			}
		}

		return true;
	}

	bool FaceGenMorphDetector::SetAlphaEmotionMorph(RE::NiAVObject* obj, float alpha)
	{
		if (!obj)
			return false;

		float materialAlpha = isEnable ? alpha : 0.0f;

		logger::trace("{:x} {} : try set alpha of the face node...", id, name);

		RE::BSGeometry* a_geometry = static_cast<RE::BSGeometry*>(obj);
		
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

		material->materialAlpha = materialAlpha;
		logger::trace("{:x} {} : set alpha of the face node {}", id, name, materialAlpha);
		return true;
	}

	float FaceGenMorphDetector::GetAlphaEmotionMorph(RE::NiAVObject* obj)
	{
		if (!obj)
			return false;

		RE::BSGeometry* a_geometry = static_cast<RE::BSGeometry*>(obj);

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

		return material->materialAlpha;
	}

	bool FaceGenMorphDetector::FixAlphaProperty(RE::NiAVObject* obj)
	{
		if (!obj)
			return false;

		logger::trace("{:x} {} : fix alpha property issue...", id, name);

		RE::BSGeometry* a_geometry = static_cast<RE::BSGeometry*>(obj);
		
		using State = RE::BSGeometry::States;

		auto effect = a_geometry->GetGeometryRuntimeData().properties[State::kEffect].get();
		if (!effect)
			return false;

		auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect);
		if (!lightingShader)
			return false;

		lightingShader->SetFlags(RE::BSShaderProperty::EShaderPropertyFlag8::kZBufferWrite, false);

		logger::trace("{:x} {} : fixed alpha property issue", id, name);
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
		if (!isOverlay)
			return true;

		if (isPlayerOnly && !isPlayer(id))
			return true;

		RE::BSTEventSource<SKSE::ModCallbackEvent>* modEvent = SKSE::GetModCallbackEventSource();

		if (modEvent)
		{
			FaceGenMorphEvent.sender = elin;
			float numArg = 1.0f;
			if (GetActiveMorphValue() < ActiveThreshold)
				numArg = 0.0f;
			FaceGenMorphEvent.numArg = numArg;
			std::string_view strArg = magic_enum::enum_name(expression_type(GetActiveMorphType()));
			nif::setBSFixedString(FaceGenMorphEvent.strArg, strArg.data());

			modEvent->SendEvent(&FaceGenMorphEvent);
			logger::trace("EmotionEvent Send : {:x} {} {} {}", id, name, numArg, strArg.data());
		}
		else
		{
			logger::error("Can't Send EmotionEvent : {:x} {}", id, name);
		}

		return true;
	}

	bool FaceGenMorphDetector::GetFaceNodeNameList()
	{
		bool result = false;
		if (!PlayerRun)
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
			
			std::string nodeName = obj->name.c_str();
			if (!PlayerRun)
				logger::debug("{:x} {} : found the Face node {}", id, name, nodeName.c_str());
			for (std::uint8_t index = 0; index < std::to_underlying(expression_type_simple::Total); index++)
			{
				expression_type_simple type = expression_type_simple(index);
				if (IsMatchName(nodeName, magic_enum::enum_name(type).data()))
				{
					FixAlphaProperty(obj);
					FaceNodeNameList.emplace(nodeName, index);
					FaceNodeAlphaList.emplace(nodeName, GetAlphaEmotionMorph(obj));
					if (!PlayerRun)
						logger::info("{:x} {} : insert the Face node {} on the list", id, name, nodeName.c_str());
					result = true;
					continue;
				}
			}
		}

		if (!result && !PlayerRun)
			logger::error("Can't get {:x} {} Facenode list", id, name);
		
		return result;
	}

	void FaceGenMorphDetector::PlayerReset(RE::Actor* actor)
	{
		if (!actor)
			return;

		elin = actor;

		PlayerRun = true;
		PlayerFirstTime = clock();
	}



	RaceSexMenuTracker::RaceSexMenuTracker()
	{

	}

	RaceSexMenuTracker::~RaceSexMenuTracker()
	{

	}

	void RaceSexMenuTracker::onEvent(const FrameEvent& e)
	{
		if (!IsRaceSexMenu.load())
			return;

		TrackingRaceSexMenu();
	}

	bool RaceSexMenuTracker::TrackingRaceSexMenu()
	{
		if (!IsRaceSexMenu.load())
			return false;

		TrackingSliderMap();
		TrackingTintMasks();
		return true;
	}

	bool RaceSexMenuTracker::InitTrackingList()
	{
		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return false;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return false;

		raceindex = GetElinRaceindex();

		if (raceindex == 0)
		{
			logger::error("Cannot find the Elin race slider data!");
			return false;
		}

		std::vector<RE::BSTArrayBase::size_type> indexlist = Config::GetSingleton().GetLookupSliderIndexs();
		AddSliderIndexOnLookupSliderMap(indexlist);

		std::vector<std::string> slidernames = Config::GetSingleton().GetLookupSliderNames();
		AddSliderNamesOnLookupSliderMap(slidernames);
		
		return true;
	}

	RE::BSTArrayBase::size_type RaceSexMenuTracker::GetElinRaceindex()
	{
		RE::BSTArrayBase::size_type index = 0;

		if (!IsPlayerElin.load())
			return index;

		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return index;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return index;

		RE::SEX sex = PlayerBase->GetSex();

		const auto& Menu = RE::UI::GetSingleton();
		auto IMenu = Menu->GetMenu(RE::RaceSexMenu::MENU_NAME).get();
		if (!IMenu)
			return index;

		RE::RaceSexMenu* RSM = (RE::RaceSexMenu*)IMenu;
		if (!RSM)
			return index;

		const auto& RSMData = RSM->GetRuntimeData();
		auto& RaceData = RSMData.sliderData[sex];

		RE::TESRace* PlayerRace = PlayerBase->GetRace();
		if (!PlayerRace)
			return index;

		concurrency::parallel_for(RE::BSTArrayBase::size_type(0), RaceData.size(), [&](RE::BSTArrayBase::size_type RaceIndex) 
		{
			if (RaceData[RaceIndex].race && RaceData[RaceIndex].race->formID == PlayerRace->formID)
			{
				index = RaceIndex;
			}
		});

		return index;
	}

	void RaceSexMenuTracker::GetTintMasks()
	{
		RE::PlayerCharacter* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto* FullTintList = Player->GetTintList();
		if (!FullTintList)
			return;

		for (std::uint32_t type = 0; type < RE::TintMask::TintType::Total; type++)
		{
			auto& TintList = FullTintList[type];
			for (auto& Tint : TintList)
			{
				LookupTintMap.emplace(Tint->tintType, Tint->abgr);
			}
		}
	}

	void RaceSexMenuTracker::AddSliderIndexOnLookupSliderMap(std::vector<RE::BSTArrayBase::size_type> sliderIndexs)
	{
		if (raceindex == 0)
			return;

		if (!IsPlayerElin.load())
			return;

		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return;

		RE::SEX sex = PlayerBase->GetSex();

		const auto& Menu = RE::UI::GetSingleton();
		RE::RaceSexMenu* RSM = (RE::RaceSexMenu*)Menu->GetMenu(RE::RaceSexMenu::MENU_NAME).get();
		if (!RSM)
			return;

		const auto& RSMData = RSM->GetRuntimeData();
		auto& RaceData = RSMData.sliderData[sex];
		auto& RaceComponent = RaceData[raceindex];
		auto& sliders = RaceComponent.sliders;

		for (auto& index : sliderIndexs)
		{
			auto& SexSliderMap = LookupSliderMap[index];
			SexSliderMap[sex] = sliders[index].value;

			logger::info("Add Elin {} Slider {} {} {} on LookupSliderMap", ((sex == RE::SEX::kMale) ? "Male" : "Female"), index, sliders[index].name, sliders[index].value);
		}
	}

	void RaceSexMenuTracker::AddSliderNamesOnLookupSliderMap(std::vector<std::string> sliderNames)
	{
		if (raceindex == 0)
			return;

		if (!IsPlayerElin.load())
			return;

		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return;

		RE::SEX sex = PlayerBase->GetSex();

		const auto& Menu = RE::UI::GetSingleton();

		RE::RaceSexMenu* RSM = (RE::RaceSexMenu*)Menu->GetMenu(RE::RaceSexMenu::MENU_NAME).get();
		if (!RSM)
			return;

		const auto& RSMData = RSM->GetRuntimeData();
		auto& RaceData = RSMData.sliderData[sex];
		auto& RaceComponent = RaceData[raceindex];
		auto& sliders = RaceComponent.sliders;
		concurrency::parallel_for(RE::BSTArrayBase::size_type(0), sliders.size(), [&](RE::BSTArrayBase::size_type sliderIndex)
		{
			std::string name = sliders[sliderIndex].name;
			if (std::find(sliderNames.begin(), sliderNames.end(), name) != sliderNames.end())
			{
				auto& SexSliderMap = LookupSliderMap[sliderIndex];
				SexSliderMap[sex] = sliders[sliderIndex].value;

				logger::info("Add Elin {} Slider {} {} {} on LookupSliderMap", ((sex == RE::SEX::kMale) ? "Male" : "Female"), sliderIndex, name, sliders[sliderIndex].value);
			}
		});
	}

	void RaceSexMenuTracker::TrackingSliderMap()
	{
		if (!IsRaceSexMenu.load())
			return;

		if (!IsPlayerElin.load())
			return;
		
		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return;

		RE::SEX sex = PlayerBase->GetSex();

		const auto& Menu = RE::UI::GetSingleton();

		RE::RaceSexMenu* RSM = (RE::RaceSexMenu*)Menu->GetMenu(RE::RaceSexMenu::MENU_NAME).get();
		if (!RSM)
			return;

		const auto& RSMData = RSM->GetRuntimeData();
		auto& RaceData = RSMData.sliderData[sex];
		auto& RaceComponent = RaceData[raceindex];
		auto& sliders = RaceComponent.sliders;

		for (auto& slidermap : LookupSliderMap)
		{
			float value = sliders[slidermap.first].value;
			if (slidermap.second[sex] == value)
				continue;

			SendSliderChangeEvent(sliders[slidermap.first].name, slidermap.first, value);
			slidermap.second[sex] = value;
		}
	}

	void RaceSexMenuTracker::TrackingTintMasks()
	{
		if (!IsRaceSexMenu.load())
			return;

		if (!IsPlayerElin.load())
			return;

		RE::PlayerCharacter* Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto* FullTintList = Player->GetTintList();
		if (!FullTintList)
			return;

		for (std::uint32_t type = 0; type < RE::TintMask::TintType::Total; type++)
		{
			auto& TintList = FullTintList[type];
			for (auto& Tint : TintList)
			{
				if (LookupTintMap[Tint->tintType] == Tint->abgr)
					continue;

				SendTintChangeEvent(magic_enum::enum_name(RE::TintMask::TintType(Tint->tintType)), Tint->abgr);
				LookupTintMap[Tint->tintType] = Tint->abgr;
			}
		}
	}

	bool RaceSexMenuTracker::SendSliderChangeEvent(std::string sliderName, RE::BSTArrayBase::size_type index, float numArg)
	{
		RE::BSTEventSource<SKSE::ModCallbackEvent>* modEvent = SKSE::GetModCallbackEventSource();

		auto Player = RE::PlayerCharacter::GetSingleton();

		if (modEvent && Player)
		{
			SliderChangeEvent.sender = Player;
			SliderChangeEvent.numArg = numArg;
			std::string strArg = std::to_string(index);
			strArg += "|";
			strArg += sliderName;
			nif::setBSFixedString(SliderChangeEvent.strArg, strArg.c_str());

			modEvent->SendEvent(&SliderChangeEvent);
			logger::trace("SliderChangeEvent Send : {} {}", strArg.c_str(), numArg);
		}
		else
		{
			logger::error("Can't Send SliderChangeEvent");
		}
		return true;
	}

	bool RaceSexMenuTracker::SendTintChangeEvent(std::string_view typeName, std::uint32_t abgr)
	{
		RE::BSTEventSource<SKSE::ModCallbackEvent>* modEvent = SKSE::GetModCallbackEventSource();

		auto Player = RE::PlayerCharacter::GetSingleton();

		if (modEvent && Player)
		{
			TintChangeEvent.sender = Player;
			TintChangeEvent.numArg = abgr;
			std::string_view strArg = typeName;
			nif::setBSFixedString(TintChangeEvent.strArg, strArg.data());

			modEvent->SendEvent(&TintChangeEvent);
			logger::trace("TintChangeEvent Send : {} {}", strArg.data(), abgr);
		}
		else
		{
			logger::error("Can't Send TintChangeEvent");
		}
		return true;
	}


	void RaceSexMenuTracker::DebugSliderPrint()
	{
		if (std::to_underlying(Config::GetSingleton().GetLogLevel()) > std::to_underlying(spdlog::level::level_enum::debug))
			return; //print only debug / trace

		auto Player = RE::PlayerCharacter::GetSingleton();
		if (!Player)
			return;

		auto PlayerBase = Player->GetActorBase();
		if (!PlayerBase)
			return;

		RE::SEX sex = PlayerBase->GetSex();

		const auto& Menu = RE::UI::GetSingleton();

		auto IMenu = Menu->GetMenu(RE::RaceSexMenu::MENU_NAME).get();
		if (!IMenu)
			return;
		
		RE::RaceSexMenu* RSM = (RE::RaceSexMenu*)IMenu;
		if (!RSM)
			return;

		raceindex = GetElinRaceindex();

		const auto& RSMData = RSM->GetRuntimeData();
		auto& RaceData = RSMData.sliderData[sex];
		auto& RaceComponent = RaceData[raceindex];
		auto& sliders = RaceComponent.sliders;

		for (RE::BSTArrayBase::size_type index = 0; index < sliders.size(); index++)
		{
			logger::info("{} : Name {} /Index {} / Index {} / ID {} / Value {}", (sex == RE::SEX::kMale) ? "Male" : "Female", sliders[index].name, sliders[index].index, index, sliders[index].id, sliders[index].value);
		}
	}
}
