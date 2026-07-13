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
badd +586 ~/Documents/rendering-reaktor/src2/App.cpp
badd +16 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
badd +14 ~/Documents/rendering-reaktor/src2/App.hpp
badd +58 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +521 ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
badd +1 ~/Documents/rendering-reaktor/assets/images/skybox.png
badd +17 ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
badd +36 ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
badd +16 ~/Documents/rendering-reaktor/src2/main.cpp
badd +3935 ~/cacheCPM/raylib/c846/src/rmodels.c
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/cacheCPM/raylib/c846/src/rmodels.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd w
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
exe '1resize ' . ((&lines * 48 + 31) / 62)
exe '2resize ' . ((&lines * 10 + 31) / 62)
exe 'vert 2resize ' . ((&columns * 181 + 120) / 240)
exe '3resize ' . ((&lines * 10 + 31) / 62)
exe 'vert 3resize ' . ((&columns * 58 + 120) / 240)
argglobal
balt ~/Documents/rendering-reaktor/src2/App.cpp
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
let s:l = 3934 - ((22 * winheight(0) + 24) / 48)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 3934
normal! 018|
wincmd w
argglobal
enew
file [dap-repl-153]
balt ~/Documents/rendering-reaktor/src2/App.hpp
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
wincmd w
argglobal
if bufexists(fnamemodify("[dap-terminal]\ LLDB:\ Launch", ":p")) | buffer [dap-terminal]\ LLDB:\ Launch | else | edit [dap-terminal]\ LLDB:\ Launch | endif
if &buftype ==# 'terminal'
  silent file [dap-terminal]\ LLDB:\ Launch
endif
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 1 - ((0 * winheight(0) + 5) / 10)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe '1resize ' . ((&lines * 48 + 31) / 62)
exe '2resize ' . ((&lines * 10 + 31) / 62)
exe 'vert 2resize ' . ((&columns * 181 + 120) / 240)
exe '3resize ' . ((&lines * 10 + 31) / 62)
exe 'vert 3resize ' . ((&columns * 58 + 120) / 240)
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
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
let s:l = 521 - ((55 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 521
normal! 019|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
argglobal
balt ~/cacheCPM/raylib/c846/src/rmodels.c
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
let s:l = 1 - ((0 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 019|
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/App.hpp
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
let s:l = 17 - ((16 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 17
normal! 017|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/main.cpp
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
let s:l = 110 - ((15 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 110
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
