# =========================================================================
#     This configuration file was generated by
#     Bakefile 0.1.9 (http://bakefile.sourceforge.net)
#     Beware that all changes made to this file will be overwritten next
#     time you run Bakefile!
# =========================================================================


# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# 'install' program location
INSTALL = install

# Location where the package is installed by 'make install'
prefix = /usr/local

#
AR = ar

#
RANLIB = ranlib

# C compiler
CC = gcc

# C++ compiler
CXX = g++

# Standard flags for CC
CFLAGS =

# Standard flags for C++
CXXFLAGS =

# Standard preprocessor flags (common for CC and CXX)
CPPFLAGS =

# Standard linker flags
LDFLAGS =

# Some meaningful string that will be added to the names
# of the target files. Any string is possible.
# For example, if the value is _vc6,
# then the name of, say, the foundation kit debug dll
# will be like this: FoundationKit_vc6_d.dll.
# The default value is an empty string.
# Backward compatibility: []
OPTION_SELECTLIB =

# The type of libraries: [0,1]
OPTION_SHARED = 1

# The type of runtime libraries.
# "default" means the same value as OPTION_SHARED: [default,0,1]
OPTION_SHARED_RUNTIME = default

# The build configuration: [0,1]
OPTION_DEBUG = 1

# Make the all-in-1 library: [0,1]
OPTION_ALLIN1 = 0

# Compile ZLib: [0,1]
OPTION_ZLIB = 1

# Compile FoundationKit: [0,1]
OPTION_FOUNDATIONKIT = 1

# Depends on:
#     OPTION_FOUNDATIONKIT
#     OPTION_ZLIB
# Compile GraphicsKit: [0,1]
OPTION_GRAPHICSKIT = 0

# Depends on:
#     OPTION_GRAPHICSKIT
# Compile ApplicationKit: [0,1]
OPTION_APPLICATIONKIT = 0

# Depends on:
#     OPTION_APPLICATIONKIT
# Compile OpenGLKit: [0,1]
OPTION_OPENGLKIT = 0

# Depends on:
#     OPTION_FOUNDATIONKIT
# Compile NetworkKit: [0,1]
OPTION_NETWORKKIT = 0

# Depends on:
#     OPTION_NETWORKKIT
# Compile RemoteObjectKit: [0,1]
OPTION_REMOTEOBJECTKIT = 0

# Depends on:
#     OPTION_FOUNDATIONKIT
# Compile InternetKit: [0,1]
OPTION_INTERNETKIT = 0

# Depends on:
#     OPTION_APPLICATIONKIT
# Compile ImageFormats: [0,1]
OPTION_IMAGEFORMATS = 0

# Compile RegExx: [0,1]
OPTION_REGEXX = 0

