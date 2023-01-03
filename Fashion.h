#pragma once

#include "Features.h"
#include <iostream>


class Fashion : public NSHFeatures::Features
{
public:
	Fashion();
	~Fashion() = default;

	void hook() override;
	void render() override;

private:
	static UINT64 __fastcall hookedFunction(UINT64 a1);
	std::add_pointer_t<decltype(hookedFunction)> originalFunction;

	std::optional<HookClass> hookClass;
};

