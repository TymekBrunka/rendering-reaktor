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
badd +1 \[dap-terminal]\ LLDB:\ Launch\ (args)
badd +111 ~/Documents/kody/rendering-reaktor/src/main.cpp
badd +14 ~/Documents/kody/rendering-reaktor/src/rendering/IndexBuffer.cpp
badd +12 src/rendering/IndexBuffer.hpp
badd +18 ~/Documents/kody/rendering-reaktor/src/rendering/BufferBase.hpp
badd +4 ~/Documents/kody/rendering-reaktor/.clang-format
badd +32 ~/Documents/kody/rendering-reaktor/src/rendering/BufferBase.cpp
badd +58 ~/Documents/kody/rendering-reaktor/src/rendering/CMakeLists.txt
badd +40 ~/Documents/kody/rendering-reaktor/src/rendering/generators.h
badd +21 ~/Documents/kody/rendering-reaktor/src/rendering/VertexBuffer.hpp
badd +2 ~/Documents/kody/rendering-reaktor/src/rendering/impl_buffer_move.hpp
badd +39 ~/Documents/kody/rendering-reaktor/src/rendering/FrameBuffer.cpp
badd +8 ~/Documents/kody/rendering-reaktor/src/rendering/FrameBuffer.hpp
badd +52 ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/bytePacker.cpp
badd +60 ~/Documents/kody/rendering-reaktor/src/rendering/assetPackers/imagePacker.cpp
badd +18 CMakeLists.txt
badd +1 DAP\ Scopes
badd +1 DAP\ Breakpoints
badd +1 DAP\ Stacks
badd +1 DAP\ Watches
badd +1 \[dap-repl-47]
badd +8 ~/Documents/kody/rendering-reaktor/src/rendering/CubeMap.cpp
badd +11 ~/Documents/kody/rendering-reaktor/src/rendering/CubeMap.hpp
badd +11 ~/Documents/kody/rendering-reaktor/src/rendering/Program.hpp
badd +7 ~/Documents/kody/rendering-reaktor/src/rendering/Program.cpp
badd +9 ~/Documents/kody/rendering-reaktor/src/rendering/Shader.hpp
badd +3 ~/Documents/kody/rendering-reaktor/src/rendering/Shader.cpp
badd +19 ~/Documents/kody/rendering-reaktor/src/rendering/Texture2d.hpp
badd +59 ~/Documents/kody/rendering-reaktor/src/rendering/Texture2d.cpp
argglobal
%argdel
edit ~/Documents/kody/rendering-reaktor/src/main.cpp
argglobal
balt ~/Documents/kody/rendering-reaktor/src/rendering/Texture2d.cpp
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
let s:l = 111 - ((10 * winheight(0) + 26) / 52)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 111
normal! 010|
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
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
