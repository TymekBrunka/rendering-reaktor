let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Documents/kody/rendering-reaktor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +4 ~/Documents/kody/rendering-reaktor/src/rendering/CubeMap.hpp
badd +129 /usr/include/GL/glew.h
badd +3 ~/Documents/kody/rendering-reaktor/src/rendering/FrameBuffer.hpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/Texture2d.hpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/Program.hpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/Shader.hpp
badd +31 ~/Documents/kody/rendering-reaktor/src/rendering/imgui/imgui_impl_opengl3.h
badd +3 ~/Documents/kody/rendering-reaktor/src/rendering/VertexBuffer.hpp
badd +1 ~/Documents/kody/rendering-reaktor/src/rendering/generators.h
badd +128 ~/Documents/kody/rendering-reaktor/src/main.cpp
badd +58 ~/Documents/kody/rendering-reaktor/src/rendering/CMakeLists.txt
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/BufferBase.hpp
badd +28 ~/Documents/kody/rendering-reaktor/CMakeLists.txt
badd +7 ~/Documents/kody/rendering-reaktor/vcpkg.json
badd +24 ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/bytePacker.cpp
badd +1 ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
badd +13 ~/Documents/kody/rendering-reaktor/src/rendering/imgui/CMakeLists.txt
badd +961 ~/Documents/kody/rendering-reaktor/src/rendering/glad/src/glad.c
badd +1955 ~/Documents/kody/rendering-reaktor/src/rendering/glad/include/glad/glad.h
badd +1 ~/Documents/kody/rendering-reaktor/src/rendering/IndexBuffer.hpp
badd +8 ~/Documents/kody/rendering-reaktor/src/rendering/rr.hpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/rr.cpp
badd +10 src/EditorClasses/EditorActionsPanel.hpp
badd +1 src/EditorClasses/EditorActionsPanel.cpp
badd +1 ~/Documents/kody/rendering-reaktor/build/skybox.frag.glsl.cpp
badd +1 ~/Documents/kody/rendering-reaktor/build/skybox.vertex.glsl.cpp
badd +10 ~/Documents/kody/rendering-reaktor/src/shaders/skybox.vertex.glsl
badd +10 ~/Documents/kody/rendering-reaktor/src/shaders/skybox.frag.glsl
argglobal
%argdel
edit ~/Documents/kody/rendering-reaktor/src/main.cpp
argglobal
balt ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/bytePacker.cpp
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
let s:l = 128 - ((24 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 128
normal! 032|
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
