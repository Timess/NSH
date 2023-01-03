#pragma once
#include <optional>
#include <iostream>
#include <array>
class Config
{
public:
	Config() = default;

	struct Logger {
		bool enable{ false };
		bool enableConsole{ false };
		bool enableAutoScroll{ true };

		bool enableLogEvent{ false };
		bool pauseLogEvent{ false };

		bool enableDoubleEvent{ false };
		bool pauseDoubleEvent{ false };
	};

	Logger logger{};

	struct Fashion {



		struct __declspec(align(4)) FashionClothesHSV {
			UINT32 __lua_object;
			UINT8 Hue;
			UINT8 Sat;
			UINT8 Val;
			UINT8 Alpha;

			FashionClothesHSV(UINT8 Hue = 0, UINT8 Sat = 0, UINT8 Val = 0, UINT8 Alpha = 0) :Hue(Hue), Sat(Sat), Val(Val), Alpha(Alpha), __lua_object(0) {}
		};

		static_assert(sizeof(FashionClothesHSV) == 0x8, "err");


		struct __declspec(align(8)) CFashionClothesPart {
			UINT32 __lua_object;
			FashionClothesHSV* A;
			FashionClothesHSV* B;
			FashionClothesHSV* C;
			FashionClothesHSV* D;
		};

		static_assert(sizeof(CFashionClothesPart) == 0x28, "err");

		struct __declspec(align(8)) CFashionClothesDyeing {
			UINT32 __lua_object;
			CFashionClothesPart* p1;
			CFashionClothesPart* p2;
			CFashionClothesPart* p3;
			CFashionClothesPart* p4;
			CFashionClothesPart* p5;
			UINT32 FashionClothesId;
		};

		static_assert(sizeof(CFashionClothesDyeing) == 0x38, "err");


		struct LFashionClothesPart {
			FashionClothesHSV A;
			FashionClothesHSV B;
			FashionClothesHSV C;
			FashionClothesHSV D;

			LFashionClothesPart(FashionClothesHSV A, FashionClothesHSV B, FashionClothesHSV C, FashionClothesHSV D) :A(A), B(B), C(C), D(D) {}
		};

		struct LFashionClothesDyeing {
			LFashionClothesPart p1;
			LFashionClothesPart p2;
			LFashionClothesPart p3;
			LFashionClothesPart p4;
			LFashionClothesPart p5;
			UINT32 FashionClothesId;

			LFashionClothesDyeing(LFashionClothesPart p1, LFashionClothesPart p2, LFashionClothesPart p3, LFashionClothesPart p4, LFashionClothesPart p5, UINT32 FashionClothesId) :
				p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), FashionClothesId(FashionClothesId) {}

			LFashionClothesDyeing() :p1({}, {}, {}, {}), p2({}, {}, {}, {}), p3({}, {}, {}, {}), p4({}, {}, {}, {}), p5({}, {}, {}, {}), FashionClothesId(0) {}
		};


		bool enable{ true };
		bool enableChangleAllTitle{ true };
		bool enableBlockOtherTitle{ true };

		bool enableChangeClothes{ true };
		bool enableChangeHairDye{ true };

		bool enableChangeClothDye{ true };

		char8_t triggerTitle[0x100]{ u8"栖云忆梦" };

		int targetTitleIndex{ 0 };
		char8_t targetTitle[0x100]{ 0 };

		UINT32 weaponDistance{ 0x2 };

		UINT8 nowHairDye{ 0 };
		int setHairDyeIndex{ 1 };

		std::array<UINT32, 0xD> nowClothes{};
		std::array<UINT32, 0xD> setClothes{
			0x0,	//未知
			0x0,	//时装 (樱岚含香=2AA12B1,霜天凝露=2AA145A,狐狸=2AA1474)
			0x2AA19E3,	//头饰 (2AA19E3)
			0x0,	//未知
			0x0,	//未知
			0x0,	//未知
			0x0,	//发型 (残月苍魄=2AA1F7D,霜天凝露=2AA1FDE,狐狸=2AA1FF4)
			0x2AA3D22,	//武器(荒羽金武器=2AA3D95，碎梦金武器=2AA3D22，荒羽火=2AA3D99，碎梦火=2AA3D78)
			0x0,	//未知
			0x0,	//未知
			0x0,	//环身(浮生轻羽=2AA4C17-8) 2AA4C14
			0x2AA4FF5,	//脚印
			0x0,	//未知
		};


		LFashionClothesDyeing nowClothDyeing{};
		LFashionClothesDyeing setClothDyeing{};


		const LFashionClothesDyeing shuangtianninglu{
			{{14,32,34,0},{13,35,29,0},{25,25,25,0},{8,25,25,0}},
			{{50,45,25,0},{26,30,25,0},{},{}},
			{{25,18,25,0},{25,36,19,0},{},{}},
			{{50,27,29,0},{24,33,24,0},{},{}},
			{{},{},{},{}},
			0x2AA145Aui32
		};


		const LFashionClothesDyeing hulifense{
			{{26,18,32},{25,12,25},{25,20,33},{23,33,25}},
			{{4,30,29},{6,29,32},{25,0,25},{25,0,37}},
			{{25,26,23},{4,30,30},{0,0,0},{24,43,24}},
			{{29,20,31},{24,38,41},{},{}},
			{{0,13,27,0},{49,4,25},{},{}},
			0x2AA1474ui32
		};

		const LFashionClothesDyeing emptyTemplate{
			{{},{},{},{}},
			{{},{},{},{}},
			{{},{},{},{}},
			{{},{},{},{}},
			{{},{},{},{}},
			0x0ui32
		};
	};

	Fashion fashion;

	struct Vehicle {
		bool enable{ true };
		bool senable{ true };
		UINT32 trigger{ 0x02AEBEB4 };
		UINT32 target{ 0x02AEC36E };

	};

	Vehicle vehicle;

	bool enableSuperBlockTitle{ true };



	bool shutDown{ false };
	bool shutDownBegin{ false };
};

extern std::optional<Config> config;