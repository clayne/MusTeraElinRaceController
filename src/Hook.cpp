#include "hook.h"

namespace Mus {
	constexpr std::uint64_t GameLoopFunction_SE = 35565;
	constexpr std::uint64_t GameLoopFunction_AE = 36564;
	constexpr std::uint64_t GameLoopFunction_VR = 0x005BAB10;

	constexpr REL::VariantID GameLoopFunction(35565, 36564, 0x005BAB10);

	EventDispatcherImpl<FrameEvent>  g_frameEventDispatcher;

	struct UnkEngine
	{
		MEMBER_FN_PREFIX(UnkEngine);

		DEFINE_MEMBER_FN_HOOK(onFrame, void, GameLoopFunction.address());

		void onFrame();

		char unk[0x16];
		bool gamePaused; // 16
	};

	void UnkEngine::onFrame()
	{
		CALL_MEMBER_FN(this, onFrame)();
		FrameEvent e;
		e.gamePaused = this->gamePaused;
		g_frameEventDispatcher.dispatch(e);
	}

	void hookEngine()
	{
		DetourAttach((void**)UnkEngine::_onFrame_GetPtrAddr(), (void*)GetFnAddr(&UnkEngine::onFrame));
	}

	void unhookEngine()
	{
		DetourDetach((void**)UnkEngine::_onFrame_GetPtrAddr(), (void*)GetFnAddr(&UnkEngine::onFrame));
	}


	void hook()
	{
		logger::trace("Skyrim Engine Hooking...");
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		hookEngine();
		DetourTransactionCommit();
	}

	void unhook()
	{
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		unhookEngine();
		DetourTransactionCommit();
	}

}
