/******************************************************
 * ArchC Resources Implementation file.               *
 * This file is automatically generated by ArchC      *
 * WITHOUT WARRANTY OF ANY KIND, either express       *
 * or implied.                                        *
 * For more information on ArchC, please visit:       *
 * http://www.archc.org                               *
 *                                                    *
 * The ArchC Team                                     *
 * Computer Systems Laboratory (LSC)                  *
 * IC-UNICAMP                                         *
 * http://www.lsc.ic.unicamp.br                       *
 ******************************************************/
 

#include "mips_arch.H"


mips_arch::mips_arch() :
  ac_arch_dec_if<mips_parms::ac_word, mips_parms::ac_Hword>(mips_parms::AC_MAX_BUFFER),
  ac_pc("ac_pc", 0),
  DM("DM", 536870912U),
  DM_mport(*this, DM),
  RB("RB"),
  npc("npc", 0),
  hi("hi", 0),
  lo("lo", 0),
  id("id", 0) {

  ac_mt_endian = mips_parms::AC_MATCH_ENDIAN;
  ac_tgt_endian = mips_parms::AC_PROC_ENDIAN;

  INST_PORT = &DM_mport;
  DATA_PORT = &DM_mport;
}

int mips_arch::globalId = 0;