# isr_wrapper_asm.S is an automatically generated file. Do not edit directly.
#
# Interrupt Service Routines
# These are the procedures which are directly referenced by the IDT.
# Though, all are simply assembly wrappers which call the high-level dispatcher.

# The CPU pushes an error code on the stack for the following ISR wrappers.

.global isr_wrapper_8
isr_wrapper_8:
    cli
    pusha
    pushl $8
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global isr_wrapper_10
isr_wrapper_10:
    cli
    pusha
    pushl $10
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global isr_wrapper_11
isr_wrapper_11:
    cli
    pusha
    pushl $11
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global isr_wrapper_12
isr_wrapper_12:
    cli
    pusha
    pushl $12
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global isr_wrapper_13
isr_wrapper_13:
    cli
    pusha
    pushl $13
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret

.global isr_wrapper_14
isr_wrapper_14:
    cli
    pusha
    pushl $14
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    addl $4, %esp  # Clean up the error code.
    sti
    iret


# The CPU does not push an error code for the following ISR wrappers.

.global isr_wrapper_0
isr_wrapper_0:
    cli
    pusha
    pushl $0
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_1
isr_wrapper_1:
    cli
    pusha
    pushl $1
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_2
isr_wrapper_2:
    cli
    pusha
    pushl $2
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_3
isr_wrapper_3:
    cli
    pusha
    pushl $3
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_4
isr_wrapper_4:
    cli
    pusha
    pushl $4
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_5
isr_wrapper_5:
    cli
    pusha
    pushl $5
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_6
isr_wrapper_6:
    cli
    pusha
    pushl $6
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_7
isr_wrapper_7:
    cli
    pusha
    pushl $7
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_9
isr_wrapper_9:
    cli
    pusha
    pushl $9
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_16
isr_wrapper_16:
    cli
    pusha
    pushl $16
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_17
isr_wrapper_17:
    cli
    pusha
    pushl $17
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_18
isr_wrapper_18:
    cli
    pusha
    pushl $18
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_19
isr_wrapper_19:
    cli
    pusha
    pushl $19
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_32
isr_wrapper_32:
    cli
    pusha
    pushl $32
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_33
isr_wrapper_33:
    cli
    pusha
    pushl $33
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_34
isr_wrapper_34:
    cli
    pusha
    pushl $34
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_35
isr_wrapper_35:
    cli
    pusha
    pushl $35
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_36
isr_wrapper_36:
    cli
    pusha
    pushl $36
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_37
isr_wrapper_37:
    cli
    pusha
    pushl $37
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_38
isr_wrapper_38:
    cli
    pusha
    pushl $38
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_39
isr_wrapper_39:
    cli
    pusha
    pushl $39
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_40
isr_wrapper_40:
    cli
    pusha
    pushl $40
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_41
isr_wrapper_41:
    cli
    pusha
    pushl $41
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_42
isr_wrapper_42:
    cli
    pusha
    pushl $42
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_43
isr_wrapper_43:
    cli
    pusha
    pushl $43
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_44
isr_wrapper_44:
    cli
    pusha
    pushl $44
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_45
isr_wrapper_45:
    cli
    pusha
    pushl $45
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_46
isr_wrapper_46:
    cli
    pusha
    pushl $46
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_47
isr_wrapper_47:
    cli
    pusha
    pushl $47
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_48
isr_wrapper_48:
    cli
    pusha
    pushl $48
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_49
isr_wrapper_49:
    cli
    pusha
    pushl $49
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_50
isr_wrapper_50:
    cli
    pusha
    pushl $50
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_51
isr_wrapper_51:
    cli
    pusha
    pushl $51
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_52
isr_wrapper_52:
    cli
    pusha
    pushl $52
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_53
isr_wrapper_53:
    cli
    pusha
    pushl $53
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_54
isr_wrapper_54:
    cli
    pusha
    pushl $54
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_55
isr_wrapper_55:
    cli
    pusha
    pushl $55
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_56
isr_wrapper_56:
    cli
    pusha
    pushl $56
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_57
isr_wrapper_57:
    cli
    pusha
    pushl $57
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_58
isr_wrapper_58:
    cli
    pusha
    pushl $58
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_59
isr_wrapper_59:
    cli
    pusha
    pushl $59
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_60
isr_wrapper_60:
    cli
    pusha
    pushl $60
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_61
isr_wrapper_61:
    cli
    pusha
    pushl $61
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_62
isr_wrapper_62:
    cli
    pusha
    pushl $62
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_63
isr_wrapper_63:
    cli
    pusha
    pushl $63
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_64
isr_wrapper_64:
    cli
    pusha
    pushl $64
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_65
isr_wrapper_65:
    cli
    pusha
    pushl $65
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_66
isr_wrapper_66:
    cli
    pusha
    pushl $66
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_67
isr_wrapper_67:
    cli
    pusha
    pushl $67
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_68
isr_wrapper_68:
    cli
    pusha
    pushl $68
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_69
isr_wrapper_69:
    cli
    pusha
    pushl $69
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_70
isr_wrapper_70:
    cli
    pusha
    pushl $70
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_71
isr_wrapper_71:
    cli
    pusha
    pushl $71
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_72
isr_wrapper_72:
    cli
    pusha
    pushl $72
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_73
isr_wrapper_73:
    cli
    pusha
    pushl $73
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_74
isr_wrapper_74:
    cli
    pusha
    pushl $74
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_75
isr_wrapper_75:
    cli
    pusha
    pushl $75
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_76
isr_wrapper_76:
    cli
    pusha
    pushl $76
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_77
isr_wrapper_77:
    cli
    pusha
    pushl $77
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_78
isr_wrapper_78:
    cli
    pusha
    pushl $78
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_79
isr_wrapper_79:
    cli
    pusha
    pushl $79
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_80
isr_wrapper_80:
    cli
    pusha
    pushl $80
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_81
isr_wrapper_81:
    cli
    pusha
    pushl $81
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_82
isr_wrapper_82:
    cli
    pusha
    pushl $82
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_83
isr_wrapper_83:
    cli
    pusha
    pushl $83
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_84
isr_wrapper_84:
    cli
    pusha
    pushl $84
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_85
isr_wrapper_85:
    cli
    pusha
    pushl $85
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_86
isr_wrapper_86:
    cli
    pusha
    pushl $86
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_87
isr_wrapper_87:
    cli
    pusha
    pushl $87
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_88
isr_wrapper_88:
    cli
    pusha
    pushl $88
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_89
isr_wrapper_89:
    cli
    pusha
    pushl $89
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_90
isr_wrapper_90:
    cli
    pusha
    pushl $90
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_91
isr_wrapper_91:
    cli
    pusha
    pushl $91
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_92
isr_wrapper_92:
    cli
    pusha
    pushl $92
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_93
isr_wrapper_93:
    cli
    pusha
    pushl $93
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_94
isr_wrapper_94:
    cli
    pusha
    pushl $94
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_95
isr_wrapper_95:
    cli
    pusha
    pushl $95
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_96
isr_wrapper_96:
    cli
    pusha
    pushl $96
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_97
isr_wrapper_97:
    cli
    pusha
    pushl $97
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_98
isr_wrapper_98:
    cli
    pusha
    pushl $98
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_99
isr_wrapper_99:
    cli
    pusha
    pushl $99
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_100
isr_wrapper_100:
    cli
    pusha
    pushl $100
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_101
isr_wrapper_101:
    cli
    pusha
    pushl $101
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_102
isr_wrapper_102:
    cli
    pusha
    pushl $102
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_103
isr_wrapper_103:
    cli
    pusha
    pushl $103
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_104
isr_wrapper_104:
    cli
    pusha
    pushl $104
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_105
isr_wrapper_105:
    cli
    pusha
    pushl $105
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_106
isr_wrapper_106:
    cli
    pusha
    pushl $106
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_107
isr_wrapper_107:
    cli
    pusha
    pushl $107
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_108
isr_wrapper_108:
    cli
    pusha
    pushl $108
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_109
isr_wrapper_109:
    cli
    pusha
    pushl $109
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_110
isr_wrapper_110:
    cli
    pusha
    pushl $110
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_111
isr_wrapper_111:
    cli
    pusha
    pushl $111
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_112
isr_wrapper_112:
    cli
    pusha
    pushl $112
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_113
isr_wrapper_113:
    cli
    pusha
    pushl $113
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_114
isr_wrapper_114:
    cli
    pusha
    pushl $114
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_115
isr_wrapper_115:
    cli
    pusha
    pushl $115
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_116
isr_wrapper_116:
    cli
    pusha
    pushl $116
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_117
isr_wrapper_117:
    cli
    pusha
    pushl $117
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_118
isr_wrapper_118:
    cli
    pusha
    pushl $118
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_119
isr_wrapper_119:
    cli
    pusha
    pushl $119
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_120
isr_wrapper_120:
    cli
    pusha
    pushl $120
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_121
isr_wrapper_121:
    cli
    pusha
    pushl $121
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_122
isr_wrapper_122:
    cli
    pusha
    pushl $122
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_123
isr_wrapper_123:
    cli
    pusha
    pushl $123
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_124
isr_wrapper_124:
    cli
    pusha
    pushl $124
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_125
isr_wrapper_125:
    cli
    pusha
    pushl $125
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_126
isr_wrapper_126:
    cli
    pusha
    pushl $126
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_127
isr_wrapper_127:
    cli
    pusha
    pushl $127
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_128
isr_wrapper_128:
    cli
    pusha
    pushl $128
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_129
isr_wrapper_129:
    cli
    pusha
    pushl $129
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_130
isr_wrapper_130:
    cli
    pusha
    pushl $130
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_131
isr_wrapper_131:
    cli
    pusha
    pushl $131
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_132
isr_wrapper_132:
    cli
    pusha
    pushl $132
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_133
isr_wrapper_133:
    cli
    pusha
    pushl $133
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_134
isr_wrapper_134:
    cli
    pusha
    pushl $134
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_135
isr_wrapper_135:
    cli
    pusha
    pushl $135
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_136
isr_wrapper_136:
    cli
    pusha
    pushl $136
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_137
isr_wrapper_137:
    cli
    pusha
    pushl $137
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_138
isr_wrapper_138:
    cli
    pusha
    pushl $138
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_139
isr_wrapper_139:
    cli
    pusha
    pushl $139
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_140
isr_wrapper_140:
    cli
    pusha
    pushl $140
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_141
isr_wrapper_141:
    cli
    pusha
    pushl $141
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_142
isr_wrapper_142:
    cli
    pusha
    pushl $142
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_143
isr_wrapper_143:
    cli
    pusha
    pushl $143
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_144
isr_wrapper_144:
    cli
    pusha
    pushl $144
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_145
isr_wrapper_145:
    cli
    pusha
    pushl $145
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_146
isr_wrapper_146:
    cli
    pusha
    pushl $146
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_147
isr_wrapper_147:
    cli
    pusha
    pushl $147
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_148
isr_wrapper_148:
    cli
    pusha
    pushl $148
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_149
isr_wrapper_149:
    cli
    pusha
    pushl $149
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_150
isr_wrapper_150:
    cli
    pusha
    pushl $150
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_151
isr_wrapper_151:
    cli
    pusha
    pushl $151
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_152
isr_wrapper_152:
    cli
    pusha
    pushl $152
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_153
isr_wrapper_153:
    cli
    pusha
    pushl $153
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_154
isr_wrapper_154:
    cli
    pusha
    pushl $154
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_155
isr_wrapper_155:
    cli
    pusha
    pushl $155
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_156
isr_wrapper_156:
    cli
    pusha
    pushl $156
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_157
isr_wrapper_157:
    cli
    pusha
    pushl $157
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_158
isr_wrapper_158:
    cli
    pusha
    pushl $158
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_159
isr_wrapper_159:
    cli
    pusha
    pushl $159
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_160
isr_wrapper_160:
    cli
    pusha
    pushl $160
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_161
isr_wrapper_161:
    cli
    pusha
    pushl $161
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_162
isr_wrapper_162:
    cli
    pusha
    pushl $162
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_163
isr_wrapper_163:
    cli
    pusha
    pushl $163
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_164
isr_wrapper_164:
    cli
    pusha
    pushl $164
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_165
isr_wrapper_165:
    cli
    pusha
    pushl $165
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_166
isr_wrapper_166:
    cli
    pusha
    pushl $166
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_167
isr_wrapper_167:
    cli
    pusha
    pushl $167
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_168
isr_wrapper_168:
    cli
    pusha
    pushl $168
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_169
isr_wrapper_169:
    cli
    pusha
    pushl $169
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_170
isr_wrapper_170:
    cli
    pusha
    pushl $170
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_171
isr_wrapper_171:
    cli
    pusha
    pushl $171
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_172
isr_wrapper_172:
    cli
    pusha
    pushl $172
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_173
isr_wrapper_173:
    cli
    pusha
    pushl $173
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_174
isr_wrapper_174:
    cli
    pusha
    pushl $174
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_175
isr_wrapper_175:
    cli
    pusha
    pushl $175
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_176
isr_wrapper_176:
    cli
    pusha
    pushl $176
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_177
isr_wrapper_177:
    cli
    pusha
    pushl $177
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_178
isr_wrapper_178:
    cli
    pusha
    pushl $178
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_179
isr_wrapper_179:
    cli
    pusha
    pushl $179
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_180
isr_wrapper_180:
    cli
    pusha
    pushl $180
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_181
isr_wrapper_181:
    cli
    pusha
    pushl $181
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_182
isr_wrapper_182:
    cli
    pusha
    pushl $182
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_183
isr_wrapper_183:
    cli
    pusha
    pushl $183
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_184
isr_wrapper_184:
    cli
    pusha
    pushl $184
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_185
isr_wrapper_185:
    cli
    pusha
    pushl $185
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_186
isr_wrapper_186:
    cli
    pusha
    pushl $186
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_187
isr_wrapper_187:
    cli
    pusha
    pushl $187
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_188
isr_wrapper_188:
    cli
    pusha
    pushl $188
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_189
isr_wrapper_189:
    cli
    pusha
    pushl $189
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_190
isr_wrapper_190:
    cli
    pusha
    pushl $190
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_191
isr_wrapper_191:
    cli
    pusha
    pushl $191
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_192
isr_wrapper_192:
    cli
    pusha
    pushl $192
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_193
isr_wrapper_193:
    cli
    pusha
    pushl $193
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_194
isr_wrapper_194:
    cli
    pusha
    pushl $194
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_195
isr_wrapper_195:
    cli
    pusha
    pushl $195
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_196
isr_wrapper_196:
    cli
    pusha
    pushl $196
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_197
isr_wrapper_197:
    cli
    pusha
    pushl $197
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_198
isr_wrapper_198:
    cli
    pusha
    pushl $198
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_199
isr_wrapper_199:
    cli
    pusha
    pushl $199
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_200
isr_wrapper_200:
    cli
    pusha
    pushl $200
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_201
isr_wrapper_201:
    cli
    pusha
    pushl $201
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_202
isr_wrapper_202:
    cli
    pusha
    pushl $202
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_203
isr_wrapper_203:
    cli
    pusha
    pushl $203
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_204
isr_wrapper_204:
    cli
    pusha
    pushl $204
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_205
isr_wrapper_205:
    cli
    pusha
    pushl $205
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_206
isr_wrapper_206:
    cli
    pusha
    pushl $206
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_207
isr_wrapper_207:
    cli
    pusha
    pushl $207
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_208
isr_wrapper_208:
    cli
    pusha
    pushl $208
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_209
isr_wrapper_209:
    cli
    pusha
    pushl $209
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_210
isr_wrapper_210:
    cli
    pusha
    pushl $210
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_211
isr_wrapper_211:
    cli
    pusha
    pushl $211
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_212
isr_wrapper_212:
    cli
    pusha
    pushl $212
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_213
isr_wrapper_213:
    cli
    pusha
    pushl $213
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_214
isr_wrapper_214:
    cli
    pusha
    pushl $214
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_215
isr_wrapper_215:
    cli
    pusha
    pushl $215
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_216
isr_wrapper_216:
    cli
    pusha
    pushl $216
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_217
isr_wrapper_217:
    cli
    pusha
    pushl $217
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_218
isr_wrapper_218:
    cli
    pusha
    pushl $218
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_219
isr_wrapper_219:
    cli
    pusha
    pushl $219
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_220
isr_wrapper_220:
    cli
    pusha
    pushl $220
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_221
isr_wrapper_221:
    cli
    pusha
    pushl $221
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_222
isr_wrapper_222:
    cli
    pusha
    pushl $222
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_223
isr_wrapper_223:
    cli
    pusha
    pushl $223
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_224
isr_wrapper_224:
    cli
    pusha
    pushl $224
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_225
isr_wrapper_225:
    cli
    pusha
    pushl $225
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_226
isr_wrapper_226:
    cli
    pusha
    pushl $226
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_227
isr_wrapper_227:
    cli
    pusha
    pushl $227
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_228
isr_wrapper_228:
    cli
    pusha
    pushl $228
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_229
isr_wrapper_229:
    cli
    pusha
    pushl $229
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_230
isr_wrapper_230:
    cli
    pusha
    pushl $230
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_231
isr_wrapper_231:
    cli
    pusha
    pushl $231
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_232
isr_wrapper_232:
    cli
    pusha
    pushl $232
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_233
isr_wrapper_233:
    cli
    pusha
    pushl $233
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_234
isr_wrapper_234:
    cli
    pusha
    pushl $234
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_235
isr_wrapper_235:
    cli
    pusha
    pushl $235
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_236
isr_wrapper_236:
    cli
    pusha
    pushl $236
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_237
isr_wrapper_237:
    cli
    pusha
    pushl $237
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_238
isr_wrapper_238:
    cli
    pusha
    pushl $238
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_239
isr_wrapper_239:
    cli
    pusha
    pushl $239
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_240
isr_wrapper_240:
    cli
    pusha
    pushl $240
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_241
isr_wrapper_241:
    cli
    pusha
    pushl $241
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_242
isr_wrapper_242:
    cli
    pusha
    pushl $242
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_243
isr_wrapper_243:
    cli
    pusha
    pushl $243
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_244
isr_wrapper_244:
    cli
    pusha
    pushl $244
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_245
isr_wrapper_245:
    cli
    pusha
    pushl $245
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_246
isr_wrapper_246:
    cli
    pusha
    pushl $246
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_247
isr_wrapper_247:
    cli
    pusha
    pushl $247
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_248
isr_wrapper_248:
    cli
    pusha
    pushl $248
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_249
isr_wrapper_249:
    cli
    pusha
    pushl $249
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_250
isr_wrapper_250:
    cli
    pusha
    pushl $250
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_251
isr_wrapper_251:
    cli
    pusha
    pushl $251
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_252
isr_wrapper_252:
    cli
    pusha
    pushl $252
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_253
isr_wrapper_253:
    cli
    pusha
    pushl $253
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_254
isr_wrapper_254:
    cli
    pusha
    pushl $254
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

.global isr_wrapper_255
isr_wrapper_255:
    cli
    pusha
    pushl $255
    call interrupt_dispatch
    addl $4, %esp # Clean up the interrupt number.
    popa
    sti
    iret

