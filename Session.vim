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
badd +115 ~/Documents/rendering-reaktor/src2/BuildTools/bytepack.c
badd +101 ~/Documents/rendering-reaktor/src2/BuildTools/imgpack.c
badd +383 ~/Documents/rendering-reaktor/src2/App.cpp
badd +256 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
badd +37 ~/Documents/rendering-reaktor/src2/App.hpp
badd +1 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +453 ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
badd +1 ~/Documents/rendering-reaktor/assets/images/skybox.png
badd +69 ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
badd +77 ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
badd +16 ~/Documents/rendering-reaktor/src2/main.cpp
badd +3934 ~/cacheCPM/raylib/c846/src/rmodels.c
badd +1 [dap-repl-153]
badd +48 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
badd +1 ~/Documents/rendering-reaktor/assets/scene.json
badd +94 dap-src://2/1000/
badd +94 dap-src://3/1018/
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
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
let s:l = 383 - ((29 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 383
normal! 020|
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
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
let s:l = 453 - ((10 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 453
normal! 065|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
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
let s:l = 254 - ((42 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 254
normal! 0
tabnext
edit ~/Documents/rendering-reaktor/src2/App.hpp
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
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
let s:l = 37 - ((29 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 37
normal! 0
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
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
let s:l = 69 - ((39 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 69
normal! 048|
tabnext
edit ~/Documents/rendering-reaktor/CMakeLists.txt
argglobal
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
let s:l = 53 - ((42 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 53
normal! 05|
tabnext 4
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
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
