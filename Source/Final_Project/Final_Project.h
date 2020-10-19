// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// ����� �α׸� ���� �ʿ��� ����
DECLARE_LOG_CATEGORY_EXTERN(Final_Project, Log, All);
#define FPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define FPLOG_S(Verbosity) UE_LOG(Final_Project, Verbosity, TEXT("%s"), *FPLOG_CALLINFO)															// �ڵ尡 ����ִ� ���� �̸��� �Լ�, �׸��� ���� ������ �߰��� Final_Project ī�װ��� �����.
#define FPLOG(Verbosity, Format, ...) UE_LOG(Final_Project, Verbosity, TEXT("%s %s"), *FPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))		// ABLOG_S ������ ���� ���ڿ��� �߰� ������ ������ �α׸� �����.
#define FPCHECK(Expr, ...) { if(!(Expr)) { FPLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}							// check�� ensure ��� ��� ��Ÿ�ӿ��� ���� �߻� �� ������ �����α׸� ��ȯ�ϵ��� ����