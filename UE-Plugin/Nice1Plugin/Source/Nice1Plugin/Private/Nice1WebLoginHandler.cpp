// Fill out your copyright notice in the Description page of Project Settings.


#include "Nice1WebLoginHandler.h"
#include "Nice1WebLoginWidget.h"

void UNice1WebLoginHandler::NotifyLogin(const FString& Account)
{
	UE_LOG(LogTemp, Display, TEXT("Received Nice1 Account: %s"), *Account);

	OnLoginSuccess.Broadcast(Account);
}
