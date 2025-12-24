// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GW_MapGeneratorWidget.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UButton;
class USlider;
class UTextBlock;
class UImage;
class UEditableTextBox;
class AGW_MapGenerator;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Map Generator Widget                                                   */
/*-------------------------------------------------------------------------*/
#pragma region GW_MapGeneratorWidget.h
UCLASS()
class GRIMWARD_API UGW_MapGeneratorWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    // Period Sliders
    UPROPERTY(meta = (BindWidget))
    USlider* TemperaturePeriodSlider;
    
    UPROPERTY(meta = (BindWidget))
    USlider* MoisturePeriodSlider;
    
    UPROPERTY(meta = (BindWidget))
    USlider* AltitudePeriodSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* VolatilityPeriodSlider;
    
    UPROPERTY(meta = (BindWidget))
    USlider* EnchantmentPeriodSlider;

    // Period Value Labels
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TemperaturePeriodLabel;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MoisturePeriodLabel;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* AltitudePeriodLabel;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* VolatilityPeriodLabel;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* EnchantmentPeriodLabel;

    // Octave Inputs
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* TemperatureOctaveInput;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* MoistureOctaveInput;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* AltitudeOctaveInput;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* VolatilityOctaveInput;
    
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* EnchantmentOctaveInput;

    // Seed Input
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* SeedInput;

    // Map Size Inputs
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* MapWidthInput;
    
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* MapHeightInput;

    // Buttons
    UPROPERTY(meta = (BindWidget))
    UButton* GenerateButton;

    UPROPERTY(meta = (BindWidget))
    UButton* RandomSeedButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveImageButton;

    // Map Display
    UPROPERTY(meta = (BindWidget))
    UImage* GeneratedMapImage;

    // Status Label
    UPROPERTY(meta = (BindWidget))
    UTextBlock* StatusLabel;
    
private:
    UPROPERTY()
    AGW_MapGenerator* MapGenerator;
    
    UPROPERTY()
    UTexture2D* MapTexture;

    // Callback functions
    UFUNCTION()
    void OnGenerateButtonClicked();
    
    UFUNCTION()
    void OnRandomSeedButtonClicked();
    
    UFUNCTION()
    void OnSaveImageButtonClicked();

    UFUNCTION()
    void OnTemperaturePeriodChanged(float Value);
    
    UFUNCTION()
    void OnMoisturePeriodChanged(float Value);
    
    UFUNCTION()
    void OnAltitudePeriodChanged(float Value);
    
    UFUNCTION()
    void OnVolatilityPeriodChanged(float Value);
    
    UFUNCTION()
    void OnEnchantmentPeriodChanged(float Value);

    // Helper functions
    void UpdateAllLabels();
    void GenerateMap();
    void SaveMapToFile();
    int32 GetSeedFromInput();
    int32 GetOctaveFromInput(UEditableTextBox* Input, int32 DefaultValue);
};
#pragma endregion
/*-------------------------------------------------------------------------*/