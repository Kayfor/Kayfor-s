// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class PROJECTTWO_PRACTICE_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Propered 'Mesh'
	UPROPERTY(VisibleAnywhere, Category = " Mesh ")
	class UStaticMeshComponent * MeshComponent;

	// Propered 'Sphere'
	UPROPERTY(VisibleAnywhere, Category = " Mesh ")
	class USphereComponent * SphereComponent;

	// Propered 'Camera' 
	UPROPERTY(VisibleAnywhere, Category = " Mesh ")
		class UCameraComponent * Camera;

	// Help with Camera motion
	UPROPERTY(VisibleAnywhere, Category = " Mesh ")
	class USpringArmComponent * SpringArm;

	// Class with pawnmovement logic
	UPROPERTY(VisibleAnywhere, Category = " Mesh ")
	class UColliderMovementComponent * OurMovementComponent;

	virtual UPawnMovementComponent * GetMovementComponent() const override;


	//  Этот макрос аналогичен С++шному ключевому слову 'inline'
	// позволяющему обратится к методу из любого места в коде
	FORCEINLINE UStaticMeshComponent * GetMeshComponent() 
	{ return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent * Mesh) 
	{	MeshComponent = Mesh;	}
	FORCEINLINE USphereComponent * GetSphereComponent()
	{	return SphereComponent;	}
	FORCEINLINE void SetSphereComponent(USphereComponent * Sphere)
	{	SphereComponent = Sphere;	}


	FORCEINLINE UCameraComponent * GetCameraComponent()
	{	return Camera; }
	FORCEINLINE void SetCameraComponent(UCameraComponent * InCamera)
	{	Camera = InCamera;	}
	FORCEINLINE USpringArmComponent * GetSpringArmComponent()
	{	return SpringArm;	}
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent * InSpringArm)
	{	SpringArm = InSpringArm;	}
	

private:
	UFUNCTION()
		void MoveForwardBackward(float input);
	UFUNCTION()
		void MoveRightLeft(float input);
	UFUNCTION()
		void MoveJump(float input);

//FVector CurrentVelocity;
};
