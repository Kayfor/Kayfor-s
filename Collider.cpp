// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h" //gives an able to use 'SphereComponent' here)
#include "Components/StaticMeshComponent.h" //gives an able to use 'StaticMeshComponent' here)
#include "UObject/ConstructorHelpers.h" //gives an able to use class 'ConstructorHelpers' here)
#include "Components/InputComponent.h"			 //gives an able to use 'InputComponent' here)
#include "Camera/CameraComponent.h"				 //gives an able to use 'CameraComponent' here)
#include "GameFramework/SpringArmComponent.h" //gives an able to use 'SpringArmComponent' here)
#include "ColliderMovementComponent.h"
#include "Components/ActorComponent.h"


// Constructor - Sets default values
ACollider::ACollider()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add 'SphereComponent' to the 'RootComponent'
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT(" RootComponent "));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT(" SphereComponent "));
	//SphereComponent->SetupAttachment(GetRootComponent());
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.f);
	// Set collision preset
	SphereComponent->SetCollisionProfileName(TEXT(" Pawn "));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT(" MeshComponent "));
	MeshComponent->SetupAttachment(GetRootComponent());

	// Construct StaticMesh from VS
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere' "));
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		// Ajust default sphere due to carcass sphere
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	// Actually create 'SpringArm'
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT(" SpringArm "));
	SpringArm->SetupAttachment(GetRootComponent());
	// Set start camera  rotation (angle = -45 degree by X)
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	// Set start camera distance from object (FoV)
	SpringArm->TargetArmLength = 400.f;
	// Make camera moves slightly smooth
	SpringArm->bEnableCameraLag = true;
	// Set length of Camera step (pix/tick)
	SpringArm->CameraLagSpeed = 3.0f;
	// --------------------------------------------------------

		//Actually create a camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT(" Camera "));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT(" OurMovementComponent "));
	OurMovementComponent->UpdatedComponent = RootComponent;

	CameraInput = FVector2D(0.f, 0.f);

	// Set defaults fo get inputs to our 'Collider' 
	AutoPossessPlayer = EAutoReceiveInput::Player0;	
}

	// Called when the game starts or when spawned
	void ACollider::BeginPlay()
	{
		Super::BeginPlay();

	}

	// Called every frame
	void ACollider::Tick(float DeltaTime)
	{
		Super::Tick(DeltaTime);

		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);

		FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
		NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);
		SpringArm->SetWorldRotation(NewSpringArmRotation);
	}

	// Called to bind functionality to input
	void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

		PlayerInputComponent->BindAxis(TEXT(" MoveForward"), this, &ACollider::MoveForwardBackward);
		PlayerInputComponent->BindAxis(TEXT(" MoveRight"), this, &ACollider::MoveRightLeft);
		//PlayerInputComponent->BindAxis(TEXT(" MoveJump "), this, &ACollider::MoveJump);

		PlayerInputComponent->BindAxis(TEXT(" CameraPitch "), this, &ACollider::PitchCamera);
		PlayerInputComponent->BindAxis(TEXT(" CameraYaw "), this, &ACollider::YawCamera);


	}

	UPawnMovementComponent * ACollider::GetMovementComponent() const
	{
		return OurMovementComponent;
	}

	// Методы-заглушки для корректного вызова 'BindAxis' 
	void ACollider::MoveForwardBackward(float input)
	{
		FVector Forward = GetActorForwardVector();
		if (OurMovementComponent)
		{
			OurMovementComponent->AddInputVector(input * Forward);
		}
	}

	void ACollider::MoveRightLeft(float input)
	{
		FVector Right = GetActorRightVector();
		if (OurMovementComponent)
		{
			OurMovementComponent->AddInputVector(input * Right);
		}
	}

	void ACollider::MoveJump(float input)
	{
		FVector Jump = GetActorUpVector();
		if (OurMovementComponent)
		{
			OurMovementComponent->AddInputVector(input * Jump);
		}
	}

	void ACollider::YawCamera(float AxisValue)
	{
		CameraInput.X = AxisValue;
	}

	void ACollider::PitchCamera(float AxisValue)
	{
		CameraInput.Y = AxisValue;
	}




