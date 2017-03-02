#pragma once
#include "PluginSDK.h"

class JinxSpells
{
public:
	~JinxSpells();

	ISpell  *Q;
	ISpell2 *W;
	ISpell2 *E;
	ISpell2 *R;

	void CreateSpells();
	void SetPrediction();
};


