// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatGPTSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"


//https://platform.openai.com/docs/api-reference/chat/create
void UChatGPTSubsystem::PostCompletions(FString Role, FString Content, FString APIKey)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://api.openai.com/v1/chat/completions");

	UE_LOG(LogTemp,Warning,TEXT("%s"),*Request->GetURL());
	Request->SetVerb(TEXT("POST"));
	FCompletionsRequest CompletionsRequest;
	CompletionsRequest.model = "gpt-3.5-turbo";
	CompletionsRequest.messages.Add(FCompletionsRequestMessage(Role,Content));
	FString CompletionsRequestJson;
	FJsonObjectConverter::UStructToJsonObjectString(CompletionsRequest,CompletionsRequestJson,0,0);
	
	const FString& RequestString  = *CompletionsRequestJson;
	UE_LOG(LogTemp,Warning,TEXT("%s"),*RequestString);

	FString AuthorizationKey = "Bearer " + APIKey;
	UE_LOG(LogTemp,Warning,TEXT("%s"),*AuthorizationKey);
	
	Request->SetContentAsString(RequestString);
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), AuthorizationKey);
	Request->OnProcessRequestComplete().BindUObject(this, &UChatGPTSubsystem::OnPostCompletions);
	Request->ProcessRequest();
	
}

void UChatGPTSubsystem::OnPostCompletions(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		FString Content = Response->GetContentAsString();
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			FCompletionsResponse CompletionsResponse;
			if (FJsonObjectConverter::JsonObjectStringToUStruct<FCompletionsResponse>(Content, &CompletionsResponse, 0, 0))
			{
				FString Message = CompletionsResponse.choices[0].message.content;
				UE_LOG(LogTemp,Warning,TEXT("%s"),*Message);
				PostCompletionsDelegate.Broadcast(CompletionsResponse, true);
				return;
			}
		}
	}
	PostCompletionsDelegate.Broadcast(FCompletionsResponse(),false);
}
