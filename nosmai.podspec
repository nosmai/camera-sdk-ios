Pod::Spec.new do |s|

  s.name             = 'nosmai'
  s.version          = '1.0.0'
  s.summary          = 'A powerful SDK for real-time face filters and beauty effects.'
  s.homepage         = 'https://github.com/nosmai/camera-sdk-ios'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'Nadeem Ahmed' => 'nadeem.ahmed@vativeapps.com' }
  s.platform         = :ios, '13.0'

  s.description      = <<-DESC
                       The Nosmai SDK for iOS empowers your application with a lightweight and powerful engine for real-time camera effects.
                       It includes stunning face filters, a complete beauty suite, and cloud-based filter delivery.
                       DESC


  s.source           = { :git => 'https://github.com/nosmai/camera-sdk-ios.git', :tag => s.version.to_s }
  
  s.ios.vendored_frameworks = 'nosmai.framework'
  
  s.source_files = 'nosmai.framework/Headers/*.h'
  s.public_header_files = 'nosmai.framework/Headers/*.h'
  s.frameworks = 'OpenGLES', 'UIKit', 'QuartzCore', 'CoreVideo', 'CoreGraphics', 'AVFoundation', 'CoreMedia', 'Metal', 'CoreML', 'Security'
  s.libraries = 'z'
  
end