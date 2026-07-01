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
badd +1 src/main.cpp
badd +3 src/shaders/composite.vertex.glsl
badd +4 src/shaders/composite.frag.glsl
badd +42 CMakeLists.txt
badd +6 src/input_handling.cpp
badd +4 src/shaders/skybox.frag.glsl
badd +21 src/rendering/FrameBuffer.cpp
badd +1 .gitignore
badd +5 src/rendering/CMakeLists.txt
badd +135 deps/ext.cmake
badd +182 ~/scoop/apps/mingw-winlibs-llvm-msvcrt/14.2.0-19.1.7-12.0.0-r3/share/cmake-3.31/Modules/FindPackageHandleStandardArgs.cmake
badd +1 build/CMakeFiles/pkgRedirects/zlib-config.cmake
badd +1 deps/raygizmo/raygizmo.c
badd +1 deps/raygizmo/raygizmo.h
badd +198 src2/raygizmo_test.cpp
badd +145 src2/fps.cpp
badd +9 src2/Texture2d.hpp
badd +2 src2/embeded/cubemap.png.cpp
badd +3 src2/embeded/cubemap.png.hpp
badd +1 src2/embeded/icon.png.hpp
badd +1 src2/embeded/icons.png.hpp
badd +77 src2/App.hpp
badd +400 src2/App.cpp
badd +10 src2/StaticAssets.cpp
badd +1 .gitmodules
badd +243 ~/cacheCPM/raylib/c846/src/raylib.h
badd +1 src2/embeded/cubemap.fs.hpp
badd +4 src2/FPScontroler.cpp
badd +2 src2/embeded/RobotoRegular.h
badd +3 src2/embeded/RobotoRegular.cpp
badd +45 src2/AssetMgr/ModelMgr.hpp
badd +106 src2/AssetMgr/ModelMgr.cpp
badd +1 ~/Documents/rendering-reaktor/src2/embeded/skinning.fs.hpp
badd +1 ~/Documents/rendering-reaktor/src2/embeded/skinning.vs.hpp
badd +1 ~/Documents/rendering-reaktor/src2/embeded/skinning.vs.cpp
badd +1 ~/Documents/rendering-reaktor/src2/Renderdoc.cpp
badd +17 ~/Documents/rendering-reaktor/src2/Zip/zip.cpp
badd +1 ~/Documents/rendering-reaktor/src2/Zip/Zip.hpp
badd +94 dap-src://1/1000/
badd +1303 dap-src://2/1000/
badd +24 dap-src://3/1000/
badd +1 ~/Documents/rendering-reaktor/src2/main2.cpp
badd +7 ~/Documents/rendering-reaktor/src2/main.cpp
badd +131 ~/Documents/rendering-reaktor/src2/SaveLoad/SaveMod.cpp
badd +16 ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
badd +1 ~/Documents/rendering-reaktor/src2/SaveLoad/Format.hpp
badd +8 ~/Documents/rendering-reaktor/src2/AssetMgr/TextureMgr.hpp
badd +213 dap-src://35/1024/
badd +3941 dap-src://35/1030/
badd +213 dap-src://37/1026/
badd +3941 dap-src://37/1032/
argglobal
%argdel
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit src2/AssetMgr/ModelMgr.hpp
argglobal
balt ~/Documents/rendering-reaktor/src2/AssetMgr/TextureMgr.hpp
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
let s:l = 36 - ((35 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 36
normal! 0
tabnext
edit src2/AssetMgr/ModelMgr.cpp
argglobal
balt src2/App.cpp
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
let s:l = 183 - ((47 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 183
normal! 0
tabnext
edit src2/App.hpp
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
let s:l = 77 - ((51 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 77
normal! 028|
tabnext
edit src2/App.cpp
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
balt src2/FPScontroler.cpp
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
let s:l = 400 - ((44 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 400
normal! 025|
tabnext
edit ~/Documents/rendering-reaktor/src2/SaveLoad/Format_0_0_1.cpp
argglobal
balt src2/StaticAssets.cpp
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
let s:l = 16 - ((15 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 0
tabnext
edit CMakeLists.txt
argglobal
balt deps/ext.cmake
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
let s:l = 42 - ((24 * winheight(0) + 29) / 59)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 42
normal! 0
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
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
