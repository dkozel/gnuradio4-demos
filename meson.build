project('gnuradio4-demos', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=2',
  ]
)

fmt_dep = dependency('fmt', required: true)
gnuradio4_dep = dependency('gnuradio4', required: true)

compile_args = ['-stdlib=libc++']
link_args = ['-stdlib=libc++', '-lc++abi']
demo_deps = [fmt_dep, gnuradio4_dep]

simple_math_exe = executable('simple_math_flowgraph',
  'demos/simple_math_flowgraph.cpp',
  dependencies: demo_deps,
  cpp_args: compile_args,
  link_args: link_args,
  install: false
)

