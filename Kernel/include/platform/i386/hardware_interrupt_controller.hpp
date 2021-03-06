#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP

#include "idt.hpp"
#include <platform/pc/pic.hpp>
#include <interrupt_asm.h>
#include <seg.h>

extern __attribute__((noreturn)) void on_interrupt_during_panic() noexcept; // in panic.cpp
 
extern "C" {
extern void isr_wrapper_0(void);
extern void isr_wrapper_1(void);
extern void isr_wrapper_2(void);
extern void isr_wrapper_3(void);
extern void isr_wrapper_4(void);
extern void isr_wrapper_5(void);
extern void isr_wrapper_6(void);
extern void isr_wrapper_7(void);
extern void isr_wrapper_8(void);
extern void isr_wrapper_9(void);
extern void isr_wrapper_10(void);
extern void isr_wrapper_11(void);
extern void isr_wrapper_12(void);
extern void isr_wrapper_13(void);
extern void isr_wrapper_14(void);
extern void isr_wrapper_16(void);
extern void isr_wrapper_17(void);
extern void isr_wrapper_18(void);
extern void isr_wrapper_19(void);
extern void isr_wrapper_32(void);
extern void isr_wrapper_33(void);
extern void isr_wrapper_34(void);
extern void isr_wrapper_35(void);
extern void isr_wrapper_36(void);
extern void isr_wrapper_37(void);
extern void isr_wrapper_38(void);
extern void isr_wrapper_39(void);
extern void isr_wrapper_40(void);
extern void isr_wrapper_41(void);
extern void isr_wrapper_42(void);
extern void isr_wrapper_43(void);
extern void isr_wrapper_44(void);
extern void isr_wrapper_45(void);
extern void isr_wrapper_46(void);
extern void isr_wrapper_47(void);
extern void isr_wrapper_48(void);
extern void isr_wrapper_49(void);
extern void isr_wrapper_50(void);
extern void isr_wrapper_51(void);
extern void isr_wrapper_52(void);
extern void isr_wrapper_53(void);
extern void isr_wrapper_54(void);
extern void isr_wrapper_55(void);
extern void isr_wrapper_56(void);
extern void isr_wrapper_57(void);
extern void isr_wrapper_58(void);
extern void isr_wrapper_59(void);
extern void isr_wrapper_60(void);
extern void isr_wrapper_61(void);
extern void isr_wrapper_62(void);
extern void isr_wrapper_63(void);
extern void isr_wrapper_64(void);
extern void isr_wrapper_65(void);
extern void isr_wrapper_66(void);
extern void isr_wrapper_67(void);
extern void isr_wrapper_68(void);
extern void isr_wrapper_69(void);
extern void isr_wrapper_70(void);
extern void isr_wrapper_71(void);
extern void isr_wrapper_72(void);
extern void isr_wrapper_73(void);
extern void isr_wrapper_74(void);
extern void isr_wrapper_75(void);
extern void isr_wrapper_76(void);
extern void isr_wrapper_77(void);
extern void isr_wrapper_78(void);
extern void isr_wrapper_79(void);
extern void isr_wrapper_80(void);
extern void isr_wrapper_81(void);
extern void isr_wrapper_82(void);
extern void isr_wrapper_83(void);
extern void isr_wrapper_84(void);
extern void isr_wrapper_85(void);
extern void isr_wrapper_86(void);
extern void isr_wrapper_87(void);
extern void isr_wrapper_88(void);
extern void isr_wrapper_89(void);
extern void isr_wrapper_90(void);
extern void isr_wrapper_91(void);
extern void isr_wrapper_92(void);
extern void isr_wrapper_93(void);
extern void isr_wrapper_94(void);
extern void isr_wrapper_95(void);
extern void isr_wrapper_96(void);
extern void isr_wrapper_97(void);
extern void isr_wrapper_98(void);
extern void isr_wrapper_99(void);
extern void isr_wrapper_100(void);
extern void isr_wrapper_101(void);
extern void isr_wrapper_102(void);
extern void isr_wrapper_103(void);
extern void isr_wrapper_104(void);
extern void isr_wrapper_105(void);
extern void isr_wrapper_106(void);
extern void isr_wrapper_107(void);
extern void isr_wrapper_108(void);
extern void isr_wrapper_109(void);
extern void isr_wrapper_110(void);
extern void isr_wrapper_111(void);
extern void isr_wrapper_112(void);
extern void isr_wrapper_113(void);
extern void isr_wrapper_114(void);
extern void isr_wrapper_115(void);
extern void isr_wrapper_116(void);
extern void isr_wrapper_117(void);
extern void isr_wrapper_118(void);
extern void isr_wrapper_119(void);
extern void isr_wrapper_120(void);
extern void isr_wrapper_121(void);
extern void isr_wrapper_122(void);
extern void isr_wrapper_123(void);
extern void isr_wrapper_124(void);
extern void isr_wrapper_125(void);
extern void isr_wrapper_126(void);
extern void isr_wrapper_127(void);
extern void isr_wrapper_128(void);
extern void isr_wrapper_129(void);
extern void isr_wrapper_130(void);
extern void isr_wrapper_131(void);
extern void isr_wrapper_132(void);
extern void isr_wrapper_133(void);
extern void isr_wrapper_134(void);
extern void isr_wrapper_135(void);
extern void isr_wrapper_136(void);
extern void isr_wrapper_137(void);
extern void isr_wrapper_138(void);
extern void isr_wrapper_139(void);
extern void isr_wrapper_140(void);
extern void isr_wrapper_141(void);
extern void isr_wrapper_142(void);
extern void isr_wrapper_143(void);
extern void isr_wrapper_144(void);
extern void isr_wrapper_145(void);
extern void isr_wrapper_146(void);
extern void isr_wrapper_147(void);
extern void isr_wrapper_148(void);
extern void isr_wrapper_149(void);
extern void isr_wrapper_150(void);
extern void isr_wrapper_151(void);
extern void isr_wrapper_152(void);
extern void isr_wrapper_153(void);
extern void isr_wrapper_154(void);
extern void isr_wrapper_155(void);
extern void isr_wrapper_156(void);
extern void isr_wrapper_157(void);
extern void isr_wrapper_158(void);
extern void isr_wrapper_159(void);
extern void isr_wrapper_160(void);
extern void isr_wrapper_161(void);
extern void isr_wrapper_162(void);
extern void isr_wrapper_163(void);
extern void isr_wrapper_164(void);
extern void isr_wrapper_165(void);
extern void isr_wrapper_166(void);
extern void isr_wrapper_167(void);
extern void isr_wrapper_168(void);
extern void isr_wrapper_169(void);
extern void isr_wrapper_170(void);
extern void isr_wrapper_171(void);
extern void isr_wrapper_172(void);
extern void isr_wrapper_173(void);
extern void isr_wrapper_174(void);
extern void isr_wrapper_175(void);
extern void isr_wrapper_176(void);
extern void isr_wrapper_177(void);
extern void isr_wrapper_178(void);
extern void isr_wrapper_179(void);
extern void isr_wrapper_180(void);
extern void isr_wrapper_181(void);
extern void isr_wrapper_182(void);
extern void isr_wrapper_183(void);
extern void isr_wrapper_184(void);
extern void isr_wrapper_185(void);
extern void isr_wrapper_186(void);
extern void isr_wrapper_187(void);
extern void isr_wrapper_188(void);
extern void isr_wrapper_189(void);
extern void isr_wrapper_190(void);
extern void isr_wrapper_191(void);
extern void isr_wrapper_192(void);
extern void isr_wrapper_193(void);
extern void isr_wrapper_194(void);
extern void isr_wrapper_195(void);
extern void isr_wrapper_196(void);
extern void isr_wrapper_197(void);
extern void isr_wrapper_198(void);
extern void isr_wrapper_199(void);
extern void isr_wrapper_200(void);
extern void isr_wrapper_201(void);
extern void isr_wrapper_202(void);
extern void isr_wrapper_203(void);
extern void isr_wrapper_204(void);
extern void isr_wrapper_205(void);
extern void isr_wrapper_206(void);
extern void isr_wrapper_207(void);
extern void isr_wrapper_208(void);
extern void isr_wrapper_209(void);
extern void isr_wrapper_210(void);
extern void isr_wrapper_211(void);
extern void isr_wrapper_212(void);
extern void isr_wrapper_213(void);
extern void isr_wrapper_214(void);
extern void isr_wrapper_215(void);
extern void isr_wrapper_216(void);
extern void isr_wrapper_217(void);
extern void isr_wrapper_218(void);
extern void isr_wrapper_219(void);
extern void isr_wrapper_220(void);
extern void isr_wrapper_221(void);
extern void isr_wrapper_222(void);
extern void isr_wrapper_223(void);
extern void isr_wrapper_224(void);
extern void isr_wrapper_225(void);
extern void isr_wrapper_226(void);
extern void isr_wrapper_227(void);
extern void isr_wrapper_228(void);
extern void isr_wrapper_229(void);
extern void isr_wrapper_230(void);
extern void isr_wrapper_231(void);
extern void isr_wrapper_232(void);
extern void isr_wrapper_233(void);
extern void isr_wrapper_234(void);
extern void isr_wrapper_235(void);
extern void isr_wrapper_236(void);
extern void isr_wrapper_237(void);
extern void isr_wrapper_238(void);
extern void isr_wrapper_239(void);
extern void isr_wrapper_240(void);
extern void isr_wrapper_241(void);
extern void isr_wrapper_242(void);
extern void isr_wrapper_243(void);
extern void isr_wrapper_244(void);
extern void isr_wrapper_245(void);
extern void isr_wrapper_246(void);
extern void isr_wrapper_247(void);
extern void isr_wrapper_248(void);
extern void isr_wrapper_249(void);
extern void isr_wrapper_250(void);
extern void isr_wrapper_251(void);
extern void isr_wrapper_252(void);
extern void isr_wrapper_253(void);
extern void isr_wrapper_254(void);
extern void isr_wrapper_255(void);
} // extern "C"

namespace i386 {

template<typename Policy>
class GenericHardwareInterruptController {
public:
    using OffsetType = typename Policy::OffsetType;
    using InterruptDescriptor = typename Policy::InterruptDescriptor;
    using InterruptDescriptorTable = typename Policy::InterruptDescriptorTable;

    GenericHardwareInterruptController() = default;

    void init(bool is_panic_context = false)
    {
        pic_.init();
        idt_.clear();
        build_idt(is_panic_context);
        idt_.load();
    }

    void disable_interrupts()
    {
        ::disable_interrupts();
    }

    void enable_interrupts()
    {
        ::enable_interrupts();
    }

    bool clear(unsigned interrupt_number)
    {
        return pic_.clear(interrupt_number);
    }

private:
    ProgrammableInterruptController pic_;
    InterruptDescriptorTable idt_;

    void build_idt(bool is_panic_context)
    {
        if (is_panic_context) {
            build_idt_for_panic_context();
        } else {
            build_idt_for_normal_kernel();
        }
    }

    void build_idt_for_panic_context()
    {
        for (int i = 0; i < 256; ++i) {
            build_idt_entry(idt_.entries[i], (uintptr_t)on_interrupt_during_panic);
        }
    }

    void build_idt_for_normal_kernel()
    {
        build_idt_entry(idt_.entries[0], (uintptr_t)isr_wrapper_0);
        build_idt_entry(idt_.entries[1], (uintptr_t)isr_wrapper_1);
        build_idt_entry(idt_.entries[2], (uintptr_t)isr_wrapper_2);
        build_idt_entry(idt_.entries[3], (uintptr_t)isr_wrapper_3);
        build_idt_entry(idt_.entries[4], (uintptr_t)isr_wrapper_4);
        build_idt_entry(idt_.entries[5], (uintptr_t)isr_wrapper_5);
        build_idt_entry(idt_.entries[6], (uintptr_t)isr_wrapper_6);
        build_idt_entry(idt_.entries[7], (uintptr_t)isr_wrapper_7);
        build_idt_entry(idt_.entries[8], (uintptr_t)isr_wrapper_8);
        build_idt_entry(idt_.entries[9], (uintptr_t)isr_wrapper_9);
        build_idt_entry(idt_.entries[10], (uintptr_t)isr_wrapper_10);
        build_idt_entry(idt_.entries[11], (uintptr_t)isr_wrapper_11);
        build_idt_entry(idt_.entries[12], (uintptr_t)isr_wrapper_12);
        build_idt_entry(idt_.entries[13], (uintptr_t)isr_wrapper_13);
        build_idt_entry(idt_.entries[14], (uintptr_t)isr_wrapper_14);
        build_idt_entry(idt_.entries[16], (uintptr_t)isr_wrapper_16);
        build_idt_entry(idt_.entries[17], (uintptr_t)isr_wrapper_17);
        build_idt_entry(idt_.entries[18], (uintptr_t)isr_wrapper_18);
        build_idt_entry(idt_.entries[19], (uintptr_t)isr_wrapper_19);
        build_idt_entry(idt_.entries[32], (uintptr_t)isr_wrapper_32);
        build_idt_entry(idt_.entries[33], (uintptr_t)isr_wrapper_33);
        build_idt_entry(idt_.entries[34], (uintptr_t)isr_wrapper_34);
        build_idt_entry(idt_.entries[35], (uintptr_t)isr_wrapper_35);
        build_idt_entry(idt_.entries[36], (uintptr_t)isr_wrapper_36);
        build_idt_entry(idt_.entries[37], (uintptr_t)isr_wrapper_37);
        build_idt_entry(idt_.entries[38], (uintptr_t)isr_wrapper_38);
        build_idt_entry(idt_.entries[39], (uintptr_t)isr_wrapper_39);
        build_idt_entry(idt_.entries[40], (uintptr_t)isr_wrapper_40);
        build_idt_entry(idt_.entries[41], (uintptr_t)isr_wrapper_41);
        build_idt_entry(idt_.entries[42], (uintptr_t)isr_wrapper_42);
        build_idt_entry(idt_.entries[43], (uintptr_t)isr_wrapper_43);
        build_idt_entry(idt_.entries[44], (uintptr_t)isr_wrapper_44);
        build_idt_entry(idt_.entries[45], (uintptr_t)isr_wrapper_45);
        build_idt_entry(idt_.entries[46], (uintptr_t)isr_wrapper_46);
        build_idt_entry(idt_.entries[47], (uintptr_t)isr_wrapper_47);
        build_idt_entry(idt_.entries[48], (uintptr_t)isr_wrapper_48);
        build_idt_entry(idt_.entries[49], (uintptr_t)isr_wrapper_49);
        build_idt_entry(idt_.entries[50], (uintptr_t)isr_wrapper_50);
        build_idt_entry(idt_.entries[51], (uintptr_t)isr_wrapper_51);
        build_idt_entry(idt_.entries[52], (uintptr_t)isr_wrapper_52);
        build_idt_entry(idt_.entries[53], (uintptr_t)isr_wrapper_53);
        build_idt_entry(idt_.entries[54], (uintptr_t)isr_wrapper_54);
        build_idt_entry(idt_.entries[55], (uintptr_t)isr_wrapper_55);
        build_idt_entry(idt_.entries[56], (uintptr_t)isr_wrapper_56);
        build_idt_entry(idt_.entries[57], (uintptr_t)isr_wrapper_57);
        build_idt_entry(idt_.entries[58], (uintptr_t)isr_wrapper_58);
        build_idt_entry(idt_.entries[59], (uintptr_t)isr_wrapper_59);
        build_idt_entry(idt_.entries[60], (uintptr_t)isr_wrapper_60);
        build_idt_entry(idt_.entries[61], (uintptr_t)isr_wrapper_61);
        build_idt_entry(idt_.entries[62], (uintptr_t)isr_wrapper_62);
        build_idt_entry(idt_.entries[63], (uintptr_t)isr_wrapper_63);
        build_idt_entry(idt_.entries[64], (uintptr_t)isr_wrapper_64);
        build_idt_entry(idt_.entries[65], (uintptr_t)isr_wrapper_65);
        build_idt_entry(idt_.entries[66], (uintptr_t)isr_wrapper_66);
        build_idt_entry(idt_.entries[67], (uintptr_t)isr_wrapper_67);
        build_idt_entry(idt_.entries[68], (uintptr_t)isr_wrapper_68);
        build_idt_entry(idt_.entries[69], (uintptr_t)isr_wrapper_69);
        build_idt_entry(idt_.entries[70], (uintptr_t)isr_wrapper_70);
        build_idt_entry(idt_.entries[71], (uintptr_t)isr_wrapper_71);
        build_idt_entry(idt_.entries[72], (uintptr_t)isr_wrapper_72);
        build_idt_entry(idt_.entries[73], (uintptr_t)isr_wrapper_73);
        build_idt_entry(idt_.entries[74], (uintptr_t)isr_wrapper_74);
        build_idt_entry(idt_.entries[75], (uintptr_t)isr_wrapper_75);
        build_idt_entry(idt_.entries[76], (uintptr_t)isr_wrapper_76);
        build_idt_entry(idt_.entries[77], (uintptr_t)isr_wrapper_77);
        build_idt_entry(idt_.entries[78], (uintptr_t)isr_wrapper_78);
        build_idt_entry(idt_.entries[79], (uintptr_t)isr_wrapper_79);
        build_idt_entry(idt_.entries[80], (uintptr_t)isr_wrapper_80);
        build_idt_entry(idt_.entries[81], (uintptr_t)isr_wrapper_81);
        build_idt_entry(idt_.entries[82], (uintptr_t)isr_wrapper_82);
        build_idt_entry(idt_.entries[83], (uintptr_t)isr_wrapper_83);
        build_idt_entry(idt_.entries[84], (uintptr_t)isr_wrapper_84);
        build_idt_entry(idt_.entries[85], (uintptr_t)isr_wrapper_85);
        build_idt_entry(idt_.entries[86], (uintptr_t)isr_wrapper_86);
        build_idt_entry(idt_.entries[87], (uintptr_t)isr_wrapper_87);
        build_idt_entry(idt_.entries[88], (uintptr_t)isr_wrapper_88);
        build_idt_entry(idt_.entries[89], (uintptr_t)isr_wrapper_89);
        build_idt_entry(idt_.entries[90], (uintptr_t)isr_wrapper_90);
        build_idt_entry(idt_.entries[91], (uintptr_t)isr_wrapper_91);
        build_idt_entry(idt_.entries[92], (uintptr_t)isr_wrapper_92);
        build_idt_entry(idt_.entries[93], (uintptr_t)isr_wrapper_93);
        build_idt_entry(idt_.entries[94], (uintptr_t)isr_wrapper_94);
        build_idt_entry(idt_.entries[95], (uintptr_t)isr_wrapper_95);
        build_idt_entry(idt_.entries[96], (uintptr_t)isr_wrapper_96);
        build_idt_entry(idt_.entries[97], (uintptr_t)isr_wrapper_97);
        build_idt_entry(idt_.entries[98], (uintptr_t)isr_wrapper_98);
        build_idt_entry(idt_.entries[99], (uintptr_t)isr_wrapper_99);
        build_idt_entry(idt_.entries[100], (uintptr_t)isr_wrapper_100);
        build_idt_entry(idt_.entries[101], (uintptr_t)isr_wrapper_101);
        build_idt_entry(idt_.entries[102], (uintptr_t)isr_wrapper_102);
        build_idt_entry(idt_.entries[103], (uintptr_t)isr_wrapper_103);
        build_idt_entry(idt_.entries[104], (uintptr_t)isr_wrapper_104);
        build_idt_entry(idt_.entries[105], (uintptr_t)isr_wrapper_105);
        build_idt_entry(idt_.entries[106], (uintptr_t)isr_wrapper_106);
        build_idt_entry(idt_.entries[107], (uintptr_t)isr_wrapper_107);
        build_idt_entry(idt_.entries[108], (uintptr_t)isr_wrapper_108);
        build_idt_entry(idt_.entries[109], (uintptr_t)isr_wrapper_109);
        build_idt_entry(idt_.entries[110], (uintptr_t)isr_wrapper_110);
        build_idt_entry(idt_.entries[111], (uintptr_t)isr_wrapper_111);
        build_idt_entry(idt_.entries[112], (uintptr_t)isr_wrapper_112);
        build_idt_entry(idt_.entries[113], (uintptr_t)isr_wrapper_113);
        build_idt_entry(idt_.entries[114], (uintptr_t)isr_wrapper_114);
        build_idt_entry(idt_.entries[115], (uintptr_t)isr_wrapper_115);
        build_idt_entry(idt_.entries[116], (uintptr_t)isr_wrapper_116);
        build_idt_entry(idt_.entries[117], (uintptr_t)isr_wrapper_117);
        build_idt_entry(idt_.entries[118], (uintptr_t)isr_wrapper_118);
        build_idt_entry(idt_.entries[119], (uintptr_t)isr_wrapper_119);
        build_idt_entry(idt_.entries[120], (uintptr_t)isr_wrapper_120);
        build_idt_entry(idt_.entries[121], (uintptr_t)isr_wrapper_121);
        build_idt_entry(idt_.entries[122], (uintptr_t)isr_wrapper_122);
        build_idt_entry(idt_.entries[123], (uintptr_t)isr_wrapper_123);
        build_idt_entry(idt_.entries[124], (uintptr_t)isr_wrapper_124);
        build_idt_entry(idt_.entries[125], (uintptr_t)isr_wrapper_125);
        build_idt_entry(idt_.entries[126], (uintptr_t)isr_wrapper_126);
        build_idt_entry(idt_.entries[127], (uintptr_t)isr_wrapper_127);
        build_idt_entry(idt_.entries[128], (uintptr_t)isr_wrapper_128);
        build_idt_entry(idt_.entries[129], (uintptr_t)isr_wrapper_129);
        build_idt_entry(idt_.entries[130], (uintptr_t)isr_wrapper_130);
        build_idt_entry(idt_.entries[131], (uintptr_t)isr_wrapper_131);
        build_idt_entry(idt_.entries[132], (uintptr_t)isr_wrapper_132);
        build_idt_entry(idt_.entries[133], (uintptr_t)isr_wrapper_133);
        build_idt_entry(idt_.entries[134], (uintptr_t)isr_wrapper_134);
        build_idt_entry(idt_.entries[135], (uintptr_t)isr_wrapper_135);
        build_idt_entry(idt_.entries[136], (uintptr_t)isr_wrapper_136);
        build_idt_entry(idt_.entries[137], (uintptr_t)isr_wrapper_137);
        build_idt_entry(idt_.entries[138], (uintptr_t)isr_wrapper_138);
        build_idt_entry(idt_.entries[139], (uintptr_t)isr_wrapper_139);
        build_idt_entry(idt_.entries[140], (uintptr_t)isr_wrapper_140);
        build_idt_entry(idt_.entries[141], (uintptr_t)isr_wrapper_141);
        build_idt_entry(idt_.entries[142], (uintptr_t)isr_wrapper_142);
        build_idt_entry(idt_.entries[143], (uintptr_t)isr_wrapper_143);
        build_idt_entry(idt_.entries[144], (uintptr_t)isr_wrapper_144);
        build_idt_entry(idt_.entries[145], (uintptr_t)isr_wrapper_145);
        build_idt_entry(idt_.entries[146], (uintptr_t)isr_wrapper_146);
        build_idt_entry(idt_.entries[147], (uintptr_t)isr_wrapper_147);
        build_idt_entry(idt_.entries[148], (uintptr_t)isr_wrapper_148);
        build_idt_entry(idt_.entries[149], (uintptr_t)isr_wrapper_149);
        build_idt_entry(idt_.entries[150], (uintptr_t)isr_wrapper_150);
        build_idt_entry(idt_.entries[151], (uintptr_t)isr_wrapper_151);
        build_idt_entry(idt_.entries[152], (uintptr_t)isr_wrapper_152);
        build_idt_entry(idt_.entries[153], (uintptr_t)isr_wrapper_153);
        build_idt_entry(idt_.entries[154], (uintptr_t)isr_wrapper_154);
        build_idt_entry(idt_.entries[155], (uintptr_t)isr_wrapper_155);
        build_idt_entry(idt_.entries[156], (uintptr_t)isr_wrapper_156);
        build_idt_entry(idt_.entries[157], (uintptr_t)isr_wrapper_157);
        build_idt_entry(idt_.entries[158], (uintptr_t)isr_wrapper_158);
        build_idt_entry(idt_.entries[159], (uintptr_t)isr_wrapper_159);
        build_idt_entry(idt_.entries[160], (uintptr_t)isr_wrapper_160);
        build_idt_entry(idt_.entries[161], (uintptr_t)isr_wrapper_161);
        build_idt_entry(idt_.entries[162], (uintptr_t)isr_wrapper_162);
        build_idt_entry(idt_.entries[163], (uintptr_t)isr_wrapper_163);
        build_idt_entry(idt_.entries[164], (uintptr_t)isr_wrapper_164);
        build_idt_entry(idt_.entries[165], (uintptr_t)isr_wrapper_165);
        build_idt_entry(idt_.entries[166], (uintptr_t)isr_wrapper_166);
        build_idt_entry(idt_.entries[167], (uintptr_t)isr_wrapper_167);
        build_idt_entry(idt_.entries[168], (uintptr_t)isr_wrapper_168);
        build_idt_entry(idt_.entries[169], (uintptr_t)isr_wrapper_169);
        build_idt_entry(idt_.entries[170], (uintptr_t)isr_wrapper_170);
        build_idt_entry(idt_.entries[171], (uintptr_t)isr_wrapper_171);
        build_idt_entry(idt_.entries[172], (uintptr_t)isr_wrapper_172);
        build_idt_entry(idt_.entries[173], (uintptr_t)isr_wrapper_173);
        build_idt_entry(idt_.entries[174], (uintptr_t)isr_wrapper_174);
        build_idt_entry(idt_.entries[175], (uintptr_t)isr_wrapper_175);
        build_idt_entry(idt_.entries[176], (uintptr_t)isr_wrapper_176);
        build_idt_entry(idt_.entries[177], (uintptr_t)isr_wrapper_177);
        build_idt_entry(idt_.entries[178], (uintptr_t)isr_wrapper_178);
        build_idt_entry(idt_.entries[179], (uintptr_t)isr_wrapper_179);
        build_idt_entry(idt_.entries[180], (uintptr_t)isr_wrapper_180);
        build_idt_entry(idt_.entries[181], (uintptr_t)isr_wrapper_181);
        build_idt_entry(idt_.entries[182], (uintptr_t)isr_wrapper_182);
        build_idt_entry(idt_.entries[183], (uintptr_t)isr_wrapper_183);
        build_idt_entry(idt_.entries[184], (uintptr_t)isr_wrapper_184);
        build_idt_entry(idt_.entries[185], (uintptr_t)isr_wrapper_185);
        build_idt_entry(idt_.entries[186], (uintptr_t)isr_wrapper_186);
        build_idt_entry(idt_.entries[187], (uintptr_t)isr_wrapper_187);
        build_idt_entry(idt_.entries[188], (uintptr_t)isr_wrapper_188);
        build_idt_entry(idt_.entries[189], (uintptr_t)isr_wrapper_189);
        build_idt_entry(idt_.entries[190], (uintptr_t)isr_wrapper_190);
        build_idt_entry(idt_.entries[191], (uintptr_t)isr_wrapper_191);
        build_idt_entry(idt_.entries[192], (uintptr_t)isr_wrapper_192);
        build_idt_entry(idt_.entries[193], (uintptr_t)isr_wrapper_193);
        build_idt_entry(idt_.entries[194], (uintptr_t)isr_wrapper_194);
        build_idt_entry(idt_.entries[195], (uintptr_t)isr_wrapper_195);
        build_idt_entry(idt_.entries[196], (uintptr_t)isr_wrapper_196);
        build_idt_entry(idt_.entries[197], (uintptr_t)isr_wrapper_197);
        build_idt_entry(idt_.entries[198], (uintptr_t)isr_wrapper_198);
        build_idt_entry(idt_.entries[199], (uintptr_t)isr_wrapper_199);
        build_idt_entry(idt_.entries[200], (uintptr_t)isr_wrapper_200);
        build_idt_entry(idt_.entries[201], (uintptr_t)isr_wrapper_201);
        build_idt_entry(idt_.entries[202], (uintptr_t)isr_wrapper_202);
        build_idt_entry(idt_.entries[203], (uintptr_t)isr_wrapper_203);
        build_idt_entry(idt_.entries[204], (uintptr_t)isr_wrapper_204);
        build_idt_entry(idt_.entries[205], (uintptr_t)isr_wrapper_205);
        build_idt_entry(idt_.entries[206], (uintptr_t)isr_wrapper_206);
        build_idt_entry(idt_.entries[207], (uintptr_t)isr_wrapper_207);
        build_idt_entry(idt_.entries[208], (uintptr_t)isr_wrapper_208);
        build_idt_entry(idt_.entries[209], (uintptr_t)isr_wrapper_209);
        build_idt_entry(idt_.entries[210], (uintptr_t)isr_wrapper_210);
        build_idt_entry(idt_.entries[211], (uintptr_t)isr_wrapper_211);
        build_idt_entry(idt_.entries[212], (uintptr_t)isr_wrapper_212);
        build_idt_entry(idt_.entries[213], (uintptr_t)isr_wrapper_213);
        build_idt_entry(idt_.entries[214], (uintptr_t)isr_wrapper_214);
        build_idt_entry(idt_.entries[215], (uintptr_t)isr_wrapper_215);
        build_idt_entry(idt_.entries[216], (uintptr_t)isr_wrapper_216);
        build_idt_entry(idt_.entries[217], (uintptr_t)isr_wrapper_217);
        build_idt_entry(idt_.entries[218], (uintptr_t)isr_wrapper_218);
        build_idt_entry(idt_.entries[219], (uintptr_t)isr_wrapper_219);
        build_idt_entry(idt_.entries[220], (uintptr_t)isr_wrapper_220);
        build_idt_entry(idt_.entries[221], (uintptr_t)isr_wrapper_221);
        build_idt_entry(idt_.entries[222], (uintptr_t)isr_wrapper_222);
        build_idt_entry(idt_.entries[223], (uintptr_t)isr_wrapper_223);
        build_idt_entry(idt_.entries[224], (uintptr_t)isr_wrapper_224);
        build_idt_entry(idt_.entries[225], (uintptr_t)isr_wrapper_225);
        build_idt_entry(idt_.entries[226], (uintptr_t)isr_wrapper_226);
        build_idt_entry(idt_.entries[227], (uintptr_t)isr_wrapper_227);
        build_idt_entry(idt_.entries[228], (uintptr_t)isr_wrapper_228);
        build_idt_entry(idt_.entries[229], (uintptr_t)isr_wrapper_229);
        build_idt_entry(idt_.entries[230], (uintptr_t)isr_wrapper_230);
        build_idt_entry(idt_.entries[231], (uintptr_t)isr_wrapper_231);
        build_idt_entry(idt_.entries[232], (uintptr_t)isr_wrapper_232);
        build_idt_entry(idt_.entries[233], (uintptr_t)isr_wrapper_233);
        build_idt_entry(idt_.entries[234], (uintptr_t)isr_wrapper_234);
        build_idt_entry(idt_.entries[235], (uintptr_t)isr_wrapper_235);
        build_idt_entry(idt_.entries[236], (uintptr_t)isr_wrapper_236);
        build_idt_entry(idt_.entries[237], (uintptr_t)isr_wrapper_237);
        build_idt_entry(idt_.entries[238], (uintptr_t)isr_wrapper_238);
        build_idt_entry(idt_.entries[239], (uintptr_t)isr_wrapper_239);
        build_idt_entry(idt_.entries[240], (uintptr_t)isr_wrapper_240);
        build_idt_entry(idt_.entries[241], (uintptr_t)isr_wrapper_241);
        build_idt_entry(idt_.entries[242], (uintptr_t)isr_wrapper_242);
        build_idt_entry(idt_.entries[243], (uintptr_t)isr_wrapper_243);
        build_idt_entry(idt_.entries[244], (uintptr_t)isr_wrapper_244);
        build_idt_entry(idt_.entries[245], (uintptr_t)isr_wrapper_245);
        build_idt_entry(idt_.entries[246], (uintptr_t)isr_wrapper_246);
        build_idt_entry(idt_.entries[247], (uintptr_t)isr_wrapper_247);
        build_idt_entry(idt_.entries[248], (uintptr_t)isr_wrapper_248);
        build_idt_entry(idt_.entries[249], (uintptr_t)isr_wrapper_249);
        build_idt_entry(idt_.entries[250], (uintptr_t)isr_wrapper_250);
        build_idt_entry(idt_.entries[251], (uintptr_t)isr_wrapper_251);
        build_idt_entry(idt_.entries[252], (uintptr_t)isr_wrapper_252);
        build_idt_entry(idt_.entries[253], (uintptr_t)isr_wrapper_253);
        build_idt_entry(idt_.entries[254], (uintptr_t)isr_wrapper_254);
        build_idt_entry(idt_.entries[255], (uintptr_t)isr_wrapper_255);
    }

    void build_idt_entry(InterruptDescriptor& entry, uintptr_t offset)
    {
        entry.set_present(true);
        entry.set_offset(static_cast<OffsetType>(offset));
        entry.set_dpl(0);
        entry.set_segment_selector(SEGSEL_KERNEL_CS);
        entry.set_type(entry.TrapGate);
    }
};

struct HardwareInterruptControllerPolicy {
    using OffsetType = uint32_t;
    using InterruptDescriptor = i386::InterruptDescriptor;
    using InterruptDescriptorTable = i386::InterruptDescriptorTable;
};

using HardwareInterruptController = i386::GenericHardwareInterruptController<HardwareInterruptControllerPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP
