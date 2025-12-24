// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/Menus/GW_MapGeneratorWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Core/ExplorationMap/GW_MapGenerator.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"
/*-------------------------------------------------------------------------*/

void UGW_MapGeneratorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Spawn Map Generator
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        MapGenerator = World->SpawnActor<AGW_MapGenerator>(AGW_MapGenerator::StaticClass(), SpawnParams);
    }

    // Bind buttons
    if (GenerateButton)
    {
        GenerateButton->OnClicked.AddDynamic(this, &UGW_MapGeneratorWidget::OnGenerateButtonClicked);
    }

    if (RandomSeedButton)
    {
        RandomSeedButton->OnClicked.AddDynamic(this, &UGW_MapGeneratorWidget::OnRandomSeedButtonClicked);
    }

    if (SaveImageButton)
    {
        SaveImageButton->OnClicked.AddDynamic(this, &UGW_MapGeneratorWidget::OnSaveImageButtonClicked);
    }

    // Bind sliders
    if (TemperaturePeriodSlider)
    {
        TemperaturePeriodSlider->OnValueChanged.AddDynamic(this, &UGW_MapGeneratorWidget::OnTemperaturePeriodChanged);
        TemperaturePeriodSlider->SetMinValue(5.0f);
        TemperaturePeriodSlider->SetMaxValue(500.0f);
        TemperaturePeriodSlider->SetValue(20.0f);
    }

    if (MoisturePeriodSlider)
    {
        MoisturePeriodSlider->OnValueChanged.AddDynamic(this, &UGW_MapGeneratorWidget::OnMoisturePeriodChanged);
        MoisturePeriodSlider->SetMinValue(5.0f);
        MoisturePeriodSlider->SetMaxValue(500.0f);
        MoisturePeriodSlider->SetValue(20.0f);
    }

    if (AltitudePeriodSlider)
    {
        AltitudePeriodSlider->OnValueChanged.AddDynamic(this, &UGW_MapGeneratorWidget::OnAltitudePeriodChanged);
        AltitudePeriodSlider->SetMinValue(5.0f);
        AltitudePeriodSlider->SetMaxValue(500.0f);
        AltitudePeriodSlider->SetValue(15.0f);
    }

    if (VolatilityPeriodSlider)
    {
        VolatilityPeriodSlider->OnValueChanged.AddDynamic(this, &UGW_MapGeneratorWidget::OnVolatilityPeriodChanged);
        VolatilityPeriodSlider->SetMinValue(5.0f);
        VolatilityPeriodSlider->SetMaxValue(300.0f);
        VolatilityPeriodSlider->SetValue(25.0f);
    }

    if (EnchantmentPeriodSlider)
    {
        EnchantmentPeriodSlider->OnValueChanged.AddDynamic(this, &UGW_MapGeneratorWidget::OnEnchantmentPeriodChanged);
        EnchantmentPeriodSlider->SetMinValue(5.0f);
        EnchantmentPeriodSlider->SetMaxValue(300.0f);
        EnchantmentPeriodSlider->SetValue(25.0f);
    }

    // Set default values for text inputs
    if (TemperatureOctaveInput) TemperatureOctaveInput->SetText(FText::FromString("8"));
    if (MoistureOctaveInput) MoistureOctaveInput->SetText(FText::FromString("8"));
    if (AltitudeOctaveInput) AltitudeOctaveInput->SetText(FText::FromString("7"));
    if (VolatilityOctaveInput) VolatilityOctaveInput->SetText(FText::FromString("4"));
    if (EnchantmentOctaveInput) EnchantmentOctaveInput->SetText(FText::FromString("4"));
    if (SeedInput) SeedInput->SetText(FText::FromString("12345"));
    if (MapWidthInput) MapWidthInput->SetText(FText::FromString("600"));
    if (MapHeightInput) MapHeightInput->SetText(FText::FromString("600"));

    // Update all labels
    UpdateAllLabels();

    // Set initial status
    if (StatusLabel)
    {
        StatusLabel->SetText(FText::FromString("Ready. Click Generate to create a map."));
    }
}

void UGW_MapGeneratorWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // Clean up map generator
    if (MapGenerator)
    {
        MapGenerator->Destroy();
        MapGenerator = nullptr;
    }
}

void UGW_MapGeneratorWidget::OnGenerateButtonClicked()
{
    GenerateMap();
}

void UGW_MapGeneratorWidget::OnRandomSeedButtonClicked()
{
    if (SeedInput)
    {
        int32 RandomSeed = FMath::Rand();
        SeedInput->SetText(FText::AsNumber(RandomSeed));
    }
}

void UGW_MapGeneratorWidget::OnSaveImageButtonClicked()
{
    SaveMapToFile();
}

void UGW_MapGeneratorWidget::OnTemperaturePeriodChanged(float Value)
{
    if (TemperaturePeriodLabel)
    {
        TemperaturePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(Value)));
        GenerateMap();
    }
}

void UGW_MapGeneratorWidget::OnMoisturePeriodChanged(float Value)
{
    if (MoisturePeriodLabel)
    {
        MoisturePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(Value)));
        GenerateMap();
    }
}

void UGW_MapGeneratorWidget::OnAltitudePeriodChanged(float Value)
{
    if (AltitudePeriodLabel)
    {
        AltitudePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(Value)));
        GenerateMap();
    }
}

void UGW_MapGeneratorWidget::OnVolatilityPeriodChanged(float Value)
{
    if (VolatilityPeriodLabel)
    {
        VolatilityPeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(Value)));
        GenerateMap();
    }
}

void UGW_MapGeneratorWidget::OnEnchantmentPeriodChanged(float Value)
{
    if (EnchantmentPeriodLabel)
    {
        EnchantmentPeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(Value)));
        GenerateMap();
    }
}

void UGW_MapGeneratorWidget::UpdateAllLabels()
{
    if (TemperaturePeriodSlider && TemperaturePeriodLabel)
    {
        TemperaturePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(TemperaturePeriodSlider->GetValue())));
    }
    if (MoisturePeriodSlider && MoisturePeriodLabel)
    {
        MoisturePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(MoisturePeriodSlider->GetValue())));
    }
    if (AltitudePeriodSlider && AltitudePeriodLabel)
    {
        AltitudePeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(AltitudePeriodSlider->GetValue())));
    }
    if (VolatilityPeriodSlider && VolatilityPeriodLabel)
    {
        VolatilityPeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(VolatilityPeriodSlider->GetValue())));
    }
    if (EnchantmentPeriodSlider && EnchantmentPeriodLabel)
    {
        EnchantmentPeriodLabel->SetText(FText::AsNumber(FMath::RoundToInt(EnchantmentPeriodSlider->GetValue())));
    }
}

void UGW_MapGeneratorWidget::GenerateMap()
{
    if (!MapGenerator)
    {
        if (StatusLabel)
        {
            StatusLabel->SetText(FText::FromString("ERROR: Map Generator is null!"));
        }
        return;
    }

    // Update status
    if (StatusLabel)
    {
        StatusLabel->SetText(FText::FromString("Generating..."));
    }

    // Get values from inputs
    int32 Seed = GetSeedFromInput();
    
    // Get map dimensions
    int32 Width = 600;
    int32 Height = 600;
    if (MapWidthInput)
    {
        Width = FCString::Atoi(*MapWidthInput->GetText().ToString());
        Width = FMath::Clamp(Width, 64, 2000);
    }
    if (MapHeightInput)
    {
        Height = FCString::Atoi(*MapHeightInput->GetText().ToString());
        Height = FMath::Clamp(Height, 64, 2000);
    }

    // Set map generator parameters
    MapGenerator->GenWidth = Width;
    MapGenerator->GenHeight = Height;
    
    MapGenerator->TemperaturePeriod = TemperaturePeriodSlider ? TemperaturePeriodSlider->GetValue() : 20.0f;
    MapGenerator->TemperatureOctaves = GetOctaveFromInput(TemperatureOctaveInput, 8);
    
    MapGenerator->MoisturePeriod = MoisturePeriodSlider ? MoisturePeriodSlider->GetValue() : 20.0f;
    MapGenerator->MoistureOctaves = GetOctaveFromInput(MoistureOctaveInput, 8);
    
    MapGenerator->AltitudePeriod = AltitudePeriodSlider ? AltitudePeriodSlider->GetValue() : 15.0f;
    MapGenerator->AltitudeOctaves = GetOctaveFromInput(AltitudeOctaveInput, 7);
    
    MapGenerator->VolatilityPeriod = VolatilityPeriodSlider ? VolatilityPeriodSlider->GetValue() : 25.0f;
    MapGenerator->VolatilityOctaves = GetOctaveFromInput(VolatilityOctaveInput, 4);
    
    MapGenerator->EnchantmentPeriod = EnchantmentPeriodSlider ? EnchantmentPeriodSlider->GetValue() : 25.0f;
    MapGenerator->EnchantmentOctaves = GetOctaveFromInput(EnchantmentOctaveInput, 4);

    // Generate the map
    MapGenerator->GenerateBiomeMap(Seed);

    // Generate texture
    MapTexture = MapGenerator->GenerateTestDebugTexture();

    if (MapTexture && GeneratedMapImage)
    {
        GeneratedMapImage->SetBrushFromTexture(MapTexture);
        
        if (StatusLabel)
        {
            int32 BiomeCount = MapGenerator->GetBiomeMap().Num();
            FString StatusText = FString::Printf(TEXT("✓ Generated! Seed: %d | Size: %dx%d | Biomes: %d"), 
                Seed, Width, Height, BiomeCount);
            StatusLabel->SetText(FText::FromString(StatusText));
        }
    }
    else
    {
        if (StatusLabel)
        {
            StatusLabel->SetText(FText::FromString("ERROR: Failed to generate texture!"));
        }
    }
}

void UGW_MapGeneratorWidget::SaveMapToFile()
{
    if (!MapTexture)
    {
        if (StatusLabel)
        {
            StatusLabel->SetText(FText::FromString("ERROR: Generate a map first!"));
        }
        return;
    }

    // Get the texture data
    FTexture2DMipMap& Mip = MapTexture->GetPlatformData()->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

    if (Data)
    {
        // Create image wrapper for PNG
        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
        TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

        int32 Width = MapTexture->GetSizeX();
        int32 Height = MapTexture->GetSizeY();

        if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(Data, Mip.BulkData.GetBulkDataSize(), Width, Height, ERGBFormat::BGRA, 8))
        {
            const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed(100);

            // Save to project directory
            FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Screenshots/");
            int32 Seed = GetSeedFromInput();
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
                if (StatusLabel)
                {
                    StatusLabel->SetText(FText::FromString(FString::Printf(TEXT("✓ Saved: %s"), *FileName)));
                }

                // Open the folder in explorer
                FPlatformProcess::ExploreFolder(*SaveDirectory);
            }
            else
            {
                if (StatusLabel)
                {
                    StatusLabel->SetText(FText::FromString("ERROR: Failed to save file!"));
                }
            }
        }

        Mip.BulkData.Unlock();
    }
}

int32 UGW_MapGeneratorWidget::GetSeedFromInput()
{
    if (SeedInput)
    {
        FString SeedString = SeedInput->GetText().ToString();
        if (SeedString.IsNumeric())
        {
            return FCString::Atoi(*SeedString);
        }
    }
    return 12345; // Default seed
}

int32 UGW_MapGeneratorWidget::GetOctaveFromInput(UEditableTextBox* Input, int32 DefaultValue)
{
    if (Input)
    {
        FString ValueString = Input->GetText().ToString();
        if (ValueString.IsNumeric())
        {
            return FMath::Clamp(FCString::Atoi(*ValueString), 1, 12);
        }
    }
    return DefaultValue;
}