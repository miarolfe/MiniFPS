project('MiniFPS', 'cpp', version: '0.3.2', default_options: ['cpp_std=c++11'])

sources = files(
  'src/main.cpp',
  'src/AudioHandler.cpp',
  'src/Button.cpp',
  'src/Camera.cpp',
  'src/Color.cpp',
  'src/Enemy.cpp',
  'src/Font.cpp',
  'src/Game.cpp',
  'src/Level.cpp',
  'src/LogHandler.cpp',
  'src/Menu.cpp',
  'src/Renderer.cpp',
  'src/Player.cpp',
  'src/Settings.cpp',
  'src/Texture.cpp',
  'src/Utilities.cpp',
  'src/Vector.cpp'
)

headers = files(
  'src/AudioHandler.h',
  'src/Button.h',
  'src/Camera.h',
  'src/Color.h',
  'src/Enemy.h',
  'src/Font.h',
  'src/Game.h',
  'src/Level.h',
  'src/LogHandler.h',
  'src/Menu.h',
  'src/Player.h',
  'src/Renderer.h',
  'src/Settings.h',
  'src/Texture.h',
  'src/Utilities.h',
  'src/Vector.h'
)

if host_machine.system() == 'darwin'
    run_command('mkdir', 'bin/MiniFPS.app')
    run_command('mkdir', 'bin/MiniFPS.app/Contents')
    run_command('mkdir', 'bin/MiniFPS.app/Contents/Frameworks')
    run_command('mkdir', 'bin/MiniFPS.app/Contents/Resources')
    run_command('mkdir', 'bin/MiniFPS.app/Contents/MacOS')

    include_directories('lib')

    sdl2_dep = dependency('SDL2', required: true, static: true, method: 'extraframework')
    sdl2_image_dep = dependency('SDL2_image', required: true, static: true, method: 'extraframework')
    sdl2_mixer_dep = dependency('SDL2_mixer', required: true, static: true, method: 'extraframework')
    sdl2_ttf_dep = dependency('SDL2_ttf', required: true, static: true, method: 'extraframework')
    include_dirs = include_directories('include', 'lib', '/Library/Frameworks/')

    install_subdir('lib/SDL2.framework', install_dir: 'Contents/Frameworks')
    install_subdir('lib/SDL2_image.framework', install_dir: 'Contents/Frameworks')
    install_subdir('lib/SDL2_mixer.framework', install_dir: 'Contents/Frameworks')
    install_subdir('lib/SDL2_ttf.framework', install_dir: 'Contents/Frameworks')
    install_subdir('assets/audio', install_dir: 'Contents/Resources')
    install_subdir('assets/fonts', install_dir: 'Contents/Resources')
    install_subdir('assets/levels', install_dir: 'Contents/Resources')
    install_subdir('assets/sprites', install_dir: 'Contents/Resources')
    install_data('assets/settings.json', install_dir: 'Contents/Resources')
    install_data('assets/testIcon.icns', install_dir: 'Contents/Resources')
    install_data('assets/Info.plist', install_dir: 'Contents')
    install_data('assets/MiniFPS.sh', install_dir: 'Contents/MacOS')

    mini_fps = executable('MiniFPS', sources + headers,
                          include_directories: include_dirs,
                          dependencies: [sdl2_dep, sdl2_image_dep, sdl2_mixer_dep, sdl2_ttf_dep],
                          install : true,
                          install_dir: 'Contents/MacOS')
endif



