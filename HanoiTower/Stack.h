#ifndef HANOI_TOWER_STACK_H
#define HANOI_TOWER_STACK_H
#pragma once
#include <vector>
#include "Cube.h"

class Stack{
public:
    void push_back(const Cube & cube);
    Cube removeTop();
    Cube & peekTop();
    unsigned size() const;

    friend std::ostream& operator<<(std:: ostream & os, const Stack & stack);

private:
    std::vector<Cube> cubes_;
};
#endif //HANOI_TOWER_STACK_H
