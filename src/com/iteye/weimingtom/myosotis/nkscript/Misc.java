package com.iteye.weimingtom.myosotis.nkscript;

public class Misc {
	public static boolean chrstr(char chr, String chrArray) {
		for (int p = 0; p < chrArray.length(); ++p) {
			if (chrArray.charAt(p) == chr) {
				return true;
			}
		}
		return false;
	}
	
	public static boolean IsConst(String str) {
		return Misc.chrstr(str.charAt(0), "0123456789") ||
			chrstr(str.charAt(0), "+-.") && 
			chrstr(str.charAt(1), "0123456789");
	}
	
	public static boolean IsVar(String str) { 
		return chrstr(str.charAt(0), "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") &&
			strstr2(str, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789") == str.length();
	}
	
	public static int strstr2(String str, String chrArray) {
		int p1;
		for (p1 = 0; p1 < str.length(); ++p1) {
			boolean ret = true;
			for (int p2 = 0; p2 < chrArray.length(); ++p2) {
				if (str.charAt(p1) == chrArray.charAt(p2)) {
					ret = false;
				}
			}
			if (ret) {
				return p1;
			}
		}
		return p1;
	}
	
	public static boolean strstrstr(String str1, String str2, char c) {
		int pFront = 0; //str2;
		int pRear = 0;
		while ((pRear = str2.indexOf(c, pFront)) != -1) {
			String buffer = str2.substring(pFront, pRear);
			if (str1.indexOf(buffer) == 0) {
				return true;
			}
			pFront = pRear + 1;
		}
		if (str1.indexOf(str2.substring(pFront)) == 0) {
			return true;
		}
		return false;
	}
	
	public static boolean strstrstr2(String str1, String str2, char c) {
		int pFront = 0; //str2;
		int pRear = 0;
		while ((pRear = str2.indexOf(c, pFront)) != -1) {
			String buffer = str2.substring(pFront, pRear);
			if (str1.compareTo(buffer) == 0) {
				return true;
			}
			pFront = pRear + 1;
		}
		if (str1.compareTo(str2.substring(pFront)) == 0) {
			return true;
		}
		return false;
	}
}
