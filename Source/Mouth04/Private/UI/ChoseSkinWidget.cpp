// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoseSkinWidget.h"
#include "ChoseMapWidget.h"
#include "SelsectPlayerWidget.h"
#include "Players/ABoxBot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UChoseSkinWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button_Next)
	{
		Button_Next->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedNext);
	}
	if (Button_Up)
	{
		Button_Up->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedUp);
	}
	Button_Pig->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedPig);
	Button_Sheep->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedSheep);
	Button_Cow->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedCow);
	Button_Chick->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedChick);
	Button_Cat->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedCat);
	Button_Anim->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedAnim);
	Button_Change->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedChange);

	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	bIsTowPlayer = GI->bIsTwoPlayerMode;
	if (bIsTowPlayer==true)//双人选择
	{
		WidgetSwitcher_PlayerMode->SetActiveWidgetIndex(1);
		Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
		CurrentPlayer = 1;
		OnClickedSheep();//默认P1是小羊
		CurrentPlayer = 2;
		OnClickedPig();//默认P2是小猪
		CurrentPlayer = 1;
	}
	else//单人选择
	{
		WidgetSwitcher_PlayerMode->SetActiveWidgetIndex(0);
		Image_ChoseP2->SetVisibility(ESlateVisibility::Hidden);		
		TextBlock_P2->SetVisibility(ESlateVisibility::Hidden);
		Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
		bCanNext=true;
		OnClickedSheep();//默认选择了小羊
	}
}

void UChoseSkinWidget::OnClickedNext()
{
	if (bCanNext==false) return;
	UE_LOG(LogTemp, Log, TEXT("UMG:点击下一页:加载地图选择界面"));
	if (!ChoseMapWidget)
	{
		TSubclassOf<UChoseMapWidget> WidgetClass = LoadClass<UChoseMapWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_ChoseMap.U_ChoseMap_C'")
		);
		if (WidgetClass)
		{
			// 先通过GetWorld()获取PlayerController
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				ChoseMapWidget = CreateWidget<UChoseMapWidget>(PC, WidgetClass);
			}
			if (ChoseMapWidget)
			{
				ChoseMapWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
	else
	{
		ChoseMapWidget->AddToViewport(); // 显示到屏幕
	}
	GI->G_WidgetChose = EG_Widget::ChoseMap;//标记进入游戏选择界面
	GI->LoadAndPlaySound2D();//播放音效
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose skin 已从视口移除！"));
}

void UChoseSkinWidget::OnClickedUp()
{
	UE_LOG(LogTemp, Log, TEXT("UMG:点击返回上一级:玩家数量选择界面"));
	if (!ChosePlayersWidget)
	{
		TSubclassOf<USelsectPlayerWidget> WidgetClass = LoadClass<USelsectPlayerWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_SelectPlayer.U_SelectPlayer_C'")
		);
		if (WidgetClass)
		{
			// 先通过GetWorld()获取PlayerController
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				ChosePlayersWidget = CreateWidget<USelsectPlayerWidget>(PC, WidgetClass);
			}
			if (ChosePlayersWidget)
			{
				ChosePlayersWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
	else
	{
		ChosePlayersWidget->AddToViewport(); // 显示到屏幕
	}	
	GI->G_WidgetChose = EG_Widget::ChosePlayer;//标记进入游戏选择界面
	GI->LoadAndPlaySound2D();//播放音效
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose skin 已从视口移除！"));
}

void UChoseSkinWidget::OnClickedChange()//双人模式下
{
	GI->LoadAndPlaySound2D();//播放音效
	if (CurrentPlayer==1)
	{
		PlayAnimation(Anim_ToP2);
		CurrentPlayer=2;
		UE_LOG(LogTemp, Log, TEXT("切换为玩家2"));
	}
	else
	{
		PlayAnimation(Anim_ToP1);
		CurrentPlayer=1;
		UE_LOG(LogTemp, Log, TEXT("切换为玩家1"));
	}
}

void UChoseSkinWidget::OnClickedSheep()
{
	GI->LoadAndPlaySound2D();//播放音效
	Image_ChoseP1->SetVisibility(ESlateVisibility::Visible);
	Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
	if (bIsTowPlayer)//双人模式
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Visible);
		if (CurrentPlayer==1)
		{
			SetImageChoseP1Padding(-330.0, -150.0, 0.0, 0.0);
			SetTextBlockP1Padding(-380.0,-90.0,0,0);
			PlayAnimation(Anim_P1Chose);
			CurrentP1Type=EGlobalPlayerType::Sheep;
			ChangePlayerTypeInGI(1,CurrentP1Type);//修改Gameinstance中的记录
		}
		if (CurrentPlayer==2)
		{
			SetImageChoseP2Padding(-330.0, -150.0, 0.0, 0.0);
			SetTextBlockP2Padding(-280.0,-90.0,0,0);
			PlayAnimation(Anim_P2Chose);
			CurrentP2Type=EGlobalPlayerType::Sheep;
			ChangePlayerTypeInGI(2,CurrentP2Type);//修改Gameinstance中的记录
			bCanNext=true;
			PlayAnimation(Anim_Next);
		}
	}
	else//单人模式
	{
		SetImageChoseP1Padding(-330.0, -150.0, 0.0, 0.0);
		SetTextBlockP1Padding(-380.0,-90.0,0,0);
		PlayAnimation(Anim_P1Chose);
		CurrentP1Type=EGlobalPlayerType::Sheep;
		ChangePlayerTypeInGI(1,CurrentP1Type);
		PlayAnimation(Anim_Next);
	}
}

void UChoseSkinWidget::OnClickedPig()
{
	GI->LoadAndPlaySound2D();//播放音效
	Image_ChoseP1->SetVisibility(ESlateVisibility::Visible);
	Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
	if (bIsTowPlayer)//双人模式
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Visible);
		if (CurrentPlayer==1)
		{
			SetImageChoseP1Padding(-0.0, -150.0, 0.0, 0.0);
			SetTextBlockP1Padding(-50.0,-90.0,0,0);
			PlayAnimation(Anim_P1Chose);
			CurrentP1Type=EGlobalPlayerType::Pig;
			ChangePlayerTypeInGI(1,CurrentP1Type);
		}
		if (CurrentPlayer==2)
		{
			SetImageChoseP2Padding(0.0, -150.0, 0.0, 0.0);
			SetTextBlockP2Padding(50.0,-90.0,0,0);
			PlayAnimation(Anim_P2Chose);
			CurrentP2Type=EGlobalPlayerType::Pig;
			ChangePlayerTypeInGI(2,CurrentP2Type);
			PlayAnimation(Anim_Next);
			bCanNext=true;
		}
	}
	else//单人模式
	{
		SetImageChoseP1Padding(0.0, -150.0, 0.0, 0.0);
		SetTextBlockP1Padding(-50.0,-90.0,0,0);
		PlayAnimation(Anim_P1Chose);
		CurrentP1Type=EGlobalPlayerType::Pig;
		ChangePlayerTypeInGI(1,CurrentP1Type);
		PlayAnimation(Anim_Next);
	}
}

void UChoseSkinWidget::OnClickedCow()
{
	GI->LoadAndPlaySound2D();//播放音效
	Image_ChoseP1->SetVisibility(ESlateVisibility::Visible);
	Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
	if (bIsTowPlayer)//双人模式
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Visible);
		if (CurrentPlayer==1)
		{
			SetImageChoseP1Padding(330.0, -150.0, 0.0, 0.0);
			SetTextBlockP1Padding(280.0,-90.0,0,0);
			PlayAnimation(Anim_P1Chose);
			CurrentP1Type=EGlobalPlayerType::Cow;
			ChangePlayerTypeInGI(1,CurrentP1Type);
		}
		if (CurrentPlayer==2)
		{
			SetImageChoseP2Padding(330.0, -150.0, 0.0, 0.0);
			SetTextBlockP2Padding(380.0,-90.0,0,0);
			PlayAnimation(Anim_P2Chose);
			CurrentP2Type=EGlobalPlayerType::Cow;
			ChangePlayerTypeInGI(2,CurrentP2Type);
			PlayAnimation(Anim_Next);
			bCanNext=true;
		}
	}
	else//单人模式
	{
		SetImageChoseP1Padding(330.0, -150.0, 0.0, 0.0);
		SetTextBlockP1Padding(280.0,-90.0,0,0);
		PlayAnimation(Anim_P1Chose);
		CurrentP1Type=EGlobalPlayerType::Cow;
		ChangePlayerTypeInGI(1,CurrentP1Type);
		PlayAnimation(Anim_Next);
	}
}

void UChoseSkinWidget::OnClickedChick()
{
	GI->LoadAndPlaySound2D();//播放音效
	Image_ChoseP1->SetVisibility(ESlateVisibility::Visible);
	Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
	if (bIsTowPlayer)//双人模式
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Visible);
		if (CurrentPlayer==1)
		{
			SetImageChoseP1Padding(-330.0, 70.0, 0.0, 0.0);
			SetTextBlockP1Padding(-380.0,130.0,0,0);
			PlayAnimation(Anim_P1Chose);
			CurrentP1Type=EGlobalPlayerType::Chick;
			ChangePlayerTypeInGI(1,CurrentP1Type);//修改Gameinstance中的记录
		}
		if (CurrentPlayer==2)
		{
			SetImageChoseP2Padding(-330.0, 70.0, 0.0, 0.0);
			SetTextBlockP2Padding(-280.0,130.0,0,0);
			PlayAnimation(Anim_P2Chose);
			CurrentP2Type=EGlobalPlayerType::Chick;
			ChangePlayerTypeInGI(2,CurrentP2Type);//修改Gameinstance中的记录
			bCanNext=true;
			PlayAnimation(Anim_Next);
			bCanNext=true;
		}
	}
	else//单人模式
	{
		SetImageChoseP1Padding(-330.0, 70.0, 0.0, 0.0);
		SetTextBlockP1Padding(-380.0,130.0,0,0);
		PlayAnimation(Anim_P1Chose);
		CurrentP1Type=EGlobalPlayerType::Chick;
		ChangePlayerTypeInGI(1,CurrentP1Type);
		bCanNext=true;
		PlayAnimation(Anim_Next);
	}
	
}

void UChoseSkinWidget::OnClickedCat()
{
	GI->LoadAndPlaySound2D();//播放音效
	Image_ChoseP1->SetVisibility(ESlateVisibility::Visible);
	Image_LockAnimal->SetVisibility(ESlateVisibility::Hidden);
	if (bIsTowPlayer)//双人模式
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Visible);
		if (CurrentPlayer==1)
		{
			SetImageChoseP1Padding(-0.0, 70.0, 0.0, 0.0);
			SetTextBlockP1Padding(-50.0,130.0,0,0);
			PlayAnimation(Anim_P1Chose);
			CurrentP1Type=EGlobalPlayerType::Cat;
			ChangePlayerTypeInGI(1,CurrentP1Type);
		}
		if (CurrentPlayer==2)
		{
			SetImageChoseP2Padding(0.0, 70.0, 0.0, 0.0);
			SetTextBlockP2Padding(50.0,130.0,0,0);
			PlayAnimation(Anim_P2Chose);
			CurrentP2Type=EGlobalPlayerType::Cat;
			ChangePlayerTypeInGI(2,CurrentP2Type);
			PlayAnimation(Anim_Next);
			bCanNext=true;
		}
	}
	else//单人模式
	{
		SetImageChoseP1Padding(0.0, 70.0, 0.0, 0.0);
		SetTextBlockP1Padding(-50.0,130.0,0,0);
		PlayAnimation(Anim_P1Chose);
		CurrentP1Type=EGlobalPlayerType::Cat;
		ChangePlayerTypeInGI(1,CurrentP1Type);
		PlayAnimation(Anim_Next);
	}
}

void UChoseSkinWidget::OnClickedAnim()
{
	
	Image_LockAnimal->SetVisibility(ESlateVisibility::Visible);
	if (CurrentPlayer==1)
	{
		Image_ChoseP1->SetVisibility(ESlateVisibility::Hidden);
		SetTextBlockP1Padding(280.0,130.0,0,0);
		CurrentP1Type=EGlobalPlayerType::None;
	}
	if (CurrentPlayer==2)
	{
		Image_ChoseP2->SetVisibility(ESlateVisibility::Hidden);
		SetTextBlockP2Padding(380.0,130.0,0,0);
		CurrentP2Type=EGlobalPlayerType::None;
	}
	bCanNext=false;
	PlayAnimation(Anim_CannotNext);
}

void UChoseSkinWidget::ChangePlayerTypeInGI(int32 CurrentPlayerNumber, EGlobalPlayerType PlayerType)
{
	TSubclassOf<AABoxBot> BP_Class=nullptr;
	switch (PlayerType)
	{
		case EGlobalPlayerType::Sheep:
			BP_Class = LoadClass<AABoxBot>(
				nullptr, 
				TEXT("/Game/Blueprints/Players/BP_Sheep.BP_Sheep_C") );
			break;
		case EGlobalPlayerType::Pig:
			BP_Class = LoadClass<AABoxBot>(
			nullptr, 
			TEXT("/Game/Blueprints/Players/BP_Pig.BP_Pig_C") );
			break;
		case EGlobalPlayerType::Cow:
			BP_Class = LoadClass<AABoxBot>(
			nullptr, 
			TEXT("/Game/Blueprints/Players/Cow.Cow_C") );
			break;
		case EGlobalPlayerType::Chick:
			BP_Class = LoadClass<AABoxBot>(
			nullptr, 
			TEXT("/Game/Blueprints/Players/Chick.Chick_C") );
			break;
		case EGlobalPlayerType::Cat:
			BP_Class = LoadClass<AABoxBot>(
			nullptr, 
			TEXT("/Game/Blueprints/Players/Cats.Cats_C") );
			break;
		case EGlobalPlayerType::None:
			BP_Class = LoadClass<AABoxBot>(
			nullptr, 
			TEXT("/Game/Blueprints/Players/BP_Sheep.BP_Sheep_C") );//默认小羊
			break;
		default:
			break;
	}
	// 检查蓝图类是否加载成功
	if (!BP_Class)
	{
		UE_LOG(LogTemp, Error, TEXT("<AABoxBot>子类加载失败！请检查路径是否正确"));
		return;
	}
	// 赋值给GameInstance的变量
	if (CurrentPlayerNumber==1) GI->G_P1SelectedClass = BP_Class;
	if (CurrentPlayerNumber==2) GI->G_P2SelectedClass = BP_Class;
}

void UChoseSkinWidget::SetImageChoseP1Padding(float Left, float Top, float Right, float Bottom)
{
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image_ChoseP1->Slot);//获取插槽
	if (!OverlaySlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("控件 Image_ChoseP1 不是 Overlay_Skin 的子控件，或插槽类型错误"));
	}
	OverlaySlot->SetPadding(FMargin(Left, Top, Right, Bottom));
}

void UChoseSkinWidget::SetImageChoseP2Padding(float Left, float Top, float Right, float Bottom)
{
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(Image_ChoseP2->Slot);
	if (!OverlaySlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("控件 Image_ChoseP2 不是 Overlay_Skin 的子控件，或插槽类型错误"));
	}
	OverlaySlot->SetPadding(FMargin(Left, Top, Right, Bottom));
}

void UChoseSkinWidget::SetTextBlockP1Padding(float Left, float Top, float Right, float Bottom)
{
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(TextBlock_P1->Slot);//获取插槽
	if (!OverlaySlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("控件 TextBlock_P1 不是 Overlay_Skin 的子控件，或插槽类型错误"));
	}
	OverlaySlot->SetPadding(FMargin(Left, Top, Right, Bottom));
}

void UChoseSkinWidget::SetTextBlockP2Padding(float Left, float Top, float Right, float Bottom)
{
	UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(TextBlock_P2->Slot);//获取插槽
	if (!OverlaySlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("控件 TextBlock_P2 不是 Overlay_Skin 的子控件，或插槽类型错误"));
	}
	OverlaySlot->SetPadding(FMargin(Left, Top, Right, Bottom));
}


