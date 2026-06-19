#pragma once

/**
 * @file thread_pool.hpp
 * Minimal header-only thread pool for C++17.
 *
 * Owns a fixed set of worker threads that process tasks from a shared
 * queue.  Tasks are enqueued via enqueue() which returns a std::future
 * to the result.
 *
 * Usage:
 * @code
 *   xnetwork::thread_pool pool;
 *   auto future = pool.enqueue([] { return 42; });
 *   int result = future.get();
 * @endcode
 *
 * The destructor waits for all pending tasks to finish and joins all
 * worker threads.  It is safe to destroy the pool while tasks are
 * still running — remaining tasks are abandoned.
 */

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>

namespace xnetwork {

    class thread_pool {
      public:
        /**
         * @param num_threads  Number of worker threads.
         *                     Defaults to hardware concurrency (min 1).
         */
        explicit thread_pool(size_t num_threads = std::thread::hardware_concurrency())
            : stop_(false) {
            if (num_threads == 0) {
                num_threads = 1;
            }
            workers_.reserve(num_threads);
            for (size_t i = 0; i < num_threads; ++i) {
                workers_.emplace_back([this] { worker_loop(); });
            }
        }

        ~thread_pool() {
            {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                stop_ = true;
            }
            condition_.notify_all();
            for (auto& w : workers_) {
                if (w.joinable()) {
                    w.join();
                }
            }
        }

        thread_pool(const thread_pool&) = delete;
        thread_pool& operator=(const thread_pool&) = delete;
        thread_pool(thread_pool&&) = delete;
        thread_pool& operator=(thread_pool&&) = delete;

        /**
         * Enqueue a callable for execution on the thread pool.
         *
         * @tparam F  Callable type (invocable, returns T)
         * @param task  Callable to execute
         * @return  std::future<T> holding the result (or exception)
         */
        template <typename F> auto enqueue(F&& task)
            -> std::future<std::invoke_result_t<std::decay_t<F>>> {
            using return_type = std::invoke_result_t<std::decay_t<F>>;

            auto packaged
                = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(task));

            auto future = packaged->get_future();
            {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                if (stop_) {
                    throw std::runtime_error("enqueue on stopped thread_pool");
                }
                tasks_.emplace([packaged]() { (*packaged)(); });
            }
            condition_.notify_one();
            return future;
        }

      private:
        void worker_loop() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        }

        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        std::mutex queue_mutex_;
        std::condition_variable condition_;
        bool stop_;
    };

}  // namespace xnetwork
