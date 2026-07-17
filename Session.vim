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
badd +95 ~/Documents/rendering-reaktor/src2/App.cpp
badd +175 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
badd +39 ~/Documents/rendering-reaktor/src2/App.hpp
badd +58 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +703 ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
badd +1 ~/Documents/rendering-reaktor/assets/images/skybox.png
badd +66 ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
badd +49 ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
badd +16 ~/Documents/rendering-reaktor/src2/main.cpp
badd +3934 ~/cacheCPM/raylib/c846/src/rmodels.c
badd +1 [dap-repl-153]
badd +39 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
badd +5 ~/Documents/rendering-reaktor/assets/scene.json
badd +94 dap-src://2/1000/
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/rendering-reaktor/src2/App.cpp
argglobal
balt dap-src://2/1000/
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
let s:l = 94 - ((28 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 94
normal! 034|
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
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
setlocal nofoldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 703 - ((56 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 703
normal! 086|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
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
exe 'vert 1resize ' . ((&columns * 119 + 120) / 240)
exe 'vert 2resize ' . ((&columns * 120 + 120) / 240)
argglobal
balt ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
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
let s:l = 48 - ((47 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 48
normal! 023|
wincmd w
argglobal
if bufexists(fnamemodify("~/Documents/rendering-reaktor/src2/App.hpp", ":p")) | buffer ~/Documents/rendering-reaktor/src2/App.hpp | else | edit ~/Documents/rendering-reaktor/src2/App.hpp | endif
if &buftype ==# 'terminal'
  silent file ~/Documents/rendering-reaktor/src2/App.hpp
endif
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
let s:l = 16 - ((1 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 03|
wincmd w
exe 'vert 1resize ' . ((&columns * 119 + 120) / 240)
exe 'vert 2resize ' . ((&columns * 120 + 120) / 240)
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
argglobal
balt ~/Documents/rendering-reaktor/src2/App.hpp
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
let s:l = 49 - ((36 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 49
normal! $
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
let s:l = 14 - ((13 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 14
normal! 052|
tabnext 2
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
