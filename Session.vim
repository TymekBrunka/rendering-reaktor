let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Documents/rendering/rendering-reaktor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
set shortmess+=aoO
badd +18 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.cpp
badd +19 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/WorkerThreads.hpp
badd +303 src/main.cpp
badd +15 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.hpp
badd +3732 ~/Documents/rendering/rendering-reaktor/src/rendering/imgui/imgui.h
badd +50 ~/Documents/rendering/rendering-reaktor/CMakeLists.txt
badd +34 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.hpp
badd +187 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.cpp
badd +17 src/utils/Logger.hpp
badd +12 ~/Documents/rendering/rendering-reaktor/src/triangleVB.cpp
badd +64 ~/Documents/rendering/rendering-reaktor/src/triangle.cpp
badd +63 src/rendering/VertexArray.cpp
badd +30 src/rendering/VertexArray.hpp
badd +19 ~/Documents/rendering/rendering-reaktor/src/rendering/IndexBuffer.cpp
badd +27 ~/Documents/rendering/rendering-reaktor/src/rendering/VertexBuffer.hpp
badd +7 ~/Documents/rendering/rendering-reaktor/vcpkg.json
badd +667 ~/Documents/rendering/rendering-reaktor/build/vcpkg_installed/x64-mingw-dynamic/include/assimp/mesh.h
badd +72 ~/Documents/rendering/rendering-reaktor/build/vcpkg_installed/x64-mingw-dynamic/include/assimp/vector3.h
badd +3 ~/Documents/rendering/rendering-reaktor/src/rendering/BufferBase.hpp
badd +42 ~/Documents/rendering/rendering-reaktor/src/rendering/BufferBase.cpp
badd +11 ~/Documents/rendering/rendering-reaktor/src/rendering/CubeMap.cpp
badd +9 ~/Documents/rendering/rendering-reaktor/src/rendering/FrameBuffer.hpp
badd +6 ~/Documents/rendering/rendering-reaktor/src/rendering/IndexBuffer.hpp
badd +3 ~/Documents/rendering/rendering-reaktor/src/rendering/Program.hpp
badd +9 ~/Documents/rendering/rendering-reaktor/src/rendering/Program.cpp
badd +19 ~/Documents/rendering/rendering-reaktor/src/rendering/Texture2d.hpp
badd +43 ~/Documents/rendering/rendering-reaktor/src/rendering/Shader.cpp
badd +3 ~/Documents/rendering/rendering-reaktor/src/rendering/Shader.hpp
badd +13 ~/Documents/rendering/rendering-reaktor/src/shaders/model.vertex.glsl
badd +3 ~/Documents/rendering/rendering-reaktor/src/main.hpp
badd +2 ~/Documents/rendering/rendering-reaktor/build/model.vertex.glsl.cpp
badd +7 ~/Documents/rendering/rendering-reaktor/src/shaders/skybox.vertex.glsl
badd +33 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/Camera.cpp
badd +5 ~/Documents/rendering/rendering-reaktor/src/shaders/model.frag.glsl
badd +58 ~/Documents/rendering/rendering-reaktor/src/rendering/Texture2d.cpp
badd +55 ~/Documents/rendering/rendering-reaktor/src/rendering/FrameBuffer.cpp
badd +4 ~/Documents/rendering/rendering-reaktor/src/rendering/impl_buffer_move.hpp
badd +1 ~/Documents/rendering/rendering-reaktor/src/shaders/empty.glsl
badd +9 ~/Documents/rendering/rendering-reaktor/src/shaders/skybox.frag.glsl
badd +1 ~/Documents/rendering/rendering-reaktor/src/input_handling.cpp
badd +10 ~/Documents/rendering/rendering-reaktor/.gitignore
argglobal
%argdel
edit src/main.cpp
argglobal
balt ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.cpp
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 303 - ((36 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 303
normal! 024|
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
