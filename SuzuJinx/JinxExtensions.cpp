#pragma once
#include "Jinx.h"
#include "PluginSDK.h"
#include "JinxExtensions.h"

Vec2 JinxExtensions::To2D(Vec3 p)
{
	return Vec2(p.x, p.y);
}

Vec3 JinxExtensions::To3D(Vec2 p)
{
	return Vec3(p.x, p.y, Jinx::Player->GetPosition().z);
}

float JinxExtensions::Dist(IUnit *to)
{
	return (Jinx::Player->ServerPosition() - to->ServerPosition()).Length() - (Jinx::Player->BoundingRadius() + to->BoundingRadius());
}

float JinxExtensions::Dist2D(IUnit *to)
{
	return (Jinx::Player->ServerPosition() - to->ServerPosition()).Length2D()  - (Jinx::Player->BoundingRadius() + to->BoundingRadius());
}

float JinxExtensions::Dist(IUnit *from, IUnit *to)
{
	return (from->ServerPosition() - to->ServerPosition()).Length();
}

float JinxExtensions::Dist2D(IUnit *from, IUnit *to)
{
	return (from->ServerPosition() - to->ServerPosition()).Length2D();
}

float JinxExtensions::Dist(Vec3 from, Vec3 to)
{
	return (from - to).Length();
}

float JinxExtensions::Dist2D(Vec3 from, Vec3 to)
{
	return (from - to).Length2D();
}

float JinxExtensions::Dist2D(Vec2 from, Vec2 to)
{
	return (from - to).Length();
}

bool JinxExtensions::IsKeyDown(IMenuOption *menuOption)
{
	return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0;
}

// units in range from trees<333
std::vector<IUnit*> UnitsInRange(IUnit* unit, float range, std::vector<IUnit*> collection)
{
	std::vector<IUnit*> units;

	if (collection.empty())
	{
		return units;
	}

	for (auto i : collection)
	{
		if (Jinx::Ex->Dist2D(i) <= range)
		{
			units.push_back(i);
		}
	}

	return units;
}

// units in range from trees<333
std::vector<IUnit*> JinxExtensions::GetEnemiesInRange(IUnit *unit, float range)
{
	return UnitsInRange(unit, range, Jinx::Enemies);
}

std::vector<IUnit*> JinxExtensions::GetAlliesInRange(IUnit *unit, float range)
{
	return UnitsInRange(unit, range, Jinx::Allies);
}

int JinxExtensions::CountEnemiesInRange(IUnit *unit, float range)
{
	return CountInRange(unit, range, Jinx::Enemies);
}

int JinxExtensions::CountAlliesInRange(IUnit *unit, float range)
{
	return CountInRange(unit, range, Jinx::Allies);
}

int JinxExtensions::CountInRange(IUnit *unit, float range, std::vector<IUnit*> units)
{
	return UnitsInRange(unit, range, units).size();
}

// check if hero on line quick idea from trees <33
bool JinxExtensions::IsOnSegment(Vec2 init, Vec2 start, Vec2 end, float inc)
{
	auto direction = (end - start).VectorNormalize();

	for (float i = 0; i < Dist2D(start, end); i = i + inc)
	{
		auto current = start + direction * i;

		if (Dist2D(init, current) <= 50)
		{
			return true;
		}
	}
}

int JinxExtensions::UnitsInR(IUnit *me, IUnit *second, float width, float range, std::vector<IUnit*> &units, bool creeps = false)
{
	auto gathered = std::vector<IUnit*>();
	auto direction = (To2D(second->ServerPosition()) - To2D(me->ServerPosition())).VectorNormalize();
	auto end = To2D(me->ServerPosition()) + direction * Dist2D(second, me);
	auto collection = GEntityList->GetAllUnits();

	for (auto unit : collection)
	{
		if (unit != nullptr && !unit->IsDead() && unit->IsVisible())
		{
			if ((unit->UnitFlags() == FL_HERO || creeps && unit->UnitFlags() == FL_CREEP))
			{
				if (Dist2D(me, unit) <= range || Dist2D(second, unit) <= range)
				{
					if (IsOnSegment(To2D(unit->ServerPosition()), To2D(me->ServerPosition()), end, 5))
					{
						gathered.push_back(unit);
					}
				}
			}
		}
	}

	units = gathered;
	return gathered.size();
}


