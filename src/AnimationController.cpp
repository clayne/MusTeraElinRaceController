#include "AnimationController.h"

namespace Mus {
	AnimationController::AnimationController(RE::Actor* actor, ControllerConfig config)
		: elin(actor)
		, id(actor->formID)
		, name(actor->GetDisplayFullName())
		, animConfig(config)
		, BeforeTime(clock()) {
		if (assignEarsNodes())
			resetEarsAnimation();

		if (assignTailNodes())
			resetTailAnimation();
	}

	AnimationController::~AnimationController() {
	}

	void AnimationController::update(RE::Actor* actor) 
	{
		if (!actor || !actor->loadedData || !actor->loadedData->data3D || !actor->loadedData->data3D.get())
			return;

		if (IsRaceSexMenu.load() && isPlayer(id))
		{
			if (!isResetRaceMenu)
			{
				if (assignEarsNodes())
					resetEarsAnimation();

				if (assignTailNodes())
					resetTailAnimation();

				isResetRaceMenu = true;
			}
			return;
		}

		elin = actor;
		isCountUpdated = false;
		currentTime = clock();
		isResetRaceMenu = false;

		condition_type type = IsCondition();

		if (assignEarsNodes())
			updateEarsAnimation(std::to_underlying(type));

		if (assignTailNodes())
			updateTailAnimation(std::to_underlying(type));

		isCannotReadNode(true);
	}

	void AnimationController::updateEarsAnimation(std::uint8_t type)
	{
		condition_type condtype = condition_type(type);
		logger::trace("{:x} {} - Update Ears Animation : {}", id, name, magic_enum::enum_name(condtype));
		if (condtype == condition_type::button)
			currentState = condtype;
		else if (currentState != condition_type::facemorph && currentState != condition_type::random)
			currentState = condtype;

		switch (currentState)
		{
		case condition_type::button:
			DoEarsAnimation(std::to_underlying(currentState));
			break;
		case condition_type::facemorph:
			MorphEarsAnimation();
			break;
		case condition_type::random:
			DoEarsAnimation(std::to_underlying(currentState));
			break;
		default:
			UnDoEarsAnimation();
			break;
		}

		updateEarsNodes();
	}

	void AnimationController::updateTailAnimation(std::uint8_t type)
	{
		condition_type condtype = condition_type(type);
		logger::trace("{:x} {} - Update Tail Animation : {}", id, name, magic_enum::enum_name(condtype));
		if (condtype == condition_type::button)
			currentState = condtype;
		else if (currentState != condition_type::facemorph && currentState != condition_type::random)
			currentState = condtype;

		switch (currentState)
		{
		case condition_type::button:
			DoTailAnimation();
			break;
		case condition_type::facemorph:
			MorphTailAnimation();
			break;
		case condition_type::random:
			DoTailAnimation();
			break;
		default:
			UnDoTailAnimation();
			break;
		}

		updateTailNodes();
	}

	void AnimationController::DoEarsAnimation(std::uint8_t state)
	{
		condition_type animstate = condition_type(state);
		if (animstate == condition_type::button)
			DoEarsLAnimationNormal();
		else if (animstate == condition_type::random)
		{
			if (RandomAnimType == anim_state2::Total)
			{
				RandomAnimType = GetRandomType();
				IsDoing = true;
			}

			logger::trace("{:x} {} - Random Ears Animation : {}", id, name, magic_enum::enum_name(RandomAnimType));

			if (!IsDoing)
			{
				switch (RandomAnimType) {
				case anim_state2::NormalL:
					if (!animConfig.Reversed)
						DoEarsLAnimationNormal();
					else
						UnDoEarsLAnimationNormal();
					break;
				case anim_state2::NormalR:
					if (!animConfig.Reversed)
						DoEarsRAnimationNormal();
					else
						UnDoEarsRAnimationNormal();
					break;
				case anim_state2::Normal:
					if (!animConfig.Reversed)
					{
						DoEarsLAnimationNormal();
						DoEarsRAnimationNormal();
					}
					else
					{
						UnDoEarsLAnimationNormal();
						UnDoEarsRAnimationNormal();
					}
					break;
				case anim_state2::AttentionL:
					if (!animConfig.Reversed)
						DoEarsLAnimationAttention();
					else
						UnDoEarsLAnimationAttention();
					break;
				case anim_state2::AttentionR:
					if (!animConfig.Reversed)
						DoEarsRAnimationAttention();
					else
						UnDoEarsRAnimationAttention();
					break;
				case anim_state2::Attention:
					if (!animConfig.Reversed)
					{
						DoEarsLAnimationAttention();
						DoEarsRAnimationAttention();
					}
					else
					{
						UnDoEarsLAnimationAttention();
						UnDoEarsRAnimationAttention();
					}
					break;
				case anim_state2::JitterL:
					if (!animConfig.Reversed)
						DoEarsLAnimationJitter();
					else
						UnDoEarsLAnimationJitter();
					break;
				case anim_state2::JitterR:
					if (!animConfig.Reversed)
						DoEarsRAnimationJitter();
					else
						UnDoEarsRAnimationJitter();
					break;
				case anim_state2::Jitter:
					if (!animConfig.Reversed)
					{
						DoEarsLAnimationJitter();
						DoEarsRAnimationJitter();
					}
					else
					{
						UnDoEarsLAnimationJitter();
						UnDoEarsRAnimationJitter();
					}
					break;
				}
			}
			else {
				switch (RandomAnimType) {
				case anim_state2::NormalL:
					if (animConfig.Reversed)
						DoEarsLAnimationNormal();
					else
						UnDoEarsLAnimationNormal();
					break;
				case anim_state2::NormalR:
					if (!animConfig.Reversed)
						DoEarsRAnimationNormal();
					else
						UnDoEarsRAnimationNormal();
					break;
				case anim_state2::Normal:
					if (animConfig.Reversed)
					{
						DoEarsLAnimationNormal();
						DoEarsRAnimationNormal();
					}
					else
					{
						UnDoEarsLAnimationNormal();
						UnDoEarsRAnimationNormal();
					}
					break;
				case anim_state2::AttentionL:
					if (animConfig.Reversed)
						DoEarsLAnimationAttention();
					else
						UnDoEarsLAnimationAttention();
					break;
				case anim_state2::AttentionR:
					if (animConfig.Reversed)
						DoEarsRAnimationAttention();
					else
						UnDoEarsRAnimationAttention();
					break;
				case anim_state2::Attention:
					if (animConfig.Reversed)
					{
						DoEarsLAnimationAttention();
						DoEarsRAnimationAttention();
					}
					else
					{
						UnDoEarsLAnimationAttention();
						UnDoEarsRAnimationAttention();
					}
					break;
				case anim_state2::JitterL:
					if (animConfig.Reversed)
						DoEarsLAnimationJitter();
					else
						UnDoEarsLAnimationJitter();
					break;
				case anim_state2::JitterR:
					if (animConfig.Reversed)
						DoEarsRAnimationJitter();
					else
						UnDoEarsRAnimationJitter();
					break;
				case anim_state2::Jitter:
					if (animConfig.Reversed)
					{
						DoEarsLAnimationJitter();
						DoEarsRAnimationJitter();
					}
					else
					{
						UnDoEarsLAnimationJitter();
						UnDoEarsRAnimationJitter();
					}
					break;
				}
			}

			if (std::to_underlying(RandomAnimType) <= std::to_underlying(anim_state2::Normal))
			{
				if (!animConfig.Reversed)
				{
					if (IsDoing && (compareNiPoint3(LEarRot, EarLLimitNormal) || compareNiPoint3(REarRot, EarRLimitNormal)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
				else
				{
					if (IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, EarLLimitNormal) || compareNiPoint3(REarRot, EarRLimitNormal)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
			}
			else if (std::to_underlying(RandomAnimType) <= std::to_underlying(anim_state2::Attention))
			{
				if (!animConfig.Reversed)
				{
					if (IsDoing && (compareNiPoint3(LEarRot, EarLLimitAttention) || compareNiPoint3(REarRot, EarRLimitAttention)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
				else
				{
					if (IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, EarLLimitAttention) || compareNiPoint3(REarRot, EarRLimitAttention)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
			}
			else if (std::to_underlying(RandomAnimType) <= std::to_underlying(anim_state2::Jitter))
			{
				if (!animConfig.Reversed)
				{
					if (IsDoing && (compareNiPoint3(LEarRot, EarLLimitJitter) || compareNiPoint3(REarRot, EarRLimitJitter)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
				else
				{
					if (IsDoing && (compareNiPoint3(LEarRot, emptyPoint) || compareNiPoint3(REarRot, emptyPoint)))
						IsDoing = false;
					else if (!IsDoing && (compareNiPoint3(LEarRot, EarLLimitJitter) || compareNiPoint3(REarRot, EarRLimitJitter)))
					{
						IsDoing = true;
						isRandomTime(true);
						currentState = condition_type::nothing;
					}
				}
			}
		
		}
	}
	

	void AnimationController::DoEarsLAnimationNormal()
	{
		if (!IsDoL)
		{
			IsDoL = true;
			velocityL = 1.0f;
		}
		logger::trace("{:x} {} - Doing EarL AnimNormal", id, name);

		LEarRot += (EarLUnitNormal * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitNormal);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot += (EarLDUnitNormal * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitNormal);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoEarsRAnimationNormal()
	{
		if (!IsDoR)
		{
			IsDoR = true;
			velocityR = 1.0f;
		}
		logger::trace("{:x} {} - Doing EarR AnimNormal", id, name);

		REarRot += (EarRUnitNormal * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitNormal);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot += (EarRDUnitNormal * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitNormal);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoEarsLAnimationAttention()
	{
		if (!IsDoL)
		{
			IsDoL = true;
			velocityL = 1.0f;
		}
		logger::trace("{:x} {} - Doing EarL Anim", id, name);

		LEarRot += (EarLUnitAttention * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitAttention);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot += (EarLDUnitAttention * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitAttention);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoEarsRAnimationAttention()
	{
		if (!IsDoR)
		{
			IsDoR = true;
			velocityR = 1.0f;
		}
		REarRot += (EarRUnitAttention * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitAttention);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot += (EarRDUnitAttention * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitAttention);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoEarsLAnimationJitter()
	{
		if (!IsDoL)
		{
			IsDoL = true;
			velocityL = 1.0f;
		}
		LEarRot += (EarLUnitJitter * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitJitter);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot += (EarLDUnitJitter * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitJitter);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoEarsRAnimationJitter()
	{
		if (!IsDoR)
		{
			IsDoR = true;
			velocityR = 1.0f;
		}
		REarRot += (EarRUnitJitter * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitJitter);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot += (EarRDUnitJitter * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitJitter);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::DoTailAnimation()
	{
		if (!IsDo)
		{
			IsDo = true;
			velocity = 1.0f;
		}
	}

	void AnimationController::UnDoEarsAnimation()
	{
		
	}

	void AnimationController::UnDoEarsLAnimationNormal()
	{
		if (IsDoL)
		{
			IsDoL = false;
			velocityL = 1.0f;
		}
		LEarRot -= (EarLUnitNormal * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitNormal);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot -= (EarLDUnitNormal * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitNormal);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoEarsRAnimationNormal()
	{
		if (IsDoR)
		{
			IsDoR = false;
			velocityR = 1.0f;
		}
		REarRot -= (EarRUnitNormal * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitNormal);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot -= (EarRDUnitNormal * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitNormal);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoEarsLAnimationAttention()
	{
		if (IsDoL)
		{
			IsDoL = false;
			velocityL = 1.0f;
		}
		LEarRot -= (EarLUnitAttention * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitAttention);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot -= (EarLDUnitAttention * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitAttention);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoEarsRAnimationAttention()
	{
		if (IsDoR)
		{
			IsDoR = false;
			velocityR = 1.0f;
		}
		REarRot -= (EarRUnitAttention * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitAttention);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot -= (EarRDUnitAttention * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitAttention);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoEarsLAnimationJitter()
	{
		if (IsDoL)
		{
			IsDoL = false;
			velocityL = 1.0f;
		}
		LEarRot -= (EarLUnitJitter * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LEarRot, EarLLimitJitter);
		setNodeRotation(en.EarL11, LEarRot);
		setNodeRotation(en.EarL21, LEarRot);

		LDEarRot -= (EarLDUnitJitter * velocityL * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(LDEarRot, EarLDLimitJitter);
		setNodeRotation(en.EarL31, LDEarRot);

		velocityL *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoEarsRAnimationJitter()
	{
		if (IsDoR)
		{
			IsDoR = false;
			velocityR = 1.0f;
		}
		REarRot -= (EarRUnitJitter * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(REarRot, EarRLimitJitter);
		setNodeRotation(en.EarR11, REarRot);
		setNodeRotation(en.EarR21, REarRot);

		RDEarRot -= (EarRDUnitJitter * velocityR * animConfig.AnimationEarsSpeed * IntervalTimeTick);
		clampNiPoint3(RDEarRot, EarRDLimitJitter);
		setNodeRotation(en.EarR31, RDEarRot);

		velocityR *= (1.0f + (0.1f * IntervalTimeTick));
	}

	void AnimationController::UnDoTailAnimation()
	{
		if (IsDo)
		{
			IsDo = false;
			velocity = 1.0f;
		}
	}

	void AnimationController::MorphEarsAnimation()
	{

	}

	void AnimationController::MorphTailAnimation()
	{

	}

	AnimationController::condition_type AnimationController::IsCondition()
	{
		if (IsPressedButton())
			return condition_type::button;
		else if (isFaceMorph())
			return condition_type::facemorph;
		else if (isRandomTime(false))
			return condition_type::random;

		return condition_type::nothing;
	}

	bool AnimationController::isRandomTime(bool isReset)
	{
		if (isReset)
		{
			isRandomWait = false;
			BeforeTime = currentTime;
			logger::trace("{:x} {} - Reset Random Time", id, name);
			return false;
		}

		if (!isRandomWait)
		{
			isRandomWait = true;
			std::srand(static_cast<unsigned int>(time(NULL)));
			int maxtime = (animConfig.FrequencyMax - animConfig.FrequencyMin) * 1000;
			RandomTime = (std::rand() % maxtime) + (animConfig.FrequencyMin * 1000);
		}

		return (currentTime - BeforeTime >= RandomTime);
	}

	bool AnimationController::isFaceMorph()
	{
		auto FaceGenData = Mus::ActorManager::GetSingleton().GetFaceGenData(id);
		if (!FaceGenData)
			return false;

		auto FaceGenAnimValue = FaceGenData->GetActiveMorphValue();

		if (FaceGenAnimValue < animConfig.EmotionAnimationActiveThreshold)
			return false;

		auto FaceGenAnimType = FaceGenData->GetActiveMorphType();

		return (animation_type::noAnim != getAnimationType(FaceGenAnimType));
	}

	animation_type AnimationController::getAnimationType(std::uint32_t expression)
	{
		FaceGenMorphDetector::expression_type type = FaceGenMorphDetector::expression_type(expression);
		switch (type)
		{
		case FaceGenMorphDetector::expression_type::dialogue_anger:
			return animConfig.DialogueAnger;
		case FaceGenMorphDetector::expression_type::dialogue_fear:
			return animConfig.DialogueFear;
		case FaceGenMorphDetector::expression_type::dialogue_happy:
			return animConfig.DialogueHappy;
		case FaceGenMorphDetector::expression_type::dialogue_sad:
			return animConfig.DialogueSad;
		case FaceGenMorphDetector::expression_type::dialogue_surprise:
			return animConfig.DialogueSurprise;
		case FaceGenMorphDetector::expression_type::dialogue_puzzled:
			return animConfig.DialoguePuzzled;
		case FaceGenMorphDetector::expression_type::dialogue_disgust:
			return animConfig.DialogueDisgust;
		case FaceGenMorphDetector::expression_type::mood_anger:
			return animConfig.MoodAnger;
		case FaceGenMorphDetector::expression_type::mood_fear:
			return animConfig.MoodFear;
		case FaceGenMorphDetector::expression_type::mood_happy:
			return animConfig.MoodHappy;
		case FaceGenMorphDetector::expression_type::mood_sad:
			return animConfig.MoodSad;
		case FaceGenMorphDetector::expression_type::mood_surprise:
			return animConfig.MoodSurprise;
		case FaceGenMorphDetector::expression_type::mood_puzzled:
			return animConfig.MoodPuzzled;
		case FaceGenMorphDetector::expression_type::mood_disgust:
			return animConfig.MoodDisgust;
		default:
			return animation_type::noAnim;
		}

		return animation_type::noAnim;
	}

	void AnimationController::resetEarsAnimation()
	{
		if (!animConfig.Reversed)
		{
			en.EarL11->local.translate = emptyPoint;
			en.EarR11->local.translate = emptyPoint;

			en.EarL21->local.translate = emptyPoint;
			en.EarR21->local.translate = emptyPoint;

			en.EarL31->local.translate = emptyPoint;
			en.EarR31->local.translate = emptyPoint;

			en.EarL11->local.rotate = emptyRotate;
			en.EarR11->local.rotate = emptyRotate;

			en.EarL21->local.rotate = emptyRotate;
			en.EarR21->local.rotate = emptyRotate;

			en.EarL31->local.rotate = emptyRotate;
			en.EarR31->local.rotate = emptyRotate;
		}
		else
		{
			en.EarL11->local.translate = emptyPoint;
			en.EarR11->local.translate = emptyPoint;

			en.EarL21->local.translate = emptyPoint;
			en.EarR21->local.translate = emptyPoint;

			en.EarL31->local.translate = emptyPoint;
			en.EarR31->local.translate = emptyPoint;

			en.EarL11->local.rotate = EarLLimitNormal;
			en.EarR11->local.rotate = EarRLimitNormal;

			en.EarL21->local.rotate = EarLLimitNormal;
			en.EarR21->local.rotate = EarRLimitNormal;

			en.EarL31->local.rotate = EarLDLimitNormal;
			en.EarR31->local.rotate = EarRDLimitNormal;
		}
	}

	void AnimationController::resetTailAnimation()
	{
		en.TailBone1->local.translate = emptyPoint;

		en.TailSpine->local.translate = emptyPoint;

		en.TailBone1->local.rotate = emptyRotate;

		en.TailSpine->local.rotate = emptyRotate;
	}

	bool AnimationController::assignEarsNodes()
	{
		isValidNode = true;

		en.EarL11 = GetVirtualNode(Nodes::EarL11);
		en.EarR11 = GetVirtualNode(Nodes::EarR11);

		en.EarL21 = GetVirtualNode(Nodes::EarL21);
		en.EarR21 = GetVirtualNode(Nodes::EarR21);

		en.EarL31 = GetVirtualNode(Nodes::EarL31);
		en.EarR31 = GetVirtualNode(Nodes::EarR31);

		return isValidNode;
	}

	bool AnimationController::assignTailNodes()
	{
		isValidNode = true;

		en.TailBone1 = GetVirtualNode(Nodes::TailBone1);

		en.TailSpine = GetVirtualNode(Nodes::TailSpine);

		return isValidNode;
	}

	void AnimationController::updateEarsNodes()
	{
		updateNode(en.EarL11);
		updateNode(en.EarR11);

		updateNode(en.EarL21);
		updateNode(en.EarR21);

		updateNode(en.EarL31);
		updateNode(en.EarR31);
	}

	void AnimationController::updateTailNodes()
	{
		updateNode(en.TailBone1);

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

	AnimationController::anim_state2 AnimationController::GetRandomType()
	{
		std::srand(static_cast<unsigned int>(time(NULL)));
		std::uint8_t total = std::to_underlying(anim_state2::Total) / 3;
		anim_state2 type = anim_state2(std::rand() % (total - 1));
		return type;
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


