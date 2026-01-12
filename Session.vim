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
badd +27 src/EditorClasses/MeshManager.hpp
badd +225 build/vcpkg_installed/x64-linux/include/nfd.h
badd +8 ~/Documents/kody/rendering-reaktor/src/EditorClasses/MeshManager.cpp
badd +4 ~/Documents/kody/rendering-reaktor/src/EditorClasses/EditorActionsPanel.hpp
badd +1 ~/Documents/kody/rendering-reaktor/src/EditorClasses/EditorActionsPanel.cpp
badd +317 src/main.cpp
badd +4 ~/Documents/kody/rendering-reaktor/vcpkg.json
badd +36 ~/Documents/kody/rendering-reaktor/CMakeLists.txt
badd +1 ~/Documents/kody/rendering-reaktor/build/vcpkg_installed/x64-linux/share/portable-file-dialogs/copyright
badd +1 ~/Documents/kody/rendering-reaktor/build/vcpkg_installed/x64-linux/share/portable-file-dialogs/vcpkg_abi_info.txt
badd +1 ~/Documents/kody/rendering-reaktor/src/tinyobjloader/tiny_obj_loader.cc
badd +1 ~/Documents/kody/rendering-reaktor/src/tinyobjloader/tiny_obj_loader.h
badd +373 ~/Documents/kody/rendering-reaktor/src/pfd/portable-file-dialogs.h
badd +26 ~/Documents/kody/rendering-reaktor/src/rendering/BufferBase.cpp
badd +1 ~/Documents/kody/rendering-reaktor/src/pfd/pfd.cpp
badd +1930 ~/Documents/kody/rendering-reaktor/src/rendering/glad/include/glad/glad.h
badd +5 ~/Documents/kody/rendering-reaktor/src/EditorClasses/logger.hpp
badd +14 ~/Documents/kody/rendering-reaktor/src/EditorClasses/logger.cpp
badd +65 /usr/include/c++/15.2.1/iostream
badd +145 /usr/include/c++/15.2.1/iosfwd
badd +92 /usr/include/c++/15.2.1/bits/ostream.h
argglobal
%argdel
edit src/main.cpp
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
balt ~/Documents/kody/rendering-reaktor/src/EditorClasses/logger.cpp
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
let s:l = 317 - ((3 * winheight(0) + 27) / 54)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 317
normal! 040|
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
