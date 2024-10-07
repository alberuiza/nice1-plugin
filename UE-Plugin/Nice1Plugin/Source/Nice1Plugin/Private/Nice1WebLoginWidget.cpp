// Fill out your copyright notice in the Description page of Project Settings.


#include "Nice1WebLoginWidget.h"
#include "SWebBrowser.h"
#include "Nice1WebLoginHandler.h"

#include "Misc/Paths.h"

void UNice1WebLoginWidget::HandleOnLoginSuccess(const FString& Account)
{
	UE_LOG(LogTemp, Display, TEXT("Logging succeeded. Account name: %s"), *Account);
	OnLoginSuccess.Broadcast(Account);
}

void UNice1WebLoginWidget::LogWebConsoleMessage(const FString& Message, const FString& Source, int32 Line)
{
	UE_LOG(LogTemp, Display, TEXT("Web console message -> Message: %s, Source:%s, Line:%d"), *Message, *Source, Line);
}

TSharedRef<SWidget> UNice1WebLoginWidget::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (WebBrowserWidget)
	{
		FString RelativePath = FPaths::ProjectPluginsDir() + TEXT("Nice1Plugin\\Content\\index.html");
		FString AbsolutePath = FPaths::ConvertRelativePathToFull(RelativePath);
		InitialURL = AbsolutePath;
		bSupportsTransparency = false;

		WebLoginHandler = NewObject<UNice1WebLoginHandler>();
		WebLoginHandler->OnLoginSuccess.AddDynamic(this, &UNice1WebLoginWidget::HandleOnLoginSuccess);

		WebBrowserWidget->BindUObject("handler", WebLoginHandler, true);

		this->OnConsoleMessage.AddDynamic(this, &UNice1WebLoginWidget::LogWebConsoleMessage);
	}

	return Widget;
}
