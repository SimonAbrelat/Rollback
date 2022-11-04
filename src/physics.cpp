#include "physics.hpp"

#include <iostream>

Physics::Physics() {
    _p1_body = Collider(100,100,50,50);
    _p2_body = Collider(530,100,50,50);
};

Physics::~Physics() {
    _physics_thread.join();
};

bool Physics::run() {
    try {
        _physics_thread = std::thread(&Physics::update, this);
    } catch(std::exception e) {
        return false;
    }
    return true;
}

void Physics::abort() {
    _run_physics = false;
    _physics_thread.join();
}

void Physics::update_inputs(const InputState& input) {
    _input_lock.lock();
    _input = input;
    _input_lock.unlock();
}

PlayerState Physics::get(bool player){
    _player_lock.lock();
    PlayerState ret {((player) ? _p1_body : _p2_body).x, f16(0)};
    _player_lock.unlock();
    return ret;
};

void Physics::update() {
    using std::chrono::operator""ms;
    int count = 0;
    while (_run_physics) {
        const auto next_cycle = std::chrono::steady_clock::now() + 17ms;
        _input_lock.lock();
        InputState curr = _input;
        _input_lock.unlock();

        _player_lock.lock();
        _p1_body.x += WALK_SPEED * curr.direction;
        //std::cout << "P1 x: " << (int) _p1_body.x << " is colliding: " << _p1_body.is_colliding(_p2_body) << '\n';
        _player_lock.unlock();

        std::this_thread::sleep_until(next_cycle);
    }
    std::terminate();
}
