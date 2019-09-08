/* Generated automatically by the program `genflags'
   from the machine description file `md'.  */

#ifndef GCC_INSN_FLAGS_H
#define GCC_INSN_FLAGS_H

#define HAVE_addsf3 (TARGET_HARD_FLOAT)
#define HAVE_adddf3 (TARGET_HARD_FLOAT)
#define HAVE_subsf3 (TARGET_HARD_FLOAT)
#define HAVE_subdf3 (TARGET_HARD_FLOAT)
#define HAVE_mulsf3 (TARGET_HARD_FLOAT)
#define HAVE_muldf3 (TARGET_HARD_FLOAT)
#define HAVE_mulditi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_umulditi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_muldi3_highpart (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_umuldi3_highpart (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_usmulditi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_usmuldi3_highpart (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_mulsi3_highpart (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_umulsi3_highpart (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_usmulsi3_highpart (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_divsi3 ((TARGET_MULDIV || ((Pulp_Cpu >= PULP_V2) && !TARGET_MASK_NOHWDIV)))
#define HAVE_udivsi3 ((TARGET_MULDIV || ((Pulp_Cpu >= PULP_V2) && !TARGET_MASK_NOHWDIV)))
#define HAVE_divdi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_udivdi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_modsi3 ((TARGET_MULDIV && ((Pulp_Cpu >= PULP_V2) && !TARGET_MASK_NOHWDIV)))
#define HAVE_umodsi3 ((TARGET_MULDIV && ((Pulp_Cpu >= PULP_V2) && !TARGET_MASK_NOHWDIV)))
#define HAVE_moddi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_umoddi3 (TARGET_MULDIV && TARGET_64BIT)
#define HAVE_divsf3 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_divdf3 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_sqrtsf2 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_sqrtdf2 ((TARGET_HARD_FLOAT && TARGET_FDIV) && (TARGET_HARD_FLOAT))
#define HAVE_fmasf4 (TARGET_HARD_FLOAT)
#define HAVE_fmadf4 (TARGET_HARD_FLOAT)
#define HAVE_fmssf4 (TARGET_HARD_FLOAT)
#define HAVE_fmsdf4 (TARGET_HARD_FLOAT)
#define HAVE_nfmasf4 (TARGET_HARD_FLOAT)
#define HAVE_nfmadf4 (TARGET_HARD_FLOAT)
#define HAVE_nfmssf4 (TARGET_HARD_FLOAT)
#define HAVE_nfmsdf4 (TARGET_HARD_FLOAT)
#define HAVE_abssf2 (TARGET_HARD_FLOAT)
#define HAVE_absdf2 (TARGET_HARD_FLOAT)
#define HAVE_abssi2 ((Pulp_Cpu>=PULP_V0))
#define HAVE_sminsf3 (TARGET_HARD_FLOAT)
#define HAVE_smindf3 (TARGET_HARD_FLOAT)
#define HAVE_smaxsf3 (TARGET_HARD_FLOAT)
#define HAVE_smaxdf3 (TARGET_HARD_FLOAT)
#define HAVE_sminsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_smaxsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_uminsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_umaxsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_avgsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_avgusi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMINMAX))
#define HAVE_popcountsi2 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_fl1si2 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_clrsbsi2 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_ctzsi2 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_rotrsi3 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_mulqisi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_umulqisi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_mulhisi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_umulhisi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_smulhi3_highpart (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_umulhi3_highpart (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_mulhhs_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_mulhhu_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_mulsNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_mulsRNr_si3 ((!TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_muluNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_muluRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_mulhhsNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_mulhhuNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_mulhhsRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_mulhhuRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_macs_si4 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_macu_si4 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_machlsu_si4 (((Pulp_Cpu==PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_machlu_si4 (((Pulp_Cpu==PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_machhs_si4 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_machhu_si4 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_machls_si4 (((Pulp_Cpu==PULP_V0) && !TARGET_MASK_NOPARTMAC))
#define HAVE_macsNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], NULL)))
#define HAVE_macuNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], NULL)))
#define HAVE_macsRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], operands[5])))
#define HAVE_macuRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], operands[5])))
#define HAVE_machhsNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], NULL)))
#define HAVE_machhuNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], NULL)))
#define HAVE_machhsRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], operands[5])))
#define HAVE_machhuRNr_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMULMACNORMROUND && riscv_valid_norm_round_imm_op(operands[4], operands[5])))
#define HAVE_maddsisi4 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOMAC))
#define HAVE_msubsisi4 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOMAC))
#define HAVE_addN_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_addNu_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_subN_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_subNu_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], NULL)))
#define HAVE_addRN_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_addRNu_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_subRN_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_subRNu_si3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOADDSUBNORMROUND && riscv_valid_norm_round_imm_op(operands[3], operands[4])))
#define HAVE_clip_maxmin ((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOCLIP && riscv_valid_clip_operands (operands[2], operands[3], 1))
#define HAVE_clip_minmax ((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOCLIP && riscv_valid_clip_operands (operands[3], operands[2], 1))
#define HAVE_clipu_maxmin ((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOCLIP && riscv_valid_clip_operands (operands[2], operands[3], 0))
#define HAVE_clipu_minmax ((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOCLIP && riscv_valid_clip_operands (operands[3], operands[2], 0))
#define HAVE_bclrsi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP && riscv_valid_bit_field_imm_operand(operands[2], NULL, 0, NULL, NULL)) && !riscv_compress_sx_imm_operand(operands[2]))
#define HAVE_bsetsi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP && riscv_valid_bit_field_imm_operand(operands[2], NULL, 1, NULL, NULL)))
#define HAVE_extvsi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP))
#define HAVE_extzvsi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP))
#define HAVE_insvsi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP))
#define HAVE_invsipat1 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOBITOP) \
   && riscv_bitmask (INTVAL (operands[4]), NULL, VOIDmode) == INTVAL (operands[5]) \
   && INTVAL(operands[2]) == ~INTVAL(operands[4]))
#define HAVE_negsf2 (TARGET_HARD_FLOAT)
#define HAVE_negdf2 (TARGET_HARD_FLOAT)
#define HAVE_one_cmplsi2 1
#define HAVE_one_cmpldi2 (TARGET_64BIT)
#define HAVE_andsi3 1
#define HAVE_anddi3 (TARGET_64BIT)
#define HAVE_iorsi3 1
#define HAVE_iordi3 (TARGET_64BIT)
#define HAVE_xorsi3 1
#define HAVE_xordi3 (TARGET_64BIT)
#define HAVE_truncdfsf2 (TARGET_HARD_FLOAT)
#define HAVE_truncdisi2 (TARGET_64BIT)
#define HAVE_zero_extendsidi2 (TARGET_64BIT)
#define HAVE_zero_extendhisi2 1
#define HAVE_zero_extendhidi2 (TARGET_64BIT)
#define HAVE_zero_extendqihi2 1
#define HAVE_zero_extendqisi2 1
#define HAVE_zero_extendqidi2 (TARGET_64BIT)
#define HAVE_extendsidi2 (TARGET_64BIT)
#define HAVE_extendqihi2 1
#define HAVE_extendqisi2 1
#define HAVE_extendqidi2 (TARGET_64BIT)
#define HAVE_extendhihi2 1
#define HAVE_extendhisi2 1
#define HAVE_extendhidi2 (TARGET_64BIT)
#define HAVE_extendsfdf2 (TARGET_HARD_FLOAT)
#define HAVE_fix_truncdfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fix_truncsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fix_truncdfdi2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_fix_truncsfdi2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_floatsidf2 (TARGET_HARD_FLOAT)
#define HAVE_floatdidf2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_floatsisf2 (TARGET_HARD_FLOAT)
#define HAVE_floatdisf2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_floatunssidf2 (TARGET_HARD_FLOAT)
#define HAVE_floatunsdidf2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_floatunssisf2 (TARGET_HARD_FLOAT)
#define HAVE_floatunsdisf2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_fixuns_truncdfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fixuns_truncsfsi2 (TARGET_HARD_FLOAT)
#define HAVE_fixuns_truncdfdi2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_fixuns_truncsfdi2 (TARGET_HARD_FLOAT && TARGET_64BIT)
#define HAVE_got_loadsi ((flag_pic) && (Pmode == SImode))
#define HAVE_got_loaddi ((flag_pic) && (Pmode == DImode))
#define HAVE_tls_add_tp_lesi ((!flag_pic || flag_pie) && (Pmode == SImode))
#define HAVE_tls_add_tp_ledi ((!flag_pic || flag_pie) && (Pmode == DImode))
#define HAVE_got_load_tls_gdsi ((flag_pic) && (Pmode == SImode))
#define HAVE_got_load_tls_gddi ((flag_pic) && (Pmode == DImode))
#define HAVE_got_load_tls_iesi ((flag_pic) && (Pmode == SImode))
#define HAVE_got_load_tls_iedi ((flag_pic) && (Pmode == DImode))
#define HAVE_loadqi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadhi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadsi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadsf_ind_reg_reg ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadv4qi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadqi_ext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadqi_uext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadhi_ext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadhi_uext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadsi_ext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadsi_uext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadsf_ext_ind_reg_reg ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadsf_uext_ind_reg_reg ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadv2hi_uext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadv4qi_ext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_loadv4qi_uext_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_storeqi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_storehi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_storesi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_storesf_ind_reg_reg ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG)) && (!TARGET_HARD_FLOAT))
#define HAVE_storev2hi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_storev4qi_ind_reg_reg (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOINDREGREG))
#define HAVE_load_evt_unit ((Pulp_Cpu>=PULP_V2))
#define HAVE_read_spr ((Pulp_Cpu>=PULP_V2))
#define HAVE_write_spr ((Pulp_Cpu>=PULP_V2))
#define HAVE_spr_bit_set ((Pulp_Cpu>=PULP_V2))
#define HAVE_spr_bit_clr ((Pulp_Cpu>=PULP_V2))
#define HAVE_writesivol ((Pulp_Cpu>=PULP_V2))
#define HAVE_writesi ((Pulp_Cpu>=PULP_V2))
#define HAVE_readsivol ((Pulp_Cpu>=PULP_V2))
#define HAVE_readsi ((Pulp_Cpu>=PULP_V2))
#define HAVE_OffsetedRead ((Pulp_Cpu>=PULP_V2))
#define HAVE_OffsetedReadOMP ((Pulp_Cpu>=PULP_V2))
#define HAVE_OffsetedReadNonVol ((Pulp_Cpu>=PULP_V2))
#define HAVE_loadqi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ind_postinc ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_ext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_uext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_uext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_uext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ext_ind_postinc ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadsf_uext_ind_postinc ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv2hi_uext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_uext_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ind_postdec ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_ext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_uext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_uext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_uext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ext_ind_postdec ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadsf_uext_ind_postdec ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv2hi_uext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_uext_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ind_post_mod ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_ext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadqi_uext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_ext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadhi_uext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_ext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsi_uext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadsf_ext_ind_post_mod ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadsf_uext_ind_post_mod ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_loadv2hi_ext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv2hi_uext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_ext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_loadv4qi_uext_ind_post_mod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storeqi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storehi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesf_ind_postinc ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_storev2hi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storev4qi_ind_postinc (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storeqi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storehi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesf_ind_postdec ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_storev2hi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storev4qi_ind_postdec (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storeqi_ind_postmod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storehi_ind_postmod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesi_ind_postmod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storesf_ind_postmod ((((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD)) && (!TARGET_HARD_FLOAT))
#define HAVE_storev2hi_ind_postmod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_storev4qi_ind_postmod (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOPOSTMOD))
#define HAVE_addhihi3 1
#define HAVE_addsihi3 1
#define HAVE_xorhihi3 1
#define HAVE_xorsihi3 1
#define HAVE_load_lowdf ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_load_lowdi ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_load_lowtf ((TARGET_HARD_FLOAT) && (TARGET_64BIT))
#define HAVE_load_highdf ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_load_highdi ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_load_hightf ((TARGET_HARD_FLOAT) && (TARGET_64BIT))
#define HAVE_store_worddf ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_store_worddi ((TARGET_HARD_FLOAT) && (!TARGET_64BIT))
#define HAVE_store_wordtf ((TARGET_HARD_FLOAT) && (TARGET_64BIT))
#define HAVE_fence 1
#define HAVE_fence_i 1
#define HAVE_ashlsi3 1
#define HAVE_ashrsi3 1
#define HAVE_lshrsi3 1
#define HAVE_ashldi3 (TARGET_64BIT)
#define HAVE_ashrdi3 (TARGET_64BIT)
#define HAVE_lshrdi3 (TARGET_64BIT)
#define HAVE_ashlsi3_extend (TARGET_64BIT)
#define HAVE_ashrsi3_extend (TARGET_64BIT)
#define HAVE_lshrsi3_extend (TARGET_64BIT)
#define HAVE_vec_initv2hi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_initv4qi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_pack_v2hi (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_pack_v4qi_lo (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_pack_v4qi_lo_first (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_pack_v4qi_hi (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_pack_v4qi_hi_first (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv2hi_internal2_1 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)  && riscv_valid_permute_operands (operands[1], operands[2], operands[3])))
#define HAVE_vec_permv2hi_internal2 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv2hi_int1 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv4qi_internal2_1 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)  && riscv_valid_permute_operands (operands[1], operands[2], operands[3])))
#define HAVE_vec_permv4qi_internal2 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv4qi_int1 (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_setv2hi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_setv4qi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_set_fisrtv2hi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_set_fisrtv4qi_internal (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_qi_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_qi_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_hi_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_hi_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_si_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_sext_si_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_qi_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_qi_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_hi_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_hi_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_si_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extract_zext_si_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extractv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_extractv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_addv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_subv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sminv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smaxv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_addv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_subv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sminv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smaxv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_addscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_subscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sminscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smaxscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_addscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_subscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sminscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smaxscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_add_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sub_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smin_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smax_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_add_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sub_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smin_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_smax_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_uminv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umaxv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_uminv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umaxv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_uminscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umaxscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_uminscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umaxscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umin_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umax_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umin_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_umax_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vlshrv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashrv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashlv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vlshrv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashrv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashlv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vlshrscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashrscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashlscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vlshrscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashrscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vashlscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgsc_swap_v2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgsc_swap_v4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgv2uhi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgv4uqi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgscv2uhi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgscv4uqi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgsc_swap_v2uhi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_avgsc_swap_v4uqi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_andv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_iorv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exorv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_andv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_iorv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exorv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_andscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_iorscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exorscv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_andscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_iorscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exorscv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_and_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_ior_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exor_swap_scv2hi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_and_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_ior_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_exor_swap_scv4qi3 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_absv2hi2 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_absv4qi2 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_negv2hi2 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_negv4qi2 (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotpv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotspscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_reduc_plus_scal_v2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotupv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotupscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotuspv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotuspscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotpv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_reduc_plus_scal_v4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotspscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotupv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotupscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotuspv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_dotuspscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdot_prodv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotspscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_udot_prodv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotupscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotuspv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotuspscv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdot_prodv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotspscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_udot_prodv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotupscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotuspv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_sdotuspscv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_eq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_ne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_lt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_ge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_gt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_eq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_ne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_le (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_lt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_ge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_gt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_gtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_ltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_geu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_leu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_gtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_ltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_geu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_leu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_sceq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scle (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_sclt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scgt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scgtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scgeu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv2hi_scleu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_sceq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scle (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_sclt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scgt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scgtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scgeu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmpv4qi_scleu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_sceq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scle (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_sclt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scgt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scgtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scgeu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v2hi_scleu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_sceq (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scne (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scle (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_sclt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scge (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scgt (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scgtu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scltu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scgeu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cmp_swap_v4qi_scleu (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_cstoresf4 (TARGET_HARD_FLOAT)
#define HAVE_cstoredf4 (TARGET_HARD_FLOAT)
#define HAVE_jump 1
#define HAVE_indirect_jumpsi (Pmode == SImode)
#define HAVE_indirect_jumpdi (Pmode == DImode)
#define HAVE_tablejumpsi 1
#define HAVE_tablejumpdi (TARGET_64BIT)
#define HAVE_blockage 1
#define HAVE_simple_return 1
#define HAVE_simple_return_internal 1
#define HAVE_simple_it_return 1
#define HAVE_eh_set_lr_si (! TARGET_64BIT)
#define HAVE_eh_set_lr_di (TARGET_64BIT)
#define HAVE_set_hwloop_lpstart 1
#define HAVE_set_hwloop_lpend 1
#define HAVE_set_hwloop_lc 1
#define HAVE_set_hwloop_lc_le 1
#define HAVE_hw_loop_prolog 1
#define HAVE_zero_cost_loop_end ((Pulp_Cpu>=PULP_V1) && !TARGET_MASK_NOHWLOOP && optimize)
#define HAVE_loop_end (((Pulp_Cpu>=PULP_V1) && !TARGET_MASK_NOHWLOOP))
#define HAVE_sibcall_internal (SIBLING_CALL_P (insn))
#define HAVE_sibcall_value_internal (SIBLING_CALL_P (insn))
#define HAVE_sibcall_value_multiple_internal (SIBLING_CALL_P (insn))
#define HAVE_call_internal 1
#define HAVE_call_value_internal 1
#define HAVE_call_value_multiple_internal 1
#define HAVE_nop 1
#define HAVE_forced_nop 1
#define HAVE_trap 1
#define HAVE_gpr_save 1
#define HAVE_gpr_restore 1
#define HAVE_gpr_restore_return 1
#define HAVE_mem_thread_fence_1 1
#define HAVE_atomic_storesi (TARGET_ATOMIC)
#define HAVE_atomic_storedi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_addsi (TARGET_ATOMIC)
#define HAVE_atomic_orsi (TARGET_ATOMIC)
#define HAVE_atomic_xorsi (TARGET_ATOMIC)
#define HAVE_atomic_andsi (TARGET_ATOMIC)
#define HAVE_atomic_adddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_ordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_xordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_anddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_addsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_orsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_xorsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_andsi (TARGET_ATOMIC)
#define HAVE_atomic_fetch_adddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_ordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_xordi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_fetch_anddi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_exchangesi (TARGET_ATOMIC)
#define HAVE_atomic_exchangedi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_cas_value_strongsi (TARGET_ATOMIC)
#define HAVE_atomic_cas_value_strongdi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_addsi3 1
#define HAVE_adddi3 (TARGET_64BIT)
#define HAVE_subsi3 1
#define HAVE_subdi3 (TARGET_64BIT)
#define HAVE_mulsi3 ((Pulp_Cpu>=PULP_V0) || TARGET_MULDIV || TARGET_MASK_RVSTD)
#define HAVE_muldi3 (((Pulp_Cpu>=PULP_V0) || TARGET_MULDIV || TARGET_MASK_RVSTD) && (TARGET_64BIT))
#define HAVE_mulsidi3 (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_umulsidi3 (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_usmulsidi3 (((TARGET_MULDIV||(Pulp_Cpu>=PULP_V2)) && !TARGET_64BIT))
#define HAVE_clzsi2 (((Pulp_Cpu>=PULP_V0) && !TARGET_MASK_NOBITOP))
#define HAVE_movdi 1
#define HAVE_pulp_omp_barrier ((Pulp_Cpu>=PULP_V2))
#define HAVE_pulp_omp_critical_start ((Pulp_Cpu>=PULP_V2))
#define HAVE_pulp_omp_critical_end ((Pulp_Cpu>=PULP_V2))
#define HAVE_movsi 1
#define HAVE_movv2hi 1
#define HAVE_movv4qi 1
#define HAVE_movmisalignv4qi 1
#define HAVE_movmisalignv2hi 1
#define HAVE_movmisalignsf 1
#define HAVE_movmisalignsi 1
#define HAVE_movhi 1
#define HAVE_movqi 1
#define HAVE_movsf 1
#define HAVE_movdf 1
#define HAVE_movti (TARGET_64BIT)
#define HAVE_clear_cache 1
#define HAVE_movmemsi (!TARGET_MEMCPY)
#define HAVE_vec_initv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_initv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_pack_v4qi (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv2hi (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_permv4qi (((Pulp_Cpu>=PULP_V2) && !(TARGET_MASK_NOVECT||TARGET_MASK_NOSHUFFLEPACK)))
#define HAVE_vec_set_firstv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_set_firstv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_setv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vec_setv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vcondv2hiv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vcondv4qiv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vconduv2hiv2hi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_vconduv4qiv4qi (((Pulp_Cpu>=PULP_V2) && !TARGET_MASK_NOVECT))
#define HAVE_condjump 1
#define HAVE_cbranchsi4 1
#define HAVE_cbranchdi4 (TARGET_64BIT)
#define HAVE_cbranchsf4 (TARGET_HARD_FLOAT)
#define HAVE_cbranchdf4 (TARGET_HARD_FLOAT)
#define HAVE_cstoresi4 1
#define HAVE_cstoredi4 (TARGET_64BIT)
#define HAVE_indirect_jump 1
#define HAVE_tablejump 1
#define HAVE_prologue 1
#define HAVE_epilogue 1
#define HAVE_sibcall_epilogue 1
#define HAVE_return (riscv_can_use_return_insn ())
#define HAVE_eh_return 1
#define HAVE_doloop_end 1
#define HAVE_sibcall 1
#define HAVE_sibcall_value 1
#define HAVE_call 1
#define HAVE_call_value 1
#define HAVE_untyped_call 1
#define HAVE_mem_thread_fence 1
#define HAVE_atomic_compare_and_swapsi (TARGET_ATOMIC)
#define HAVE_atomic_compare_and_swapdi ((TARGET_ATOMIC) && (TARGET_64BIT))
#define HAVE_atomic_test_and_set (TARGET_ATOMIC)
extern rtx        gen_addsf3                          (rtx, rtx, rtx);
extern rtx        gen_adddf3                          (rtx, rtx, rtx);
extern rtx        gen_subsf3                          (rtx, rtx, rtx);
extern rtx        gen_subdf3                          (rtx, rtx, rtx);
extern rtx        gen_mulsf3                          (rtx, rtx, rtx);
extern rtx        gen_muldf3                          (rtx, rtx, rtx);
extern rtx        gen_mulditi3                        (rtx, rtx, rtx);
extern rtx        gen_umulditi3                       (rtx, rtx, rtx);
extern rtx        gen_muldi3_highpart                 (rtx, rtx, rtx);
extern rtx        gen_umuldi3_highpart                (rtx, rtx, rtx);
extern rtx        gen_usmulditi3                      (rtx, rtx, rtx);
extern rtx        gen_usmuldi3_highpart               (rtx, rtx, rtx);
extern rtx        gen_mulsi3_highpart                 (rtx, rtx, rtx);
extern rtx        gen_umulsi3_highpart                (rtx, rtx, rtx);
extern rtx        gen_usmulsi3_highpart               (rtx, rtx, rtx);
extern rtx        gen_divsi3                          (rtx, rtx, rtx);
extern rtx        gen_udivsi3                         (rtx, rtx, rtx);
extern rtx        gen_divdi3                          (rtx, rtx, rtx);
extern rtx        gen_udivdi3                         (rtx, rtx, rtx);
extern rtx        gen_modsi3                          (rtx, rtx, rtx);
extern rtx        gen_umodsi3                         (rtx, rtx, rtx);
extern rtx        gen_moddi3                          (rtx, rtx, rtx);
extern rtx        gen_umoddi3                         (rtx, rtx, rtx);
extern rtx        gen_divsf3                          (rtx, rtx, rtx);
extern rtx        gen_divdf3                          (rtx, rtx, rtx);
extern rtx        gen_sqrtsf2                         (rtx, rtx);
extern rtx        gen_sqrtdf2                         (rtx, rtx);
extern rtx        gen_fmasf4                          (rtx, rtx, rtx, rtx);
extern rtx        gen_fmadf4                          (rtx, rtx, rtx, rtx);
extern rtx        gen_fmssf4                          (rtx, rtx, rtx, rtx);
extern rtx        gen_fmsdf4                          (rtx, rtx, rtx, rtx);
extern rtx        gen_nfmasf4                         (rtx, rtx, rtx, rtx);
extern rtx        gen_nfmadf4                         (rtx, rtx, rtx, rtx);
extern rtx        gen_nfmssf4                         (rtx, rtx, rtx, rtx);
extern rtx        gen_nfmsdf4                         (rtx, rtx, rtx, rtx);
extern rtx        gen_abssf2                          (rtx, rtx);
extern rtx        gen_absdf2                          (rtx, rtx);
extern rtx        gen_abssi2                          (rtx, rtx);
extern rtx        gen_sminsf3                         (rtx, rtx, rtx);
extern rtx        gen_smindf3                         (rtx, rtx, rtx);
extern rtx        gen_smaxsf3                         (rtx, rtx, rtx);
extern rtx        gen_smaxdf3                         (rtx, rtx, rtx);
extern rtx        gen_sminsi3                         (rtx, rtx, rtx);
extern rtx        gen_smaxsi3                         (rtx, rtx, rtx);
extern rtx        gen_uminsi3                         (rtx, rtx, rtx);
extern rtx        gen_umaxsi3                         (rtx, rtx, rtx);
extern rtx        gen_avgsi3                          (rtx, rtx, rtx);
extern rtx        gen_avgusi3                         (rtx, rtx, rtx);
extern rtx        gen_popcountsi2                     (rtx, rtx);
extern rtx        gen_fl1si2                          (rtx, rtx);
extern rtx        gen_clrsbsi2                        (rtx, rtx);
extern rtx        gen_ctzsi2                          (rtx, rtx);
extern rtx        gen_rotrsi3                         (rtx, rtx, rtx);
extern rtx        gen_mulqisi3                        (rtx, rtx, rtx);
extern rtx        gen_umulqisi3                       (rtx, rtx, rtx);
extern rtx        gen_mulhisi3                        (rtx, rtx, rtx);
extern rtx        gen_umulhisi3                       (rtx, rtx, rtx);
extern rtx        gen_smulhi3_highpart                (rtx, rtx, rtx);
extern rtx        gen_umulhi3_highpart                (rtx, rtx, rtx);
extern rtx        gen_mulhhs_si3                      (rtx, rtx, rtx);
extern rtx        gen_mulhhu_si3                      (rtx, rtx, rtx);
extern rtx        gen_mulsNr_si3                      (rtx, rtx, rtx, rtx);
extern rtx        gen_mulsRNr_si3                     (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_muluNr_si3                      (rtx, rtx, rtx, rtx);
extern rtx        gen_muluRNr_si3                     (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_mulhhsNr_si3                    (rtx, rtx, rtx, rtx);
extern rtx        gen_mulhhuNr_si3                    (rtx, rtx, rtx, rtx);
extern rtx        gen_mulhhsRNr_si3                   (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_mulhhuRNr_si3                   (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_macs_si4                        (rtx, rtx, rtx, rtx);
extern rtx        gen_macu_si4                        (rtx, rtx, rtx, rtx);
extern rtx        gen_machlsu_si4                     (rtx, rtx, rtx, rtx);
extern rtx        gen_machlu_si4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_machhs_si4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_machhu_si4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_machls_si4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_macsNr_si3                      (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_macuNr_si3                      (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_macsRNr_si3                     (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_macuRNr_si3                     (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_machhsNr_si3                    (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_machhuNr_si3                    (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_machhsRNr_si3                   (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_machhuRNr_si3                   (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_maddsisi4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_msubsisi4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_addN_si3                        (rtx, rtx, rtx, rtx);
extern rtx        gen_addNu_si3                       (rtx, rtx, rtx, rtx);
extern rtx        gen_subN_si3                        (rtx, rtx, rtx, rtx);
extern rtx        gen_subNu_si3                       (rtx, rtx, rtx, rtx);
extern rtx        gen_addRN_si3                       (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_addRNu_si3                      (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_subRN_si3                       (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_subRNu_si3                      (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_clip_maxmin                     (rtx, rtx, rtx, rtx);
extern rtx        gen_clip_minmax                     (rtx, rtx, rtx, rtx);
extern rtx        gen_clipu_maxmin                    (rtx, rtx, rtx, rtx);
extern rtx        gen_clipu_minmax                    (rtx, rtx, rtx, rtx);
extern rtx        gen_bclrsi3                         (rtx, rtx, rtx);
extern rtx        gen_bsetsi3                         (rtx, rtx, rtx);
extern rtx        gen_extvsi                          (rtx, rtx, rtx, rtx);
extern rtx        gen_extzvsi                         (rtx, rtx, rtx, rtx);
extern rtx        gen_insvsi                          (rtx, rtx, rtx, rtx);
extern rtx        gen_invsipat1                       (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_negsf2                          (rtx, rtx);
extern rtx        gen_negdf2                          (rtx, rtx);
extern rtx        gen_one_cmplsi2                     (rtx, rtx);
extern rtx        gen_one_cmpldi2                     (rtx, rtx);
extern rtx        gen_andsi3                          (rtx, rtx, rtx);
extern rtx        gen_anddi3                          (rtx, rtx, rtx);
extern rtx        gen_iorsi3                          (rtx, rtx, rtx);
extern rtx        gen_iordi3                          (rtx, rtx, rtx);
extern rtx        gen_xorsi3                          (rtx, rtx, rtx);
extern rtx        gen_xordi3                          (rtx, rtx, rtx);
extern rtx        gen_truncdfsf2                      (rtx, rtx);
extern rtx        gen_truncdisi2                      (rtx, rtx);
extern rtx        gen_zero_extendsidi2                (rtx, rtx);
extern rtx        gen_zero_extendhisi2                (rtx, rtx);
extern rtx        gen_zero_extendhidi2                (rtx, rtx);
extern rtx        gen_zero_extendqihi2                (rtx, rtx);
extern rtx        gen_zero_extendqisi2                (rtx, rtx);
extern rtx        gen_zero_extendqidi2                (rtx, rtx);
extern rtx        gen_extendsidi2                     (rtx, rtx);
extern rtx        gen_extendqihi2                     (rtx, rtx);
extern rtx        gen_extendqisi2                     (rtx, rtx);
extern rtx        gen_extendqidi2                     (rtx, rtx);
extern rtx        gen_extendhihi2                     (rtx, rtx);
extern rtx        gen_extendhisi2                     (rtx, rtx);
extern rtx        gen_extendhidi2                     (rtx, rtx);
extern rtx        gen_extendsfdf2                     (rtx, rtx);
extern rtx        gen_fix_truncdfsi2                  (rtx, rtx);
extern rtx        gen_fix_truncsfsi2                  (rtx, rtx);
extern rtx        gen_fix_truncdfdi2                  (rtx, rtx);
extern rtx        gen_fix_truncsfdi2                  (rtx, rtx);
extern rtx        gen_floatsidf2                      (rtx, rtx);
extern rtx        gen_floatdidf2                      (rtx, rtx);
extern rtx        gen_floatsisf2                      (rtx, rtx);
extern rtx        gen_floatdisf2                      (rtx, rtx);
extern rtx        gen_floatunssidf2                   (rtx, rtx);
extern rtx        gen_floatunsdidf2                   (rtx, rtx);
extern rtx        gen_floatunssisf2                   (rtx, rtx);
extern rtx        gen_floatunsdisf2                   (rtx, rtx);
extern rtx        gen_fixuns_truncdfsi2               (rtx, rtx);
extern rtx        gen_fixuns_truncsfsi2               (rtx, rtx);
extern rtx        gen_fixuns_truncdfdi2               (rtx, rtx);
extern rtx        gen_fixuns_truncsfdi2               (rtx, rtx);
extern rtx        gen_got_loadsi                      (rtx, rtx);
extern rtx        gen_got_loaddi                      (rtx, rtx);
extern rtx        gen_tls_add_tp_lesi                 (rtx, rtx, rtx, rtx);
extern rtx        gen_tls_add_tp_ledi                 (rtx, rtx, rtx, rtx);
extern rtx        gen_got_load_tls_gdsi               (rtx, rtx);
extern rtx        gen_got_load_tls_gddi               (rtx, rtx);
extern rtx        gen_got_load_tls_iesi               (rtx, rtx);
extern rtx        gen_got_load_tls_iedi               (rtx, rtx);
extern rtx        gen_loadqi_ind_reg_reg              (rtx, rtx, rtx);
extern rtx        gen_loadhi_ind_reg_reg              (rtx, rtx, rtx);
extern rtx        gen_loadsi_ind_reg_reg              (rtx, rtx, rtx);
extern rtx        gen_loadsf_ind_reg_reg              (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_ind_reg_reg            (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_ind_reg_reg            (rtx, rtx, rtx);
extern rtx        gen_loadqi_ext_ind_reg_reg          (rtx, rtx, rtx);
extern rtx        gen_loadqi_uext_ind_reg_reg         (rtx, rtx, rtx);
extern rtx        gen_loadhi_ext_ind_reg_reg          (rtx, rtx, rtx);
extern rtx        gen_loadhi_uext_ind_reg_reg         (rtx, rtx, rtx);
extern rtx        gen_loadsi_ext_ind_reg_reg          (rtx, rtx, rtx);
extern rtx        gen_loadsi_uext_ind_reg_reg         (rtx, rtx, rtx);
extern rtx        gen_loadsf_ext_ind_reg_reg          (rtx, rtx, rtx);
extern rtx        gen_loadsf_uext_ind_reg_reg         (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_ext_ind_reg_reg        (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_uext_ind_reg_reg       (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_ext_ind_reg_reg        (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_uext_ind_reg_reg       (rtx, rtx, rtx);
extern rtx        gen_storeqi_ind_reg_reg             (rtx, rtx, rtx);
extern rtx        gen_storehi_ind_reg_reg             (rtx, rtx, rtx);
extern rtx        gen_storesi_ind_reg_reg             (rtx, rtx, rtx);
extern rtx        gen_storesf_ind_reg_reg             (rtx, rtx, rtx);
extern rtx        gen_storev2hi_ind_reg_reg           (rtx, rtx, rtx);
extern rtx        gen_storev4qi_ind_reg_reg           (rtx, rtx, rtx);
extern rtx        gen_load_evt_unit                   (rtx, rtx, rtx);
extern rtx        gen_read_spr                        (rtx, rtx);
extern rtx        gen_write_spr                       (rtx, rtx);
extern rtx        gen_spr_bit_set                     (rtx, rtx);
extern rtx        gen_spr_bit_clr                     (rtx, rtx);
extern rtx        gen_writesivol                      (rtx, rtx, rtx);
extern rtx        gen_writesi                         (rtx, rtx, rtx);
extern rtx        gen_readsivol                       (rtx, rtx, rtx);
extern rtx        gen_readsi                          (rtx, rtx, rtx);
extern rtx        gen_OffsetedRead                    (rtx, rtx, rtx);
extern rtx        gen_OffsetedReadOMP                 (rtx, rtx, rtx);
extern rtx        gen_OffsetedReadNonVol              (rtx, rtx, rtx);
extern rtx        gen_loadqi_ind_postinc              (rtx, rtx);
extern rtx        gen_loadhi_ind_postinc              (rtx, rtx);
extern rtx        gen_loadsi_ind_postinc              (rtx, rtx);
extern rtx        gen_loadsf_ind_postinc              (rtx, rtx);
extern rtx        gen_loadv2hi_ind_postinc            (rtx, rtx);
extern rtx        gen_loadv4qi_ind_postinc            (rtx, rtx);
extern rtx        gen_loadqi_ext_ind_postinc          (rtx, rtx);
extern rtx        gen_loadqi_uext_ind_postinc         (rtx, rtx);
extern rtx        gen_loadhi_ext_ind_postinc          (rtx, rtx);
extern rtx        gen_loadhi_uext_ind_postinc         (rtx, rtx);
extern rtx        gen_loadsi_ext_ind_postinc          (rtx, rtx);
extern rtx        gen_loadsi_uext_ind_postinc         (rtx, rtx);
extern rtx        gen_loadsf_ext_ind_postinc          (rtx, rtx);
extern rtx        gen_loadsf_uext_ind_postinc         (rtx, rtx);
extern rtx        gen_loadv2hi_ext_ind_postinc        (rtx, rtx);
extern rtx        gen_loadv2hi_uext_ind_postinc       (rtx, rtx);
extern rtx        gen_loadv4qi_ext_ind_postinc        (rtx, rtx);
extern rtx        gen_loadv4qi_uext_ind_postinc       (rtx, rtx);
extern rtx        gen_loadqi_ind_postdec              (rtx, rtx);
extern rtx        gen_loadhi_ind_postdec              (rtx, rtx);
extern rtx        gen_loadsi_ind_postdec              (rtx, rtx);
extern rtx        gen_loadsf_ind_postdec              (rtx, rtx);
extern rtx        gen_loadv2hi_ind_postdec            (rtx, rtx);
extern rtx        gen_loadv4qi_ind_postdec            (rtx, rtx);
extern rtx        gen_loadqi_ext_ind_postdec          (rtx, rtx);
extern rtx        gen_loadqi_uext_ind_postdec         (rtx, rtx);
extern rtx        gen_loadhi_ext_ind_postdec          (rtx, rtx);
extern rtx        gen_loadhi_uext_ind_postdec         (rtx, rtx);
extern rtx        gen_loadsi_ext_ind_postdec          (rtx, rtx);
extern rtx        gen_loadsi_uext_ind_postdec         (rtx, rtx);
extern rtx        gen_loadsf_ext_ind_postdec          (rtx, rtx);
extern rtx        gen_loadsf_uext_ind_postdec         (rtx, rtx);
extern rtx        gen_loadv2hi_ext_ind_postdec        (rtx, rtx);
extern rtx        gen_loadv2hi_uext_ind_postdec       (rtx, rtx);
extern rtx        gen_loadv4qi_ext_ind_postdec        (rtx, rtx);
extern rtx        gen_loadv4qi_uext_ind_postdec       (rtx, rtx);
extern rtx        gen_loadqi_ind_post_mod             (rtx, rtx, rtx);
extern rtx        gen_loadhi_ind_post_mod             (rtx, rtx, rtx);
extern rtx        gen_loadsi_ind_post_mod             (rtx, rtx, rtx);
extern rtx        gen_loadsf_ind_post_mod             (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_ind_post_mod           (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_ind_post_mod           (rtx, rtx, rtx);
extern rtx        gen_loadqi_ext_ind_post_mod         (rtx, rtx, rtx);
extern rtx        gen_loadqi_uext_ind_post_mod        (rtx, rtx, rtx);
extern rtx        gen_loadhi_ext_ind_post_mod         (rtx, rtx, rtx);
extern rtx        gen_loadhi_uext_ind_post_mod        (rtx, rtx, rtx);
extern rtx        gen_loadsi_ext_ind_post_mod         (rtx, rtx, rtx);
extern rtx        gen_loadsi_uext_ind_post_mod        (rtx, rtx, rtx);
extern rtx        gen_loadsf_ext_ind_post_mod         (rtx, rtx, rtx);
extern rtx        gen_loadsf_uext_ind_post_mod        (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_ext_ind_post_mod       (rtx, rtx, rtx);
extern rtx        gen_loadv2hi_uext_ind_post_mod      (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_ext_ind_post_mod       (rtx, rtx, rtx);
extern rtx        gen_loadv4qi_uext_ind_post_mod      (rtx, rtx, rtx);
extern rtx        gen_storeqi_ind_postinc             (rtx, rtx);
extern rtx        gen_storehi_ind_postinc             (rtx, rtx);
extern rtx        gen_storesi_ind_postinc             (rtx, rtx);
extern rtx        gen_storesf_ind_postinc             (rtx, rtx);
extern rtx        gen_storev2hi_ind_postinc           (rtx, rtx);
extern rtx        gen_storev4qi_ind_postinc           (rtx, rtx);
extern rtx        gen_storeqi_ind_postdec             (rtx, rtx);
extern rtx        gen_storehi_ind_postdec             (rtx, rtx);
extern rtx        gen_storesi_ind_postdec             (rtx, rtx);
extern rtx        gen_storesf_ind_postdec             (rtx, rtx);
extern rtx        gen_storev2hi_ind_postdec           (rtx, rtx);
extern rtx        gen_storev4qi_ind_postdec           (rtx, rtx);
extern rtx        gen_storeqi_ind_postmod             (rtx, rtx, rtx);
extern rtx        gen_storehi_ind_postmod             (rtx, rtx, rtx);
extern rtx        gen_storesi_ind_postmod             (rtx, rtx, rtx);
extern rtx        gen_storesf_ind_postmod             (rtx, rtx, rtx);
extern rtx        gen_storev2hi_ind_postmod           (rtx, rtx, rtx);
extern rtx        gen_storev4qi_ind_postmod           (rtx, rtx, rtx);
extern rtx        gen_addhihi3                        (rtx, rtx, rtx);
extern rtx        gen_addsihi3                        (rtx, rtx, rtx);
extern rtx        gen_xorhihi3                        (rtx, rtx, rtx);
extern rtx        gen_xorsihi3                        (rtx, rtx, rtx);
extern rtx        gen_load_lowdf                      (rtx, rtx);
extern rtx        gen_load_lowdi                      (rtx, rtx);
extern rtx        gen_load_lowtf                      (rtx, rtx);
extern rtx        gen_load_highdf                     (rtx, rtx, rtx);
extern rtx        gen_load_highdi                     (rtx, rtx, rtx);
extern rtx        gen_load_hightf                     (rtx, rtx, rtx);
extern rtx        gen_store_worddf                    (rtx, rtx, rtx);
extern rtx        gen_store_worddi                    (rtx, rtx, rtx);
extern rtx        gen_store_wordtf                    (rtx, rtx, rtx);
extern rtx        gen_fence                           (void);
extern rtx        gen_fence_i                         (void);
extern rtx        gen_ashlsi3                         (rtx, rtx, rtx);
extern rtx        gen_ashrsi3                         (rtx, rtx, rtx);
extern rtx        gen_lshrsi3                         (rtx, rtx, rtx);
extern rtx        gen_ashldi3                         (rtx, rtx, rtx);
extern rtx        gen_ashrdi3                         (rtx, rtx, rtx);
extern rtx        gen_lshrdi3                         (rtx, rtx, rtx);
extern rtx        gen_ashlsi3_extend                  (rtx, rtx, rtx);
extern rtx        gen_ashrsi3_extend                  (rtx, rtx, rtx);
extern rtx        gen_lshrsi3_extend                  (rtx, rtx, rtx);
extern rtx        gen_vec_initv2hi_internal           (rtx, rtx);
extern rtx        gen_vec_initv4qi_internal           (rtx, rtx);
extern rtx        gen_vec_pack_v2hi                   (rtx, rtx, rtx);
extern rtx        gen_vec_pack_v4qi_lo                (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_pack_v4qi_lo_first          (rtx, rtx, rtx);
extern rtx        gen_vec_pack_v4qi_hi                (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_pack_v4qi_hi_first          (rtx, rtx, rtx);
extern rtx        gen_vec_permv2hi_internal2_1        (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv2hi_internal2          (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv2hi_int1               (rtx, rtx, rtx);
extern rtx        gen_vec_permv4qi_internal2_1        (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv4qi_internal2          (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv4qi_int1               (rtx, rtx, rtx);
extern rtx        gen_vec_setv2hi_internal            (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_setv4qi_internal            (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_set_fisrtv2hi_internal      (rtx, rtx, rtx);
extern rtx        gen_vec_set_fisrtv4qi_internal      (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_qi_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_qi_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_hi_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_hi_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_si_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_sext_si_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_qi_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_qi_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_hi_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_hi_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_si_v2hi        (rtx, rtx, rtx);
extern rtx        gen_vec_extract_zext_si_v4qi        (rtx, rtx, rtx);
extern rtx        gen_vec_extractv2hi                 (rtx, rtx, rtx);
extern rtx        gen_vec_extractv4qi                 (rtx, rtx, rtx);
extern rtx        gen_addv2hi3                        (rtx, rtx, rtx);
extern rtx        gen_subv2hi3                        (rtx, rtx, rtx);
extern rtx        gen_sminv2hi3                       (rtx, rtx, rtx);
extern rtx        gen_smaxv2hi3                       (rtx, rtx, rtx);
extern rtx        gen_addv4qi3                        (rtx, rtx, rtx);
extern rtx        gen_subv4qi3                        (rtx, rtx, rtx);
extern rtx        gen_sminv4qi3                       (rtx, rtx, rtx);
extern rtx        gen_smaxv4qi3                       (rtx, rtx, rtx);
extern rtx        gen_addscv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_subscv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_sminscv2hi3                     (rtx, rtx, rtx);
extern rtx        gen_smaxscv2hi3                     (rtx, rtx, rtx);
extern rtx        gen_addscv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_subscv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_sminscv4qi3                     (rtx, rtx, rtx);
extern rtx        gen_smaxscv4qi3                     (rtx, rtx, rtx);
extern rtx        gen_add_swap_scv2hi3                (rtx, rtx, rtx);
extern rtx        gen_sub_swap_scv2hi3                (rtx, rtx, rtx);
extern rtx        gen_smin_swap_scv2hi3               (rtx, rtx, rtx);
extern rtx        gen_smax_swap_scv2hi3               (rtx, rtx, rtx);
extern rtx        gen_add_swap_scv4qi3                (rtx, rtx, rtx);
extern rtx        gen_sub_swap_scv4qi3                (rtx, rtx, rtx);
extern rtx        gen_smin_swap_scv4qi3               (rtx, rtx, rtx);
extern rtx        gen_smax_swap_scv4qi3               (rtx, rtx, rtx);
extern rtx        gen_uminv2hi3                       (rtx, rtx, rtx);
extern rtx        gen_umaxv2hi3                       (rtx, rtx, rtx);
extern rtx        gen_uminv4qi3                       (rtx, rtx, rtx);
extern rtx        gen_umaxv4qi3                       (rtx, rtx, rtx);
extern rtx        gen_uminscv2hi3                     (rtx, rtx, rtx);
extern rtx        gen_umaxscv2hi3                     (rtx, rtx, rtx);
extern rtx        gen_uminscv4qi3                     (rtx, rtx, rtx);
extern rtx        gen_umaxscv4qi3                     (rtx, rtx, rtx);
extern rtx        gen_umin_swap_scv2hi3               (rtx, rtx, rtx);
extern rtx        gen_umax_swap_scv2hi3               (rtx, rtx, rtx);
extern rtx        gen_umin_swap_scv4qi3               (rtx, rtx, rtx);
extern rtx        gen_umax_swap_scv4qi3               (rtx, rtx, rtx);
extern rtx        gen_vlshrv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_vashrv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_vashlv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_vlshrv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_vashrv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_vashlv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_vlshrscv2hi3                    (rtx, rtx, rtx);
extern rtx        gen_vashrscv2hi3                    (rtx, rtx, rtx);
extern rtx        gen_vashlscv2hi3                    (rtx, rtx, rtx);
extern rtx        gen_vlshrscv4qi3                    (rtx, rtx, rtx);
extern rtx        gen_vashrscv4qi3                    (rtx, rtx, rtx);
extern rtx        gen_vashlscv4qi3                    (rtx, rtx, rtx);
extern rtx        gen_avgv2hi3                        (rtx, rtx, rtx);
extern rtx        gen_avgv4qi3                        (rtx, rtx, rtx);
extern rtx        gen_avgscv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_avgscv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_avgsc_swap_v2hi3                (rtx, rtx, rtx);
extern rtx        gen_avgsc_swap_v4qi3                (rtx, rtx, rtx);
extern rtx        gen_avgv2uhi3                       (rtx, rtx, rtx);
extern rtx        gen_avgv4uqi3                       (rtx, rtx, rtx);
extern rtx        gen_avgscv2uhi3                     (rtx, rtx, rtx);
extern rtx        gen_avgscv4uqi3                     (rtx, rtx, rtx);
extern rtx        gen_avgsc_swap_v2uhi3               (rtx, rtx, rtx);
extern rtx        gen_avgsc_swap_v4uqi3               (rtx, rtx, rtx);
extern rtx        gen_andv2hi3                        (rtx, rtx, rtx);
extern rtx        gen_iorv2hi3                        (rtx, rtx, rtx);
extern rtx        gen_exorv2hi3                       (rtx, rtx, rtx);
extern rtx        gen_andv4qi3                        (rtx, rtx, rtx);
extern rtx        gen_iorv4qi3                        (rtx, rtx, rtx);
extern rtx        gen_exorv4qi3                       (rtx, rtx, rtx);
extern rtx        gen_andscv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_iorscv2hi3                      (rtx, rtx, rtx);
extern rtx        gen_exorscv2hi3                     (rtx, rtx, rtx);
extern rtx        gen_andscv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_iorscv4qi3                      (rtx, rtx, rtx);
extern rtx        gen_exorscv4qi3                     (rtx, rtx, rtx);
extern rtx        gen_and_swap_scv2hi3                (rtx, rtx, rtx);
extern rtx        gen_ior_swap_scv2hi3                (rtx, rtx, rtx);
extern rtx        gen_exor_swap_scv2hi3               (rtx, rtx, rtx);
extern rtx        gen_and_swap_scv4qi3                (rtx, rtx, rtx);
extern rtx        gen_ior_swap_scv4qi3                (rtx, rtx, rtx);
extern rtx        gen_exor_swap_scv4qi3               (rtx, rtx, rtx);
extern rtx        gen_absv2hi2                        (rtx, rtx);
extern rtx        gen_absv4qi2                        (rtx, rtx);
extern rtx        gen_negv2hi2                        (rtx, rtx);
extern rtx        gen_negv4qi2                        (rtx, rtx);
extern rtx        gen_dotpv2hi                        (rtx, rtx, rtx);
extern rtx        gen_dotspscv2hi_le                  (rtx, rtx, rtx);
extern rtx        gen_reduc_plus_scal_v2hi            (rtx, rtx);
extern rtx        gen_dotupv2hi                       (rtx, rtx, rtx);
extern rtx        gen_dotupscv2hi_le                  (rtx, rtx, rtx);
extern rtx        gen_dotuspv2hi                      (rtx, rtx, rtx);
extern rtx        gen_dotuspscv2hi_le                 (rtx, rtx, rtx);
extern rtx        gen_dotpv4qi                        (rtx, rtx, rtx);
extern rtx        gen_reduc_plus_scal_v4qi            (rtx, rtx);
extern rtx        gen_dotspscv4qi_le                  (rtx, rtx, rtx);
extern rtx        gen_dotupv4qi                       (rtx, rtx, rtx);
extern rtx        gen_dotupscv4qi_le                  (rtx, rtx, rtx);
extern rtx        gen_dotuspv4qi                      (rtx, rtx, rtx);
extern rtx        gen_dotuspscv4qi_le                 (rtx, rtx, rtx);
extern rtx        gen_sdot_prodv2hi                   (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotspscv2hi_le                 (rtx, rtx, rtx, rtx);
extern rtx        gen_udot_prodv2hi                   (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotupscv2hi_le                 (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotuspv2hi                     (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotuspscv2hi_le                (rtx, rtx, rtx, rtx);
extern rtx        gen_sdot_prodv4qi                   (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotspscv4qi_le                 (rtx, rtx, rtx, rtx);
extern rtx        gen_udot_prodv4qi                   (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotupscv4qi_le                 (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotuspv4qi                     (rtx, rtx, rtx, rtx);
extern rtx        gen_sdotuspscv4qi_le                (rtx, rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_eq                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_ne                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_le                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_lt                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_ge                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_gt                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_eq                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_ne                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_le                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_lt                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_ge                      (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_gt                      (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_gtu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_ltu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_geu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_leu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_gtu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_ltu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_geu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_leu                     (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_sceq                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scne                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scle                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_sclt                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scge                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scgt                    (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scgtu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scltu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scgeu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv2hi_scleu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_sceq                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scne                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scle                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_sclt                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scge                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scgt                    (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scgtu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scltu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scgeu                   (rtx, rtx, rtx);
extern rtx        gen_cmpv4qi_scleu                   (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_sceq              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scne              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scle              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_sclt              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scge              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scgt              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scgtu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scltu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scgeu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v2hi_scleu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_sceq              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scne              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scle              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_sclt              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scge              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scgt              (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scgtu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scltu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scgeu             (rtx, rtx, rtx);
extern rtx        gen_cmp_swap_v4qi_scleu             (rtx, rtx, rtx);
extern rtx        gen_cstoresf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoredf4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_jump                            (rtx);
extern rtx        gen_indirect_jumpsi                 (rtx);
extern rtx        gen_indirect_jumpdi                 (rtx);
extern rtx        gen_tablejumpsi                     (rtx, rtx);
extern rtx        gen_tablejumpdi                     (rtx, rtx);
extern rtx        gen_blockage                        (void);
extern rtx        gen_simple_return                   (void);
extern rtx        gen_simple_return_internal          (rtx);
extern rtx        gen_simple_it_return                (void);
extern rtx        gen_eh_set_lr_si                    (rtx);
extern rtx        gen_eh_set_lr_di                    (rtx);
extern rtx        gen_set_hwloop_lpstart              (rtx, rtx, rtx);
extern rtx        gen_set_hwloop_lpend                (rtx, rtx, rtx);
extern rtx        gen_set_hwloop_lc                   (rtx, rtx, rtx);
extern rtx        gen_set_hwloop_lc_le                (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_hw_loop_prolog                  (rtx, rtx);
extern rtx        gen_zero_cost_loop_end              (rtx, rtx, rtx);
extern rtx        gen_loop_end                        (rtx, rtx, rtx);
extern rtx        gen_sibcall_internal                (rtx, rtx);
extern rtx        gen_sibcall_value_internal          (rtx, rtx, rtx);
extern rtx        gen_sibcall_value_multiple_internal (rtx, rtx, rtx, rtx);
extern rtx        gen_call_internal                   (rtx, rtx);
extern rtx        gen_call_value_internal             (rtx, rtx, rtx);
extern rtx        gen_call_value_multiple_internal    (rtx, rtx, rtx, rtx);
extern rtx        gen_nop                             (void);
extern rtx        gen_forced_nop                      (void);
extern rtx        gen_trap                            (void);
extern rtx        gen_gpr_save                        (rtx);
extern rtx        gen_gpr_restore                     (rtx);
extern rtx        gen_gpr_restore_return              (rtx);
extern rtx        gen_mem_thread_fence_1              (rtx, rtx);
extern rtx        gen_atomic_storesi                  (rtx, rtx, rtx);
extern rtx        gen_atomic_storedi                  (rtx, rtx, rtx);
extern rtx        gen_atomic_addsi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_orsi                     (rtx, rtx, rtx);
extern rtx        gen_atomic_xorsi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_andsi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_adddi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_ordi                     (rtx, rtx, rtx);
extern rtx        gen_atomic_xordi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_anddi                    (rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_addsi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_orsi               (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_xorsi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_andsi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_adddi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_ordi               (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_xordi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_fetch_anddi              (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_exchangesi               (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_exchangedi               (rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_cas_value_strongsi       (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_cas_value_strongdi       (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_addsi3                          (rtx, rtx, rtx);
extern rtx        gen_adddi3                          (rtx, rtx, rtx);
extern rtx        gen_subsi3                          (rtx, rtx, rtx);
extern rtx        gen_subdi3                          (rtx, rtx, rtx);
extern rtx        gen_mulsi3                          (rtx, rtx, rtx);
extern rtx        gen_muldi3                          (rtx, rtx, rtx);
extern rtx        gen_mulsidi3                        (rtx, rtx, rtx);
extern rtx        gen_umulsidi3                       (rtx, rtx, rtx);
extern rtx        gen_usmulsidi3                      (rtx, rtx, rtx);
extern rtx        gen_clzsi2                          (rtx, rtx);
extern rtx        gen_movdi                           (rtx, rtx);
extern rtx        gen_pulp_omp_barrier                (void);
extern rtx        gen_pulp_omp_critical_start         (void);
extern rtx        gen_pulp_omp_critical_end           (void);
extern rtx        gen_movsi                           (rtx, rtx);
extern rtx        gen_movv2hi                         (rtx, rtx);
extern rtx        gen_movv4qi                         (rtx, rtx);
extern rtx        gen_movmisalignv4qi                 (rtx, rtx);
extern rtx        gen_movmisalignv2hi                 (rtx, rtx);
extern rtx        gen_movmisalignsf                   (rtx, rtx);
extern rtx        gen_movmisalignsi                   (rtx, rtx);
extern rtx        gen_movhi                           (rtx, rtx);
extern rtx        gen_movqi                           (rtx, rtx);
extern rtx        gen_movsf                           (rtx, rtx);
extern rtx        gen_movdf                           (rtx, rtx);
extern rtx        gen_movti                           (rtx, rtx);
extern rtx        gen_clear_cache                     (rtx, rtx);
extern rtx        gen_movmemsi                        (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_initv2hi                    (rtx, rtx);
extern rtx        gen_vec_initv4qi                    (rtx, rtx);
extern rtx        gen_vec_pack_v4qi                   (rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv2hi                    (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_permv4qi                    (rtx, rtx, rtx, rtx);
extern rtx        gen_vec_set_firstv2hi               (rtx, rtx, rtx);
extern rtx        gen_vec_set_firstv4qi               (rtx, rtx, rtx);
extern rtx        gen_vec_setv2hi                     (rtx, rtx, rtx);
extern rtx        gen_vec_setv4qi                     (rtx, rtx, rtx);
extern rtx        gen_vcondv2hiv2hi                   (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_vcondv4qiv4qi                   (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_vconduv2hiv2hi                  (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_vconduv4qiv4qi                  (rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_condjump                        (rtx, rtx);
extern rtx        gen_cbranchsi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchdi4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchsf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_cbranchdf4                      (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoresi4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_cstoredi4                       (rtx, rtx, rtx, rtx);
extern rtx        gen_indirect_jump                   (rtx);
extern rtx        gen_tablejump                       (rtx, rtx);
extern rtx        gen_prologue                        (void);
extern rtx        gen_epilogue                        (void);
extern rtx        gen_sibcall_epilogue                (void);
extern rtx        gen_return                          (void);
extern rtx        gen_eh_return                       (rtx);
extern rtx        gen_doloop_end                      (rtx, rtx);
#define GEN_SIBCALL(A, B, C, D) gen_sibcall ((A), (B), (C), (D))
extern rtx        gen_sibcall                         (rtx, rtx, rtx, rtx);
#define GEN_SIBCALL_VALUE(A, B, C, D, E) gen_sibcall_value ((A), (B), (C), (D))
extern rtx        gen_sibcall_value                   (rtx, rtx, rtx, rtx);
#define GEN_CALL(A, B, C, D) gen_call ((A), (B), (C), (D))
extern rtx        gen_call                            (rtx, rtx, rtx, rtx);
#define GEN_CALL_VALUE(A, B, C, D, E) gen_call_value ((A), (B), (C), (D))
extern rtx        gen_call_value                      (rtx, rtx, rtx, rtx);
extern rtx        gen_untyped_call                    (rtx, rtx, rtx);
extern rtx        gen_mem_thread_fence                (rtx);
extern rtx        gen_atomic_compare_and_swapsi       (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_compare_and_swapdi       (rtx, rtx, rtx, rtx, rtx, rtx, rtx, rtx);
extern rtx        gen_atomic_test_and_set             (rtx, rtx, rtx);

#endif /* GCC_INSN_FLAGS_H */
