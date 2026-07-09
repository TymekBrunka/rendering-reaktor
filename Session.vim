let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
doautoall SessionLoadPre
silent only
silent tabonly
cd ~/Documents/kody/rendering-reaktor
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
set shortmess+=aoO
badd +98 ~/Documents/kody/rendering-reaktor/src2/BuildTools/bytepack.c
badd +5 ~/Documents/kody/rendering-reaktor/build/bytepack.c
badd +5 ~/Documents/kody/rendering-reaktor/build/bytepack.h
badd +4 ~/Documents/kody/rendering-reaktor/assets/data.txt
badd +77 ~/Documents/kody/rendering-reaktor/src2/BuildTools/imgpack.c
badd +7988 ~/Documents/kody/rendering-reaktor/src2/BuildTools/stb_image.h
badd +97 ~/Documents/kody/rendering-reaktor/CMakeLists.txt
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/kody/rendering-reaktor/src2/BuildTools/imgpack.c
argglobal
balt ~/Documents/kody/rendering-reaktor/src2/BuildTools/stb_image.h
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
let s:l = 77 - ((25 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 77
normal! 05|
tabnext
edit ~/Documents/kody/rendering-reaktor/src2/BuildTools/bytepack.c
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
let s:l = 98 - ((54 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 98
normal! 014|
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
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
