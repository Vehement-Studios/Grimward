// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GW_MainMenuWidget.generated.h"
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
    class UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ShopButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* InventoryButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AchievementsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    // Player Info Display
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PlayerLevelText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PlayerCurrencyText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ExperienceText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ExperienceBar;

    // Character Class Display (Optional - can show selected class)
    UPROPERTY(meta = (BindWidget))
    class UImage* CharacterClassIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CharacterClassText;

    // Background/Decoration (Optional)
    UPROPERTY(meta = (BindWidgetOptional))
    class UImage* BackgroundImage;

    UPROPERTY(meta = (BindWidgetOptional))
    class UImage* LogoImage;

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
    class UGW_GameInstance* GetGWGameInstance() const;

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