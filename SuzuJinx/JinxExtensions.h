#pragma once
#include "PluginSDK.h"

class JinxExtensions
{
public:
	~JinxExtensions();

	Vec2 To2D(Vec3 p);
	Vec3 To3D(Vec2 p);

	float Dist(IUnit *to);
	float Dist2D(IUnit *to);
	float Dist(IUnit *from, IUnit *to);
	float Dist2D(IUnit *from, IUnit *to);
	float Dist(Vec3 from, Vec3 to);
	float Dist2D(Vec3 from, Vec3 to);
	float Dist2D(Vec2 from, Vec2 to);

	bool IsKeyDown(IMenuOption *menuOption);
	bool IsOnSegment(Vec2 init, Vec2 start, Vec2 end, float inc);

	std::vector<IUnit *> GetEnemiesInRange(IUnit *unit, float range);
	std::vector<IUnit *> GetAlliesInRange(IUnit *unit, float range);

	int CountEnemiesInRange(IUnit *unit, float range);
	int CountAlliesInRange(IUnit *unit, float range);

	int CountInRange(IUnit *unit, float range, std::vector<IUnit *> units);
	int UnitsInR(IUnit *me, IUnit *second, float width, float range, std::vector<IUnit *> &units, bool creeps);
};