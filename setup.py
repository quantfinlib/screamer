# setup.py

from setuptools import setup
from setuptools import Extension
from setuptools.command.build_ext import build_ext
import sys
import pybind11
import os

class CMakeExtension(Extension):
    def __init__(self, name):
        super().__init__(name, sources=[])

class CMakeBuild(build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)
    
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cfg = 'Release'

        cmake_args = [
            f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}',
            f'-DCMAKE_BUILD_TYPE={cfg}',
            f'-DPYTHON_EXECUTABLE={sys.executable}',
        ]

        build_args = []

        if sys.platform.startswith('win'):
            cmake_args += ['-A', 'x64']
            build_args += ['--config', cfg]

        build_temp = self.build_temp
        if not os.path.exists(build_temp):
            os.makedirs(build_temp)

        os.chdir(build_temp)
        self.spawn(['cmake', '..'] + cmake_args)
        self.spawn(['cmake', '--build', '.', '--target', 'screamer_bindings'] + build_args)
        os.chdir('..')

setup(
    name='screamer',
    version='0.1.0',
    author='Your Name',
    author_email='your.email@example.com',
    description='Your package description',
    long_description='Long description of your package',
    packages=['screamer'],
    ext_modules=[CMakeExtension('screamer_bindings')],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    python_requires='>=3.7',
)
