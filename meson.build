project('cnm-remastered', 'c')
deps_dir = include_directories('deps/include')
glfw_dep = dependency('glfw3')
cglm_dep = dependency('cglm')
gl_dep = [dependency('gl'), dependency('x11'), dependency('xrandr'), dependency('xi')]
thread_dep = dependency('threads')

sources = [
    'src/main.c',

    'src/util/util.c',
    'src/util/math.c',

    'src/gfx/shader.c',
    'src/gfx/model.c',
    'src/gfx/texture.c',
    'src/gfx/camera.c',
    'src/gfx/lineRenderer.c'
]
sources_deps = [
    'deps/src/glad.c'
]

run_target('run', command : 'scripts/run.sh')

executable(
    'cnm',
    sources, sources_deps,
    dependencies : [glfw_dep, cglm_dep, gl_dep, thread_dep],
    include_directories : [deps_dir]
)
