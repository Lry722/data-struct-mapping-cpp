# from SCons import Environment, Glob
import os

env = Environment(CXXFLAGS = '-std=c++20')

headers = Glob('include/*.h')
library = SConscript('./SConscript', exports='env')

if env.Detect('windows'):
    prefix = 'C:\\Program Files\\'
else:
    prefix = '/usr/local/'

env.Install(prefix + 'lib', library)

os.makedirs(prefix + 'include/dsmap-cpp', exist_ok=True);
env.Install(prefix + 'include/dsmap-cpp', headers)

env.Alias('install', prefix)