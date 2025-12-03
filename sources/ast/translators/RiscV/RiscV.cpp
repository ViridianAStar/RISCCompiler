#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory_resource>

/**
 * AI HAS BEEN USED TO MODIFY CODE IN THIS FILE.
 * --Readability-- | --Comments-- | --Structure--
 */
class RiscV {
    template<typename K, typename V>
    using MAP = std::pmr::unordered_map<K, V>;

    using STR = std::string;

public:
    /**
     * ISA Extensions and "Selector"
     */
    enum ISAExt {
        RV32I = 1 << 0,
        RV64I = 1 << 1,
        M = 1 << 2,
        A = 1 << 3,
        F = 1 << 4,
        D = 1 << 5,
        C = 1 << 6,
        SYSTEM = 1 << 7,
        Zicsr = 1 << 8,
        Zifencei = 1 << 9
    };

    /**
     * Profile Selector
     */
    struct Profiles {
        static constexpr uint32_t RV32I = ISAExt::RV32I;
        static constexpr uint32_t RV64I = ISAExt::RV64I;
        static constexpr uint32_t RV64IM = RV64I | ISAExt::M;
        static constexpr uint32_t RV64GC = RV64IM | ISAExt::A | ISAExt::F |
                                           ISAExt::D | ISAExt::C |
                                           ISAExt::Zicsr | ISAExt::Zifencei;

        // Add constants for extensions used in Opcodes
        static constexpr uint32_t M = ISAExt::M;
        static constexpr uint32_t SYSTEM = ISAExt::SYSTEM;
        static constexpr uint32_t Zicsr = ISAExt::Zicsr;
        static constexpr uint32_t Zifencei = ISAExt::Zifencei;
    };

    /**
     * Struct for opcodes
     */
    struct OpcodeInfo {
        uint8_t opcode; // 7-bit opcode
        uint8_t funct3; // 3-bit funct3
        uint8_t funct7; // 7-bit funct7
        char type; // R I S B U J
        uint32_t extMask; // which ISA extensions this instruction belongs to
    };

    /**
     * Struct for registers
     */
    struct RegInfo {
        uint8_t code;
        const char *name;
    };

    /**
     * Enum of all possible operations
     */
    enum operationTable {
        add, addi, sub, neg,
        mul, mulh, mulhu, mulhsu,
        div, rem,

        AND, ANDi,
        OR, ORi,
        XOR, XORi,
        NOT,

        sll, slli,
        srl, srli,
        sra, srai,

        slt, slti,
        sltu, sltiu,

        // loads / stores
        lb, lbu,
        lh, lhu,
        lw,
        sb, sh, sw,

        // RV64 loads/stores
        lwu, ld,
        sd,

        // upper immediates
        lui, auipc,

        // jumps
        jal, jalr,

        // branches
        beq, bne, blt, bge, bltu, bgeu,

        // system
        ecall, ebreak,

        // CSR reads
        rdcycle, rdcycleh,
        rdtime, rdtimeh,
        rdinstret, rdinstreth,

        // fence
        fence,

        // pseudoinstructions
        mv, nop, li, la, j, call, ret,
        beqz, bnez, bltz, bgez
    };

    /**
     * Enum of all standard registers
     */
    enum regTable {
        zero, ra, sp, gp, tp,
        t0, t1, t2,
        fp, s0,
        s1,
        a0, a1, a2, a3, a4, a5, a6, a7,
        s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
        t3, t4, t5, t6
    };

    /**
     * Default ISA
     */
    uint32_t EnabledISA = Profiles::RV64I;

    /**
     * constructs a RiscV translation object and selects the ISA from the provided profile.
     *
     * @param isaMask isaMask to enable
     */
    explicit RiscV(const uint32_t isaMask = Profiles::RV64I)
        : EnabledISA(isaMask) {
    }

    /**
     * Determines the legality of an operation on a given ISA
     *
     * @param op operation
     * @return if operation is legal on selected ISA
     */
    bool isLegal(const operationTable op) const {
        const auto it = Opcodes.find(op);
        if (it == Opcodes.end()) return false;
        return (it->second.extMask & EnabledISA) != 0;
    }

    /**
     * Enable given extension
     *
     * @param extMask extension to enable
     */
    void enableExtension(const uint32_t extMask) {
        EnabledISA |= extMask;
    }

    /**
     * Disable given extension
     *
     * @param extMask extension to disable
     */
    void disableExtension(const uint32_t extMask) {
        EnabledISA &= ~extMask;
    }

    /**
     * Check if an extension is enabled.
     *
     * @param extMask extension to check for
     * @return true if enabled false otherwise
     */
    bool isExtensionEnabled(const uint32_t extMask) const {
        return (EnabledISA & extMask) != 0;
    }


    /**
     * Table of Registers and their related codes and names.
     */
    const MAP<regTable, RegInfo> Registers = {
        {zero, {0, "x0"}}, {ra, {1, "x1"}}, {sp, {2, "x2"}},
        {gp, {3, "x3"}}, {tp, {4, "x4"}},

        {t0, {5, "x5"}}, {t1, {6, "x6"}}, {t2, {7, "x7"}},

        {fp, {8, "x8"}}, {s0, {8, "x8"}},
        {s1, {9, "x9"}},

        {a0, {10, "x10"}}, {a1, {11, "x11"}}, {a2, {12, "x12"}},
        {a3, {13, "x13"}}, {a4, {14, "x14"}}, {a5, {15, "x15"}},
        {a6, {16, "x16"}}, {a7, {17, "x17"}},

        {s2, {18, "x18"}}, {s3, {19, "x19"}}, {s4, {20, "x20"}},
        {s5, {21, "x21"}}, {s6, {22, "x22"}}, {s7, {23, "x23"}},
        {s8, {24, "x24"}}, {s9, {25, "x25"}}, {s10, {26, "x26"}},
        {s11, {27, "x27"}},

        {t3, {28, "x28"}}, {t4, {29, "x29"}},
        {t5, {30, "x30"}}, {t6, {31, "x31"}}
    };

    /**
     * Table of Operations and their opcodes.
     */
    const MAP<operationTable, OpcodeInfo> Opcodes = {

        // ---------------- R-Type ----------------
        {add, {0b0110011, 0b000, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {sub, {0b0110011, 0b000, 0b0100000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {AND, {0b0110011, 0b111, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {OR, {0b0110011, 0b110, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {XOR, {0b0110011, 0b100, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {sll, {0b0110011, 0b001, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {srl, {0b0110011, 0b101, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {sra, {0b0110011, 0b101, 0b0100000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {slt, {0b0110011, 0b010, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},
        {sltu, {0b0110011, 0b011, 0b0000000, 'R', Profiles::RV32I | Profiles::RV64I}},

        // ---------------- M-extension ----------------
        {mul, {0b0110011, 0b000, 0b0000001, 'R', Profiles::RV64IM}},
        {mulh, {0b0110011, 0b001, 0b0000001, 'R', Profiles::RV64IM}},
        {mulhu, {0b0110011, 0b011, 0b0000001, 'R', Profiles::RV64IM}},
        {mulhsu, {0b0110011, 0b010, 0b0000001, 'R', Profiles::RV64IM}},
        {div, {0b0110011, 0b100, 0b0000001, 'R', Profiles::RV64IM}},
        {rem, {0b0110011, 0b110, 0b0000001, 'R', Profiles::RV64IM}},

        // ---------------- I-Type ----------------
        {addi, {0b0010011, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {ANDi, {0b0010011, 0b111, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {ORi, {0b0010011, 0b110, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {XORi, {0b0010011, 0b100, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {slli, {0b0010011, 0b001, 0b0000000, 'I', Profiles::RV32I | Profiles::RV64I}},
        {srli, {0b0010011, 0b101, 0b0000000, 'I', Profiles::RV32I | Profiles::RV64I}},
        {srai, {0b0010011, 0b101, 0b0100000, 'I', Profiles::RV32I | Profiles::RV64I}},
        {slti, {0b0010011, 0b010, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {sltiu, {0b0010011, 0b011, 0, 'I', Profiles::RV32I | Profiles::RV64I}},

        // ---------------- Loads ----------------
        {lb, {0b0000011, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {lbu, {0b0000011, 0b100, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {lh, {0b0000011, 0b001, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {lhu, {0b0000011, 0b101, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {lw, {0b0000011, 0b010, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {lwu, {0b0000011, 0b110, 0, 'I', Profiles::RV64I}},
        {ld, {0b0000011, 0b011, 0, 'I', Profiles::RV64I}},

        // ---------------- Stores ----------------
        {sb, {0b0100011, 0b000, 0, 'S', Profiles::RV32I | Profiles::RV64I}},
        {sh, {0b0100011, 0b001, 0, 'S', Profiles::RV32I | Profiles::RV64I}},
        {sw, {0b0100011, 0b010, 0, 'S', Profiles::RV32I | Profiles::RV64I}},
        {sd, {0b0100011, 0b011, 0, 'S', Profiles::RV64I}},

        // ---------------- Upper immediates ----------------
        {lui, {0b0110111, 0, 0, 'U', Profiles::RV32I | Profiles::RV64I}},
        {auipc, {0b0010111, 0, 0, 'U', Profiles::RV32I | Profiles::RV64I}},

        // ---------------- Jumps ----------------
        {jal, {0b1101111, 0, 0, 'J', Profiles::RV32I | Profiles::RV64I}},
        {jalr, {0b1100111, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},

        // ---------------- Branches ----------------
        {beq, {0b1100011, 0b000, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bne, {0b1100011, 0b001, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {blt, {0b1100011, 0b100, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bge, {0b1100011, 0b101, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bltu, {0b1100011, 0b110, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bgeu, {0b1100011, 0b111, 0, 'B', Profiles::RV32I | Profiles::RV64I}},

        // ---------------- System ----------------
        {ecall, {0b1110011, 0b000, 0, 'I', Profiles::SYSTEM}},
        {ebreak, {0b1110011, 0b000, 1, 'I', Profiles::SYSTEM}},

        // ---------------- CSR reads (Zicsr) ----------------
        {rdcycle, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},
        {rdcycleh, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},
        {rdtime, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},
        {rdtimeh, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},
        {rdinstret, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},
        {rdinstreth, {0b1110011, 0b010, 0, 'I', Profiles::Zicsr}},

        // ---------------- FENCE (Zifencei) ----------------
        {fence, {0b0001111, 0b000, 0, 'I', Profiles::Zifencei}},

        // ---------------- Pseudoinstructions ----------------
        {mv, {0b0010011, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {nop, {0b0010011, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {li, {0b0010011, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},
        {la, {0b0010111, 0, 0, 'U', Profiles::RV32I | Profiles::RV64I}},
        {j, {0b1101111, 0, 0, 'J', Profiles::RV32I | Profiles::RV64I}},
        {call, {0b1101111, 0, 0, 'J', Profiles::RV32I | Profiles::RV64I}},
        {ret, {0b1100111, 0b000, 0, 'I', Profiles::RV32I | Profiles::RV64I}},

        {beqz, {0b1100011, 0b000, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bnez, {0b1100011, 0b001, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bltz, {0b1100011, 0b100, 0, 'B', Profiles::RV32I | Profiles::RV64I}},
        {bgez, {0b1100011, 0b101, 0, 'B', Profiles::RV32I | Profiles::RV64I}}
    };

    /**
     * Struct for building instructions
     */
    struct Instruction {
        RiscV::operationTable op; // Which operation (add, sub, lw, etc.)
        uint8_t rd = 0; // Destination register
        uint8_t rs1 = 0; // Source register 1
        uint8_t rs2 = 0; // Source register 2
        int32_t imm = 0; // Immediate (for I, S, B, U, J types)

        /**
         * Encode the instruction into a 32-bit word based on type.
         * Returns 0 if operation is unknown.
         */
        [[nodiscard]] uint32_t encode(const RiscV &cpu) const {
            const auto it = cpu.Opcodes.find(op);
            if (it == cpu.Opcodes.end()) return 0;

            const auto &info = it->second;
            uint32_t word = 0;

            switch (info.type) {
                case 'R':
                    word = (info.funct7 << 25) | (rs2 << 20) | (rs1 << 15) |
                           (info.funct3 << 12) | (rd << 7) | info.opcode;
                    break;

                case 'I':
                    word = ((imm & 0xFFF) << 20) | (rs1 << 15) |
                           (info.funct3 << 12) | (rd << 7) | info.opcode;
                    break;

                case 'S': {
                    const uint32_t imm11_5 = (imm >> 5) & 0x7F;
                    const uint32_t imm4_0 = imm & 0x1F;
                    word = (imm11_5 << 25) | (rs2 << 20) | (rs1 << 15) |
                           (info.funct3 << 12) | (imm4_0 << 7) | info.opcode;
                    break;
                }

                case 'B': {
                    const uint32_t imm12 = (imm >> 12) & 0x1;
                    const uint32_t imm10_5 = (imm >> 5) & 0x3F;
                    const uint32_t imm4_1 = (imm >> 1) & 0xF;
                    const uint32_t imm11 = (imm >> 11) & 0x1;
                    word = (imm12 << 31) | (imm11 << 7) | (imm10_5 << 25) |
                           (rs2 << 20) | (rs1 << 15) | (info.funct3 << 12) |
                           (imm4_1 << 8) | info.opcode;
                    break;
                }

                case 'U':
                    word = ((imm & 0xFFFFF000)) | (rd << 7) | info.opcode;
                    break;

                case 'J': {
                    const uint32_t imm20 = (imm >> 20) & 0x1;
                    const uint32_t imm10_1 = (imm >> 1) & 0x3FF;
                    const uint32_t imm11 = (imm >> 11) & 0x1;
                    const uint32_t imm19_12 = (imm >> 12) & 0xFF;
                    word = (imm20 << 31) | (imm19_12 << 12) | (imm11 << 20) |
                           (imm10_1 << 21) | (rd << 7) | info.opcode;
                    break;
                }

                default:
                    break;
            }

            return word;
        }
    };
};
