#pragma once
#include "Enums/ItemAddResultType.h"
#include "Support/MSGText.h"
#include "ItemAddResult.generated.h"

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Item | Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Item | Add Result")
	int32 TotalAmount;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Item | Add Result")
	int32 RemainingAmount;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category="Item | Add Result")
	EItemAddResultType OperationResult;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category="Item | Add Result")
	FName ItemName;
	
	void SetupItem(FName CurrentItemName, const int32 CurrentTotalAmount)
	{
		ItemName = CurrentItemName;
		RemainingAmount = TotalAmount = CurrentTotalAmount;
	}
	
	void RemoveCount(const int32 RemoveCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("FItemAddResult::RemoveCount"));
		ActualAmountAdded += RemoveCount;
		RemainingAmount = TotalAmount - ActualAmountAdded;
		
		if (RemainingAmount <= 0)
		{
			OperationResult = EItemAddResultType::EAllItemAdded;
			ResultMessage = UMSGText::GetAddAll(TotalAmount, ItemName);
		}
		else if (RemainingAmount < TotalAmount)
		{
			OperationResult = EItemAddResultType::EPartialAmountItemAdded;
			ResultMessage = UMSGText::GetAddPartial(TotalAmount, ActualAmountAdded,ItemName);
		}else
		{
			OperationResult = EItemAddResultType::ENoItemAdded;
		}
	}
	
	//Information message that can be passed with the result
	UPROPERTY(EditInstanceOnly,BlueprintReadOnly,Category="Item | Add Result")
	FText ResultMessage;

	static FItemAddResult AddNone(const FText& ErrorText)
	{
		FItemAddResult ItemAddResult;
		ItemAddResult.ActualAmountAdded=0;
		ItemAddResult.OperationResult = EItemAddResultType::ENoItemAdded;
		ItemAddResult.ResultMessage = ErrorText;

		return ItemAddResult;
	}

	static FItemAddResult AddAll(const int32 Count,const FText& ErrorText)
	{
		FItemAddResult ItemAddResult;
		ItemAddResult.ActualAmountAdded=Count;
		ItemAddResult.OperationResult = EItemAddResultType::EAllItemAdded;
		ItemAddResult.ResultMessage = ErrorText;

		return ItemAddResult;
	}
};


