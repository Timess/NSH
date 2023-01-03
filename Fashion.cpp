#include "Fashion.h"
#include "Hooks.h"
#include "Config.h"
#include "Logger.h"

#include <array>
#include <map>


using namespace asmjit;
using namespace asmjit::x86;

Fashion::Fashion() :
	originalFunction(nullptr)
{
	hookClass.emplace(g_trampolineAddress, hooks->GacRunnerNG + ????????????????????????);
	logger->log("%s Code base: 0x%p\n", __FUNCTION__, hookClass->hookcode.baseAddress());

}


UINT64 __fastcall Fashion::hookedFunction(UINT64 a1) {

// ??????????????????????????????????????????????????????????????????
	return hooks->fashion->originalFunction(a1);
}

void Fashion::render() {

	static const std::map<int, const char8_t*> indexOfClothesName{
		{ 0, u8"未知" },
		{ 1, u8"时装" },
		{ 2, u8"头饰" },
		{ 3, u8"未知" },
		{ 4, u8"未知" },
		{ 5, u8"未知" },
		{ 6, u8"发型" },
		{ 7, u8"武器" },
		{ 8, u8"未知" },
		{ 9, u8"未知" },
		{ 0xA, u8"环身" },
		{ 0xB, u8"脚印" },
		{ 0xC, u8"未知" },
	};
	

	static const std::map<UINT32, const char8_t*> indexOfHairDyeName{
		{???????u8"无"},
		{???????,u8"天霓·樱晖"},
		{???????,u8"天霓·棠仙"},
		{???????,u8"天霓·香雪"},
		{???????,u8"天霓·冰绡"},
		{???????,u8"仙绛·曦月"},
		{???????,u8"仙绛·火舞"},
		{???????,u8"浮光·镜海"},
		{???????,u8"浮光·芍药"},
		{???????,u8"绝品·白夜"},
		{???????,u8"绝品·金尊"},
	};

	if (config->fashion.enable) {
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_FirstUseEver);

		ImGui::Begin((char*)u8"时装称号修改", nullptr, windowFlags);

		ImGui::Checkbox((char*)u8"替换全部称号", &config->fashion.enableChangleAllTitle);
		ImGui::SameLine();
		ImGui::Checkbox((char*)u8"屏蔽其他人称号", &config->fashion.enableBlockOtherTitle);

		if (!config->fashion.enableChangleAllTitle) {
			ImGui::InputText((char*)u8"替换称号", (char*)config->fashion.triggerTitle, sizeof(config->fashion.triggerTitle));
			ImGui::SameLine();

		}

		ImGui::Combo((char*)u8"目标称号", &config->fashion.targetTitleIndex, (char*)u8"缥缈云流鹤上仙\0武林之巅独孤求败\0天江城城主\0广州活动称号\0仙袂红袖卷轻纱\0自定义\0");
		if (config->fashion.targetTitleIndex == 5) {
			ImGui::SameLine();
			ImGui::InputText((char*)u8"自定义目标称号", (char*)config->fashion.targetTitle, sizeof(config->fashion.targetTitle));
		}

		ImGui::Checkbox((char*)u8"修改时装", &config->fashion.enableChangeClothes);
		ImGui::SameLine();
		ImGui::Checkbox((char*)u8"修改发型染色", &config->fashion.enableChangeHairDye);
		ImGui::SameLine();
		ImGui::Checkbox((char*)u8"修改时装染色", &config->fashion.enableChangeClothDye);

		ImGui::InputUInt((char*)u8"武器距离匹配", &config->fashion.weaponDistance, 1, 10);

		if (config->fashion.enableChangeClothes) {

			ImGui::BeginGroup();

			for (int i = 0; i < config->fashion.nowClothes.size(); i++) {
				if (strcmp((char*)indexOfClothesName.at(i), (char*)u8"未知")) {
					ImGui::PushID(0x10000000 + i);
					ImGui::Text((char*)u8"当前时装[%s] => %X", indexOfClothesName.at(i), config->fashion.nowClothes[i]);
					ImGui::PopID();
				}
			}


			for (int i = 0; i < config->fashion.setClothes.size(); i++) {
				if (strcmp((char*)indexOfClothesName.at(i), (char*)u8"未知")) {
					ImGui::PushID(0x10000100 + i);
					char temp[128]{};
					sprintf(temp, (char*)u8"替换时装[%s]", indexOfClothesName.at(i));
					ImGui::InputUInt(temp, &config->fashion.setClothes[i], 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
					ImGui::SameLine();
					if (ImGui::Button((char*)u8"清空")) {
						config->fashion.setClothes[i] = 0x0;
					}
					ImGui::PopID();
				}
			}


			if (ImGui::Button((char*)u8"时装 - 樱岚含香")) {
				config->fashion.setClothes[1] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"时装 - 霜????????) {
				config->fashion.setClothes[1] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"时装 - 狐????????
				config->fashion.setClothes[1] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"时装 - 泳????????
				config->fashion.setClothes[1] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"时装 - 泳????????
				config->fashion.setClothes[1] = ????????;
			}									????????
												????????
												????????
			if (ImGui::Button((char*)u8"发型 - 残????????) {
				config->fashion.setClothes[6] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"发型 - 霜????????) {
				config->fashion.setClothes[6] = ????????;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"发型 - 狐????????
				config->fashion.setClothes[6] = ????????;
			}									????????
												????????
			ImGui::Text((char*)u8"荒羽金=2AA3D95????????2AA3D22");
			ImGui::Text((char*)u8"荒羽火=2AA3D99????????2AA3D78");
												????????
			if (ImGui::Button((char*)u8"一键预设????????露")) {
				config->fashion.setClothes[1] = ????????;
				config->fashion.setClothes[6] = ????????;
				config->fashion.setHairDyeIndex ????????
				config->fashion.setClothDyeing =????????fashion.shuangtianninglu;
			}									????????
			ImGui::SameLine();					????????
			if (ImGui::Button((char*)u8"一键预设????????色")) {
				config->fashion.setClothes[1] = ????????;
				config->fashion.setClothes[6] = ????????;
				config->fashion.setHairDyeIndex = 2;
				config->fashion.setClothDyeing = config->fashion.hulifense;
			}

			ImGui::EndGroup();


		}

		ImGui::SameLine();
		ImGui::BeginGroup();

		if (config->fashion.enableChangeHairDye) {
			ImGui::Text((char*)u8"当前发型染色[%s] => %X", indexOfHairDyeName.at(config->fashion.nowHairDye), config->fashion.nowHairDye);
			ImGui::Combo((char*)u8"目标发型染色", &config->fashion.setHairDyeIndex, (char*)u8"默认\0天霓·樱晖\0天霓·棠仙\0天霓·香雪\0天霓·冰绡\0仙绛·曦月\0仙绛·火舞\0浮光·镜海\0浮光·芍药\0绝品·白夜\0绝品·金尊\0");
		}
		ImGui::Dummy({ 1.f,20.f });
		if (config->fashion.enableChangeClothDye) {
			ImGui::BeginGroup();

			ImGui::Text((char*)u8"当前染色时装 => %X", config->fashion.nowClothDyeing.FashionClothesId);


#define TextPartOnce(x,y) \
ImGui::Text((char*)u8"%s[Hue] => %d, [Sat] => %d, [Val] => %d, [Alpha] => %d",\
				#y,\
				config->fashion.nowClothDyeing.##x##.##y##.Hue,\
				config->fashion.nowClothDyeing.##x##.##y##.Sat,\
				config->fashion.nowClothDyeing.##x##.##y##.Val,\
				config->fashion.nowClothDyeing.##x##.##y##.Alpha\
				)
#define TextPart(x) \
TextPartOnce(x,A); \
TextPartOnce(x,B); \
TextPartOnce(x,C); \
TextPartOnce(x,D)

			ImGui::Text((char*)u8"当前Part1 染色");
			TextPart(p1);

			ImGui::Text((char*)u8"当前Part2 染色");
			TextPart(p2);

			ImGui::Text((char*)u8"当前Part3 染色");
			TextPart(p3);

			ImGui::Text((char*)u8"当前Part4 染色");
			TextPart(p4);

			ImGui::Text((char*)u8"当前Part5 染色");
			TextPart(p5);

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::BeginGroup();

			ImGui::PushID(0x10000200);
			ImGui::InputUInt((char*)u8"替换时装染色", &config->fashion.setClothDyeing.FashionClothesId, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
			ImGui::SameLine();
			if (ImGui::Button((char*)u8"当前")) {
				config->fashion.setClothDyeing.FashionClothesId = config->fashion.nowClothes[1];
			}
			ImGui::SameLine();
			if (ImGui::Button((char*)u8"清空")) {
				config->fashion.setClothDyeing.FashionClothesId = 0;
				memset(&config->fashion.setClothDyeing, 0, sizeof(config->fashion.setClothDyeing));
			}
			ImGui::PopID();




#define InputPartProp(x,y,z) \
ImGui::SetNextItemWidth(30.f); \
ImGui::InputUInt8((char*)u8#y"["#z"]", &config->fashion.setClothDyeing.##x##.##y##.##z##, 0, 0, 0); \
ImGui::SameLine()

#define InputPush(y) \
ImGui::PushID(_pushId + y)


#define InputPop() \
ImGui::PopID()

#define InputPartPartOnce(x,y) \
InputPartProp(x, y, Hue); \
InputPartProp(x, y, Sat); \
InputPartProp(x, y, Val); \
InputPartProp(x, y, Alpha); \
if (ImGui::Button((char*)u8"清空")) { \
	config->fashion.setClothDyeing.##x##.##y##.Hue = 0; \
	config->fashion.setClothDyeing.##x##.##y##.Sat = 0; \
	config->fashion.setClothDyeing.##x##.##y##.Val = 0; \
	config->fashion.setClothDyeing.##x##.##y##.Alpha = 0; \
} \

#define InputPartOnce(b,x,y) \
InputPush(b); \
InputPartPartOnce(x,y); \
InputPop()

#define InputPart(a,x) \
UINT32 _pushId = a; \
InputPartOnce(0x1,x,A); \
InputPartOnce(0x2,x,B); \
InputPartOnce(0x3,x,C); \
InputPartOnce(0x4,x,D)

			ImGui::Text((char*)u8"修改Part1 染色");
			{
				InputPart(0x10000200, p1);
			}
			ImGui::Text((char*)u8"修改Part2 染色");
			{
				InputPart(0x10000210, p2);
			}
			ImGui::Text((char*)u8"修改Part3 染色");
			{
				InputPart(0x10000220, p3);
			}
			ImGui::Text((char*)u8"修改Part4 染色");
			{
				InputPart(0x10000230, p4);
			}
			ImGui::Text((char*)u8"修改Part5 染色");
			{
				InputPart(0x10000240, p5);
			}

			if (ImGui::Button((char*)u8"预设 - 霜天凝露")) {
				config->fashion.setClothDyeing = config->fashion.shuangtianninglu;
			}
			ImGui::SameLine();
			if (ImGui::Button((char*)u8"预设 - 狐狸粉色")) {
				config->fashion.setClothDyeing = config->fashion.hulifense;
			}


			ImGui::EndGroup();
		}



		ImGui::EndGroup();

		ImGui::End();
	}

}

void Fashion::hook() {
#pragma warning(push)
#pragma warning(disable:4003)
	DOHOOK()
	{
		a1.jmp(ptr(rip));
		a1.embedUInt64((UINT64)HOOKFUNC());


		a2.embed((void*)_this->hookAddress, /x?);
		a2.jmp(ptr(rip));
		a2.embedUInt64(_this->hookAddress + /x?);

		return _this->trampolineAddress;
	});
#pragma warning(pop)
}