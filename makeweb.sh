emcc -o index.html src/*.cpp \
    -Os -Wall -I../libs/raylib/raylib/include \
    -L/usr/lib64/raylib -s USE_GLFW=3 -s USE_WEBGL2=1 \
    -s ASYNCIFY \
    --shell-file ../libs/raylib/src/minshell.html \
    -DPLATFORM_WEB ../libs/raylib/raylib/libraylib.a \
    --preload-file assets
