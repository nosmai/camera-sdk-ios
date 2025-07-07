Pod::Spec.new do |s|
  s.name             = 'NosmaiCameraSDK'
  s.version          = '1.0.6'
  s.summary          = 'iOS SDK for applying real-time camera filters with live preview.'
  s.description      = <<-DESC
    Nosmai is a closed-source iOS SDK that allows developers to apply real-time visual filters on a live camera feed.
    It enables a seamless and interactive user experience through dynamic overlays and effects.

    To use the SDK, developers must register a project through the Nosmai portal and obtain a unique API key.
    The API key is used to initialize the camera view and enable filtering capabilities.
  DESC
  s.homepage = 'https://cocoapods.org/pods/NosmaiCameraSDK'
  s.license          = { :type => 'Proprietary', :text => 'See LICENSE file' }
  s.author           = { 'Nosmai' => 'admin@nosmai.com' }
  s.platform         = :ios, '14.0'

  s.source           = { :http => 'https://github.com/nosmai/camera-sdk-ios/releases/download/1.0.6/nosmai.framework.zip' }

  s.vendored_frameworks = 'nosmai.framework'

  s.static_framework = true
  s.frameworks = 'OpenGLES', 'UIKit', 'QuartzCore', 'CoreVideo', 'CoreGraphics', 'AVFoundation', 'CoreMedia', 'Metal', 'CoreML', 'Security'

  # Exclude simulator architectures (only build for devices)
  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64 x86_64' }
end
