#pragma once

class JinxModes
{
public:
	~JinxModes();

	void Combo();
	void Harass();
	void WaveClear();
	void JungleClear();
	void Flee();
	void Auto();
	void OnGapcloser(GapCloserSpell args);
	void Interrupter(InterruptibleSpell args);
	void BeforeAttack(IUnit* target);
};
