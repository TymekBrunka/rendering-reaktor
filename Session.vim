let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Documents/rendering-reaktor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
set shortmess+=aoO
badd +1 ~/Documents/bettyergl/deps/ext.cmake
badd +3 ~/Documents/bettyergl/CMakeLists.txt
badd +22 ~/Documents/bettyergl/deps/CPM.cmake
badd +17 ~/Documents/bettyergl/src/bettergl/CMakeLists.txt
badd +110 ~/Documents/bettyergl/src/main.cpp
badd +134 ~/Documents/bettyergl/src/bettergl/Debugging.cpp
badd +36 ~/Documents/bettyergl/src/bettergl/include/bettergl/Assets.hpp
badd +30 ~/Documents/bettyergl/src/bettergl/Assets.cpp
badd +36 ~/Documents/bettyergl/src/bettergl/assetPackers/bytePacker.cpp
badd +41 ~/Documents/bettyergl/src/bettergl/assetPackers/imagePacker.cpp
badd +8 ~/Documents/bettyergl/gex.ping.hpp
badd +6 ~/Documents/bettyergl/gexignored.hpp
badd +6 ~/Documents/bettyergl/src/shaders/vertex.glsl
badd +322 ~/Documents/rendering-reaktor/src/main.cpp
badd +3 ~/Documents/rendering-reaktor/src/shaders/composite.vertex.glsl
badd +4 ~/Documents/rendering-reaktor/src/shaders/composite.frag.glsl
badd +11 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +6 ~/Documents/rendering-reaktor/src/input_handling.cpp
badd +4 ~/Documents/rendering-reaktor/src/shaders/skybox.frag.glsl
badd +21 ~/Documents/rendering-reaktor/src/rendering/FrameBuffer.cpp
badd +10 ~/Documents/rendering-reaktor/.gitignore
badd +37 ~/Documents/rendering-reaktor/src/rendering/CMakeLists.txt
argglobal
%argdel
edit ~/Documents/rendering-reaktor/.gitignore
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
balt ~/Documents/rendering-reaktor/src/rendering/CMakeLists.txt
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
let s:l = 10 - ((9 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 10
normal! 0
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
