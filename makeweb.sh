emcc -o index.html main.cpp \
    -Os -Wall -Iraylib/src \
    -Lpath_to_raylib_lib -s USE_GLFW=3 -s USE_WEBGL2=1 \
    -s ASYNCIFY \
    --shell-file raylib/src/minshell.html \
    -DPLATFORM_WEB raylib/src/libraylib.web.a \
    --preload-file resources
