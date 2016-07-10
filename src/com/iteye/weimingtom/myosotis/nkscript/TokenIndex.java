package com.iteye.weimingtom.myosotis.nkscript;

/**
 * FIXME: sort
 * @author Administrator
 *
 */
public class TokenIndex {
	public int index;
	public TokenArray pTokenArray;
	
	public TokenIndex() {
		
	}
	
	//FIXME:
	public int getIndex() {
		return Get();
	}
	
	public int Get() {
		String p = this.pTokenArray.GetArray().get(this.index).GetStr();
		if (Misc.strstrstr2(p, "=,+=,-=", ',')) {
			return 0;
		} 
		if (p.indexOf("||") == 0) {
			return 10;
		}
		if (p.indexOf("&&") == 0) {
			return 20;
		}
		if (Misc.strstrstr2(p, "==,!=", ',')) {
			return 30;
		}
		if (Misc.strstrstr2(p, "<,<=,>,>=", ',')) {
			return 40;
		}
		if (Misc.strstrstr2(p, "+,-", ',')) {
			return 50;
		}
		if (Misc.strstrstr2(p, "*,/,%", ',')) {
			return 60;
		}
		return -1;
	}
}
