#include "AppRuntime.h"

#include "WorkQueue.h"

namespace Babylon
{
    AppRuntime::AppRuntime(std::string rootUrl)
        : m_rootUrl{std::move(rootUrl)}
        , m_workQueue{std::make_unique<WorkQueue>([this] { RunPlatformTier(); })}
    {
        Dispatch([this](Napi::Env env) {
            JsRuntime::CreateForJavaScript(env, [this](auto func) { m_workQueue->Append(std::move(func)); });
        });
    }

    AppRuntime::~AppRuntime()
    {
    }

    const std::string& AppRuntime::RootUrl() const
    {
        return m_rootUrl;
    }

    void AppRuntime::Run(Napi::Env env)
    {
        m_workQueue->Run(env);
    }

    void AppRuntime::Suspend()
    {
        m_workQueue->Suspend();
    }

    void AppRuntime::Resume()
    {
        m_workQueue->Resume();
    }

    void AppRuntime::Dispatch(std::function<void(Napi::Env)> func)
    {
        m_workQueue->Append(std::move(func));
    }
}
