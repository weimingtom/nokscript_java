package com.iteye.weimingtom.myosotis.nkscript;

import java.util.ArrayList;
import java.util.List;

public class TokenArray {
	private List<Token> vctpToken = new ArrayList<Token>();

	public TokenArray() {
		
	}
	
	public List<Token> GetArray() {
		return this.vctpToken;
	}
	
	public boolean CutToToken(String str) {
		int p = 0;
		do {
			String former = null;
			if (this.vctpToken.size() > 0) {
				former = (this.vctpToken.get(this.vctpToken.size() - 1)).GetStr();
			}
			Token pToken = new Token();
			p = pToken.Cut(str, p, former);
			if (p != -1 && pToken.GetStr() != null) {
				this.vctpToken.add(pToken);
			} else {
				pToken = null;
			}
		} while (p != -1);
		return true;
	}

	public void allprintf() {
		for (int i = 0; i < this.vctpToken.size(); ++i) {
			this.vctpToken.get(i).Printf();
		}
	}
	
	public int GetCorrespondence(int index, String kakko1, String kakko2) {
		int i;
		int depth = 0;
		for (i = index; i != this.vctpToken.size() && this.vctpToken.get(i) != null; ++i) {
			if (this.vctpToken.get(i).GetStr() != null) {
				if (this.vctpToken.get(i).GetStr().equals(kakko2)) {
					depth--;
					if (depth == 0) {
						return i;
					}
				} else if (this.vctpToken.get(i).GetStr().equals(kakko1)) {
					depth++;
				}
			}
		}
		return -1;
	}
	
	public int Search(int index, String str) {
		for (int i = index; i != this.vctpToken.size(); ++i) {
			if (this.vctpToken.get(i).GetStr().equals(str)) {
				return i;
			}
		}
		return -1;
	}
	
	public int SearchCommma(int first, int last) {
		int depth = 0;
		for (int i = first; i < last; ++i) {
			if (",".equals(this.vctpToken.get(i).GetStr()) && depth == 0) {
				return i;
			}
			if ("(".equals(this.vctpToken.get(i).GetStr())) {
				depth++;
			}
			if (")".equals(this.vctpToken.get(i).GetStr())) {
				depth--;
			}
		}
		return -1;
	}
	
	public int SearchEnzansi(String str, char c, int first, int last) {
		int depth = 0;
		for (int i = first; i < last; ++i) {
			if (Misc.strstrstr2(this.vctpToken.get(i).GetStr(), str, c) && depth == 0) {
				return i;
			}
			if ("(".equals(this.vctpToken.get(i).GetStr())) {
				depth++;
			}
			if (")".equals(this.vctpToken.get(i).GetStr())) {
				depth--;
			}
		}
		return -1;
	}
}
