project('mini-fps', 'cpp', version: '0.0.3', default_options: ['cpp_std=c++11'])

sources = files(
  'src/main.cpp',
  'src/Camera.cpp',
  'src/Level.cpp',
  'src/Renderer.cpp',
  'src/Player.cpp',
  'src/Settings.cpp',
  'src/Utilities.cpp',
)

headers = files(
  'src/Camera.h',
  'src/Color.h',
  'src/Level.h',
  'src/Player.h',
  'src/Renderer.h',
  'src/Settings.h',
  'src/Utilities.h',
)

if host_machine.system() == 'darwin'
    run_command('mkdir', 'bin/mini-fps.app')
    run_command('mkdir', 'bin/mini-fps.app/Contents')
    run_command('mkdir', 'bin/mini-fps.app/Contents/Frameworks')
    run_command('mkdir', 'bin/mini-fps.app/Contents/Resources')
    run_command('mkdir', 'bin/mini-fps.app/Contents/MacOS')

    include_directories('lib')

    sdl2_dep = dependency('SDL2', required: true, static: true, method: 'extraframework')
    sdl2_image_dep = dependency('SDL2_image', required: true, static: true, method: 'extraframework')
    sdl2_ttf_dep = dependency('SDL2_ttf', required: true, static: true, method: 'extraframework')
    include_dirs = include_directories('include', 'lib', '/Library/Frameworks/')

    install_subdir('lib/SDL2.framework', install_dir: 'Contents/Frameworks')
    install_subdir('lib/SDL2_image.framework', install_dir: 'Contents/Frameworks')
    install_subdir('lib/SDL2_ttf.framework', install_dir: 'Contents/Frameworks')
    install_subdir('assets/levels', install_dir: 'Contents/Resources')
    install_subdir('assets/sprites', install_dir: 'Contents/Resources')
    install_data('assets/settings.json', install_dir: 'Contents/Resources')
    install_data('assets/testIcon.icns', install_dir: 'Contents/Resources')
    install_data('assets/Info.plist', install_dir: 'Contents')
    install_data('assets/mini-fps.sh', install_dir: 'Contents/MacOS')

    mini_fps = executable('mini-fps', sources + headers,
                          include_directories: include_dirs,
                          dependencies: [sdl2_dep, sdl2_image_dep, sdl2_ttf_dep],
                          install : true,
                          install_dir: 'Contents/MacOS')
endif



