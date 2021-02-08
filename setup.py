from distutils.core import setup, Extension

setup(name="CRT", version="1.0",
      ext_modules=[Extension("crt", ["pyvector3d.cpp", "source/vector3d.cpp"])])