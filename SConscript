import os

Import('env')
env.Append(CPPPATH=['include'])

sources = Glob('src/*.cpp')

library_name = 'libdsmap'
library = env.StaticLibrary(library_name, sources)

Return('library')
