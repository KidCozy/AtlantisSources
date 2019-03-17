// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

#include"PlayerCharacter.h"

#include"Engine.h"

#define ABCHECK(Expr, ...) {if(!(Expr)) { return __VA_ARGS__;}}