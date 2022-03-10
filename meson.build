project('cnm-remastered', 'c')
glfw_dep = dependency('glfw3')
cglm_dep = dependency('cglm')
gl_dep = [dependency('gl'), dependency('x11'), dependency('xrandr'), dependency('xi')]
ext_dep = dependency('gthread-2.0')
executable('cnm', 'main.c', dependencies : [glfw_dep, cglm_dep, gl_dep, ext_dep])
