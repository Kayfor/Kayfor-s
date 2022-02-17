// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTWO_PRACTICE_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

		// Sort of constructor for AnimInstance class
public:

		virtual void NativeInitializeAnimation() override;

		UFUNCTION(BlueprintCallable, Category = AnimationProperty)
		void UpdateAnimationProperties();

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsInAir;
	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
			class AMain* Main;
};
