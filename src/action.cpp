#include "action.hpp"

#include <iostream>

void OneShotTimer::tick() const
{
    timerPtr->expires_after(interval);
    timerPtr->async_wait([this](boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[OneShotTimer] error: " << ec.message() << std::endl;
            return;
        }

        this->callback();
    });
}

void PeriodicTimer::tick() const
{
    timerPtr->expires_after(interval);
    timerPtr->async_wait([this](boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[PeriodicTimer] error: " << ec.message() << std::endl;
            return;
        }

        this->callback();
        this->tick();
    });
}

void DetachedOneShotTimer::tick() const
{
    timerPtr->expires_after(interval);
    timerPtr->async_wait(
        [self = shared_from_this()](boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[DetachedOneShotTimer] error: " << ec.message()
                      << std::endl;
            return;
        }

        self->callback();
    });
}

void DetachedPeriodicTimer::tick() const
{
    timerPtr->expires_after(interval);
    timerPtr->async_wait(
        [self = shared_from_this()](boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[DetachedPeriodicTimer] error: " << ec.message()
                      << std::endl;
            return;
        }
        if (self->isStopped())
        {
            std::cerr << "[DetachedPeriodicTimer] stopped" << std::endl;
            return;
        }

        self->callback();
        self->tick();
    });
}

void deployAction(std::shared_ptr<boost::asio::steady_timer> timer,
                  Milliseconds intervalMs, Callback action, bool& stop)
{
    timer->expires_after(intervalMs);
    timer->async_wait([timer, action = std::move(action), intervalMs,
                       &stop](boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[RepeatingActionTimer] error: " << ec.message()
                      << std::endl;
            return;
        }
        if (stop)
        {
            std::cerr << "[RepeatingActionTimer] stopped" << std::endl;
            return;
        }
        else
        {
            action();
            deployAction(timer, intervalMs, std::move(action), stop);
        }
    });
}

void singleAction(boost::asio::io_context& ioc, Milliseconds delayMs,
                  Callback&& action)
{
    auto timer = std::make_unique<boost::asio::steady_timer>(ioc);
    timer->expires_after(delayMs);

    timer->async_wait([timer = std::move(timer), action = std::move(action)](
                          boost::system::error_code ec) {
        if (ec)
        {
            std::cerr << "[SingleActionTimer] error: " << ec.message()
                      << std::endl;
            return;
        }
        action();
    });
}

void repeatingAction(boost::asio::io_context& ioc, Milliseconds intervalMs,
                     Callback&& action, bool& stop)
{
    auto timer = std::make_shared<boost::asio::steady_timer>(ioc);
    deployAction(timer, intervalMs, std::move(action), stop);
}
