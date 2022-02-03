#pragma once
#include "Commands.hpp"
#include "Interpreter.hpp"
#include<memory>
namespace {
    bool add_cmd = Interpreter::getInstance().registerCreator("+",std::make_unique<Add>());

    bool sub_cmd = Interpreter::getInstance().registerCreator("-", std::make_unique<Sub>());

    bool mod_cmd = Interpreter::getInstance().registerCreator("mod", std::make_unique<Mod>());

    bool div_cmd = Interpreter::getInstance().registerCreator("/", std::make_unique<Div>());

    bool equal_cmd = Interpreter::getInstance().registerCreator("==", std::make_unique<Equal>());

    bool more_cmd = Interpreter::getInstance().registerCreator(">", std::make_unique<More>());

    bool less_cmd = Interpreter::getInstance().registerCreator("<", std::make_unique<Less>());

    bool write_cmd = Interpreter::getInstance().registerCreator(".", std::make_unique<Write>());

    bool mul_cmd = Interpreter::getInstance().registerCreator("*", std::make_unique<Mul>());

    bool dup_cmd = Interpreter::getInstance().registerCreator("dup", std::make_unique<Dup>());

    bool drop_cmd = Interpreter::getInstance().registerCreator("drop", std::make_unique<Drop>());

    bool swap_cmd = Interpreter::getInstance().registerCreator("swap", std::make_unique<Swap>());

    bool rot_cmd = Interpreter::getInstance().registerCreator("rot", std::make_unique<Rot>());

    bool over_cmd = Interpreter::getInstance().registerCreator("over", std::make_unique<Over>());

    bool cr_cmd = Interpreter::getInstance().registerCreator("cr", std::make_unique<Cr>());

    bool emit_cmd = Interpreter::getInstance().registerCreator("emit", std::make_unique<Emit>());
}