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
badd +4 ~/Documents/kody/awaria/hmm.json
badd +2 ~/Documents/kody/awaria/.gitignore
badd +119 ~/Documents/kody/awaria/src/Main.hx
badd +3 ~/Documents/kody/awaria/compile.hx
badd +3 ~/Documents/kody/awaria/compile.hxml
badd +18 ~/Documents/kody/awaria/res/style.css
badd +135 ~/Documents/kody/awaria/.haxelib/domkit/git/domkit/Component.hx
badd +5 ~/Documents/kody/awaria/.haxelib/heaps/git/h2d/domkit/Object.hx
badd +1196 ~/Documents/kody/awaria/.haxelib/heaps/git/h2d/domkit/BaseComponents.hx
badd +1 ~/Documents/kody/awaria/.haxelib/domkit/git/domkit/Object.hx
badd +49 ~/Documents/kody/awaria/.haxelib/domkit/git/domkit/Properties.hx
badd +16 ~/Documents/kody/awaria/.haxelib/domkit/git/domkit/CssParser.hx
badd +6 ~/Documents/kody/awaria/.haxelib/domkit/git/sample/CustomComponent.hx
badd +68 ~/Documents/kody/awaria-cpp/deps/ext.cmake
badd +95 ~/Documents/kody/awaria-cpp/src/main.cpp
badd +2 ~/Documents/kody/awaria-cpp/CMakeLists.txt
badd +1 ~/Documents/bettyergl/CMakeLists.txt
badd +17 ~/Documents/bettyergl/src/bettergl/CMakeLists.txt
badd +110 ~/Documents/bettyergl/src/main.cpp
badd +36 ~/Documents/bettyergl/src/bettergl/include/bettergl/Assets.hpp
badd +30 ~/Documents/bettyergl/src/bettergl/Assets.cpp
badd +36 ~/Documents/bettyergl/src/bettergl/assetPackers/bytePacker.cpp
badd +41 ~/Documents/bettyergl/src/bettergl/assetPackers/imagePacker.cpp
badd +6 ~/Documents/bettyergl/gexignored.hpp
badd +6 ~/Documents/bettyergl/src/shaders/vertex.glsl
badd +3 ~/Documents/bettyergl/deps/ext.cmake
badd +22 ~/Documents/bettyergl/deps/CPM.cmake
badd +134 ~/Documents/bettyergl/src/bettergl/Debugging.cpp
badd +8 ~/Documents/bettyergl/gex.ping.hpp
badd +322 ~/Documents/rendering-reaktor/src/main.cpp
badd +3 ~/Documents/rendering-reaktor/src/shaders/composite.vertex.glsl
badd +4 ~/Documents/rendering-reaktor/src/shaders/composite.frag.glsl
badd +35 ~/Documents/rendering-reaktor/CMakeLists.txt
badd +6 ~/Documents/rendering-reaktor/src/input_handling.cpp
badd +4 ~/Documents/rendering-reaktor/src/shaders/skybox.frag.glsl
badd +21 ~/Documents/rendering-reaktor/src/rendering/FrameBuffer.cpp
badd +1 ~/Documents/rendering-reaktor/.gitignore
badd +5 ~/Documents/rendering-reaktor/src/rendering/CMakeLists.txt
badd +198 ~/Documents/rendering-reaktor/deps/ext.cmake
badd +182 ~/scoop/apps/mingw-winlibs-llvm-msvcrt/14.2.0-19.1.7-12.0.0-r3/share/cmake-3.31/Modules/FindPackageHandleStandardArgs.cmake
badd +1 ~/Documents/rendering-reaktor/build/CMakeFiles/pkgRedirects/zlib-config.cmake
badd +1 ~/Documents/rendering-reaktor/deps/raygizmo/raygizmo.c
badd +1 ~/Documents/rendering-reaktor/deps/raygizmo/raygizmo.h
badd +136 ~/Documents/rendering-reaktor/src2/raygizmo_test.cpp
badd +145 ~/Documents/rendering-reaktor/src2/fps.cpp
badd +1 ~/Documents/rendering-reaktor/src2/main3.cpp
badd +6 ~/Documents/rendering-reaktor/src2/main.cpp
badd +9 ~/Documents/rendering-reaktor/src2/Texture2d.hpp
badd +2 ~/Documents/rendering-reaktor/src2/embeded/cubemap.png.cpp
badd +3 ~/Documents/rendering-reaktor/src2/embeded/cubemap.png.hpp
badd +82 ~/Documents/rendering-reaktor/src2/main2.cpp
badd +1 ~/Documents/rendering-reaktor/src2/embeded/icon.png.hpp
badd +1 ~/Documents/rendering-reaktor/src2/embeded/icons.png.hpp
badd +45 ~/Documents/rendering-reaktor/src2/App.hpp
badd +2 ~/Documents/rendering-reaktor/src2/App.cpp
badd +8 ~/Documents/rendering-reaktor/src2/StaticAssets.cpp
badd +1 ~/Documents/rendering-reaktor/.gitmodules
badd +421 ~/cacheCPM/raylib/c846/src/raylib.h
badd +1 ~/Documents/rendering-reaktor/src2/embeded/cubemap.fs.hpp
badd +69 ~/Documents/rendering-reaktor/src2/FPScontroler.cpp
badd +2 ~/Documents/rendering-reaktor/src2/embeded/RobotoRegular.h
badd +3 ~/Documents/rendering-reaktor/src2/embeded/RobotoRegular.cpp
badd +6 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
badd +20 ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.cpp
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
let s:l = 17 - ((16 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 17
normal! 04|
tabnext
edit ~/Documents/rendering-reaktor/src2/AssetMgr/ModelMgr.hpp
argglobal
balt ~/cacheCPM/raylib/c846/src/raylib.h
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
let s:l = 6 - ((5 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 03|
tabnext
edit ~/Documents/rendering-reaktor/src2/App.cpp
argglobal
balt ~/Documents/rendering-reaktor/src2/StaticAssets.cpp
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
let s:l = 195 - ((24 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 195
normal! 022|
tabnext
edit ~/Documents/rendering-reaktor/src2/FPScontroler.cpp
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
let s:l = 7 - ((6 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 7
normal! 033|
tabnext
edit ~/Documents/rendering-reaktor/CMakeLists.txt
argglobal
balt ~/Documents/bettyergl/CMakeLists.txt
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
let s:l = 37 - ((36 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 37
normal! 0
tabnext
edit ~/Documents/rendering-reaktor/deps/ext.cmake
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
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
let s:l = 198 - ((49 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 198
normal! 05|
tabnext 6
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
