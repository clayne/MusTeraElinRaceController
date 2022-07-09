#include "RE/N/NiAVObject.h"

namespace RE {
	void NiAVObject::IPerformOp(PerformOpFunc& a_func)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IPerformOp)>(0x26, 0x27, this, a_func);
	}

	void NiAVObject::IAttachProperty(NiAlphaProperty* a_property)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IAttachProperty)>(0x27, 0x28, this, a_property);
	}

	void NiAVObject::ISetMaterialNeedsUpdate(bool a_needsUpdate)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::ISetMaterialNeedsUpdate)>(0x28, 0x29, this, a_needsUpdate);
	}

	void NiAVObject::ISetDefaultMaterialNeedsUpdateFlag(bool a_flag)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::ISetDefaultMaterialNeedsUpdateFlag)>(0x29, 0x2A, this, a_flag);
	}

	NiAVObject* NiAVObject::IGetObjectByName(const BSFixedString& a_name)
	{
		return REL::RelocateVirtual<decltype(&NiAVObject::IGetObjectByName)>(0x2A, 0x2B, this, a_name);
	}

	void NiAVObject::ISetSelectiveUpdateFlags(bool& a_selectiveUpdate, bool a_selectiveUpdateTransforms, bool& a_rigid)
	{
		return REL::RelocateVirtual<decltype(&NiAVObject::ISetSelectiveUpdateFlags)>(0x2B, 0x2C, this, a_selectiveUpdate, a_selectiveUpdateTransforms, a_rigid);
	}

	void NiAVObject::IUpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateDownwardPass)>(0x2C, 0x2D, this, a_data, a_arg2);
	}

	void NiAVObject::IUpdateSelectedDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateSelectedDownwardPass)>(0x2D, 0x2E, this, a_data, a_arg2);
	}

	void NiAVObject::IUpdateRigidDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateRigidDownwardPass)>(0x2E, 0x2F, this, a_data, a_arg2);
	}

	void NiAVObject::IUpdateWorldBound()
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateWorldBound)>(0x2F, 0x30, this);
	}

	void NiAVObject::IUpdateWorldData(NiUpdateData* a_data)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateWorldData)>(0x30, 0x31, this, a_data);
	}

	void NiAVObject::IUpdateTransformAndBounds(NiUpdateData& a_data)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IUpdateTransformAndBounds)>(0x31, 0x32, this, a_data);
	}

	void NiAVObject::IPreAttachUpdate(NiNode* a_parent, NiUpdateData& a_data)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IPreAttachUpdate)>(0x32, 0x33, this, a_parent, a_data);
	}

	void NiAVObject::IPostAttachUpdate()
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IPostAttachUpdate)>(0x33, 0x34, this);
	}

	void NiAVObject::IOnVisible(NiCullingProcess& a_process)
	{
		REL::RelocateVirtual<decltype(&NiAVObject::IOnVisible)>(0x34, 0x35, this, a_process);
	}
}