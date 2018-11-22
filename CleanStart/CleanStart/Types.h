/*
Types.h
	Enthält alle gemeinsam genutzen Typen.

State
	State wird verwendet um den State des Environment
	zwischen Environment und Actor auzutauschen.

Reward
	Reward wird verwendet damit das Environment einem
	Actor mitteilen kann wie viel ein State wert ist.

Action
	Actions werden verwendet um einem Actor mitzuteilen
	welche Actions im Environment angewand werden können.
	Falls Aktionen nicht ausgeführt werden können, aber
	existieren sollte die Variable is_possible auf false 
	gesetzt werden.
	Um das Design einheitlich zu halten sollten die Actions 
	am besten definiert werden indem eine  enum Actions 
	z.B. 
	enum Actions { UP, DOWN, ..., NO_ACTION } 
	erstellt wird.

Pos<T>
	Pos ist die x y z Position in einem Raum, oder einer Fläche

Area<T>
	Area ist eine 2 dimensionale (x,y)-Position in einer Fläche.
	Die obere linke Ecke wird als Pos(0,0) angenommen:
	0 1 2.. x
	1
	2
	:
	y

*/
#pragma once
#include <vector>

namespace Ai_Arena
{
	using State  = std::vector<float>;
	using Reward = float;

	struct Action
	{
		int  action;
		bool is_possible;
		Action() : is_possible(false){}
		Action(const int action, const bool is_possible = true)
			:
			action(action),
			is_possible(is_possible)
		{}
	};

	template <class T>
	struct Pos
	{
		T x = 0;
		T y = 0;
		T z = 0;
		Pos(T x = 0, T y = 0, T z = 0)
			:
			x(x), y(y), z(z)
		{}

		bool operator==(const Pos& rhs) const
		{
			return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
		}
	};
	using Pos_int   = Pos<int>;
	using Pos_float = Pos<float>;

	template <class T>
	struct Area
	{
		Pos<T> upper_left;
		Pos<T> lower_right;
		Area(T x1, T y1, T x2, T y2)
			:
			upper_left (x1, y1),
			lower_right(x2, y2)
		{}
		Area(Pos<T> upper_left, Pos<T> lower_right)
			:
			upper_left (upper_left),
			lower_right(lower_right)
		{}

		bool is_inside(const Pos<T>& pos)
		{
			return pos.x <= lower_right.x
				&& pos.x >= upper_left.x
				&& pos.y >= upper_left.y
				&& pos.y <= lower_right.y;
		}
	};
	using Area_int   = Area<int>;
	using Area_float = Area<float>;
}