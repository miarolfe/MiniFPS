project('mini-fps', 'cpp', version: '0.0.3', default_options: ['cpp_std=c++11'])

sources = files(
  'src/main.cpp',
  'src/Camera.cpp',
  'src/Level.cpp',
  'src/Renderer.cpp',
  'src/Player.cpp',
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

run_command('mkdir', 'bin/mini-fps.app')
run_command('mkdir', 'bin/mini-fps.app/Contents')
run_command('mkdir', 'bin/mini-fps.app/Contents/Frameworks')
run_command('mkdir', 'bin/mini-fps.app/Contents/Resources')
run_command('mkdir', 'bin/mini-fps.app/Contents/MacOS')

include_directories('lib')

sdl2_dep = dependency('SDL2', required: true, static: true, method: 'extraframework')
sdl2_image_dep = dependency('SDL2_image', required: true, static: true, method: 'extraframework')
include_dirs = include_directories('include', 'lib', '/Library/Frameworks/')

install_subdir('lib/SDL2.framework', install_dir: 'Contents/Frameworks')
install_subdir('lib/SDL2_image.framework', install_dir: 'Contents/Frameworks')
install_subdir('assets/levels', install_dir: 'Contents/Resources')
install_subdir('assets/sprites', install_dir: 'Contents/Resources')
install_data('assets/settings.json', install_dir: 'Contents/Resources')
install_data('assets/testIcon.icns', install_dir: 'Contents/Resources')
install_data('assets/Info.plist', install_dir: 'Contents')
install_data('assets/mini-fps.sh', install_dir: 'Contents/MacOS')

mini_fps = executable('mini-fps', sources + headers,
                      include_directories: include_dirs,
                      dependencies: [sdl2_dep, sdl2_image_dep],
                      install : true,
                      install_dir: 'Contents/MacOS',
                      cpp_args: '-Flib/'
                      )

run_command('install_name_tool', '-change', '@rpath/SDL2.framework/Versions/A/SDL2', '@executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2', '${MESON_INSTALL_PREFIX}/Contents/MacOS/mini-fps')
run_command('install_name_tool', '-change', '@rpath/SDL2_image.framework/Versions/A/SDL2_image', '@executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image', '${MESON_INSTALL_PREFIX}/Contents/MacOS/mini-fps')



