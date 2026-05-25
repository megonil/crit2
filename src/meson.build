inc = [include_directories('../includes/')]

llvm = dependency('llvm', version: '>=22')
deps = [llvm]

subdir('lexer')

libs = [lexer_lib]

executable('crit2', 'main.cpp', link_with: libs, include_directories: inc, install: true)
