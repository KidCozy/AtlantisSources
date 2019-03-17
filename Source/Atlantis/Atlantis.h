// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

#include"Character/PlayerCharacter.h"

#include"Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(Atlantis, Log, All);
#define ABCHECK(Expr, ...) {if(!(Expr)) { return __VA_ARGS__;}}
#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(Atlantis, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(Atlantis, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))