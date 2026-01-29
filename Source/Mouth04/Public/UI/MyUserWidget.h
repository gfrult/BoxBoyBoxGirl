#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

//前向声明
class UButton;	   //按钮控件
class UTextBlock;  // 文本块控件
class UImage;      // 图片控件
class UTexture2D;  // 纹理类
class UHorizontalBox;
class UMyGameInstance;



UCLASS()
class MOUTH04_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void ShowSettingWidget();
	virtual void NativeConstruct() override;//重写NativeConstruct()函数:通常用来做初始化逻辑（比如绑定按钮点击事件、播放初始动画）；
	virtual void NativeDestruct() override;// 重写销毁函数：取消订阅（避免野指针） ==========
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ShowSettingBtn;
	
	// 玩家1的控件变量:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_P1MaxNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_P1CanUseNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_P1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_P1Box;
	// 玩家2的控件变量:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_P2MaxNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_P2CanUseNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_P2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_P2Box;
	//玩家2的条形框
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "UI|HorizontalBox")
	TObjectPtr<UHorizontalBox> HorizontalBox_P2;

	// ========== 通用函数：修改任意文本块内容 ==========
	UFUNCTION(BlueprintCallable, Category = "UI|Text")
	void SetTextBlockContent(
		UPARAM(DisplayName="目标文本块") UTextBlock* TargetTextBlock,
		UPARAM(DisplayName="新文本内容") const FText& NewText	);
	
	// ========== 通用函数：修改任意图片控件的纹理 ==========
	UFUNCTION(BlueprintCallable, Category = "UI|Image")
	void SetImageByPath(
		UPARAM(DisplayName="目标图片控件") UImage* TargetImage,
		UPARAM(DisplayName="图片路径") const FString& ImagePath
	);
	
	
	//P1的现有箱子数更新函数
	UFUNCTION(BlueprintCallable, Category = "UI|BoxNumber")
	void UpdateP1RemainingBoxNumberText(int32 NewNumber);
	UFUNCTION(BlueprintCallable, Category = "UI|BoxNumber")
	void UpdateP2RemainingBoxNumberText(int32 NewNumber);
	
	UFUNCTION()
	void BindPlayerTexturesToImages(TSubclassOf<AABoxBot> PlayerSelectedClass, UImage* TargetImage_Player, UImage* TargetImage_Box);



	
private:
	// 委托处理函数：接收剩余箱子数并更新UI
	UFUNCTION()
	void OnRemainingBoxNumberChanged(int32 NewNumber);
	UFUNCTION()
	void OnP2RemainingBoxNumberChanged(int32 NewNumber);
	// GameInstance指针（缓存，避免重复Cast）
	TObjectPtr<UMyGameInstance> GI;
	
	
	
	
};
