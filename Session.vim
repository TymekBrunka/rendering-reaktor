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
badd +17 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.cpp
badd +19 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/WorkerThreads.hpp
badd +182 src/main.cpp
badd +10 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.hpp
badd +3732 ~/Documents/rendering/rendering-reaktor/src/rendering/imgui/imgui.h
badd +21 ~/Documents/rendering/rendering-reaktor/CMakeLists.txt
badd +39 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.hpp
badd +6 ~/Documents/rendering/rendering-reaktor/src/EditorClasses/MeshManager.cpp
badd +17 src/utils/Logger.hpp
argglobal
%argdel
edit ~/Documents/rendering/rendering-reaktor/src/EditorClasses/EditorActionsPanel.cpp
argglobal
balt src/utils/Logger.hpp
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
let s:l = 20 - ((19 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 20
normal! 045|
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
