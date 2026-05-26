inc = [include_directories('../includes/')]

llvm = dependency('llvm', version: '>=22')
deps = [llvm]

subdir('lexer')

libs = [lexer_lib]

executable(
	'crit2',
	'main.cpp',
	link_with: libs,
	include_directories: inc,
	install: true,
	dependencies: deps,
)

# use cmake to provide catch2 main
catch2 = dependency('Catch2', method: 'cmake', modules: ['Catch2::Catch2WithMain'])
test_source = files('../test/test_lexer.cpp')

test_executable = executable(
	'crit2_test',
	test_source,
	link_with: libs,
	include_directories: inc,
	dependencies: deps + [catch2],
	cpp_args: ['-DPROJ_SOURCE_DIR="' + meson.project_source_root() + '"'],
)

test('crit2', test_executable)
