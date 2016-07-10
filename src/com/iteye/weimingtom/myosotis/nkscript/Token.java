package com.iteye.weimingtom.myosotis.nkscript;

public class Token {
	private String m_pString = null;
	
	public Token() {
		m_pString = null;
	}
	
	public String GetStr() {
		return m_pString;
	}
	
	public void Printf() {
		//FIXME:
		//Outputf("%s\n",m_pString);
		System.out.println(m_pString + "\n");
	}

	public int Cut(String str, int pStr, String former) {
		if (str == null || str.length() == 0) {
			m_pString = null;
			return -1;
		}
		int pFront = pStr;
		while (Misc.chrstr(str.charAt(pFront), " \t\n\r") != false) {
			pFront++;
			if (pFront == str.length()) {
				return -1;
			}
		}
		if (str.indexOf("/*", pFront) == pFront) {
			int p = str.indexOf("*/", pFront);
			if (p == -1) {
				return -1;
			}
			return Cut(str, p + 2, former);
		} else if (str.indexOf("//", pFront) == pFront) {
			int p = str.indexOf("\r\n", pFront);
			if (p == -1) {
				return -1;
			}
			return Cut(str, p + 1, former);
		} else if (Misc.chrstr(str.charAt(pFront), "0123456789") ||	
			Misc.chrstr(str.charAt(pFront), "+-") && 
			Misc.chrstr(str.charAt(pFront + 1), "0123456789") &&
			(former == null ||
			!Misc.IsConst(former) && 
			!Misc.IsVar(former) && 
			former.compareTo(")") != 0)) {
			int pRear = Misc.strstr2(str.substring(pFront + 1), ".0123456789");
			m_pString = str.substring(pFront, pRear);
			return pRear;
		} else if (str.indexOf("\"", pFront) == pFront) {
			int pRear = str.indexOf('\"', pFront + 1);
			if (pRear != -1) {
				m_pString = str.substring(pFront, pRear + 1);
			}
			return pRear + 1;
		} else if (Misc.strstrstr(str.substring(pFront), "+=,-=,++,--,==,!=,<=,>=,||,&&",',')) {
			m_pString = str.substring(pFront, pFront + 2);
			return pFront + 2;
		} else if (Misc.chrstr(str.charAt(pFront), "()+-*=/<>&|!#%'~^[]{}@`?:;,.")) {
			m_pString = str.substring(pFront, pFront + 1);
			return pFront + 1;
		} else if (Misc.chrstr(str.charAt(pFront), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_")) {
			int pRear = Misc.strstr2(str.substring(pFront), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
			m_pString = str.substring(pFront, pRear);
			return pRear;
		} else {
			MBF_ERR(str.charAt(pFront) + ":token cut faild¡£");
			return -1;
		}
	}
	
	private static void MB_ERR(String str) {
		System.out.println(str);
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
}
