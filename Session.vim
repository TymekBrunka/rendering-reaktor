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
badd +106 ~/Documents/kody/rendering-reaktor/src/main.cpp
badd +73 ~/Documents/kody/rendering-reaktor/src/input_handling.cpp
badd +14 ~/Documents/kody/rendering-reaktor/src/utils/Logger.hpp
badd +15 ~/Documents/kody/rendering-reaktor/src/rendering/Shader.cpp
badd +11 ~/Documents/kody/rendering-reaktor/src/rendering/FrameBuffer.hpp
badd +10 ~/Documents/kody/rendering-reaktor/src/rendering/Program.hpp
badd +12 ~/Documents/kody/rendering-reaktor/src/rendering/Shader.hpp
badd +10 ~/Documents/kody/rendering-reaktor/src/rendering/Texture2d.hpp
badd +26 ~/Documents/kody/rendering-reaktor/src/rendering/VertexArray.cpp
badd +56 ~/Documents/kody/rendering-reaktor/src/rendering/Program.cpp
badd +112 ../bettergl/src/bettergl/Program.cpp
badd +10 ~/Documents/kody/rendering-reaktor/src/shaders/skybox.frag.glsl
badd +13 ~/Documents/kody/rendering-reaktor/src/rendering/FrameBuffer.cpp
badd +13 ~/Documents/kody/rendering-reaktor/src/shaders/composite.frag.glsl
badd +33 ~/Documents/kody/rendering-reaktor/src/rendering/rr.cpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/stb_image.cpp
badd +89 ~/Documents/kody/rendering-reaktor/deps/ext.cmake
badd +4 ~/Documents/kody/rendering-reaktor/vcpkg.json
badd +1 ~/Documents/kody/rendering-reaktor/deps/CPM.cmake
badd +7 ~/Documents/kody/rendering-reaktor/CMakeLists.txt
badd +8 ~/Documents/kody/rendering-reaktor/src/rendering/CMakeLists.txt
badd +1 ~/Documents/kody/rendering-reaktor/deps/imgui/README.md
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/kody/rendering-reaktor/vcpkg.json
argglobal
balt ~/Documents/kody/rendering-reaktor/deps/ext.cmake
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
let s:l = 3 - ((2 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 3
normal! 05|
tabnext
edit ~/Documents/kody/rendering-reaktor/deps/ext.cmake
argglobal
balt ~/Documents/kody/rendering-reaktor/CMakeLists.txt
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
let s:l = 89 - ((51 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 89
normal! 0
tabnext 2
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
