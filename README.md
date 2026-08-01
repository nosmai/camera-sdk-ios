# Nosmai Camera SDK for iOS

Nosmai Camera SDK provides real-time camera effects, built-in beauty and makeup,
face reshape, background replacement, recording support, processed frame output,
and local or cloud `.nosmai` packages for iOS applications.

[![iOS 15+](https://img.shields.io/badge/iOS-15%2B-blue)](https://developer.apple.com/ios/)
[![Version 3.0.0](https://img.shields.io/badge/version-3.0.0-blue)](../../releases/tag/v3.0.0)
[![Architecture arm64](https://img.shields.io/badge/architecture-arm64-lightgrey)](https://developer.apple.com/documentation/xcode/build-settings-reference)

## Current release

| Item | Value |
|---|---|
| SDK version | `3.0.0` |
| CocoaPods name | `NosmaiCameraSDK` |
| Minimum iOS version | iOS 15.0 |
| Supported architecture | `arm64` device |
| Binary | Dynamic `nosmai.framework` |

Version 3.0.0 supports physical ARM64 iPhone and iPad devices. The release does
not include an iOS Simulator slice.

## Features

- Real-time GPU camera preview
- Built-in beauty, makeup, face reshape, and color adjustment
- Face-tracked AR and 3D `.nosmai` effects
- Local and cloud filter catalogs
- Image, color, blur, video, and authored background replacement
- Filtered video recording
- Processed pixel buffer output for streaming integrations

## Installation

Add the SDK to your `Podfile`:

```ruby
platform :ios, '15.0'

target 'YourApp' do
  use_frameworks!
  pod 'NosmaiCameraSDK', '3.0.0'
end
```

Install the pod:

```bash
pod install --repo-update
```

Open the generated `.xcworkspace`, not the `.xcodeproj`.

For a manual installation, download `nosmai.framework.zip` and `SHA256SUMS`
from [GitHub Releases](../../releases/latest), verify the archive, unzip it, and
embed `nosmai.framework` in the application target with `Embed & Sign`.

```bash
shasum -a 256 -c SHA256SUMS
```

## App permissions

Add only the usage descriptions required by your application to `Info.plist`:

```xml
<key>NSCameraUsageDescription</key>
<string>Camera access is required for real-time effects.</string>

<!-- Required when recording audio. -->
<key>NSMicrophoneUsageDescription</key>
<string>Microphone access is required for video recording.</string>

<!-- Required when selecting an image or video from Photos. -->
<key>NSPhotoLibraryUsageDescription</key>
<string>Photo access is required to select media.</string>

<!-- Required when saving captures directly to Photos. -->
<key>NSPhotoLibraryAddUsageDescription</key>
<string>Photo access is required to save captures.</string>
```

Request camera, microphone, and Photos authorization from the application before
using the related capability.

## Basic camera setup

Import the framework in Objective-C:

```objc
#import <nosmai/Nosmai.h>
```

Swift applications can expose the same import through their Objective-C bridging
header.

Initialize the SDK once, attach a preview view, and start capture:

```objc
NosmaiCore *core = [NosmaiCore shared];

[core initializeWithAPIKey:NOSMAI_LICENSE_KEY
                completion:^(BOOL success, NSError *error) {
    if (!success) {
        // Show a user-friendly license error.
        return;
    }

    [core.camera attachToView:self.previewView];
    [core.camera startCapture];
}];
```

A valid Nosmai license key is required. Do not print the key, store it in plain
text, or send it to analytics and crash reporting services.

## Apply a `.nosmai` package

Pass the local path of a bundled or downloaded package to the asynchronous apply
method. The SDK reads the package type and routes it to the correct render slot.

```objc
[core.effects applyEffect:localNosmaiPath
               completion:^(BOOL success, NSError *error) {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (success) {
            // Update selected UI state.
        } else {
            // Show error.localizedDescription.
        }
    });
}];
```

Supported package types are:

| Manifest type | Purpose | Behavior |
|---|---|---|
| `filter` | Color grade or full-frame filter | Replaces the current color filter |
| `effect` | Face-tracked AR or 3D effect | Replaces the current AR slot |
| `beauty_effect` | Packaged face-mesh makeup or beauty | Shares the AR slot with `effect` |
| `background` | Authored background replacement | Replaces the current background package |

## Remove packages

Use the scoped method when only one render slot should be removed:

```objc
[core.effects clearFilter];
[core.effects clearAREffect];
[core.effects clearBackgroundSegmentation];
```

`clearBackgroundSegmentation` removes a manual blur, color, image, or video
background. If a `NosmaiFilterInfo` object is available for a `.nosmai`
background package, use `[[NosmaiSDK sharedInstance] removeEffectInfo:info]` to
let the SDK route the removal by package type.

## Built-in beauty and makeup

Built-in beauty filters use direct APIs and are separate from `.nosmai`
packages:

```objc
[core.effects applySkinSmoothing:0.5f];
[core.effects applySkinWhitening:0.3f];
[core.effects applyLipstickWithStyle:NosmaiLipstickStyleMatte colorIndex:0];

// Removes makeup, face reshape, and eye color effects.
[core.effects removeAllBeautyEffects];
```

Intensity values are generally normalized from `0.0f` to `1.0f`. Follow the
public header documentation for APIs with a different range.

## Background replacement

The SDK supports manual backgrounds and authored background packages:

```objc
NosmaiBackgroundSegmentationConfig *background =
    [NosmaiBackgroundSegmentationConfig new];
background.mode = NosmaiBackgroundSegmentationModeColor;
background.replacementColor = UIColor.systemBlueColor;
[core.effects setBackgroundSegmentation:background];

[core.effects clearBackgroundSegmentation];
```

Call UI-related methods on the main thread and update controls from completion
callbacks or pipeline-state notifications.

## Filter coexistence rules

The SDK maintains independent slots for color filters, AR effects, and
background packages.

| Combination | Result |
|---|---|
| `filter` with `effect` | Both can remain active |
| `filter` with `beauty_effect` | Both can remain active |
| `filter` with `background` | Both can remain active |
| `background` with an overlay-only `effect` | Both can remain active |
| `effect` with `beauty_effect` | The latest package replaces the AR slot |
| Built-in beauty or makeup with `effect` | The latest mode clears the other mode |
| Built-in beauty or makeup with `beauty_effect` | The latest mode clears the other mode |

An AR effect that owns its background can replace other background content.

## Observe active state

Use `NosmaiPipelineStateDidChangeNotification` or the effects delegate to keep
selected buttons and sheets synchronized with the actual SDK state.

```objc
[[NSNotificationCenter defaultCenter]
    addObserver:self
       selector:@selector(pipelineStateDidChange:)
           name:NosmaiPipelineStateDidChangeNotification
         object:nil];

- (void)pipelineStateDidChange:(NSNotification *)notification {
    NosmaiPipelineState *state =
        notification.userInfo[NosmaiPipelineStateUserInfoKey];

    // Read state.activeFilterPath, state.activeEffectPath,
    // state.activeEffectType, and state.activeBackgroundPackagePath.
}
```

Remove observers when their UI owner is released. Finalize loading and selected
state from apply callbacks or the pipeline listener. Do not assume that an
asynchronous apply succeeded immediately after calling it.

## Cloud packages

Cloud catalog version `2.0.0` is the API schema version and is separate from the
iOS SDK version.

```objc
NosmaiCloudFilterRequestOptions *options =
    [NosmaiCloudFilterRequestOptions defaultOptions];
options.filterType = @"effect";

[core.effects getCloudFiltersWithOptions:options
                              completion:^(NSArray<NSDictionary *> *filters,
                                           NosmaiCloudFilterPaginationInfo *pagination,
                                           NSError *error) {
    // Render the catalog or show a retry state.
}];
```

Download a selected item, show progress, and apply only the returned local path:

```objc
[core.effects downloadCloudFilter:filterId
                          progress:^(float progress) {
    // Update download progress.
} completion:^(BOOL success, NSString *localPath, NSError *error) {
    if (success && localPath.length > 0) {
        [core.effects applyEffect:localPath completion:nil];
    }
}];
```

Valid cloud types are `filter`, `effect`, `beauty_effect`, and `background`.
Avoid network work on the main thread and prevent duplicate downloads for the
same item.

## Recording and processed frames

Start recording after the preview pipeline is ready and use an application-
writable output URL:

```objc
[core.camera startRecordingToURL:outputURL
                      completion:^(NSError *error) {
    // Update recording UI.
}];

[core.camera stopRecordingWithCompletion:^(NSURL *fileURL, NSError *error) {
    // Save, preview, or share the completed recording.
}];
```

Processed pixel buffer output is available for authorized streaming and custom
render integrations. Disable callbacks when the consumer is no longer active.

## Lifecycle

Stop processing when the camera owner is no longer active:

```objc
[core.camera stopCapture];
[core.camera detachFromView];
[core cleanup];
```

Release application observers, frame callbacks, and recording state before the
owning screen is destroyed. Initialize a new SDK session only when required by
the application lifecycle.

## Troubleshooting

### App does not run in the Simulator

Version 3.0.0 contains an ARM64 device framework. Run the app on a physical
iPhone or iPad.

### `No such module` or missing header

Run `pod install --repo-update`, open the `.xcworkspace`, and confirm the
application target uses iOS 15.0 or later. For manual installation, verify that
`nosmai.framework` is configured as `Embed & Sign`.

### A filter does not appear

Confirm that:

- the SDK is initialized with a valid license
- camera permission is granted
- processing has started
- the `.nosmai` file exists at the supplied local path
- the asynchronous completion did not return an error

### Cloud catalog is empty

Confirm network availability, license access, requested `filterType`, and cloud
schema version. Show a retry state for temporary network failures.

### App Store reports a missing privacy manifest

Version 3.0.0 embeds `PrivacyInfo.xcprivacy` inside `nosmai.framework`. Confirm
the final archived app contains
`Frameworks/nosmai.framework/PrivacyInfo.xcprivacy`.

## Production notes

- Internal diagnostic logging is disabled in production builds.
- Production status output is limited to initialization and license results.
- License keys and tokens are not written to SDK logs.
- Each release includes `SHA256SUMS` for artifact verification.
- The host application signs the embedded framework during archive and export.

## License and support

Nosmai Camera SDK is proprietary commercial software. Public access to this
repository or its release assets does not grant permission to use, modify,
reverse engineer, redistribute, sublicense, or resell the SDK. Runtime use
requires written authorization from Nosmai and a valid Nosmai license key.

See [LICENSE](LICENSE) for the proprietary notice. Third-party components remain
subject to their own license terms. Contact Nosmai for licensing and support.
