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
badd +4 src/EditorClasses/MeshManager.hpp
badd +225 build/vcpkg_installed/x64-linux/include/nfd.h
badd +26 src/EditorClasses/MeshManager.cpp
badd +4 src/EditorClasses/EditorActionsPanel.hpp
badd +10 src/EditorClasses/EditorActionsPanel.cpp
badd +348 src/main.cpp
badd +4 vcpkg.json
badd +28 CMakeLists.txt
badd +1 build/vcpkg_installed/x64-linux/share/portable-file-dialogs/copyright
badd +1 build/vcpkg_installed/x64-linux/share/portable-file-dialogs/vcpkg_abi_info.txt
badd +1 src/tinyobjloader/tiny_obj_loader.cc
badd +1 src/tinyobjloader/tiny_obj_loader.h
badd +373 src/pfd/portable-file-dialogs.h
badd +2 src/rendering/BufferBase.cpp
badd +1 src/pfd/pfd.cpp
badd +1930 src/rendering/glad/include/glad/glad.h
badd +65 /usr/include/c++/15.2.1/iostream
badd +145 /usr/include/c++/15.2.1/iosfwd
badd +92 /usr/include/c++/15.2.1/bits/ostream.h
badd +18 src/rendering/CMakeLists.txt
badd +25 src/utils/Logger.hpp
badd +128 src/EditorClasses/WorkerThreads.hpp
badd +75 Session.vim
badd +1 src/rendering/BufferBase.hpp
badd +98 /usr/include/c++/15.2.1/condition_variable
argglobal
%argdel
edit src/EditorClasses/EditorActionsPanel.cpp
argglobal
balt src/EditorClasses/WorkerThreads.hpp
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
let s:l = 95 - ((51 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 95
normal! 027|
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
