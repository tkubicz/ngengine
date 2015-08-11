#include "NGE/Core/Process.hpp"
#include "NGE/Tools/Logger.hpp"
#include <memory>
using namespace NGE::Core;

Process::Process() {
    state = UNINITIALIZED;
}

Process::~Process() {
    if (child) {
        child->OnAbort();
    }
}

void Process::OnInit() {
    state = RUNNING;
}

void Process::OnSuccess() { }

void Process::OnFail() { }

void Process::OnAbort() { }

Process::State Process::GetState() const {
    return state;
}

bool Process::IsAlive() const {
    return state == RUNNING || state == PAUSED;
}

bool Process::IsDead() const {
    return state == SUCCEEDED || state == FAILED || state == ABORTED;
}

bool Process::IsRemoved() const {
    return state == REMOVED;
}

bool Process::IsPaused() const {
    return state == PAUSED;
}

void Process::AttachChild(StrongProcessPtr child) {
    if (this->child) {
        this->child->AttachChild(child);
    } else {
        this->child = child;
    }
}

StrongProcessPtr Process::RemoveChild() {
    if (child) {
        // Keep the child from getting destroyed when clear.
        StrongProcessPtr ptrChild = child;
        child.reset();
        return ptrChild;
    }

    return StrongProcessPtr();
}

StrongProcessPtr Process::PeekChild() {
    return child;
}

void Process::SetState(State state) {
    this->state = state;
}

