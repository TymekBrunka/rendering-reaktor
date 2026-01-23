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
badd +32 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.cpp
badd +19 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/WorkerThreads.hpp
badd +331 src/main.cpp
badd +10 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.hpp
badd +3732 ~/Documents/rendering/rendering-reaktor/src/rendering/imgui/imgui.h
badd +21 ~/Documents/rendering/rendering-reaktor/CMakeLists.txt
badd +29 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.hpp
badd +135 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.cpp
badd +17 src/utils/Logger.hpp
badd +12 ~/Documents/rendering/rendering-reaktor/src/triangleVB.cpp
badd +64 ~/Documents/rendering/rendering-reaktor/src/triangle.cpp
badd +2 src/rendering/VertexArray.cpp
badd +13 src/rendering/VertexArray.hpp
badd +9 ~/Documents/rendering/rendering-reaktor/src/rendering/IndexBuffer.cpp
badd +13 ~/Documents/rendering/rendering-reaktor/src/rendering/VertexBuffer.hpp
argglobal
%argdel
edit src/main.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 120 + 120) / 240)
exe 'vert 2resize ' . ((&columns * 119 + 120) / 240)
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
let s:l = 331 - ((24 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 331
normal! 05|
wincmd w
argglobal
if bufexists(fnamemodify("src/rendering/VertexArray.cpp", ":p")) | buffer src/rendering/VertexArray.cpp | else | edit src/rendering/VertexArray.cpp | endif
if &buftype ==# 'terminal'
  silent file src/rendering/VertexArray.cpp
endif
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
let s:l = 2 - ((1 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 2
normal! 019|
wincmd w
exe 'vert 1resize ' . ((&columns * 120 + 120) / 240)
exe 'vert 2resize ' . ((&columns * 119 + 120) / 240)
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
