//-------------------------------
//
// ”Ä—p’è”ƒŠƒXƒg [common.h]
// Author: fuma sato
//
//-------------------------------
#pragma once

constexpr bool OK = true;
constexpr bool NO = false;
constexpr bool ON = true;
constexpr bool OFF = false;
using Index = size_t;
constexpr Index INVALID_ID = Index(-1);

using VECTORTAG = enum
{
	X = 0,
	Y,
	Z,
	W
};