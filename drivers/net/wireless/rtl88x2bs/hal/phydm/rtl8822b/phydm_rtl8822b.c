/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#if 0
	/* ============================================================
	*  include files
	* ============================================================ */
#endif

#include "mp_precomp.h"
#include "../phydm_precomp.h"

#if (RTL8822B_SUPPORT == 1)


void
phydm_dynamic_switch_htstf_mumimo_8822b(
	struct PHY_DM_STRUCT		*p_dm_odm
)
{
	/*if rssi > 40dBm, enable HT-STF gain controller, otherwise, if rssi < 40dBm, disable the controller*/
	/*add by Chun-Hung Ho 20160711 */
	if (p_dm_odm->rssi_min >= 40)
		odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(17), 0x1);
	else if (p_dm_odm->rssi_min < 35)
		odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(17), 0x0);

}

void
phydm_dynamic_parameters_ota(
	struct PHY_DM_STRUCT		*p_dm_odm
)
{
	u8	rssi_l2h = 40, rssi_h2l = 35;

	if ((*p_dm_odm->p_channel <= 14) && (*p_dm_odm->p_band_width == ODM_BW20M)) {
		if (p_dm_odm->rssi_min >= rssi_l2h) {
			odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(17), 0x1);
			odm_set_bb_reg(p_dm_odm, 0x98c, 0x7fc0000, 0x0);
			odm_set_bb_reg(p_dm_odm, 0x818, 0x7000000, 0x1);
			/*odm_set_bb_reg(p_dm_odm, 0xc04, BIT(18), 0x1);*/
			/*odm_set_bb_reg(p_dm_odm, 0xe04, BIT(18), 0x1);*/
			odm_set_bb_reg(p_dm_odm, 0xc04, BIT(18), 0x0);
			odm_set_bb_reg(p_dm_odm, 0xe04, BIT(18), 0x0);
			if (p_dm_odm->p_advance_ota & PHYDM_HP_OTA_SETTING_A) {
				odm_set_bb_reg(p_dm_odm, 0x19d8, MASKDWORD, 0x444);
				odm_set_bb_reg(p_dm_odm, 0x19d4, MASKDWORD, 0x4444aaaa);
			} else if (p_dm_odm->p_advance_ota & PHYDM_HP_OTA_SETTING_B) {
				odm_set_bb_reg(p_dm_odm, 0x19d8, MASKDWORD, 0x444);
				odm_set_bb_reg(p_dm_odm, 0x19d4, MASKDWORD, 0x444444aa);
			}
		} else if (p_dm_odm->rssi_min < rssi_h2l) {
			odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(17), 0x0);
			odm_set_bb_reg(p_dm_odm, 0x98c, MASKDWORD, 0x43440000);
			odm_set_bb_reg(p_dm_odm, 0x818, BIT(26), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc04, BIT(18), 0x0);
			odm_set_bb_reg(p_dm_odm, 0xe04, BIT(18), 0x0);
			odm_set_bb_reg(p_dm_odm, 0x19d8, MASKDWORD, 0xaaa);
			odm_set_bb_reg(p_dm_odm, 0x19d4, MASKDWORD, 0xaaaaaaaa);
		}
	} else {
			//odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(17), 0x0);
			odm_set_bb_reg(p_dm_odm, 0x98c, MASKDWORD, 0x43440000);
			odm_set_bb_reg(p_dm_odm, 0x818, BIT(26), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc04, BIT(18), 0x0);
			odm_set_bb_reg(p_dm_odm, 0xe04, BIT(18), 0x0);
			odm_set_bb_reg(p_dm_odm, 0x19d8, MASKDWORD, 0xaaa);
			odm_set_bb_reg(p_dm_odm, 0x19d4, MASKDWORD, 0xaaaaaaaa);
		}
}


static
void
_setTxACaliValue(
	struct PHY_DM_STRUCT		*p_dm_odm,
	u8						eRFPath,
	u8 						offset,
	u8 						TxABiaOffset
	)
{
	u32 modiTxAValue = 0;
	u8 tmp1Byte = 0;
	boolean bMinus = false;
	u8 compValue = 0;

	
		switch (offset) {
		case 0x0:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X10124);
			break;
		case 0x1:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X10524);
			break;
		case 0x2:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X10924);
			break;
		case 0x3:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X10D24);
			break;
		case 0x4:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X30164);
			break;
		case 0x5:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X30564);
			break;
		case 0x6:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X30964);
			break;
		case 0x7:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X30D64);
			break;
		case 0x8:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X50195);
			break;
		case 0x9:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X50595);
			break;
		case 0xa:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X50995);
			break;
		case 0xb:
			odm_set_rf_reg(p_dm_odm, eRFPath, 0x18, 0xFFFFF, 0X50D95);
			break;
		default:
			ODM_RT_TRACE(p_dm_odm, ODM_COMP_COMMON, ODM_DBG_LOUD, ("Invalid TxA band offset...\n"));
			return;
			break;
	}

	/* Get TxA value */
	modiTxAValue = odm_get_rf_reg(p_dm_odm, eRFPath, 0x61, 0xFFFFF);
	tmp1Byte = (u8)modiTxAValue&(BIT(3)|BIT(2)|BIT(1)|BIT(0));

	/* check how much need to calibration */
		switch (TxABiaOffset) {
		case 0xF6:
			bMinus = true;
			compValue = 3;
			break;
			
		case 0xF4:
			bMinus = true;
			compValue = 2;
			break;
			
		case 0xF2:
			bMinus = true;
			compValue = 1;
			break;
			
		case 0xF3:
			bMinus = false;
			compValue = 1;
			break;
			
		case 0xF5:
			bMinus = false;
			compValue = 2;
			break;
			
		case 0xF7:
			bMinus = false;
			compValue = 3;
			break;
			
		case 0xF9:
			bMinus = false;
			compValue = 4;
			break;
		
		/* do nothing case */
		case 0xF0:
		default:
			ODM_RT_TRACE(p_dm_odm, ODM_COMP_COMMON, ODM_DBG_LOUD, ("No need to do TxA bias current calibration\n"));
			return;
			break;
	}

	/* calc correct value to calibrate */
	if (bMinus) {
		if (tmp1Byte >= compValue) {
			tmp1Byte -= compValue;
			//modiTxAValue += tmp1Byte;
		} else {
			tmp1Byte = 0;
		}
	} else {
		tmp1Byte += compValue;
		if (tmp1Byte >= 7) {
			tmp1Byte = 7;
		}
	}

	/* Write back to RF reg */
	odm_set_rf_reg(p_dm_odm, eRFPath, 0x30, 0xFFFF, (offset<<12|(modiTxAValue&0xFF0)|tmp1Byte));
}

static
void
_txaBiasCali4eachPath(
	struct PHY_DM_STRUCT		*p_dm_odm,
	u8	 eRFPath,
	u8	 efuseValue
	)
{
	/* switch on set TxA bias */
	odm_set_rf_reg(p_dm_odm, eRFPath, 0xEF, 0xFFFFF, 0x200);

	/* Set 12 sets of TxA value */
	_setTxACaliValue(p_dm_odm, eRFPath, 0x0, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x1, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x2, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x3, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x4, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x5, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x6, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x7, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x8, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0x9, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0xa, efuseValue);
	_setTxACaliValue(p_dm_odm, eRFPath, 0xb, efuseValue);

	// switch off set TxA bias
	odm_set_rf_reg(p_dm_odm, eRFPath, 0xEF, 0xFFFFF, 0x0);
}

/* for 8822B PCIE D-cut patch only */
/* Normal driver and MP driver need this patch */

void
phydm_txcurrentcalibration(
	struct PHY_DM_STRUCT		*p_dm_odm
	)
{
	u8			efuse0x3D8, efuse0x3D7;
	u32			origRF0x18PathA = 0, origRF0x18PathB = 0;

	if (!(p_dm_odm->support_ic_type & ODM_RTL8822B))
		return;

	ODM_RT_TRACE(p_dm_odm, ODM_COMP_MP, ODM_DBG_LOUD, ("8822b 5g tx current calibration 0x3d7=0x%X 0x3d8=0x%X\n", p_dm_odm->efuse0x3d7, p_dm_odm->efuse0x3d8));

	/* save original 0x18 value */
	origRF0x18PathA = odm_get_rf_reg(p_dm_odm, ODM_RF_PATH_A, 0x18, 0xFFFFF);
	origRF0x18PathB = odm_get_rf_reg(p_dm_odm, ODM_RF_PATH_B, 0x18, 0xFFFFF);
	
	/* define efuse content */
		efuse0x3D8 = p_dm_odm->efuse0x3d8;
		efuse0x3D7 = p_dm_odm->efuse0x3d7;
	
	/* check efuse content to judge whether need to calibration or not */
	if (0xFF == efuse0x3D7) {
		ODM_RT_TRACE(p_dm_odm, ODM_COMP_MP, ODM_DBG_LOUD, ("efuse content 0x3D7 == 0xFF, No need to do TxA cali\n"));
		return;
	}

	/* write RF register for calibration */
	_txaBiasCali4eachPath(p_dm_odm, ODM_RF_PATH_A, efuse0x3D7);
	_txaBiasCali4eachPath(p_dm_odm, ODM_RF_PATH_B, efuse0x3D8);
	
	/* restore original 0x18 value */
	odm_set_rf_reg(p_dm_odm, ODM_RF_PATH_A, 0x18, 0xFFFFF, origRF0x18PathA);
	odm_set_rf_reg(p_dm_odm, ODM_RF_PATH_B, 0x18, 0xFFFFF, origRF0x18PathB);
}

void
phydm_1rcca_setting(
	struct 	PHY_DM_STRUCT		*p_dm_odm,
	boolean enable_1rcca
)
{
	u32 reg_32;

	reg_32 = odm_get_bb_reg(p_dm_odm, 0xa04, 0x0f000000);

	/* Enable or disable 1RCCA setting accrodding to the control from driver */
	if (enable_1rcca == true) {
		if (reg_32 == 0x0) {
			odm_set_bb_reg(p_dm_odm, 0x808, MASKBYTE0, 0x13); /* CCK path-a */
		} else if (reg_32 == 0x5) {
			odm_set_bb_reg(p_dm_odm, 0x808, MASKBYTE0, 0x23); /* CCK path-b */
		}
	} else {
		odm_set_bb_reg(p_dm_odm, 0x808, MASKBYTE0, 0x33); /* disable 1RCCA */
		odm_set_bb_reg(p_dm_odm, 0xa04, 0x0f000000, 0x0); /* CCK default is at path-a */
	}
}

void
phydm_dynamic_select_cck_path_8822b(
	struct 	PHY_DM_STRUCT		*p_dm
)
{
	struct _FALSE_ALARM_STATISTICS	*p_fa_cnt = (struct _FALSE_ALARM_STATISTICS *)phydm_get_structure(p_dm, PHYDM_FALSEALMCNT);
	struct phydm_rtl8822b_struct	*p_8822b = &p_dm->phydm_rtl8822b;

	if (p_dm->ap_total_num > 10) {
		if (p_8822b->path_judge & BIT(2))
			odm_set_bb_reg(p_dm, 0xa04, 0x0f000000, 0x0);	/*fix CCK Path A if AP nums > 10*/
		return;
	}

	if (p_8822b->path_judge & BIT(2))
		return;

	ODM_RT_TRACE(p_dm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
		("phydm 8822b cck rx path selection start\n"));

	if (p_8822b->path_judge & ODM_RF_A) {
		p_8822b->path_a_cck_fa = (u16)p_fa_cnt->cnt_cck_fail;
		p_8822b->path_judge &= ~ODM_RF_A;
		odm_set_bb_reg(p_dm, 0xa04, 0x0f000000, 0x5);	/*change to path B collect CCKFA*/
	} else if (p_8822b->path_judge & ODM_RF_B) {
		p_8822b->path_b_cck_fa = (u16)p_fa_cnt->cnt_cck_fail;
		p_8822b->path_judge &= ~ODM_RF_B;

		if (p_8822b->path_a_cck_fa <= p_8822b->path_b_cck_fa)
			odm_set_bb_reg(p_dm, 0xa04, 0x0f000000, 0x0);	/*FA A<=B choose A*/
		else
			odm_set_bb_reg(p_dm, 0xa04, 0x0f000000, 0x5);	/*FA B>A choose B*/

		p_8822b->path_judge |= BIT(2);	/*it means we have already choosed cck rx path*/
	}

	ODM_RT_TRACE(p_dm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
		("path_a_fa = %d, path_b_fa = %d\n", p_8822b->path_a_cck_fa, p_8822b->path_b_cck_fa));
	
}

void
phydm_somlrxhp_setting(
	struct 	PHY_DM_STRUCT		*p_dm_odm,
	boolean switch_soml
)
{
	if (switch_soml == true) {
		odm_set_bb_reg(p_dm_odm, 0x19a8, MASKDWORD, 0xd10a0000);
	/* Following are RxHP settings for T2R as always low, workaround for OTA test, required to classify */
		odm_set_bb_reg(p_dm_odm, 0xc04, (BIT(21)|BIT(18)), 0x0);
		odm_set_bb_reg(p_dm_odm, 0xe04, (BIT(21)|BIT(18)), 0x0);
	} else {
		odm_set_bb_reg(p_dm_odm, 0x19a8, MASKDWORD, 0x010a0000);
		odm_set_bb_reg(p_dm_odm, 0xc04, (BIT(21)|BIT(18)), 0x0);
		odm_set_bb_reg(p_dm_odm, 0xe04, (BIT(21)|BIT(18)), 0x0);
	}
	
	/* Dynamic RxHP setting with SoML on/off apply on all RFE type, except of QFN eFEM (1,6,7,9) */
	if ((switch_soml == false) && ((p_dm_odm->rfe_type == 1) || (p_dm_odm->rfe_type == 6) || (p_dm_odm->rfe_type == 7) || (p_dm_odm->rfe_type == 9))) {
		odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108000);
		odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x0);
	}
		
	if (*p_dm_odm->p_channel <= 14) {
		if ((switch_soml == true) && (!((p_dm_odm->rfe_type == 3) || (p_dm_odm->rfe_type == 5) || (p_dm_odm->rfe_type == 8)))) {
			odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108000);
			odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x0);
		}
	} else if (*p_dm_odm->p_channel > 35) {
		if (switch_soml == true) {
			odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108000);
			odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x0);
		}
	}

#if 0
	if (!((p_dm_odm->rfe_type == 1) || (p_dm_odm->rfe_type == 6) || (p_dm_odm->rfe_type == 7) || (p_dm_odm->rfe_type == 9))) {
		if (*p_dm_odm->p_channel <= 14) {
			/* TFBGA iFEM SoML on/off with RxHP always high-to-low */
			if (!((p_dm_odm->rfe_type == 3) || (p_dm_odm->rfe_type == 5))) {
				if (switch_soml == true) {
				odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108000);
				odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x0);
				odm_set_bb_reg(p_dm_odm, 0xc04, (BIT(18)|BIT(21)), 0x0);
				odm_set_bb_reg(p_dm_odm, 0xe04, (BIT(18)|BIT(21)), 0x0);
				} else {
				odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108492);
				odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x1);
				}
			}
		} else if (*p_dm_odm->p_channel > 35) {
			if ((switch_soml == true) && (!((p_dm_odm->rfe_type == 1) || (p_dm_odm->rfe_type == 6) || (p_dm_odm->rfe_type == 7) || (p_dm_odm->rfe_type == 9)))) {
				odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108000);
				odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x0);
				odm_set_bb_reg(p_dm_odm, 0xc04, (BIT(18)|BIT(21)), 0x0);
				odm_set_bb_reg(p_dm_odm, 0xe04, (BIT(18)|BIT(21)), 0x0);
			} else {
				odm_set_bb_reg(p_dm_odm, 0x8cc, MASKDWORD, 0x08108492);
				odm_set_bb_reg(p_dm_odm, 0x8d8, BIT(27), 0x1);
			}	
		}
		/* ODM_RT_TRACE(p_dm_odm, ODM_COMP_COMMON, ODM_DBG_LOUD, ("Dynamic RxHP control with SoML is enable !!\n")); */
	}	
#endif
}

void
phydm_dynamic_ant_weighting_8822b(
	struct PHY_DM_STRUCT		*p_dm_odm
)
{
	u8 rssi_l2h = 43, rssi_h2l = 37;
	u8 reg_8;

	if (p_dm_odm->is_disable_dym_ant_weighting)
		return;

	if (*p_dm_odm->p_channel <= 14) {
		if (p_dm_odm->rssi_min >= rssi_l2h) {
			odm_set_bb_reg(p_dm_odm, 0x98c, 0x7fc0000, 0x0);

			/*equal weighting*/
			reg_8 = (u8)odm_get_bb_reg(p_dm_odm, 0xf94, BIT(0)|BIT(1)|BIT(2));
			/*ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_TRACE, ("Equal weighting ,rssi_min = %d\n, 0xf94[2:0] = 0x%x\n", p_dm_odm->rssi_min, reg_8));*/
		} else if (p_dm_odm->rssi_min <= rssi_h2l) {
			odm_set_bb_reg(p_dm_odm, 0x98c, MASKDWORD, 0x43440000);
	
			/*fix sec_min_wgt = 1/2*/
			reg_8 = (u8)odm_get_bb_reg(p_dm_odm, 0xf94, BIT(0)|BIT(1)|BIT(2));
			/*ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_TRACE, ("AGC weighting ,rssi_min = %d\n, 0xf94[2:0] = 0x%x\n", p_dm_odm->rssi_min, reg_8));*/
        }
	} else {
			odm_set_bb_reg(p_dm_odm, 0x98c, MASKDWORD, 0x43440000);

			reg_8 = (u8)odm_get_bb_reg(p_dm_odm, 0xf94, BIT(0)|BIT(1)|BIT(2));
			/*ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_TRACE, ("AGC weighting ,rssi_min = %d\n, 0xf94[2:0] = 0x%x\n", p_dm_odm->rssi_min, reg_8));*/
		/*fix sec_min_wgt = 1/2*/
    }

}

void
phydm_rxagc_switch_8822b(
		struct PHY_DM_STRUCT		*p_dm_odm,
		boolean enable_rxagc_switch
)
{
	if ((p_dm_odm->rfe_type == 15) || (p_dm_odm->rfe_type == 16)) {
		ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
		("Microsoft case!\n"));
	} else {
		ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
		("Not Microsoft case\n"));
		return;
	}

	if (enable_rxagc_switch == true) {
		if ((*p_dm_odm->p_channel >= 36) && (*p_dm_odm->p_channel <= 64)) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x5);
		} else if ((*p_dm_odm->p_channel >= 100) && (*p_dm_odm->p_channel <= 144)) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x2);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x6);
		} else if (*p_dm_odm->p_channel >= 149) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x3);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x7);
		}
		p_dm_odm->brxagcswitch = true;
		ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
		("Microsoft case! AGC table (path-b) is switched!\n"));
	} else {
		if ((*p_dm_odm->p_channel >= 36) && (*p_dm_odm->p_channel <= 64)) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x1);
		} else if ((*p_dm_odm->p_channel >= 100) && (*p_dm_odm->p_channel <= 144)) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x2);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x2);
		} else if (*p_dm_odm->p_channel >= 149) {
			odm_set_bb_reg(p_dm_odm, 0x958, BIT(4), 0x1);
			odm_set_bb_reg(p_dm_odm, 0xc1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x3);
			odm_set_bb_reg(p_dm_odm, 0xe1c, (BIT(11)|BIT(10)|BIT(9)|BIT(8)), 0x3);
		}
		p_dm_odm->brxagcswitch = false;
		ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_LOUD,
				("AGC table are the same on path-a and b\n"));
	}
		
}

void
phydm_hwsetting_8822b(
	struct PHY_DM_STRUCT		*p_dm_odm
)
{
	struct phydm_rtl8822b_struct	*p_8822b = &p_dm_odm->phydm_rtl8822b;

	if((p_dm_odm->p_advance_ota & PHYDM_HP_OTA_SETTING_A) || (p_dm_odm->p_advance_ota & PHYDM_HP_OTA_SETTING_B)) {
		phydm_dynamic_parameters_ota(p_dm_odm);
	} else {
		if (p_dm_odm->bhtstfdisabled == 0)
			phydm_dynamic_switch_htstf_mumimo_8822b(p_dm_odm);
			ODM_RT_TRACE(p_dm_odm, ODM_PHY_CONFIG, ODM_DBG_TRACE, ("Dynamic HTSTF is enable\n"));
		}

	if (p_dm_odm->p_advance_ota & PHYDM_ASUS_OTA_SETTING) {
		if (p_dm_odm->rssi_min <= 20)
			phydm_somlrxhp_setting(p_dm_odm, false);
		else if (p_dm_odm->rssi_min >= 25)
			phydm_somlrxhp_setting(p_dm_odm, true);
	}

	if ((p_dm_odm->p_advance_ota & PHYDM_ASUS_OTA_SETTING_CCK_PATH) || (p_dm_odm->p_advance_ota & PHYDM_HP_OTA_SETTING_CCK_PATH)) {
		if (p_dm_odm->is_linked)
			phydm_dynamic_select_cck_path_8822b(p_dm_odm);
		else
			p_8822b->path_judge |= ((~ BIT(2)) | ODM_RF_A | ODM_RF_B);
	}
	
}

#endif	/* RTL8822B_SUPPORT == 1 */
