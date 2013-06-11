// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/test/layouttest_support.h"

#include "base/callback.h"
#include "base/lazy_instance.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/common/gpu/image_transport_surface.h"
#include "content/renderer/render_thread_impl.h"
#include "content/renderer/render_view_impl.h"
#include "content/renderer/renderer_webkitplatformsupport_impl.h"
#include "third_party/WebKit/public/platform/WebGamepads.h"
#include "third_party/WebKit/Tools/DumpRenderTree/chromium/TestRunner/public/WebTestProxy.h"

#if defined(OS_WIN) && !defined(USE_AURA)
#include "content/browser/web_contents/web_contents_drag_win.h"
#endif

#if defined(OS_MACOSX)
#include "content/browser/renderer_host/popup_menu_helper_mac.h"
#endif

using WebKit::WebGamepads;
using WebKit::WebRect;
using WebKit::WebSize;
using WebTestRunner::WebTestProxy;
using WebTestRunner::WebTestProxyBase;

namespace content {

namespace {

base::LazyInstance<base::Callback<void(RenderView*, WebTestProxyBase*)> >::Leaky
    g_callback = LAZY_INSTANCE_INITIALIZER;

RenderViewImpl* CreateWebTestProxy(RenderViewImplParams* params) {
  typedef WebTestProxy<RenderViewImpl, RenderViewImplParams*> ProxyType;
  ProxyType* render_view_proxy = new ProxyType(
      reinterpret_cast<RenderViewImplParams*>(params));
  if (g_callback == 0)
    return render_view_proxy;
  g_callback.Get().Run(
      static_cast<RenderView*>(render_view_proxy), render_view_proxy);
  return render_view_proxy;
}

}  // namespace


void EnableWebTestProxyCreation(
    const base::Callback<void(RenderView*, WebTestProxyBase*)>& callback) {
  g_callback.Get() = callback;
  RenderViewImpl::InstallCreateHook(CreateWebTestProxy);
}

void SetMockGamepads(const WebGamepads& pads) {
  RendererWebKitPlatformSupportImpl::SetMockGamepadsForTesting(pads);
}

void EnableRendererLayoutTestMode() {
  RenderThreadImpl::current()->set_layout_test_mode(true);
}

void EnableBrowserLayoutTestMode() {
#if defined(OS_MACOSX)
  ImageTransportSurface::SetAllowOSMesaForTesting(true);
  PopupMenuHelper::DontShowPopupMenuForTesting();
#elif defined(OS_WIN) && !defined(USE_AURA)
  WebContentsDragWin::DisableDragDropForTesting();
#endif
  RenderWidgetHostImpl::DisableResizeAckCheckForTesting();
}

int GetLocalSessionHistoryLength(RenderView* render_view) {
  return static_cast<RenderViewImpl*>(render_view)
      ->GetLocalSessionHistoryLengthForTesting();
}

void SyncNavigationState(RenderView* render_view) {
  static_cast<RenderViewImpl*>(render_view)->SyncNavigationState();
}

void SetFocusAndActivate(RenderView* render_view, bool enable) {
  static_cast<RenderViewImpl*>(render_view)
      ->SetFocusAndActivateForTesting(enable);
}

void ForceResizeRenderView(RenderView* render_view,
                           const WebSize& new_size) {
  RenderViewImpl* render_view_impl = static_cast<RenderViewImpl*>(render_view);
  render_view_impl->setWindowRect(WebRect(render_view_impl->rootWindowRect().x,
                                          render_view_impl->rootWindowRect().y,
                                          new_size.width,
                                          new_size.height));
}

void SetDeviceScaleFactor(RenderView* render_view, float factor) {
  static_cast<RenderViewImpl*>(render_view)
      ->SetDeviceScaleFactorForTesting(factor);
}

void EnableAutoResizeMode(RenderView* render_view,
                          const WebSize& min_size,
                          const WebSize& max_size) {
  static_cast<RenderViewImpl*>(render_view)
      ->EnableAutoResizeForTesting(min_size, max_size);
}

void DisableAutoResizeMode(RenderView* render_view, const WebSize& new_size) {
  static_cast<RenderViewImpl*>(render_view)
      ->DisableAutoResizeForTesting(new_size);
}

scoped_refptr<base::MessageLoopProxy> GetMediaThreadMessageLoopProxy() {
  return RenderThreadImpl::current()->GetMediaThreadMessageLoopProxy();
}

}  // namespace content