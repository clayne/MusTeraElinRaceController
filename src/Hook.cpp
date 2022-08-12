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

	/*
	constexpr REL::VariantID SliderLookup(51534, 52409, 0x008E39B0);
	void SlidersHook()
	{
		struct SliderLookup_Entry_Code : Xbyak::CodeGenerator {
			SliderLookup_Entry_Code(void* buf, std::uint64_t funcAddr, std::uint64_t targetAddr) : Xbyak::CodeGenerator(4096, buf)
			{
				Xbyak::Label retnLabel;
				Xbyak::Label funcLabel;

				lea(rcx, ptr[rax + rbx]);		 // Load Slider into RCX
				call(ptr[rip + funcLabel]);		 // Call function
				movss(xmm6, xmm0);				 // Move return into register
				mov(rcx, ptr[rcx + 0x18]);		 // Restore overwrite (this assumes our call doesnt clobber RCX)
				jmp(ptr[rip + retnLabel]);		 // Jump back

				L(funcLabel);
				dq(funcAddr);

				L(retnLabel);
				dq(targetAddr + 0x5);
			}
		};

		void* codeBuf = g_localTrampoline.StartAlloc();
		SliderLookup_Entry_Code code(codeBuf, uintptr_t(SliderLookup_Hooked), SliderLookup.GetUIntPtr());
		g_localTrampoline.EndAlloc(code.getCurr());

		g_branchTrampoline.Write5Branch(SliderLookup_Target.GetUIntPtr(), uintptr_t(code.getCode()));
	}
	*/

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
