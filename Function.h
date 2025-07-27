#pragma once

class Function {
public:
    virtual float evaluate(float x) const = 0;
    virtual ~Function() = default;
};
