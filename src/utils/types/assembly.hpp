// Copyright 2020 xensik. All rights reserved.
//
// Use of this source code is governed by a GNU GPLv3 license
// that can be found in the LICENSE file.

#ifndef _LUI_ASSEMBLY_HPP_
#define _LUI_ASSEMBLY_HPP_

namespace lui
{

struct data
{
enum class t
{
    NIL, BOOLEAN, LIGHTUSERDATA, NUMBER, STRING, TABLE, FUNCTION,
    USERDATA, THREAD, IFUNCTION, CFUNCTION, UI64, STRUCT,
};
    t type_;
    std::string value_;

    data(t type, const std::string& value): type_(type), value_(value) {}

    auto type() -> t
    {
        return type_;
    }

    auto value() -> std::string
    {
        return value_;
    }

    void to_literal()
    {
        value_ = "\""s += value_ += "\"";
    }
};

struct kst
{
    data data_;

    kst(data::t type, const std::string& value): data_(data(type, value)) {}

    auto print() -> std::string
    {
        return "KST(" + data_.value_ + ")";
    }
};

struct reg
{
    std::uint32_t   index_;
    data            data_;

    reg(std::uint32_t index): index_(index), data_(lui::data(data::t::NIL, "nil")) {}

    auto print() -> std::string
    {
        return utils::string::va("REG(%02X)", index_);
    }
};

struct type_info
{
    std::uint32_t id;
    std::string name;

    type_info(std::uint32_t id, const std::string& name) : id(id), name(name) {}
};

struct instruction
{
    std::uint32_t index;
    std::uint32_t value;
    std::string data;

    std::uint8_t OP;
    std::uint32_t A;
    std::uint32_t B;
    std::uint32_t C;
    std::uint32_t Bx;
    std::int32_t sBx;
    bool sZero;

    instruction(std::uint32_t index, std::uint32_t value, std::uint8_t OP, std::uint32_t A,
        std::uint32_t B, std::uint32_t C, std::uint32_t Bx, std::int32_t sBx, bool sZero)
        : index(index),value(value), OP(OP), A(A), B(B), C(C), Bx(Bx), sBx(sBx), 
            sZero(sZero) {}
};

struct function
{
    // String source_name               -- IW engine stripped
    // Integer line_defined             -- IW engine stripped
    // Integer last_line_defined        -- IW engine stripped
    std::uint32_t upval_count;
    std::uint32_t param_count;
    std::uint8_t  vararg_flags;         // 1=VARARG_HASARG, 2=VARARG_ISVARARG, 4=VARARG_NEEDSARG
    std::uint32_t register_count;       // -- maximum stack size
    // List instructions
    std::uint64_t instruction_count;
    std::vector<std::unique_ptr<instruction>> instructions;
    // List constants
    std::uint32_t constant_count;
    std::vector<kst> constants;

    std::uint32_t debug;
    std::uint32_t sub_func_count;
    std::vector<std::unique_ptr<function>> sub_funcs;
    // List src line position list      -- IW engine stripped
    // List local var list              -- IW engine stripped
    // List upvalue list                -- IW engine stripped

    std::string name;
};

struct header
{
    std::uint32_t magic;                // HKS_DUMP_SIGNATURE 1B 4C 75 61 : 0x61754C1B
    std::uint8_t lua_version;           // 0x51
    std::uint8_t format_version;        // 0x0D
    std::uint8_t endianness;            // 0x01
    std::uint8_t size_of_int;           // 0x04
    std::uint8_t size_of_size_t;        //  ?
    std::uint8_t size_of_intruction;    // 0x04
    std::uint8_t size_of_lua_number;    // 0x04
    std::uint8_t integral_flag;         // 0x01
    std::uint8_t build_flags;           // 0x03
    std::uint8_t referenced_mode;       // 0x01
    std::uint32_t type_count;           // 13
    std::vector<type_info> types;
};

struct file
{
    header header;
    std::unique_ptr<function> main;
};

using kst_ptr = std::unique_ptr<kst>;
using header_ptr = std::unique_ptr<header>;
using instruction_ptr = std::unique_ptr<instruction>;
using function_ptr = std::unique_ptr<function>;
using file_ptr = std::unique_ptr<file>;

} // namespace lui

#endif // _LUI_ASSEMBLY_HPP_
