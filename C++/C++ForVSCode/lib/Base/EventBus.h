#pragma once
#include <functional>
#include <vector>
#include <mutex>
#include <tuple>

template <class T, typename... Args>
class EventBus {
public:
    using MemberFunc = void (T::*)(Args...);

    void AddListener(T* instance, MemberFunc method) {
        std::lock_guard<std::mutex> lock(m_listenersMutex);
        m_listeners.emplace_back(instance, method, [=](Args... args) {
            (instance->*method)(args...);
        });
    }

    void RemoveListener(T* instance, MemberFunc method) {
        std::lock_guard<std::mutex> lock(m_listenersMutex);
        m_listeners.erase(
            std::remove_if(m_listeners.begin(), m_listeners.end(),
                [&](const Listener& l) {
                    return l.instance == instance && l.method == method;
                }),
            m_listeners.end());
    }

    void Notify(Args... args) {
        std::lock_guard<std::mutex> lock(m_listenersMutex);
        for (auto& listener : m_listeners) {
            listener.fn(args...);
        }
    }

private:
    struct Listener {
        T* instance;
        MemberFunc method;
        std::function<void(Args...)> fn;

        Listener(T* i, MemberFunc m, std::function<void(Args...)> f)
            : instance(i), method(m), fn(std::move(f)) {}
    };

    std::vector<Listener> m_listeners;
    std::mutex m_listenersMutex;
};
