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
badd +94 dap-src://2/1000/
badd +115 ~/Documents/rendering-reaktor/src2/BuildTools/bytepack.c
badd +101 ~/Documents/rendering-reaktor/src2/BuildTools/imgpack.c
badd +604 ~/Documents/rendering-reaktor/src2/App.cpp
badd +43 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
badd +20 ~/Documents/rendering-reaktor/src2/App.hpp
badd +126 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +721 ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
badd +1 ~/Documents/rendering-reaktor/assets/images/skybox.png
badd +75 ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
badd +77 ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
badd +16 ~/Documents/rendering-reaktor/src2/main.cpp
badd +3934 ~/cacheCPM/raylib/c846/src/rmodels.c
badd +1 [dap-repl-153]
badd +13 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
badd +1 ~/Documents/rendering-reaktor/assets/scene.json
badd +94 dap-src://3/1018/
badd +89 ~/Documents/rendering-reaktor/deps/ext.cmake
badd +10 ~/Documents/rendering-reaktor/.gitignore
badd +1 ~/Documents/rendering-reaktor/assets/shaders/skinning.vs
badd +1 ~/Documents/rendering-reaktor/assets/shaders/skinning.fs
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/rendering-reaktor/src2/App.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
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
let s:l = 604 - ((44 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 604
normal! 045|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
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
let s:l = 44 - ((24 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 44
normal! 023|
tabnext
edit ~/Documents/rendering-reaktor/assets/shaders/skinning.fs
argglobal
balt ~/Documents/rendering-reaktor/assets/shaders/skinning.vs
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
let s:l = 1 - ((0 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
tabnext
edit ~/Documents/rendering-reaktor/src2/App.hpp
argglobal
balt ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
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
normal! 016|
tabnext
edit ~/Documents/rendering-reaktor/CMakeLists.txt
argglobal
balt ~/Documents/rendering-reaktor/deps/ext.cmake
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
let s:l = 126 - ((31 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 126
normal! 03|
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
