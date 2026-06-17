#ifndef NDEBUG

#include <renderdoc_app.h>
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

#include <assert.h>

RENDERDOC_API_1_1_2 *rdoc_api = NULL;

bool LoadRenderDoc() {
#ifdef _WIN32
  // At init, on windows
  if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
    pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
    int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
    // assert(ret == 1);
    return ret != 1;
  }
#else
  // At init, on linux/android.
  // For android replace librenderdoc.so with libVkLayer_GLES_RenderDoc.so
  if (void *mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD)) {
    pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
    int ret = RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
    // assert(ret == 1);
    return ret != 1;
  }
#endif
  return true;
}

void UnloadRenderDoc() {
  if (NULL != rdoc_api) {
    rdoc_api->Shutdown();
  }
}

bool RenderDocIsFrameCapturing() {
  if (NULL != rdoc_api)
    return rdoc_api->IsFrameCapturing();
  return false;
}

void RenderDocBeginFrameCapture() {
  if (NULL != rdoc_api) {
    rdoc_api->StartFrameCapture(NULL, NULL);
  }
}

void RenderDocEndFrameCapture() {
  // stop the capture
  if (NULL != rdoc_api) {
    rdoc_api->EndFrameCapture(NULL, NULL);
  }
}

#endif
