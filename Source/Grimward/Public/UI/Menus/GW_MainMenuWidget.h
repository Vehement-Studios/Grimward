// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GW_MainMenuWidget.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UButton;
class UTextBlock;
class UImage;
class UProgressBar;
class UGW_GameInstance;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Main Menu Widget                                                       */
/*-------------------------------------------------------------------------*/
#pragma region GW_MainMenuWidget.h
UCLASS()
class GRIMWARD_API UGW_MainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    // ========================================
    // Meta Bind Widget References - MUST match UMG widget names exactly
    // ========================================
    
    // Main Buttons
    UPROPERTY(meta = (BindWidget))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ShopButton;

    UPROPERTY(meta = (BindWidget))
    UButton* InventoryButton;

    UPROPERTY(meta = (BindWidget))
    UButton* AchievementsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    // Player Info Display
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerLevelText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerCurrencyText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ExperienceText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ExperienceBar;

    // Character Class Display (Optional - can show selected class)
    UPROPERTY(meta = (BindWidget))
    UImage* CharacterClassIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CharacterClassText;

    // Background/Decoration (Optional)
    UPROPERTY(meta = (BindWidgetOptional))
    UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidgetOptional))
    UImage* LogoImage;

    // ========================================
    // Button Click Handlers
    // ========================================
    
    UFUNCTION()
    void OnPlayButtonClicked();

    UFUNCTION()
    void OnShopButtonClicked();

    UFUNCTION()
    void OnInventoryButtonClicked();

    UFUNCTION()
    void OnAchievementsButtonClicked();

    UFUNCTION()
    void OnSettingsButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();

    // ========================================
    // UI Update Functions
    // ========================================
    
    UFUNCTION(BlueprintCallable, Category = "Grimward|UI")
    void UpdatePlayerStats();

    UFUNCTION(BlueprintCallable, Category = "Grimward|UI")
    void UpdateCharacterInfo();

    UFUNCTION(BlueprintCallable, Category = "Grimward|UI")
    void PlayButtonHoverSound();

    UFUNCTION(BlueprintCallable, Category = "Grimward|UI")
    void PlayButtonClickSound();

private:
    // Helper function to get game instance
    UGW_GameInstance* GetGWGameInstance() const;

    // Sound effects (optional)
    UPROPERTY(EditDefaultsOnly, Category = "Grimward|Audio")
    USoundBase* ButtonHoverSound;

    UPROPERTY(EditDefaultsOnly, Category = "Grimward|Audio")
    USoundBase* ButtonClickSound;

    // Character class icons
    UPROPERTY(EditDefaultsOnly, Category = "Grimward|UI")
    UTexture2D* KnightIcon;

    UPROPERTY(EditDefaultsOnly, Category = "Grimward|UI")
    UTexture2D* WizardIcon;

    UPROPERTY(EditDefaultsOnly, Category = "Grimward|UI")
    UTexture2D* RogueIcon;
};
#pragma endregion
/*-------------------------------------------------------------------------*/