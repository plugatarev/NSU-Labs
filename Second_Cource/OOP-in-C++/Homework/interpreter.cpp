#include <iostream>
#include <cstdint>
#include <cassert>


class Command {
 public:
    virtual void apply(const char * c, uint8_t * ptr) {
        std::cout << "Hello from command" << std::endl;
    }
};

class Add: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        (*ptr)++;
    }
};

class Sub: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        std::cout << "Hello from sub" << std::endl;
        // (*ptr)--;
    }
};

class Right: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        ptr++;
    }
};

class Left: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        ptr--;
    }
};

class Write: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        std::cout << unsigned(*ptr) << std::endl;
    }
};

class Read: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        std::cin >> (*ptr);
    }
};

void find_pos(const char* c){
    int balance = 0;
    char mode = 0;
    if(*c == '['){
        balance++;
        mode = 1;
    }
	else if(*c == ']'){
        balance--;
        mode = -1;
    }

	while(balance != 0){
        c+=mode;
	if(*c == '[') balance++;
	else if(*c == ']') balance--;
    }
}

class Loop: public Command {
    void apply(const char * c, uint8_t * ptr) override {
        if (*ptr == 0) find_pos(c);
	c++;
    }
};

class Pool: public Command {
    void apply(const char* c, uint8_t * ptr) override {
        if(*ptr != 0) find_pos(c);
    }
};

Command * get_cmd(char c) {
    switch (c) {
        case '+':
            return new Add();
        case '-':
            return new Sub();
        case '>':
            return new Right();
        case '<':
            return new Left();
        case '.':
            return new Write();
        case ',':
            return new Read();
        case ']':
            return new Pool;
        case '[':
            return new Loop;
        default:
            assert(false);
    }
}

void interpret(const char * c, uint8_t * ptr) {
    while (*c != '\0') {
        Command * command = get_cmd(*c);
        command->apply(c, ptr);
        c++;
    }
}

int main(int argc, char const *argv[]) {
    Sub s = Sub();
    Command * sub = &s;
    char c = 'c';
    uint8_t data[100] = {};
    // sub->apply(&c, data);
    size_t vptr = *(size_t *) sub;
    typedef void (*apply_t) (const char * c, uint8_t * ptr);
    apply_t func = *(apply_t *) vptr;
    func(&c, data);


    // std::string cmds;
    // uint8_t data[100] = {};
    // uint8_t * ptr = data;
    // while (true) {
    //     std::cin >> cmds;
    //     const char * s = cmds.c_str();
    //     const char * c = s;
    //     interpret(c, ptr);
    // }
    return 0;
}
