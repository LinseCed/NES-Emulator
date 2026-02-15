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
    OVERFLOW = 1 << 6,
    NEGATIVE = 1 << 7,
};

enum class OpCode : uint8_t {
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
    IND,
    IDX_IND,
    IND_IDX,
};

struct Instruction {
    OpCode opcode;
    AddressMode mode;
    uint8_t cycles;
};

constexpr Instruction instructions[256] = {
    Instruction{OpCode::BRK, AddressMode::IMPL,7},          // 0x00
    Instruction{OpCode::ORA, AddressMode::IDX_IND, 6},      // 0x01
    Instruction{OpCode::JAM, AddressMode::NONE, 0},         // 0x02
    Instruction{OpCode::SLO, AddressMode::IDX_IND, 8},      // 0x03
    Instruction{OpCode::NOP, AddressMode::ZPG, 3},          // 0x04
    Instruction{OpCode::ORA, AddressMode::ZPG, 3},          // 0x05
    Instruction{OpCode::ASL, AddressMode::ZPG, 5},          // 0x06
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x07
    Instruction{OpCode::PHP, AddressMode::IMPL, 3},         // 0x08
    Instruction{OpCode::ORA, AddressMode::IMT, 2},          // 0x09
    Instruction{OpCode::ASL, AddressMode::ACC, 2},          // 0x0A
    Instruction{OpCode::ANC, AddressMode::IMT, 2},          // 0x0B
    Instruction{OpCode::NOP, AddressMode::ABS, 4},          // 0x0C
    Instruction{OpCode::ORA, AddressMode::ABS, 4},          // 0x0D
    Instruction{OpCode::ASL, AddressMode::ABS, 6},          // 0x0E
    Instruction{OpCode::SLO, AddressMode::ABS, 6},          // 0x0F

    Instruction{OpCode::BPL, AddressMode::REL, 2},          // 0x10
    Instruction{OpCode::ORA, AddressMode::IND_IDX, 5},      // 0x11
    Instruction{OpCode::JAM, AddressMode::NONE, 0},         // 0x12
    Instruction{OpCode::SLO, AddressMode::IND_IDX, 8},      // 0x13
    Instruction{OpCode::NOP, AddressMode::ZPG_X, 4},        // 0x14
    Instruction{OpCode::ORA, AddressMode::ZPG_X, 4},        // 0x15
    Instruction{OpCode::ASL, AddressMode::ZPG_X, 6},        // 0x16
    Instruction{OpCode::SLO, AddressMode::ZPG_X, 6},        // 0x17
    Instruction{OpCode::CLC, AddressMode::IMPL, 2},         // 0x18
    Instruction{OpCode::ORA, AddressMode::ABS_Y, 4},        // 0x19
    Instruction{OpCode::NOP, AddressMode::IMPL, 2},         // 0x1A
    Instruction{OpCode::SLO, AddressMode::ABS_Y, 7},        // 0x1B
    Instruction{OpCode::NOP, AddressMode::ABS_X, 4},        // 0x1C
    Instruction{OpCode::ORA, AddressMode::ABS_X, 4},        // 0x1D
    Instruction{OpCode::ASL, AddressMode::ABS_X, 7},        // 0x1E
    Instruction{OpCode::SLO, AddressMode::ABS_X, 7},        // 0x1F

    // ----- Cycles wrong

    Instruction{OpCode::JSR, AddressMode::ABS, 5},          // 0x20
    Instruction{OpCode::AND, AddressMode::IDX_IND, 5},          // 0x21
    Instruction{OpCode::JAM, AddressMode::NONE, 5},          // 0x22
    Instruction{OpCode::RLA, AddressMode::IDX_IND, 5},          // 0x23
    Instruction{OpCode::BIT, AddressMode::ZPG, 5},          // 0x24
    Instruction{OpCode::AND, AddressMode::ZPG, 5},          // 0x25
    Instruction{OpCode::ROL, AddressMode::ZPG, 5},          // 0x26
    Instruction{OpCode::RLA, AddressMode::ZPG, 5},          // 0x27
    Instruction{OpCode::PLP, AddressMode::IMPL, 5},          // 0x28
    Instruction{OpCode::AND, AddressMode::IMT, 5},          // 0x29
    Instruction{OpCode::ROL, AddressMode::ACC, 5},          // 0x2A
    Instruction{OpCode::ANC, AddressMode::IMT, 5},          // 0x2B
    Instruction{OpCode::BIT, AddressMode::ABS, 5},          // 0x2C
    Instruction{OpCode::AND, AddressMode::ABS, 5},          // 0x2D
    Instruction{OpCode::ROL, AddressMode::ABS, 5},          // 0x2E
    Instruction{OpCode::RLA, AddressMode::ABS, 5},          // 0x2F

    Instruction{OpCode::BMI, AddressMode::ZPG, 5},          // 0x30
    Instruction{OpCode::AND, AddressMode::ZPG, 5},          // 0x31
    Instruction{OpCode::JAM, AddressMode::ZPG, 5},          // 0x32
    Instruction{OpCode::RLA, AddressMode::ZPG, 5},          // 0x33
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x34
    Instruction{OpCode::AND, AddressMode::ZPG, 5},          // 0x35
    Instruction{OpCode::ROL, AddressMode::ZPG, 5},          // 0x36
    Instruction{OpCode::RLA, AddressMode::ZPG, 5},          // 0x37
    Instruction{OpCode::SEC, AddressMode::ZPG, 5},          // 0x38
    Instruction{OpCode::AND, AddressMode::ZPG, 5},          // 0x39
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x3A
    Instruction{OpCode::RLA, AddressMode::ZPG, 5},          // 0x3B
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x3C
    Instruction{OpCode::AND, AddressMode::ZPG, 5},          // 0x3D
    Instruction{OpCode::ROL, AddressMode::ZPG, 5},          // 0x3E
    Instruction{OpCode::RLA, AddressMode::ZPG, 5},          // 0x3F

    Instruction{OpCode::RTI, AddressMode::ZPG, 5},          // 0x40
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x41
    Instruction{OpCode::JAM, AddressMode::ZPG, 5},          // 0x42
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x43
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x44
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x45
    Instruction{OpCode::LSR, AddressMode::ZPG, 5},          // 0x46
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x47
    Instruction{OpCode::CLI, AddressMode::ZPG, 5},          // 0x48
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x49
    Instruction{OpCode::LSR, AddressMode::ZPG, 5},          // 0x4A
    Instruction{OpCode::ALR, AddressMode::ZPG, 5},          // 0x4B
    Instruction{OpCode::JMP, AddressMode::ZPG, 5},          // 0x4C
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x4D
    Instruction{OpCode::LSR, AddressMode::ZPG, 5},          // 0x4E
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x4F

    Instruction{OpCode::BVC, AddressMode::ZPG, 5},          // 0x50
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x51
    Instruction{OpCode::JAM, AddressMode::ZPG, 5},          // 0x52
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x53
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x54
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x55
    Instruction{OpCode::LSR, AddressMode::ZPG, 5},          // 0x56
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x57
    Instruction{OpCode::CLI, AddressMode::ZPG, 5},          // 0x58
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x59
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x5A
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x5B
    Instruction{OpCode::NOP, AddressMode::ZPG, 5},          // 0x5C
    Instruction{OpCode::EOR, AddressMode::ZPG, 5},          // 0x5D
    Instruction{OpCode::LSR, AddressMode::ZPG, 5},          // 0x5E
    Instruction{OpCode::SRE, AddressMode::ZPG, 5},          // 0x5F

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x60
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x61
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x62
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x63
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x64
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x65
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x66
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x67
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x68
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x69
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6A
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6B
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6C
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6D
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6E
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x6F

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x70
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x71
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x72
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x73
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x74
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x75
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x76
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x77
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x78
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x79
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7A
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7B
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7C
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7D
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7E
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x7F

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x80
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x81
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x82
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x83
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x84
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x85
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x86
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x87
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x88
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x89
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8A
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8B
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8C
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8D
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8E
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x8F

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x90
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x91
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x92
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x93
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x94
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x95
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x96
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x97
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x98
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x99
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9A
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9B
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9C
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9D
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9E
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0x9F

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xA9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAD
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xAF

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xB9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBD
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xBF

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xC9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCD
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xCF

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xD9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDD
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xDF

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xE9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xEA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xEB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xEC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xED
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xEE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xEF

    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF0
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF1
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF2
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF3
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF4
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF5
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF6
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF7
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF8
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xF9
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFA
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFB
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFC
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFD
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFE
    Instruction{OpCode::SLO, AddressMode::ZPG, 5},          // 0xFF
};

class CPU6502 {
public:
    void connectNMIInterruptLine(InterruptLine&);
    void connectAPUInterruptLine(InterruptLine&);
    void connectMapperInterruptLine(InterruptLine&);
    void connectBus(Bus& b);
private:
    [[nodiscard]] uint16_t readAddress(uint16_t) const;
    uint8_t pop();
    void push(uint8_t value);
    void pollInterrupts();
    void execute();
    void reset();
    uint8_t a = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint16_t pc = 0;
    uint8_t sp = 0xfd;
    uint8_t sr = 0x34;
    bool oddCycle = false;
    uint8_t instructionCycle = 0;
    Bus* bus = nullptr;
    InterruptLine* nmiIRQ = nullptr;
    InterruptLine* apuIRQ = nullptr;
    InterruptLine* mapperIRQ = nullptr;
};


#endif //LINES_CPU6502_H