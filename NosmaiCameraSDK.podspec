Pod::Spec.new do |s|
  s.name             = 'NosmaiCameraSDK'
  s.version          = '3.0.0'
  s.summary          = 'Real-time camera effects, beauty, AR, backgrounds, and recording for iOS.'
  s.description      = <<-DESC
    Nosmai Camera SDK is a proprietary iOS framework for real-time camera
    effects, built-in beauty and makeup, face reshape, background replacement,
    recording, processed frame output, and local or cloud .nosmai packages.
    A valid Nosmai license key and written authorization are required.
  DESC
  s.homepage         = 'https://github.com/nosmai/camera-sdk-ios'
  s.license          = { :type => 'Proprietary', :text => 'See LICENSE file' }
  s.author           = { 'Nosmai' => 'admin@nosmai.com' }
  s.platform         = :ios, '15.0'
  s.swift_version    = '5.0'

  s.source           = {
    :http => 'https://github.com/nosmai/camera-sdk-ios/releases/download/v3.0.0/nosmai.framework.zip'
  }

  s.vendored_frameworks = 'nosmai.framework'
  s.requires_arc = true
  s.frameworks = 'OpenGLES', 'UIKit', 'QuartzCore', 'CoreVideo',
                 'CoreGraphics', 'CoreImage', 'AVFoundation', 'CoreMedia',
                 'Metal', 'MetalKit', 'CoreML', 'Accelerate', 'Security'
  s.libraries = 'z', 'c++'

  s.pod_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64 x86_64'
  }
  s.user_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64 x86_64'
  }
end
