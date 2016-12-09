#pragma once

void asm_de_wrapper();
void asm_db_wrapper();
void asm_nmi_wrapper();
void asm_bp_wrapper();
void asm_of_wrapper();
void asm_br_wrapper();
void asm_ud_wrapper();
void asm_nm_wrapper();
void asm_df_wrapper();
void asm_cso_wrapper();
void asm_ts_wrapper();
void asm_np_wrapper();
void asm_ss_wrapper();
void asm_gp_wrapper();
void asm_pf_wrapper();
void asm_mf_wrapper();
void asm_ac_wrapper();
void asm_mc_wrapper();
void asm_xf_wrapper();

void asm_timer_wrapper();
void asm_keyboard_wrapper();

void disable_interrupts();
void enable_interrupts();
