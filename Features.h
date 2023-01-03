#pragma once
#include "HookClass.h"



namespace NSHFeatures {
	class Features
	{
	public:
		Features(const Features&) = delete;
		void operator=(const Features&) = delete;
		virtual ~Features() {};

		virtual void hook() {};
		virtual void render() {};

	protected:
		Features() {};
	};

}