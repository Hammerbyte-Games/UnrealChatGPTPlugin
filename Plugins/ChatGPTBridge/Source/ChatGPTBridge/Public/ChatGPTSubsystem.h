// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
#include "ChatGPTSubsystem.generated.h"




USTRUCT(BlueprintType)
struct FCompletionsRequestMessage
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString content;

	FCompletionsRequestMessage() {};

	FCompletionsRequestMessage( FString _role, FString _content ){

		role = _role;
		content = _content;
  
	}
  
};

USTRUCT(BlueprintType)
struct FCompletionsRequest
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString model;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCompletionsRequestMessage> messages;

	FCompletionsRequest() {};

	FCompletionsRequest( FString _model, TArray<FCompletionsRequestMessage> _messages ){

		model = _model;
		messages = _messages;
  
	}
  
	/* Don't Forget to setup your project
	Add #include "Runtime/JsonUtilities/Public/JsonObjectConverter.h" in 
	file with this structs.
	Also you need add "Json", "JsonUtilities" in Build.cs */

	FCompletionsRequest(FString _json_){
		FCompletionsRequest _tmpCompletionsRequest;
    
		FJsonObjectConverter::JsonObjectStringToUStruct<FCompletionsRequest>(
		_json_,
		&_tmpCompletionsRequest,
	0, 0);
    
		model = _tmpCompletionsRequest.model;
		messages = _tmpCompletionsRequest.messages;
	}
  
};


USTRUCT(BlueprintType)
struct FCompletionsResponseUsage
{

  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 prompt_tokens;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 completion_tokens;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 total_tokens;

  FCompletionsResponseUsage() {};

  FCompletionsResponseUsage( int32 _prompt_tokens, int32 _completion_tokens, int32 _total_tokens ){

    prompt_tokens = _prompt_tokens;
    completion_tokens = _completion_tokens;
    total_tokens = _total_tokens;
  
  }
  
};

USTRUCT(BlueprintType)
struct FCompletionsResponseChoiceMessage
{

  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString role;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString content;

  FCompletionsResponseChoiceMessage() {};

  FCompletionsResponseChoiceMessage( FString _role, FString _content ){

    role = _role;
    content = _content;
  
  }
  
};

USTRUCT(BlueprintType)
struct FCompletionsResponseChoice
{

  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 index;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FCompletionsResponseChoiceMessage message;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString finish_reason;

  FCompletionsResponseChoice() {};

  FCompletionsResponseChoice( int32 _index, FCompletionsResponseChoiceMessage _message, FString _finish_reason ){

    index = _index;
    message = _message;
    finish_reason = _finish_reason;
  
  }
  
};

USTRUCT(BlueprintType)
struct FCompletionsResponse
{

  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString id;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FString object;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 created;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FCompletionsResponseChoice> choices;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FCompletionsResponseUsage usage;

  FCompletionsResponse() {};

  FCompletionsResponse( FString _id, FString _object, int32 _created, TArray<FCompletionsResponseChoice> _choices, FCompletionsResponseUsage _usage ){

    id = _id;
    object = _object;
    created = _created;
    choices = _choices;
    usage = _usage;
  
  }
  
  /* Don't Forget to setup your project
  Add #include "Runtime/JsonUtilities/Public/JsonObjectConverter.h" in 
  file with this structs.
  Also you need add "Json", "JsonUtilities" in Build.cs */

  FCompletionsResponse(FString _json_){
    FCompletionsResponse _tmpCompletionsResponse;
    
		FJsonObjectConverter::JsonObjectStringToUStruct<FCompletionsResponse>(
		_json_,
		&_tmpCompletionsResponse,
    0, 0);
    
    id = _tmpCompletionsResponse.id;
    object = _tmpCompletionsResponse.object;
    created = _tmpCompletionsResponse.created;
    choices = _tmpCompletionsResponse.choices;
    usage = _tmpCompletionsResponse.usage;
  }
  
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPostCompletionDelegate, FCompletionsResponse, CompletionsResponse, bool,bSuccess);
/**
 * 
 */
UCLASS(Config=Engine)
class CHATGPTBRIDGE_API UChatGPTSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(config, EditAnywhere, Category=ChatGPTBridge, DisplayName = "API Token")
	FString API_Token;
	
	UPROPERTY(BlueprintAssignable)
	FPostCompletionDelegate PostCompletionsDelegate;
	
	UFUNCTION(BlueprintCallable)
	void PostCompletions(FString Role, FString Content, FString APIKey);

private:
	void OnPostCompletions(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

};
