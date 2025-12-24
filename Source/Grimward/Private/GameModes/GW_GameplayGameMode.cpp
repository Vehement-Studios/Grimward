// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Public/GameModes/GW_GameplayGameMode.h"
#include "Kismet/GameplayStatics.h"

// Image Testing:
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"
#include "UI/Menus/GW_MapGeneratorWidget.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_GameplayGameMode.cpp
AGW_GameplayGameMode::AGW_GameplayGameMode()
{
	UE_LOG(LogTemp, Log, TEXT("[Grimward] Gameplay Game Mode Created"));
	
}

void AGW_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("[Grimward] Gameplay Game Mode BeginPlay"));

	// Play gameplay music
	if (GameplayMusic)
	{
		// TODO: Add dynamic gameplay Music System here
		
		UGameplayStatics::PlaySound2D(this, GameplayMusic, 0.5f, 1.0f, 0.0f);
	}

	// Set proper input mode for gameplay
	SetupGameplayInput();
	
	// Spawn the map generator
	if (!MapGenerator)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		MapGenerator = GetWorld()->SpawnActor<AGW_MapGenerator>(AGW_MapGenerator::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        
		if (MapGenerator)
		{
			UE_LOG(LogTemp, Log, TEXT("MapGenerator spawned successfully"));
		}
	}
	
	if (UGW_MapGeneratorWidget* Widget = CreateWidget<UGW_MapGeneratorWidget>(GetWorld(), MapGeneratorWidgetClass))
	{
		Widget->AddToViewport();
        
		// Set input mode to UI
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(Widget->TakeWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void AGW_GameplayGameMode::GenerateDebugMap(int32 Seed)
{
	if (!MapGenerator)
    {
        FActorSpawnParameters SpawnParams;
        MapGenerator = GetWorld()->SpawnActor<AGW_MapGenerator>(AGW_MapGenerator::StaticClass());
    }
    
    if (MapGenerator)
    {
        UE_LOG(LogTemp, Warning, TEXT("Generating map with seed: %d"), Seed);
        
        MapGenerator->GenerateBiomeMap(Seed);
        UTexture2D* DebugTexture = MapGenerator->GenerateTestDebugTexture();
        
        if (DebugTexture)
        {
            // Get the texture data
            FTexture2DMipMap& Mip = DebugTexture->GetPlatformData()->Mips[0];
            void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);
            
            if (Data)
            {
                // Create image wrapper for PNG
                IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
                TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
                
                // Set the raw data
                int32 Width = DebugTexture->GetSizeX();
                int32 Height = DebugTexture->GetSizeY();
                
                if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(Data, Mip.BulkData.GetBulkDataSize(), Width, Height, ERGBFormat::BGRA, 8))
                {
                    // Compress to PNG
                    const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed(100);
                    
                    // Save to project directory
                    FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Screenshots/");
                    FString FileName = FString::Printf(TEXT("BiomeMap_Seed_%d.png"), Seed);
                    FString FilePath = SaveDirectory + FileName;
                    
                    // Ensure directory exists
                    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                    if (!PlatformFile.DirectoryExists(*SaveDirectory))
                    {
                        PlatformFile.CreateDirectory(*SaveDirectory);
                    }
                    
                    // Write file
                    if (FFileHelper::SaveArrayToFile(CompressedData, *FilePath))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("✓ Map saved to: %s"), *FilePath);
                        
                        // Open the folder in explorer
                        FPlatformProcess::ExploreFolder(*SaveDirectory);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("✗ Failed to save file to: %s"), *FilePath);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("✗ Failed to compress image data"));
                }
                
                Mip.BulkData.Unlock();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("✗ Failed to lock texture data"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("✗ Failed to generate debug texture"));
        }
    }
}

void AGW_GameplayGameMode::SetupGameplayInput()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		// Set input mode to game only (allows character movement)
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
        
		// Hide mouse cursor during gameplay
		PC->bShowMouseCursor = false;
        
		UE_LOG(LogTemp, Log, TEXT("[Grimward] Input mode set to Game Only, cursor hidden"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not get Player Controller in SetupGameplayInput"));
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/