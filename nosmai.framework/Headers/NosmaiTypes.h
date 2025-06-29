/*
 * NosmaiTypes.h
 * Nosmai SDK Types and Enumerations - PROFESSIONAL INTERFACE
 *
 * Created by Nosmai SDK Team
 * Copyright © 2024 Nosmai. All rights reserved.
 */

#ifndef NOSMAI_TYPES_H
#define NOSMAI_TYPES_H

#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark - SDK State Management

/**
 * SDK initialization and runtime states
 */
typedef NS_ENUM(NSInteger, NosmaiState) {
  NosmaiStateUninitialized = 0,  // SDK not initialized
  NosmaiStateInitializing,       // License verification in progress
  NosmaiStateReady,              // SDK ready to use
  NosmaiStateError,              // Initialization failed
  NosmaiStatePaused,             // SDK paused (app in background)
  NosmaiStateTerminated          // SDK terminated
};

#pragma mark - Camera Types

/**
 * Camera position enumeration
 */
typedef NS_ENUM(NSInteger, NosmaiCameraPosition) {
  NosmaiCameraPositionBack = 0,
  NosmaiCameraPositionFront
};

/**
 * Camera state enumeration
 */
typedef NS_ENUM(NSInteger, NosmaiCameraState) {
  NosmaiCameraStateStopped = 0,
  NosmaiCameraStateStarting,
  NosmaiCameraStateRunning,
  NosmaiCameraStateStopping,
  NosmaiCameraStateError
};

/**
 * Video orientation modes
 */
typedef NS_ENUM(NSInteger, NosmaiVideoOrientation) {
  NosmaiVideoOrientationPortrait = 0,
  NosmaiVideoOrientationPortraitUpsideDown,
  NosmaiVideoOrientationLandscapeLeft,
  NosmaiVideoOrientationLandscapeRight
};

#pragma mark - Effect Types

/**
 * Effect loading state
 */
typedef NS_ENUM(NSInteger, NosmaiEffectState) {
  NosmaiEffectStateNotLoaded = 0,
  NosmaiEffectStateLoading,
  NosmaiEffectStateReady,
  NosmaiEffectStateError
};

#pragma mark - Error Codes

/**
 * SDK error codes
 */
typedef NS_ENUM(NSInteger, NosmaiErrorCode) {
  NosmaiErrorCodeUnknown = 1000,
  NosmaiErrorCodeLicenseInvalid = 1001,
  NosmaiErrorCodeLicenseExpired = 1002,
  NosmaiErrorCodeNetworkError = 1003,
  NosmaiErrorCodeCameraPermissionDenied = 1004,
  NosmaiErrorCodeCameraNotAvailable = 1005,
  NosmaiErrorCodeEffectLoadFailed = 1006,
  NosmaiErrorCodeInitializationFailed = 1007,
  NosmaiErrorCodeResourceNotFound = 1008,
  NosmaiErrorCodeInvalidParameter = 1009,
  NosmaiErrorCodeMemoryError = 1010
};

#pragma mark - Completion Blocks

/**
 * SDK initialization completion block
 */
typedef void (^NosmaiInitializationCompletion)(BOOL success,
                                               NSError* _Nullable error);

/**
 * Effect loading completion block
 */
typedef void (^NosmaiEffectLoadCompletion)(BOOL success,
                                           NSError* _Nullable error);

/**
 * Cloud filter download progress block
 */
typedef void (^NosmaiDownloadProgress)(float progress);

/**
 * Cloud filter download completion block
 */
typedef void (^NosmaiDownloadCompletion)(BOOL success,
                                         NSString* _Nullable localPath,
                                         NSError* _Nullable error);

#pragma mark - Constants

/**
 * Error domain for Nosmai SDK
 */
extern NSErrorDomain const NosmaiErrorDomain;

/**
 * Current SDK version
 */
extern NSString* const NosmaiSDKVersion;

/**
 * Notification names
 */
extern NSNotificationName const NosmaiStateDidChangeNotification;
extern NSNotificationName const NosmaiCameraStateDidChangeNotification;
extern NSNotificationName const NosmaiEffectStateDidChangeNotification;

#pragma mark - Face Detection

/**
 * Face information structure
 */
@interface NosmaiFaceInfo : NSObject
@property(nonatomic, assign) CGRect boundingBox;
@property(nonatomic, assign) NSInteger faceID;
@property(nonatomic, assign) float confidence;
@property(nonatomic, assign) BOOL hasLandmarks;
@end

#pragma mark - Configuration

/**
 * Camera configuration
 */
@interface NosmaiCameraConfig : NSObject
@property(nonatomic, assign) NosmaiCameraPosition position;
@property(nonatomic, strong) NSString* sessionPreset;
@property(nonatomic, assign) NSInteger frameRate;
@property(nonatomic, assign) NosmaiVideoOrientation orientation;
@property(nonatomic, assign) BOOL enableMirroring;
@property(nonatomic, assign) AVCaptureFlashMode flashMode;
@end

/**
 * SDK configuration
 */
@interface NosmaiConfig : NSObject
@property(nonatomic, strong) NSString* apiKey;
@property(nonatomic, assign) BOOL enableDebugLogging;
@property(nonatomic, assign) BOOL enableFaceDetection;
@property(nonatomic, assign) NSTimeInterval licenseCheckTimeout;
@property(nonatomic, strong, nullable) NSString* cloudFilterCachePath;
@end

#pragma mark - Recording Types

/**
 * Video recording quality presets
 */
typedef NS_ENUM(NSInteger, NosmaiVideoQuality) {
  NosmaiVideoQualityLow = 0,     // 480p, 1 Mbps
  NosmaiVideoQualityMedium = 1,  // 720p, 2.5 Mbps
  NosmaiVideoQualityHigh = 2,    // 1080p, 4 Mbps
  NosmaiVideoQualityUltra = 3    // 1080p, 8 Mbps
};

/**
 * Recording configuration
 */
@interface NosmaiRecordingConfig : NSObject

/**
 * Video quality preset (default: NosmaiVideoQualityHigh)
 */
@property(nonatomic, assign) NosmaiVideoQuality videoQuality;

/**
 * Include audio in recording (default: YES)
 */
@property(nonatomic, assign) BOOL includeAudio;

/**
 * Maximum recording duration in seconds (0 = unlimited, default: 0)
 */
@property(nonatomic, assign) NSTimeInterval maxDuration;

/**
 * Custom output directory (nil = default temp directory)
 */
@property(nonatomic, strong, nullable) NSURL* outputDirectory;

/**
 * Custom video dimensions (CGSizeZero = use preview size)
 */
@property(nonatomic, assign) CGSize videoSize;

/**
 * Frame rate (0 = default 30 fps)
 */
@property(nonatomic, assign) NSInteger frameRate;

/**
 * Create default configuration
 */
+ (instancetype)defaultConfig;

@end

#pragma mark - Delegate Protocols

/**
 * Main SDK delegate
 */
@protocol NosmaiDelegate <NSObject>
@optional
- (void)nosmaiDidChangeState:(NosmaiState)newState;
- (void)nosmaiDidFailWithError:(NSError*)error;
- (void)nosmaiDidDetectFaces:(NSArray<NosmaiFaceInfo*>*)faces;
- (void)nosmaiDidUpdateFilters:(NSDictionary<NSString*, NSArray<NSDictionary*>*>*)organizedFilters;
@end

/**
 * Camera delegate
 */
@protocol NosmaiCameraDelegate <NSObject>
@optional
- (void)nosmaiCameraDidChangeState:(NosmaiCameraState)newState;
- (void)nosmaiCameraDidFailWithError:(NSError*)error;
- (void)nosmaiCameraDidCaptureFrame;
@end

/**
 * Effects delegate
 */
@protocol NosmaiEffectsDelegate <NSObject>
@optional
- (void)nosmaiEffectDidChangeState:(NosmaiEffectState)newState
                         forEffect:(NSString*)effectID;
- (void)nosmaiEffectDidFailWithError:(NSError*)error
                           forEffect:(NSString*)effectID;
@end

NS_ASSUME_NONNULL_END

#endif /* NOSMAI_TYPES_H */
