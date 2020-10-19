// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// 디버깅 로그를 위해 필요한 문장
DECLARE_LOG_CATEGORY_EXTERN(Final_Project, Log, All);
#define FPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define FPLOG_S(Verbosity) UE_LOG(Final_Project, Verbosity, TEXT("%s"), *FPLOG_CALLINFO)															// 코드가 들어있는 파일 이름과 함수, 그리고 라인 정보를 추가해 Final_Project 카테고리에 남긴다.
#define FPLOG(Verbosity, Format, ...) UE_LOG(Final_Project, Verbosity, TEXT("%s %s"), *FPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))		// ABLOG_S 정보에 형식 문자열로 추가 정보를 지정해 로그를 남긴다.
#define FPCHECK(Expr, ...) { if(!(Expr)) { FPLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}							// check나 ensure 어설션 대신 런타임에서 문제 발생 시 붉은색 에러로그를 반환하도록 해줌