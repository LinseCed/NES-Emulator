//
// Created by Linus Bauer on 13.02.26.
//

#ifndef LINES_CPU6502_H
#define LINES_CPU6502_H
#include <cstdint>

#include "InterruptLine.h"

#define STACK_START 0x0100

class Bus;

enum StatusFlag : uint8_t {
    CARRY = 1,
    ZERO = 1 << 1,
    INTERRUPT = 1 << 2,
    DECIMAL = 1 << 3,
    BREAK = 1 << 4,
    CPU_OVERFLOW = 1 << 6,
    NEGATIVE = 1 << 7,
};

enum class Operation : uint8_t {
    ADC,
    AND,
    ASL,
    BCC,
    BCS,
    BEQ,
    BIT,
    BMI,
    BNE,
    BPL,
    BRK,
    BVC,
    BVS,
    CLC,
    CLD,
    CLI,
    CLV,
    CMP,
    CPX,
    CPY,
    DEC,
    DEX,
    DEY,
    EOR,
    INC,
    INX,
    INY,
    JMP,
    JSR,
    LDA,
    LDX,
    LDY,
    LSR,
    NOP,
    ORA,
    PHA,
    PHP,
    PLA,
    PLP,
    ROL,
    ROR,
    RTI,
    RTS,
    SBC,
    SEC,
    SED,
    SEI,
    STA,
    STX,
    STY,
    TAX,
    TAY,
    TSX,
    TXA,
    TXS,
    TYA,
    ALR,
    ANC,
    ANE,
    ARR,
    AXS,
    LAX,
    LAS,
    SAX,
    SHA,
    SHS,
    SHX,
    SHY,
    DCP,
    ISB,
    RLA,
    RRA,
    SLO,
    SRE,
    SKB,
    IGN,
    JAM,
    TAS,
    LXA,
    SBX,
    ISC,
    USBC,
};

enum class AddressMode : uint8_t {
    NONE,
    ACC,
    IMPL,
    IMT,
    ZPG,
    ZPG_X,
    ZPG_Y,
    ABS,
    ABS_X,
    ABS_Y,
    REL,
    IDX_IND,
    IND_IDX,
    IND,
};

struct Instruction {
    Operation operation;
    AddressMode mode;
    uint8_t cycles;
};

constexpr Instruction instructions[256] = {
    Instruction{Operation::BRK, AddressMode::IMPL,7},          // 0x00
    Instruction{Operation::ORA, AddressMode::IDX_IND, 6},      // 0x01
    Instruction{Operation::JAM, AddressMode::NONE, 0},         // 0x02
    Instruction{Operation::SLO, AddressMode::IDX_IND, 8},      // 0x03
    Instruction{Operation::NOP, AddressMode::ZPG, 3},          // 0x04
    Instruction{Operation::ORA, AddressMode::ZPG, 3},          // 0x05
    Instruction{Operation::ASL, AddressMode::ZPG, 5},          // 0x06
    Instruction{Operation::SLO, AddressMode::ZPG, 5},          // 0x07
    Instruction{Operation::PHP, AddressMode::IMPL, 3},         // 0x08
    Instruction{Operation::ORA, AddressMode::IMT, 2},          // 0x09
    Instruction{Operation::ASL, AddressMode::ACC, 2},          // 0x0A
    Instruction{Operation::ANC, AddressMode::IMT, 2},          // 0x0B
    Instruction{Operation::NOP, AddressMode::ABS, 4},          // 0x0C
    Instruction{Operation::ORA, AddressMode::ABS, 4},          // 0x0D
    Instruction{Operation::ASL, AddressMode::ABS, 6},          // 0x0E
    Instruction{Operation::SLO, AddressMode::ABS, 6},          // 0x0F

    Instruction{Operation::BPL, AddressMode::REL, 2},          // 0x10
    Instruction{Operation::ORA, AddressMode::IND_IDX, 5},      // 0x11
    Instruction{Operation::JAM, AddressMode::NONE, 0},         // 0x12
    Instruction{Operation::SLO, AddressMode::IND_IDX, 8},      // 0x13
    Instruction{Operation::NOP, AddressMode::ZPG_X, 4},        // 0x14
    Instruction{Operation::ORA, AddressMode::ZPG_X, 4},        // 0x15
    Instruction{Operation::ASL, AddressMode::ZPG_X, 6},        // 0x16
    Instruction{Operation::SLO, AddressMode::ZPG_X, 6},        // 0x17
    Instruction{Operation::CLC, AddressMode::IMPL, 2},         // 0x18
    Instruction{Operation::ORA, AddressMode::ABS_Y, 4},        // 0x19
    Instruction{Operation::NOP, AddressMode::IMPL, 2},         // 0x1A
    Instruction{Operation::SLO, AddressMode::ABS_Y, 7},        // 0x1B
    Instruction{Operation::NOP, AddressMode::ABS_X, 4},        // 0x1C
    Instruction{Operation::ORA, AddressMode::ABS_X, 4},        // 0x1D
    Instruction{Operation::ASL, AddressMode::ABS_X, 7},        // 0x1E
    Instruction{Operation::SLO, AddressMode::ABS_X, 7},        // 0x1F

    Instruction{Operation::JSR, AddressMode::ABS, 6},          // 0x20
    Instruction{Operation::AND, AddressMode::IDX_IND, 6},      // 0x21
    Instruction{Operation::JAM, AddressMode::NONE, 0},         // 0x22
    Instruction{Operation::RLA, AddressMode::IDX_IND, 8},      // 0x23
    Instruction{Operation::BIT, AddressMode::ZPG, 3},          // 0x24
    Instruction{Operation::AND, AddressMode::ZPG, 3},          // 0x25
    Instruction{Operation::ROL, AddressMode::ZPG, 5},          // 0x26
    Instruction{Operation::RLA, AddressMode::ZPG, 5},          // 0x27
    Instruction{Operation::PLP, AddressMode::IMPL, 4},         // 0x28
    Instruction{Operation::AND, AddressMode::IMT, 2},          // 0x29
    Instruction{Operation::ROL, AddressMode::ACC, 2},          // 0x2A
    Instruction{Operation::ANC, AddressMode::IMT, 2},          // 0x2B
    Instruction{Operation::BIT, AddressMode::ABS, 4},          // 0x2C
    Instruction{Operation::AND, AddressMode::ABS, 4},          // 0x2D
    Instruction{Operation::ROL, AddressMode::ABS, 6},          // 0x2E
    Instruction{Operation::RLA, AddressMode::ABS, 6},          // 0x2F

    // ----- Cycles wrong

    Instruction{Operation::BMI, AddressMode::REL, 5},          // 0x30
    Instruction{Operation::AND, AddressMode::IND_IDX, 5},      // 0x31
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x32
    Instruction{Operation::RLA, AddressMode::IND_IDX, 5},      // 0x33
    Instruction{Operation::NOP, AddressMode::ZPG_X, 5},        // 0x34
    Instruction{Operation::AND, AddressMode::ZPG_X, 5},        // 0x35
    Instruction{Operation::ROL, AddressMode::ZPG_X, 5},        // 0x36
    Instruction{Operation::RLA, AddressMode::ZPG_X, 5},        // 0x37
    Instruction{Operation::SEC, AddressMode::IMPL, 5},         // 0x38
    Instruction{Operation::AND, AddressMode::ABS_Y, 5},        // 0x39
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0x3A
    Instruction{Operation::RLA, AddressMode::ABS_Y, 5},        // 0x3B
    Instruction{Operation::NOP, AddressMode::ABS_X, 5},        // 0x3C
    Instruction{Operation::AND, AddressMode::ABS_X, 5},        // 0x3D
    Instruction{Operation::ROL, AddressMode::ABS_X, 5},        // 0x3E
    Instruction{Operation::RLA, AddressMode::ABS_X, 5},        // 0x3F

    Instruction{Operation::RTI, AddressMode::IMPL, 5},         // 0x40
    Instruction{Operation::EOR, AddressMode::IDX_IND, 5},      // 0x41
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x42
    Instruction{Operation::SRE, AddressMode::IDX_IND, 5},      // 0x43
    Instruction{Operation::NOP, AddressMode::ZPG, 5},          // 0x44
    Instruction{Operation::EOR, AddressMode::ZPG, 5},          // 0x45
    Instruction{Operation::LSR, AddressMode::ZPG, 5},          // 0x46
    Instruction{Operation::SRE, AddressMode::ZPG, 5},          // 0x47
    Instruction{Operation::PHA, AddressMode::IMPL, 5},         // 0x48
    Instruction{Operation::EOR, AddressMode::IMT, 5},          // 0x49
    Instruction{Operation::LSR, AddressMode::ACC, 5},          // 0x4A
    Instruction{Operation::ALR, AddressMode::IMT, 5},          // 0x4B
    Instruction{Operation::JMP, AddressMode::ABS, 5},          // 0x4C
    Instruction{Operation::EOR, AddressMode::ABS, 5},          // 0x4D
    Instruction{Operation::LSR, AddressMode::ABS, 5},          // 0x4E
    Instruction{Operation::SRE, AddressMode::ABS, 5},          // 0x4F

    Instruction{Operation::BVC, AddressMode::REL, 5},          // 0x50
    Instruction{Operation::EOR, AddressMode::IND_IDX, 5},      // 0x51
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x52
    Instruction{Operation::SRE, AddressMode::IND_IDX, 5},      // 0x53
    Instruction{Operation::NOP, AddressMode::ZPG_X, 5},        // 0x54
    Instruction{Operation::EOR, AddressMode::ZPG_X, 5},        // 0x55
    Instruction{Operation::LSR, AddressMode::ZPG_X, 5},        // 0x56
    Instruction{Operation::SRE, AddressMode::ZPG_X, 5},        // 0x57
    Instruction{Operation::CLI, AddressMode::IMPL, 5},         // 0x58
    Instruction{Operation::EOR, AddressMode::ABS_Y, 5},        // 0x59
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0x5A
    Instruction{Operation::SRE, AddressMode::ABS_Y, 5},        // 0x5B
    Instruction{Operation::NOP, AddressMode::ABS_X, 5},        // 0x5C
    Instruction{Operation::EOR, AddressMode::ABS_X, 5},        // 0x5D
    Instruction{Operation::LSR, AddressMode::ABS_X, 5},        // 0x5E
    Instruction{Operation::SRE, AddressMode::ABS_X, 5},        // 0x5F

    Instruction{Operation::RTS, AddressMode::IMPL, 5},         // 0x60
    Instruction{Operation::ADC, AddressMode::IDX_IND, 5},      // 0x61
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x62
    Instruction{Operation::RRA, AddressMode::IDX_IND, 5},      // 0x63
    Instruction{Operation::NOP, AddressMode::ZPG, 5},          // 0x64
    Instruction{Operation::ADC, AddressMode::ZPG, 5},          // 0x65
    Instruction{Operation::ROR, AddressMode::ZPG, 5},          // 0x66
    Instruction{Operation::RRA, AddressMode::ZPG, 5},          // 0x67
    Instruction{Operation::PLA, AddressMode::IMPL, 5},         // 0x68
    Instruction{Operation::ADC, AddressMode::IMT, 5},          // 0x69
    Instruction{Operation::ROR, AddressMode::ACC, 5},          // 0x6A
    Instruction{Operation::ARR, AddressMode::IMT, 5},          // 0x6B
    Instruction{Operation::JMP, AddressMode::IND, 5},          // 0x6C
    Instruction{Operation::ADC, AddressMode::ABS, 5},          // 0x6D
    Instruction{Operation::ROR, AddressMode::ABS, 5},          // 0x6E
    Instruction{Operation::RRA, AddressMode::ABS, 5},          // 0x6F

    Instruction{Operation::BVS, AddressMode::REL, 5},          // 0x70
    Instruction{Operation::ADC, AddressMode::IND_IDX, 5},      // 0x71
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x72
    Instruction{Operation::RRA, AddressMode::IND_IDX, 5},      // 0x73
    Instruction{Operation::NOP, AddressMode::ZPG_X, 5},        // 0x74
    Instruction{Operation::ADC, AddressMode::ZPG_X, 5},        // 0x75
    Instruction{Operation::ROR, AddressMode::ZPG_X, 5},        // 0x76
    Instruction{Operation::RRA, AddressMode::ZPG_X, 5},        // 0x77
    Instruction{Operation::SEI, AddressMode::IMPL, 5},         // 0x78
    Instruction{Operation::ADC, AddressMode::ABS_Y, 5},        // 0x79
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0x7A
    Instruction{Operation::RRA, AddressMode::ABS_Y, 5},        // 0x7B
    Instruction{Operation::NOP, AddressMode::ABS_X, 5},        // 0x7C
    Instruction{Operation::ADC, AddressMode::ABS_X, 5},        // 0x7D
    Instruction{Operation::ROR, AddressMode::ABS_X, 5},        // 0x7E
    Instruction{Operation::RRA, AddressMode::ABS_X, 5},        // 0x7F

    Instruction{Operation::NOP, AddressMode::IMT, 5},          // 0x80
    Instruction{Operation::STA, AddressMode::IDX_IND, 5},      // 0x81
    Instruction{Operation::NOP, AddressMode::IMT, 5},          // 0x82
    Instruction{Operation::SAX, AddressMode::IDX_IND, 5},      // 0x83
    Instruction{Operation::STY, AddressMode::ZPG, 5},          // 0x84
    Instruction{Operation::STA, AddressMode::ZPG, 5},          // 0x85
    Instruction{Operation::STX, AddressMode::ZPG, 5},          // 0x86
    Instruction{Operation::SAX, AddressMode::ZPG, 5},          // 0x87
    Instruction{Operation::DEY, AddressMode::IMPL, 5},         // 0x88
    Instruction{Operation::NOP, AddressMode::IMT, 5},          // 0x89
    Instruction{Operation::TXA, AddressMode::IMPL, 5},         // 0x8A
    Instruction{Operation::ANE, AddressMode::IMT, 5},          // 0x8B
    Instruction{Operation::STY, AddressMode::ABS, 5},          // 0x8C
    Instruction{Operation::STA, AddressMode::ABS, 5},          // 0x8D
    Instruction{Operation::STX, AddressMode::ABS, 5},          // 0x8E
    Instruction{Operation::SAX, AddressMode::ABS, 5},          // 0x8F

    Instruction{Operation::BCC, AddressMode::REL, 5},          // 0x90
    Instruction{Operation::STA, AddressMode::IND_IDX, 5},      // 0x91
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0x92
    Instruction{Operation::SHA, AddressMode::IND_IDX, 5},      // 0x93
    Instruction{Operation::STY, AddressMode::ZPG_X, 5},        // 0x94
    Instruction{Operation::STA, AddressMode::ZPG_X, 5},        // 0x95
    Instruction{Operation::STX, AddressMode::ZPG_X, 5},        // 0x96
    Instruction{Operation::SAX, AddressMode::ZPG_Y, 5},        // 0x97
    Instruction{Operation::TYA, AddressMode::IMPL, 5},         // 0x98
    Instruction{Operation::STA, AddressMode::ABS_Y, 5},        // 0x99
    Instruction{Operation::TXS, AddressMode::IMPL, 5},         // 0x9A
    Instruction{Operation::TAS, AddressMode::ABS_Y, 5},        // 0x9B
    Instruction{Operation::SHY, AddressMode::ABS_X, 5},        // 0x9C
    Instruction{Operation::STA, AddressMode::ABS_X, 5},        // 0x9D
    Instruction{Operation::SHX, AddressMode::ABS_Y, 5},        // 0x9E
    Instruction{Operation::SHA, AddressMode::ABS_Y, 5},        // 0x9F

    Instruction{Operation::LDY, AddressMode::IMT, 5},          // 0xA0
    Instruction{Operation::LDA, AddressMode::IDX_IND, 5},          // 0xA1
    Instruction{Operation::LDX, AddressMode::IMT, 5},          // 0xA2
    Instruction{Operation::LAX, AddressMode::IDX_IND, 5},          // 0xA3
    Instruction{Operation::LDY, AddressMode::ZPG, 5},          // 0xA4
    Instruction{Operation::LDA, AddressMode::ZPG, 5},          // 0xA5
    Instruction{Operation::LDX, AddressMode::ZPG, 5},          // 0xA6
    Instruction{Operation::LAX, AddressMode::ZPG, 5},          // 0xA7
    Instruction{Operation::TAY, AddressMode::IMPL, 5},          // 0xA8
    Instruction{Operation::LDA, AddressMode::IMT, 5},          // 0xA9
    Instruction{Operation::TAX, AddressMode::IMPL, 5},          // 0xAA
    Instruction{Operation::LXA, AddressMode::IMT, 5},          // 0xAB
    Instruction{Operation::LDY, AddressMode::ABS, 5},          // 0xAC
    Instruction{Operation::LDA, AddressMode::ABS, 5},          // 0xAD
    Instruction{Operation::LDX, AddressMode::ABS, 5},          // 0xAE
    Instruction{Operation::LAX, AddressMode::ABS, 5},          // 0xAF

    Instruction{Operation::BCS, AddressMode::REL, 5},          // 0xB0
    Instruction{Operation::LDA, AddressMode::IND_IDX, 5},      // 0xB1
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0xB2
    Instruction{Operation::LAX, AddressMode::IND_IDX, 5},      // 0xB3
    Instruction{Operation::LDY, AddressMode::ZPG_X, 5},        // 0xB4
    Instruction{Operation::LDA, AddressMode::ZPG_X, 5},        // 0xB5
    Instruction{Operation::LDX, AddressMode::ZPG_Y, 5},        // 0xB6
    Instruction{Operation::LAX, AddressMode::ZPG_Y, 5},        // 0xB7
    Instruction{Operation::CLV, AddressMode::IMPL, 5},         // 0xB8
    Instruction{Operation::LDA, AddressMode::ABS_Y, 5},        // 0xB9
    Instruction{Operation::TSX, AddressMode::IMPL, 5},         // 0xBA
    Instruction{Operation::LAS, AddressMode::ABS_Y, 5},        // 0xBB
    Instruction{Operation::LDY, AddressMode::ABS_X, 5},        // 0xBC
    Instruction{Operation::LDA, AddressMode::ABS_X, 5},        // 0xBD
    Instruction{Operation::LDX, AddressMode::ABS_Y, 5},        // 0xBE
    Instruction{Operation::LAX, AddressMode::ABS_Y, 5},        // 0xBF

    Instruction{Operation::CPY, AddressMode::IMT, 5},          // 0xC0
    Instruction{Operation::CMP, AddressMode::IDX_IND, 5},      // 0xC1
    Instruction{Operation::NOP, AddressMode::IMT, 5},          // 0xC2
    Instruction{Operation::DCP, AddressMode::IDX_IND, 5},      // 0xC3
    Instruction{Operation::CPY, AddressMode::ZPG, 5},          // 0xC4
    Instruction{Operation::CMP, AddressMode::ZPG, 5},          // 0xC5
    Instruction{Operation::DEC, AddressMode::ZPG, 5},          // 0xC6
    Instruction{Operation::DCP, AddressMode::ZPG, 5},          // 0xC7
    Instruction{Operation::INY, AddressMode::IMPL, 5},         // 0xC8
    Instruction{Operation::CMP, AddressMode::IMT, 5},          // 0xC9
    Instruction{Operation::DEX, AddressMode::IMPL, 5},         // 0xCA
    Instruction{Operation::SBX, AddressMode::IMT, 5},          // 0xCB
    Instruction{Operation::CPY, AddressMode::ABS, 5},          // 0xCC
    Instruction{Operation::CMP, AddressMode::ABS, 5},          // 0xCD
    Instruction{Operation::DEC, AddressMode::ABS, 5},          // 0xCE
    Instruction{Operation::DCP, AddressMode::ABS, 5},          // 0xCF

    Instruction{Operation::BNE, AddressMode::REL, 5},          // 0xD0
    Instruction{Operation::CMP, AddressMode::IND_IDX, 5},      // 0xD1
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0xD2
    Instruction{Operation::DCP, AddressMode::IND_IDX, 5},      // 0xD3
    Instruction{Operation::NOP, AddressMode::ZPG_X, 5},        // 0xD4
    Instruction{Operation::CMP, AddressMode::ZPG_X, 5},        // 0xD5
    Instruction{Operation::DEC, AddressMode::ZPG_X, 5},        // 0xD6
    Instruction{Operation::DCP, AddressMode::ZPG_X, 5},        // 0xD7
    Instruction{Operation::CLD, AddressMode::IMPL, 5},         // 0xD8
    Instruction{Operation::CMP, AddressMode::ABS_Y, 5},        // 0xD9
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0xDA
    Instruction{Operation::DCP, AddressMode::ABS_Y, 5},        // 0xDB
    Instruction{Operation::NOP, AddressMode::ABS_X, 5},        // 0xDC
    Instruction{Operation::CMP, AddressMode::ABS_X, 5},        // 0xDD
    Instruction{Operation::DEC, AddressMode::ABS_X, 5},        // 0xDE
    Instruction{Operation::DCP, AddressMode::ABS_X, 5},        // 0xDF

    Instruction{Operation::CPX, AddressMode::IMT, 5},          // 0xE0
    Instruction{Operation::SBC, AddressMode::IDX_IND, 5},      // 0xE1
    Instruction{Operation::NOP, AddressMode::IMT, 5},          // 0xE2
    Instruction{Operation::ISC, AddressMode::IDX_IND, 5},      // 0xE3
    Instruction{Operation::CPX, AddressMode::ZPG, 5},          // 0xE4
    Instruction{Operation::SBC, AddressMode::ZPG, 5},          // 0xE5
    Instruction{Operation::INC, AddressMode::ZPG, 5},          // 0xE6
    Instruction{Operation::ISC, AddressMode::ZPG, 5},          // 0xE7
    Instruction{Operation::INX, AddressMode::IMPL, 5},         // 0xE8
    Instruction{Operation::SBC, AddressMode::IMT, 5},          // 0xE9
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0xEA
    Instruction{Operation::USBC, AddressMode::IMT, 5},         // 0xEB
    Instruction{Operation::CPX, AddressMode::ABS, 5},          // 0xEC
    Instruction{Operation::SBC, AddressMode::ABS, 5},          // 0xED
    Instruction{Operation::INC, AddressMode::ABS, 5},          // 0xEE
    Instruction{Operation::ISC, AddressMode::ABS, 5},          // 0xEF

    Instruction{Operation::BEQ, AddressMode::REL, 5},          // 0xF0
    Instruction{Operation::SBC, AddressMode::IND_IDX, 5},      // 0xF1
    Instruction{Operation::JAM, AddressMode::NONE, 5},         // 0xF2
    Instruction{Operation::ISC, AddressMode::IND_IDX, 5},      // 0xF3
    Instruction{Operation::NOP, AddressMode::ZPG_X, 5},        // 0xF4
    Instruction{Operation::SBC, AddressMode::ZPG_X, 5},        // 0xF5
    Instruction{Operation::INC, AddressMode::ZPG_X, 5},        // 0xF6
    Instruction{Operation::ISC, AddressMode::ZPG_X, 5},        // 0xF7
    Instruction{Operation::SED, AddressMode::IMPL, 5},         // 0xF8
    Instruction{Operation::SBC, AddressMode::ABS_Y, 5},        // 0xF9
    Instruction{Operation::NOP, AddressMode::IMPL, 5},         // 0xFA
    Instruction{Operation::ISC, AddressMode::ABS_Y, 5},        // 0xFB
    Instruction{Operation::NOP, AddressMode::ABS_X, 5},        // 0xFC
    Instruction{Operation::SBC, AddressMode::ABS_X, 5},        // 0xFD
    Instruction{Operation::INC, AddressMode::ABS_X, 5},        // 0xFE
    Instruction{Operation::ISC, AddressMode::ABS_X, 5},        // 0xFF
};

struct InstructionState {
    Instruction instruction{};
    uint16_t address = 0;
    uint8_t cycle = 0;
    bool branchTaken = false;
    bool pageCrossed = false;
};

class CPU6502 {
public:
    void connectNMIInterruptLine(InterruptLine&);
    void connectAPUInterruptLine(InterruptLine&);
    void connectMapperInterruptLine(InterruptLine&);
    void connectBus(Bus& b);
    void startDMA(uint8_t);
    void execute();
private:
    [[nodiscard]] uint16_t readAddress(uint16_t) const;
    uint8_t pop();
    void push(uint8_t value);
    void pollInterrupts();
    void branch(uint8_t mask, uint8_t predicate);
    void reset();
    void prepareBranch();
    static bool pageCrossed(uint16_t base, uint16_t addr) ;
    [[nodiscard]] uint16_t getAddress(Instruction instruction);
    InstructionState current;
    uint8_t a = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint16_t pc = 0;
    uint8_t sp = 0xfd;
    uint8_t sr = 0x34;
    bool oddCycle = false;
    Bus* bus = nullptr;
    InterruptLine* nmiIRQ = nullptr;
    InterruptLine* apuIRQ = nullptr;
    InterruptLine* mapperIRQ = nullptr;

    void setFlag(const StatusFlag flag, const bool value) {
        if (value) {
            sr |= flag;
        } else {
            sr &= ~flag;
        }
    }

    [[nodiscard]] bool getFlag(const StatusFlag flag) const {
        return sr & flag;
    }

    void updateZN(uint8_t value) {
        setFlag(ZERO, value == 0);
        setFlag(NEGATIVE, value & 0x80);
    }

};


#endif //LINES_CPU6502_H