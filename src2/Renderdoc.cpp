#ifndef NDEBUG

#include <renderdoc_app.h>
#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#if defined(_WIN64)
#define _AMD64_
#undef _X86_
#else
#undef _AMD64_
#define _X86_
#endif

#ifdef _WIN32
#include <libloaderapi.h>
#include <minwindef.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef __linux__
#include <dlfcn.h>
#endif

#include <assert.h>

RENDERDOC_API_1_1_2 *renderdoc = NULL;

bool LoadRenderDoc() {
#ifdef _WIN32
  // At init, on windows
  if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
    pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
    int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&renderdoc);
    // assert(ret == 1);
    return ret == 1;
  }
#else
  // At init, on linux/android.
  // For android replace librenderdoc.so with libVkLayer_GLES_RenderDoc.so
  if (void *mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD)) {
    pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
    int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&renderdoc);
    // assert(ret == 1);
    return ret == 1;
  }
#endif
  return true;
}

void UnloadRenderDoc() {
  if (NULL != renderdoc) {
    renderdoc->Shutdown();
  }
}

bool RenderDocIsFrameCapturing() {
  if (NULL != renderdoc)
    return renderdoc->IsFrameCapturing();
  return false;
}

void RenderDocBeginFrameCapture() {
  if (NULL != renderdoc) {
    renderdoc->StartFrameCapture(NULL, NULL);
  }
}

void RenderDocEndFrameCapture() {
  // stop the capture
  if (NULL != renderdoc) {
    renderdoc->EndFrameCapture(NULL, NULL);
  }
}

#else

bool LoadRenderDoc() { return false; }
void UnloadRenderDoc() {}
bool RenderDocIsFrameCapturing() { return false; }
void RenderDocBeginFrameCapture() {}
void RenderDocEndFrameCapture() {}

#endif
