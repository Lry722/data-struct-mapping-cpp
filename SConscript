import os

Import('env')
env.Append(CPPPATH=[Dir('.').abspath + '/include'])

sources = Glob('src/*.cpp')

library_name = 'libdsmap'
library = env.Library(library_name, sources)

Return('library')
