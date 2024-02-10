# from SCons import Environment, Glob
import os

env = Environment(CXXFLAGS = '-std=c++20')

print(env['PLATFORM'])
if env['PLATFORM'] == 'win32':
    prefix = './build/'
    env['LIBSUFFIX']='.lib'
else:
    prefix = '/usr/local/'

headers = Glob('include/*.h')
library = SConscript('./SConscript', exports='env')

env.Install(prefix + 'lib', library)

os.makedirs(prefix + 'include/dsmap-cpp', exist_ok=True);
env.Install(prefix + 'include/dsmap-cpp', headers)

env.Alias('install', prefix)