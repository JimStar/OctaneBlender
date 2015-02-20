# - Try to find OpenGLES
# Once done this will define
#
#  OPENGLES_FOUND           - system has OpenGLES and EGL
#  OPENGL_EGL_FOUND         - system has EGL
#  OPENGLES_INCLUDE_DIR     - the GLES include directory
#  OPENGLES_LIBRARY         - the GLES library
#  OPENGLES_EGL_INCLUDE_DIR - the EGL include directory
#  OPENGLES_EGL_LIBRARY     - the EGL library
#  OPENGLES_LIBRARIES       - all libraries needed for OpenGLES
#  OPENGLES_INCLUDES        - all includes needed for OpenGLES

# If OPENGLES_ROOT_DIR was defined in the environment, use it.
IF(NOT OPENGLES_ROOT_DIR AND NOT $ENV{OPENGLES_ROOT_DIR} STREQUAL "")
  SET(OPENGLES_ROOT_DIR $ENV{OPENGLES_ROOT_DIR})
ENDIF()

SET(_opengles_SEARCH_DIRS
  ${OPENGLES_ROOT_DIR}
  /usr/local
)

FIND_PATH(OPENGLES_INCLUDE_DIR
  NAMES
    GLES2/gl2.h
  HINTS
    ${_opengles_SEARCH_DIRS}
)

FIND_LIBRARY(OPENGLES_LIBRARY
  NAMES
    GLESv2
  PATHS
    ${_opengles_SEARCH_DIRS}
  PATH_SUFFIXES
    lib64 lib
)

FIND_PATH(OPENGLES_EGL_INCLUDE_DIR
  NAMES
    EGL/egl.h
  HINTS
    ${_opengles_SEARCH_DIRS}
)

FIND_LIBRARY(OPENGLES_EGL_LIBRARY
  NAMES
    EGL
  HINTS
    ${_opengles_SEARCH_DIRS}
  PATH_SUFFIXES
    lib64 lib
)

IF(OPENGLES_EGL_LIBRARY AND OPENGLES_EGL_INCLUDE_DIR)
  SET(OPENGL_EGL_FOUND "YES")
ELSE()
  SET(OPENGL_EGL_FOUND "NO")
ENDIF()

IF(OPENGLES_LIBRARY AND OPENGLES_INCLUDE_DIR AND
   OPENGLES_EGL_LIBRARY AND OPENGLES_EGL_INCLUDE_DIR)
  SET(OPENGLES_LIBRARIES ${OPENGLES_LIBRARY} ${OPENGLES_LIBRARIES}
                           ${OPENGLES_EGL_LIBRARY})
  SET(OPENGLES_INCLUDES ${OPENGLES_INCLUDE_DIR} ${OPENGLES_EGL_INCLUDE_DIR})
  SET(OPENGLES_FOUND "YES")
ELSE()
  SET(OPENGLES_FOUND "NO")
ENDIF()

MARK_AS_ADVANCED(
  OPENGLES_EGL_INCLUDE_DIR
  OPENGLES_EGL_LIBRARY
  OPENGLES_INCLUDE_DIR
)

UNSET(_opengles_SEARCH_DIRS)