// Copyright 2016 NAVER Corp. All rights reserved.

#include "CafeSDKPluginPrivatePCH.h"
#include "IOSCafeSdk.h"
#include "IOSAppDelegate.h"
#include "IOSView.h"
#include "IOSPlatformString.h"


static void ListenNCSDKOpenURL(UIApplication* application, NSURL* url, NSString* sourceApplication, id annotation)
{
    [[NCSDKLoginManager getSharedInstance] finishNaverLoginWithURL:url];
}

FIOSCafeSdk::FIOSCafeSdk()
{
}

void FIOSCafeSdk::Init(FString ClientId, FString ClientSecret, int32 CafeId)
{
    [[CafeCallbackObject getSharedInstance] setSDKInfoWithClientId:ClientId.GetNSString()
                                                      clientSecret:ClientSecret.GetNSString()
                                                            cafeId:CafeId];
//    If you want to use NaverAppLogin uncomment it
//    FIOSCoreDelegates::OnOpenURL.AddStatic(&ListenNCSDKOpenURL);
}
void FIOSCafeSdk::InitGlobal(FString ClientId, int32 CafeId) {
    [[CafeCallbackObject getSharedInstance] setGlobalSDKInfoWithClientId:ClientId.GetNSString()
                                                             communityId:CafeId];
}

void FIOSCafeSdk::StartHome() const
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startHome)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}

void FIOSCafeSdk::StartNotice() const
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startNotice)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}

void FIOSCafeSdk::StartEvent() const
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startEvent)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}

void FIOSCafeSdk::StartMenu() const
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startMenu)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}


void FIOSCafeSdk::StartProfile() const
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startProfile)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}

void FIOSCafeSdk::StartWrite() const
{
    dispatch_block_t block = ^{
        [[NCSDKManager getSharedInstance] presentArticlePostViewController];
    };
    [[CafeCallbackObject getSharedInstance] startMainThreadWithBlock:block];
}

void FIOSCafeSdk::StartImageWrite(FString ImageUri) const
{
    dispatch_block_t block = ^{
        [[NCSDKManager getSharedInstance] presentArticlePostViewControllerWithType:kGLArticlePostTypeImage
                                                                          filePath:[NSString stringWithFString:ImageUri]];
    };
    [[CafeCallbackObject getSharedInstance] startMainThreadWithBlock:block];
}

void FIOSCafeSdk::StartVideoWrite(FString VideoUri) const
{
    dispatch_block_t block = ^{
        [[NCSDKManager getSharedInstance] presentArticlePostViewControllerWithType:kGLArticlePostTypeVideo
                                                                          filePath:[NSString stringWithFString:VideoUri]];
    };
    [[CafeCallbackObject getSharedInstance] startMainThreadWithBlock:block];
}

void FIOSCafeSdk::StartMore()
{
    [[CafeCallbackObject getSharedInstance] performSelectorOnMainThread:@selector(startMore)
                                                             withObject:nil
                                                          waitUntilDone:NO];
}
bool FIOSCafeSdk::IsShow() const
{
    return false;
}

void FIOSCafeSdk::SyncGameUserId(FString GameUserId) const
{
    [[NCSDKManager getSharedInstance] syncGameUserId:[NSString stringWithFString:GameUserId]];
}

void FIOSCafeSdk::StartWidget() const
{
    
}
void FIOSCafeSdk::StopWidget() const
{
    [[NCSDKManager getSharedInstance] stopWidget];
}
void FIOSCafeSdk::ShowWidgetWhenUnloadSdk(bool bUse) const
{
    [[NCSDKManager getSharedInstance] setShowWidgetWhenUnloadSDK:bUse];
}
void FIOSCafeSdk::SetWidgetStartPosition(bool bIsLeft, int HeightPercentage) const
{
    
}

void FIOSCafeSdk::SetUseVideoRecord(bool bUse) const
{
    //Available Unreal Engine 4.13 (Support for the Apple Replay Kit Framework)
    [[NCSDKManager getSharedInstance] setUseWidgetVideoRecord:false];
}
void FIOSCafeSdk::SetThemeColor(FString ThemeColorCSSString, FString TabBackgroundColorCSSString) const
{
    [[NCSDKManager getSharedInstance] setThemeColor:[NSString stringWithFString:ThemeColorCSSString]];
}

bool FIOSCafeSdk::IsSupportedOSVersion() const
{
    bool isSupported = false;
    static NSString *reqSysVer = @"7.0";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
    {
        isSupported = true;
    }
    return isSupported;
}

// statistics.
void FIOSCafeSdk::SendNewUser(FString GameUserId, FString Market)
{
    [NCSDKStatistics sendNewUser:GameUserId.GetNSString()
                       andMarket:Market.GetNSString()];
    
}
void FIOSCafeSdk::SendPayUser(FString GameUserId, double Pay, FString ProductCode, FString Currency, FString Market)
{
    [NCSDKStatistics sendPayUser:GameUserId.GetNSString()
                          andPay:Pay
                  andProductCode:ProductCode.GetNSString()
                     andCurrency:Currency.GetNSString()
                       andMarket:Market.GetNSString()];
}

//Naver Login 
void FIOSCafeSdk::Init(FString ClientId, FString ClientSecret)
{
    Init(ClientId, ClientSecret, -1);
}

void FIOSCafeSdk::Login()
{
    [[CafeCallbackObject getSharedInstance] requestNaverLogin];
}

void FIOSCafeSdk::Logout()
{
    [[NCNaverLoginManager getSharedInstance] naverIdLogout];
}

bool FIOSCafeSdk::IsLogin()
{
    return [[NCNaverLoginManager getSharedInstance] isNaverIdLogin];
}

void FIOSCafeSdk::GetProfile()
{
    [[NCNaverLoginManager getSharedInstance] getNaverIdProfile];
}


@implementation CafeCallbackObject
#pragma mark - NCSDKDelegate
+ (CafeCallbackObject*)getSharedInstance
{
    static CafeCallbackObject *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance  = [[[self class] alloc] init];
    });
    [sharedInstance setParentViewController];
    return sharedInstance;
}
- (void)setSDKInfoWithClientId:(NSString *)clientId clientSecret:(NSString *)clientSecret cafeId:(NSInteger)cafeId {
    
    [[NCSDKManager getSharedInstance] setNaverLoginClientId:clientId
                                     naverLoginClientSecret:clientSecret
                                                     cafeId:cafeId];
    [[NCSDKManager getSharedInstance] setOrientationIsLandscape:YES];
}
- (void)setGlobalSDKInfoWithClientId:(NSString *)clientId communityId:(NSInteger)communityId {
    [[NCSDKManager getSharedInstance] setNeoIdConsumerKey:clientId communityId:communityId];
    [[NCSDKManager getSharedInstance] setOrientationIsLandscape:YES];
}
- (void)setParentViewController {
    [[NCSDKManager getSharedInstance] setParentViewController:[IOSAppDelegate GetDelegate].IOSController];
    [[NCSDKManager getSharedInstance] setNcSDKDelegate:self];
}
- (void)startHome {
    [[NCSDKManager getSharedInstance] presentMainViewController];
}
- (void)startNotice {
    [[NCSDKManager getSharedInstance] presentMainViewControllerWithTabIndex:kGLTabTypeNotice];
}
- (void)startEvent {
    [[NCSDKManager getSharedInstance] presentMainViewControllerWithTabIndex:kGLTabTypeEvent];
}
- (void)startMenu {
    [[NCSDKManager getSharedInstance] presentMainViewControllerWithTabIndex:kGLTabTypeMenuList];
}
- (void)startProfile {
    [[NCSDKManager getSharedInstance] presentMainViewControllerWithTabIndex:kGLTabTypeProfile];
}
- (void)startMore {
    [[NCSDKManager getSharedInstance] presentEtc];
}
- (void)startMainThreadWithBlock:(void (^)(void))block {
    if (![[NSThread currentThread] isMainThread]) {
        dispatch_async(dispatch_get_main_queue(), block);
    } else {
        block();
    }
}
- (void)requestNaverLogin {
    [[NCNaverLoginManager getSharedInstance] setNcNaverLoginManagerDelegate:self];
    [[NCNaverLoginManager getSharedInstance] naverIdLogin];
}
#pragma mark NCSDKDelegate
- (void)ncSDKViewDidLoad {
    FCafeSDKPluginModule::OnCafeSdkStarted.Broadcast();
}
- (void)ncSDKViewDidUnLoad {
    FCafeSDKPluginModule::OnCafeSdkStopped.Broadcast();
}
- (void)ncSDKJoinedCafeMember {
    FCafeSDKPluginModule::OnCafeSdkJoined.Broadcast();
}
- (void)ncSDKPostedArticleAtMenu:(NSInteger)menuId attachImageCount:(NSInteger)imageCount attachVideoCount:(NSInteger)videoCount {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.6 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        FCafeSDKPluginModule::OnCafeSdkPostedArticle.Broadcast(menuId, imageCount, videoCount);
    });
}
- (void)ncSDKPostedCommentAtArticle:(NSInteger)articleId {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.6 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        FCafeSDKPluginModule::OnCafeSdkPostedComment.Broadcast(articleId);
    });
}
- (void)ncSDKRequestScreenShot {
    FScreenshotRequest::RequestScreenshot("CafeSdkScreenshot.png", false, false);
}
- (void)ncSDKDidVoteAtArticle:(NSInteger)articleId {
    FCafeSDKPluginModule::OnCafeSdkDidVote.Broadcast(articleId);
}

//widget
- (void)ncSDKWidgetPostArticleWithImage {
    FScreenshotRequest::RequestScreenshot("CafeSdkScreenshot.png", false, false);
}
- (void)ncSDKWidgetSuccessVideoRecord {
    FCafeSDKPluginModule::OnCafeSdkRecordFinish.Broadcast("");
}

#pragma mark - NCNaverLoginManagerDelegate
- (void)ncSDKLoginCallback {
    FCafeSDKPluginModule::OnLoggedIn.Broadcast(true);
}

- (void)ncSDKGetProfile:(NSString *)result {
    FString jsonString = UTF8_TO_TCHAR(result);
    FCafeSDKPluginModule::OnGetProfile.Broadcast(jsonString);
}

@end
