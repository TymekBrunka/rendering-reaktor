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
badd +26 ~/Documents/kody/rendering-reaktor/deps/ext.cmake
badd +49 ~/Documents/kody/rendering-reaktor/deps/zipconf.h
badd +4 ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/config.h
badd +1 ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/zipconf.h
badd +159 ~/.cache/CPM/xlsxio/f803/CMakeLists.txt
badd +364 ~/Documents/kody/rendering-reaktor/src/main.cpp
badd +40 ~/Documents/kody/rendering-reaktor/CMakeLists.txt
badd +1 ~/Documents/kody/rendering-reaktor/build/composite.vertex.glsl.cpp
badd +5 ~/Documents/kody/rendering-reaktor/build/composite.vertex.glsl.hpp
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/kody/rendering-reaktor/src/main.cpp
argglobal
balt ~/Documents/kody/rendering-reaktor/build/composite.vertex.glsl.hpp
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal nofoldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 355 - ((34 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 355
normal! 0
tabnext
edit ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/zipconf.h
argglobal
balt ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/config.h
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
let s:l = 1 - ((0 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit ~/Documents/kody/rendering-reaktor/deps/ext.cmake
argglobal
balt ~/.cache/CPM/xlsxio/f803/CMakeLists.txt
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
let s:l = 40 - ((16 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 40
normal! 0
tabnext 3
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
