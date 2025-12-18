// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Public/UI/Menus/GW_MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Public/Core/GW_GameInstance.h"
#include "Public/Core/GW_GameData.h"
#include "Sound/SoundBase.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region GW_MainMenuWidget.cpp
void UGW_MainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Log, TEXT("[Grimward] Main Menu Widget Constructed"));

    // Bind button click events
    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnPlayButtonClicked);
        UE_LOG(LogTemp, Log, TEXT("[Grimward] Play button bound"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[Grimward] PlayButton is NULL! Check UMG widget name matches 'PlayButton'"));
    }

    if (ShopButton)
    {
        ShopButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnShopButtonClicked);
    }

    if (InventoryButton)
    {
        InventoryButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnInventoryButtonClicked);
    }

    if (AchievementsButton)
    {
        AchievementsButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnAchievementsButtonClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnSettingsButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UGW_MainMenuWidget::OnQuitButtonClicked);
    }

    // Update UI with current player data
    UpdatePlayerStats();
    UpdateCharacterInfo();

    // Set input mode to UI only
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(this->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void UGW_MainMenuWidget::NativeDestruct()
{
    // Unbind events to prevent memory leaks
    if (PlayButton)
    {
        PlayButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnPlayButtonClicked);
    }
    if (ShopButton)
    {
        ShopButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnShopButtonClicked);
    }
    if (InventoryButton)
    {
        InventoryButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnInventoryButtonClicked);
    }
    if (AchievementsButton)
    {
        AchievementsButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnAchievementsButtonClicked);
    }
    if (SettingsButton)
    {
        SettingsButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnSettingsButtonClicked);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveDynamic(this, &UGW_MainMenuWidget::OnQuitButtonClicked);
    }

    Super::NativeDestruct();
}

void UGW_MainMenuWidget::OnPlayButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Play button clicked - Loading ThirdPersonMap"));
    
    PlayButtonClickSound();

    // Load the default Unreal Engine top-down map with Manny
    // In UE5, the template map is typically "ThirdPersonMap" or you can specify your own
    UGameplayStatics::OpenLevel(this, FName("ThirdPersonMap"));
}

void UGW_MainMenuWidget::OnShopButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Shop button clicked"));
    PlayButtonClickSound();
    
    // TODO: Open shop widget
    // For now, just log
    // Later: Create and show shop widget overlay
}

void UGW_MainMenuWidget::OnInventoryButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Inventory button clicked"));
    PlayButtonClickSound();
    
    // TODO: Open inventory widget
}

void UGW_MainMenuWidget::OnAchievementsButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Achievements button clicked"));
    PlayButtonClickSound();
    
    // TODO: Open achievements widget
}

void UGW_MainMenuWidget::OnSettingsButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Settings button clicked"));
    PlayButtonClickSound();
    
    // TODO: Open settings widget
}

void UGW_MainMenuWidget::OnQuitButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("[Grimward] Quit button clicked"));
    PlayButtonClickSound();
    
    // Quit the game
    UKismetSystemLibrary::QuitGame(
        this, 
        GetWorld()->GetFirstPlayerController(), 
        EQuitPreference::Quit, 
        true
    );
}

void UGW_MainMenuWidget::UpdatePlayerStats()
{
    UGW_GameInstance* GameInstance = GetGWGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Grimward] Could not get game instance in UpdatePlayerStats"));
        return;
    }

    const FGW_PlayerProfile& Profile = GameInstance->PlayerProfile;

    // Update level text
    if (PlayerLevelText)
    {
        FString LevelString = FString::Printf(TEXT("Level %d"), Profile.Level);
        PlayerLevelText->SetText(FText::FromString(LevelString));
    }

    // Update currency text
    if (PlayerCurrencyText)
    {
        FString CurrencyString = FString::Printf(TEXT("Gold: %d"), Profile.Currency);
        PlayerCurrencyText->SetText(FText::FromString(CurrencyString));
    }

    // Calculate experience for next level
    int32 ExpNeeded = FMath::RoundToInt(100.0f * FMath::Pow(static_cast<float>(Profile.Level), 1.5f));
    
    // Update experience text
    if (ExperienceText)
    {
        FString ExpString = FString::Printf(TEXT("EXP: %d / %d"), Profile.Experience, ExpNeeded);
        ExperienceText->SetText(FText::FromString(ExpString));
    }

    // Update experience bar
    if (ExperienceBar)
    {
        float ExpPercent = ExpNeeded > 0 ? static_cast<float>(Profile.Experience) / ExpNeeded : 0.0f;
        ExperienceBar->SetPercent(ExpPercent);
    }

    UE_LOG(LogTemp, Log, TEXT("[Grimward] Updated player stats - Level: %d, Currency: %d, Exp: %d/%d"), 
        Profile.Level, Profile.Currency, Profile.Experience, ExpNeeded);
}

void UGW_MainMenuWidget::UpdateCharacterInfo()
{
    UGW_GameInstance* GameInstance = GetGWGameInstance();
    if (!GameInstance) return;

    const FGW_PlayerProfile& Profile = GameInstance->PlayerProfile;

    // Update character class text
    if (CharacterClassText)
    {
        FString ClassName;
        switch (Profile.CharacterClass)
        {
            case EGW_CharacterClass::Knight:
                ClassName = TEXT("Knight");
                break;
            case EGW_CharacterClass::Wizard:
                ClassName = TEXT("Wizard");
                break;
            case EGW_CharacterClass::Rogue:
                ClassName = TEXT("Rogue");
                break;
            default:
                ClassName = TEXT("Adventurer");
                break;
        }
        CharacterClassText->SetText(FText::FromString(ClassName));
    }

    // Update character class icon
    if (CharacterClassIcon)
    {
        UTexture2D* IconToUse = nullptr;
        switch (Profile.CharacterClass)
        {
            case EGW_CharacterClass::Knight:
                IconToUse = KnightIcon;
                break;
            case EGW_CharacterClass::Wizard:
                IconToUse = WizardIcon;
                break;
            case EGW_CharacterClass::Rogue:
                IconToUse = RogueIcon;
                break;
        }

        if (IconToUse)
        {
            CharacterClassIcon->SetBrushFromTexture(IconToUse);
        }
    }

    // Update player name (you can make this customizable)
    if (PlayerNameText)
    {
        PlayerNameText->SetText(FText::FromString(TEXT("Adventurer")));
    }
}

void UGW_MainMenuWidget::PlayButtonHoverSound()
{
    if (ButtonHoverSound)
    {
        UGameplayStatics::PlaySound2D(this, ButtonHoverSound);
    }
}

void UGW_MainMenuWidget::PlayButtonClickSound()
{
    if (ButtonClickSound)
    {
        UGameplayStatics::PlaySound2D(this, ButtonClickSound);
    }
}

UGW_GameInstance* UGW_MainMenuWidget::GetGWGameInstance() const
{
    return Cast<UGW_GameInstance>(UGameplayStatics::GetGameInstance(this));
}
#pragma endregion
/*-------------------------------------------------------------------------*/