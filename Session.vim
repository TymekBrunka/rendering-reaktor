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
badd +76 ~/Documents/kody/rendering-reaktor/deps/ext.cmake
badd +49 ~/Documents/kody/rendering-reaktor/deps/zipconf.h
badd +4 ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/config.h
badd +1 ~/Documents/kody/rendering-reaktor/build/_deps/libzip-build/zipconf.h
badd +148 ~/.cache/CPM/xlsxio/f803/CMakeLists.txt
badd +4 ~/Documents/kody/rendering-reaktor/.gitignore
badd +1465 ~/Documents/kody/rendering-reaktor/build_windows/build.ninja
badd +43 ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
badd +1 term://~/Documents/kody/rendering-reaktor//7760:/usr/bin/zsh
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/kody/rendering-reaktor/deps/ext.cmake
argglobal
balt ~/Documents/kody/rendering-reaktor/.gitignore
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
let s:l = 76 - ((6 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 76
normal! 010|
tabnext
edit ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
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
let s:l = 43 - ((42 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 43
normal! 0
tabnext
edit ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
argglobal
balt ~/Documents/kody/rendering-reaktor/build_windows/build.ninja
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
let s:l = 1 - ((0 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
argglobal
if bufexists(fnamemodify("term://~/Documents/kody/rendering-reaktor//7760:/usr/bin/zsh", ":p")) | buffer term://~/Documents/kody/rendering-reaktor//7760:/usr/bin/zsh | else | edit term://~/Documents/kody/rendering-reaktor//7760:/usr/bin/zsh | endif
if &buftype ==# 'terminal'
  silent file term://~/Documents/kody/rendering-reaktor//7760:/usr/bin/zsh
endif
balt ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 55 - ((54 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 55
normal! 0
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
