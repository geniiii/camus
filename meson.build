project('camus', 'c',
	version: '1.0.0',
	license: ['MIT'],
	meson_version: '>= 0.50.0',
	default_options: ['warning_level=3', 'buildtype=debugoptimized', 'c_std=c99']
)

inc = [include_directories('include')]

project_sources = []
project_headers = []

subdir('include')
subdir('src')

cc = meson.get_compiler('c')

camus_bin = executable('camus',
  main_source,
  include_directories: inc,
  extra_files: project_headers)